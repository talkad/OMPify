


#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "libsigrok.h"
#include "libsigrok-internal.h"

#define LOG_PREFIX "strutil"









SR_PRIV int sr_atol(const char *str, long *ret)
{
	long tmp;
	char *endptr = NULL;

	errno = 0;
	tmp = strtol(str, &endptr, 0);

	if (!endptr || *endptr || errno) {
		if (!errno)
			errno = EINVAL;
		return SR_ERR;
	}

	*ret = tmp;
	return SR_OK;
}



SR_PRIV int sr_atoi(const char *str, int *ret)
{
	long tmp;

	if (sr_atol(str, &tmp) != SR_OK)
		return SR_ERR;

	if ((int) tmp != tmp) {
		errno = ERANGE;
		return SR_ERR;
	}

	*ret = (int) tmp;
	return SR_OK;
}



SR_PRIV int sr_atod(const char *str, double *ret)
{
	double tmp;
	char *endptr = NULL;

	errno = 0;
	tmp = strtof(str, &endptr);

	if (!endptr || *endptr || errno) {
		if (!errno)
			errno = EINVAL;
		return SR_ERR;
	}

	*ret = tmp;
	return SR_OK;
}



SR_PRIV int sr_atof(const char *str, float *ret)
{
	double tmp;

	if (sr_atod(str, &tmp) != SR_OK)
		return SR_ERR;

	if ((float) tmp != tmp) {
		errno = ERANGE;
		return SR_ERR;
	}

	*ret = (float) tmp;
	return SR_OK;
}



SR_PRIV int sr_atof_ascii(const char *str, float *ret)
{
	double tmp;
	char *endptr = NULL;

	errno = 0;
	tmp = g_ascii_strtod(str, &endptr);

	if (!endptr || *endptr || errno) {
		if (!errno)
			errno = EINVAL;
		return SR_ERR;
	}

	

	


	*ret = (float) tmp;
	return SR_OK;
}



SR_API char *sr_si_string_u64(uint64_t x, const char *unit)
{
	uint8_t i;
	uint64_t quot, divisor[] = {
		SR_HZ(1), SR_KHZ(1), SR_MHZ(1), SR_GHZ(1),
		SR_GHZ(1000), SR_GHZ(1000 * 1000), SR_GHZ(1000 * 1000 * 1000),
	};
	const char *p, prefix[] = "\0kMGTPE";
	char fmt[16], fract[20] = "", *f;

	if (unit == NULL)
		unit = "";

	for (i = 0; (quot = x / divisor[i]) >= 1000; i++);

	if (i) {
		sprintf(fmt, ".%%0%d"PRIu64, i * 3);
		f = fract + sprintf(fract, fmt, x % divisor[i]) - 1;

		while (f >= fract && strchr("0.", *f))
			*f-- = 0;
	}

	p = prefix + i;

	return g_strdup_printf("%" PRIu64 "%s %.1s%s", quot, fract, p, unit);
}



SR_API char *sr_samplerate_string(uint64_t samplerate)
{
	return sr_si_string_u64(samplerate, "Hz");
}



SR_API char *sr_period_string(uint64_t frequency)
{
	char *o;
	int r;

	

	if (!(o = g_try_malloc0(30 + 1))) {
		sr_err("%s: o malloc failed", __func__);
		return NULL;
	}

	if (frequency >= SR_GHZ(1))
		r = snprintf(o, 30, "%" PRIu64 " ns", frequency / 1000000000);
	else if (frequency >= SR_MHZ(1))
		r = snprintf(o, 30, "%" PRIu64 " us", frequency / 1000000);
	else if (frequency >= SR_KHZ(1))
		r = snprintf(o, 30, "%" PRIu64 " ms", frequency / 1000);
	else
		r = snprintf(o, 30, "%" PRIu64 " s", frequency);

	if (r < 0) {
		

		g_free(o);
		return NULL;
	}

	return o;
}



SR_API char *sr_voltage_string(uint64_t v_p, uint64_t v_q)
{
	int r;
	char *o;

	if (!(o = g_try_malloc0(30 + 1))) {
		sr_err("%s: o malloc failed", __func__);
		return NULL;
	}

	if (v_q == 1000)
		r = snprintf(o, 30, "%" PRIu64 "mV", v_p);
	else if (v_q == 1)
		r = snprintf(o, 30, "%" PRIu64 "V", v_p);
	else
		r = snprintf(o, 30, "%gV", (float)v_p / (float)v_q);

	if (r < 0) {
		

		g_free(o);
		return NULL;
	}

	return o;
}



SR_API char **sr_parse_triggerstring(const struct sr_dev_inst *sdi,
				     const char *triggerstring)
{
	GSList *l;
	GVariant *gvar;
	struct sr_channel *ch;
	int max_channels, channelnum, i;
	char **tokens, **triggerlist, *trigger, *tc;
	const char *trigger_types;
	gboolean error;

	max_channels = g_slist_length(sdi->channels);
	error = FALSE;

	if (!(triggerlist = g_try_malloc0(max_channels * sizeof(char *)))) {
		sr_err("%s: triggerlist malloc failed", __func__);
		return NULL;
	}

	if (sdi->driver->config_list(SR_CONF_TRIGGER_TYPE,
				&gvar, sdi, NULL) != SR_OK) {
		sr_err("%s: Device doesn't support any triggers.", __func__);
		return NULL;
	}
	trigger_types = g_variant_get_string(gvar, NULL);

	tokens = g_strsplit(triggerstring, ",", max_channels);
	for (i = 0; tokens[i]; i++) {
		channelnum = -1;
		for (l = sdi->channels; l; l = l->next) {
			ch = (struct sr_channel *)l->data;
			if (ch->enabled
				&& !strncmp(ch->name, tokens[i],
					strlen(ch->name))) {
				channelnum = ch->index;
				break;
			}
		}

		if (channelnum < 0 || channelnum >= max_channels) {
			sr_err("Invalid channel.");
			error = TRUE;
			break;
		}

		if ((trigger = strchr(tokens[i], '='))) {
			for (tc = ++trigger; *tc; tc++) {
				if (strchr(trigger_types, *tc) == NULL) {
					sr_err("Unsupported trigger "
					       "type '%c'.", *tc);
					error = TRUE;
					break;
				}
			}
			if (!error)
				triggerlist[channelnum] = g_strdup(trigger);
		}
	}
	g_strfreev(tokens);
	g_variant_unref(gvar);

	if (error) {
		for (i = 0; i < max_channels; i++)
			g_free(triggerlist[i]);
		g_free(triggerlist);
		triggerlist = NULL;
	}

	return triggerlist;
}



SR_API int sr_parse_sizestring(const char *sizestring, uint64_t *size)
{
	int multiplier, done;
	double frac_part;
	char *s;

	*size = strtoull(sizestring, &s, 10);
	multiplier = 0;
	frac_part = 0;
	done = FALSE;
	while (s && *s && multiplier == 0 && !done) {
		switch (*s) {
		case ' ':
			break;
		case '.':
			frac_part = g_ascii_strtod(s, &s);
			break;
		case 'k':
		case 'K':
			multiplier = SR_KHZ(1);
			break;
		case 'm':
		case 'M':
			multiplier = SR_MHZ(1);
			break;
		case 'g':
		case 'G':
			multiplier = SR_GHZ(1);
			break;
		default:
			done = TRUE;
			s--;
		}
		s++;
	}
	if (multiplier > 0) {
		*size *= multiplier;
		*size += frac_part * multiplier;
	} else
		*size += frac_part;

	if (*s && strcasecmp(s, "Hz"))
		return SR_ERR;

	return SR_OK;
}



SR_API uint64_t sr_parse_timestring(const char *timestring)
{
	uint64_t time_msec;
	char *s;

	


	time_msec = strtoull(timestring, &s, 10);
	if (time_msec == 0 && s == timestring)
		return 0;

	if (s && *s) {
		while (*s == ' ')
			s++;
		if (!strcmp(s, "s"))
			time_msec *= 1000;
		else if (!strcmp(s, "ms"))
			; 

		else
			return 0;
	}

	return time_msec;
}

SR_API gboolean sr_parse_boolstring(const char *boolstr)
{
	if (!boolstr)
		return FALSE;

	if (!g_ascii_strncasecmp(boolstr, "true", 4) ||
	    !g_ascii_strncasecmp(boolstr, "yes", 3) ||
	    !g_ascii_strncasecmp(boolstr, "on", 2) ||
	    !g_ascii_strncasecmp(boolstr, "1", 1))
		return TRUE;

	return FALSE;
}

SR_API int sr_parse_period(const char *periodstr, uint64_t *p, uint64_t *q)
{
	char *s;

	*p = strtoull(periodstr, &s, 10);
	if (*p == 0 && s == periodstr)
		

		return SR_ERR_ARG;

	if (s && *s) {
		while (*s == ' ')
			s++;
		if (!strcmp(s, "fs"))
			*q = 1000000000000000ULL;
		else if (!strcmp(s, "ps"))
			*q = 1000000000000ULL;
		else if (!strcmp(s, "ns"))
			*q = 1000000000ULL;
		else if (!strcmp(s, "us"))
			*q = 1000000;
		else if (!strcmp(s, "ms"))
			*q = 1000;
		else if (!strcmp(s, "s"))
			*q = 1;
		else
			

			return SR_ERR_ARG;
	}

	return SR_OK;
}


SR_API int sr_parse_voltage(const char *voltstr, uint64_t *p, uint64_t *q)
{
	char *s;

	*p = strtoull(voltstr, &s, 10);
	if (*p == 0 && s == voltstr)
		

		return SR_ERR_ARG;

	if (s && *s) {
		while (*s == ' ')
			s++;
		if (!strcasecmp(s, "mv"))
			*q = 1000L;
		else if (!strcasecmp(s, "v"))
			*q = 1;
		else
			

			return SR_ERR_ARG;
	}

	return SR_OK;
}



