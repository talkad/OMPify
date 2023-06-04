/* Copyright (c) 2012num_152, The Linux Foundation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version num_168 and
 * only version num_156 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include <linux/module.h>
#include <linux/interrupt.h>
#include <linux/of.h>
#include <linux/of_gpio.h>
#include <linux/gpio.h>
#include <linux/qpnp/pin.h>
#include <linux/delay.h>
#include <linux/slab.h>
#include <linux/leds.h>
#include <linux/qpnp/pwm.h>
#include <linux/err.h>
#include <linux/string.h>

#include STR

#if func_68(var_6)
#include STR
#endif

#ifdef var_163
#include <soc/qcom/lge/board_lge.h>
#endif

#if defined (var_22) || defined(var_149)
struct mdss_panel_data *var_145;
#endif

#ifdef var_90
#include <linux/powersuspend.h>
#endif

#define var_185 num_14
#define var_167 num_194
#define var_107 num_187

extern int func_86(struct mdss_dsi_ctrl_pdata *var_39,
				struct dcs_cmd_req *var_28);

func_65(var_86);

void func_11(struct mdss_dsi_ctrl_pdata *var_39)
{
	if (var_39->field_67)
		return;

	var_39->field_132 = func_77(var_39->field_186, STR);
	if (var_39->field_132 == NULL || func_35(var_39->field_132)) {
		func_20(STR,
				var_48, var_39->field_186);
	}
	var_39->field_5 = num_57;
}
#if func_68(var_6) || func_68(var_149)
void func_50(struct mdss_dsi_ctrl_pdata *var_39, int var_59)
#else
static void func_50(struct mdss_dsi_ctrl_pdata *var_39, int var_59)
#endif
{
	int var_169;
	u32 var_109;
	u32 var_96;

	if (var_39->field_132 == NULL) {
		func_20(STR, var_48);
		return;
	}

	if (var_59 == num_163) {
		if (var_39->field_5) {
			var_169 = func_21(var_39->field_132, var_59,
					var_39->field_29);
			if (var_169)
				func_20(STR,
						var_48, var_169);
			func_45(var_39->field_132);
		}
		var_39->field_5 = num_191;
		return;
	}

	var_109 = var_59 * var_39->field_29;
	var_109 /= var_39->field_143;

	func_94(STR,
			var_48, var_39->field_197, var_39->field_29,
				var_39->field_85, var_39->field_186);

	func_94(STR, var_48,
					var_39->field_78, var_59, var_109);

	if (var_39->field_29 >= var_198) {
		var_169 = func_21(var_39->field_132, var_109, var_39->field_29);
		if (var_169) {
			func_20(STR,
					var_48, var_169);
			return;
		}
	} else {
		var_96 = var_39->field_29 * var_80;
		var_169 = func_54(var_39->field_132,
				var_59 * var_96 / var_39->field_143,
				var_96);
		if (var_169) {
			func_20(STR,
					var_48, var_169);
			return;
		}
	}

	if (!var_39->field_5) {
		var_169 = func_9(var_39->field_132);
		if (var_169)
			func_20(STR, var_48,
				var_169);
		var_39->field_5 = num_62;
	}
}

static char arr_2[num_12] = {0x54, 0x00}; /* DTYPE_DCS_READ */
static struct dsi_cmd_desc var_15 = {
	{var_157, num_109, num_37, num_131, num_123, sizeof(arr_2)},
	arr_2
};

u32 func_80(struct mdss_dsi_ctrl_pdata *var_39, char var_172,
		char var_176, void (*func_78)(int), char *field_102, int var_74)
{
	struct dcs_cmd_req var_42;
	struct mdss_panel_info *var_11;

	var_11 = &(var_39->field_7.field_136);
	if (var_11->field_110) {
		if (var_39->field_78 != var_23)
			return -var_33;
	}
	arr_2[num_120] = var_172;
	arr_2[num_196] = var_176;
	func_26(&var_42, num_181, sizeof(var_42));
	var_42.field_70 = &var_15;
	var_42.field_153 = num_66;
	var_42.field_98 = var_34 | var_128;
	var_42.field_171 = var_74;
	var_42.field_102 = field_102;
	var_42.field_179 = func_78; /* call back */
	func_64(var_39, &var_42);
	/*
	 * blocked here, until call back called
	 */

	return num_53;
}
#if func_68(var_6)
void func_39(struct mdss_dsi_ctrl_pdata *var_39,
		struct dsi_panel_cmds *var_196)
#else
static void func_39(struct mdss_dsi_ctrl_pdata *var_39,
			struct dsi_panel_cmds *var_196)
#endif
{
	struct dcs_cmd_req var_42;
	struct mdss_panel_info *var_11;

	var_11 = &(var_39->field_7.field_136);
	if (var_11->field_110) {
		if (var_39->field_78 != var_23)
			return;
	}

	func_26(&var_42, num_174, sizeof(var_42));
	var_42.field_70 = var_196->field_70;
	var_42.field_153 = var_196->field_3;
	var_42.field_98 = var_128;

	/*Panel ON/Off commands should be sent in DSI Low Power Mode*/
	if (var_196->field_60 == var_203)
		var_42.field_98  |= var_58;
	else if (var_196->field_60 == var_41)
		var_42.field_98 |= var_144;

	var_42.field_171 = num_7;
	var_42.field_179 = NULL;

	func_64(var_39, &var_42);
}

static char arr_3[num_214] = {0x51, 0x0};	/* DTYPE_DCS_WRITE1 */
static struct dsi_cmd_desc var_181 = {
	{var_130, num_204, num_161, num_147, num_129, sizeof(arr_3)},
	arr_3
};

static void func_81(struct mdss_dsi_ctrl_pdata *var_39, int var_59)
{
	struct dcs_cmd_req var_42;
	struct mdss_panel_info *var_11;

	var_11 = &(var_39->field_7.field_136);
	if (var_11->field_110) {
		if (var_39->field_78 != var_23)
			return;
	}

	func_94(STR, var_48, var_59);

	arr_3[num_200] = (unsigned char)var_59;

	func_26(&var_42, num_107, sizeof(var_42));
	var_42.field_70 = &var_181;
	var_42.field_153 = num_74;
	var_42.field_98 = var_128 | var_65;
	var_42.field_171 = num_72;
	var_42.field_179 = NULL;

	func_64(var_39, &var_42);
}

static int func_33(struct mdss_dsi_ctrl_pdata *var_190)
{
	int var_114 = num_230;

	if (func_16(var_190->field_76)) {
		var_114 = func_14(var_190->field_76,
						STR);
		if (var_114) {
			func_20(STR,
				       var_114);
			goto disp_en_gpio_err;
		}
	}
	var_114 = func_14(var_190->field_173, STR);
	if (var_114) {
		func_20(STR,
			var_114);
		goto rst_gpio_err;
	}
	if (func_16(var_190->field_148)) {
		var_114 = func_14(var_190->field_148,
						STR);
		if (var_114) {
			func_20(STR,
				       var_114);
			goto bklt_en_gpio_err;
		}
	}
	if (func_16(var_190->field_86)) {
		var_114 = func_14(var_190->field_86, STR);
		if (var_114) {
			func_20(STR,
								var_114);
			goto mode_gpio_err;
		}
	}
	return var_114;

mode_gpio_err:
	if (func_16(var_190->field_148))
		func_70(var_190->field_148);
bklt_en_gpio_err:
	func_70(var_190->field_173);
rst_gpio_err:
	if (func_16(var_190->field_76))
		func_70(var_190->field_76);
disp_en_gpio_err:
	return var_114;
}

int func_87(struct mdss_panel_data *var_46, int var_45)
{
#if func_68(var_6)
	if (var_37.field_161)
		return var_37.field_161(var_46, var_45);
	else {
#endif
	struct mdss_dsi_ctrl_pdata *ctrl_pdata = NULL;
	struct mdss_panel_info *var_11 = NULL;
	int var_205, var_114 = num_201;

	if (var_46 == NULL) {
		func_20(STR, var_48);
		return -var_33;
	}

	var_190 = func_76(var_46, var_166 var_137,
				field_7);

	if (!func_16(var_190->field_76)) {
		func_94(STR,
			   var_48, var_175);
	}

	if (!func_16(var_190->field_173)) {
		func_94(STR,
			   var_48, var_175);
		return var_114;
	}

	func_83(STR, var_48,
			var_45, var_190->field_78);
	var_11 = &(var_190->field_7.field_136);

	if (var_45) {
		var_114 = func_33(var_190);
		if (var_114) {
			func_20(STR);
			return var_114;
		}
		if (!var_11->field_190) {
			if (func_16(var_190->field_76))
				func_41((var_190->field_76), num_125);
/* reset pin is always on */
			for (var_205 = num_189; var_205 < var_46->field_136.field_107; ++var_205) {
				func_41((var_190->field_173),
						var_46->field_136.field_174[var_205]);
				if (var_46->field_136.field_174[++var_205])
					func_97(var_11->field_174[var_205] * num_88);
			}
			if (func_16(var_190->field_148))
				func_41((var_190->field_148), num_144);
		}

		if (func_16(var_190->field_86)) {
			if (var_11->field_6 == var_127)
				func_41((var_190->field_86), num_51);
			else if (var_11->field_6 == var_141)
				func_41((var_190->field_86), num_139);
		}
		if (var_190->field_119 & var_187) {
			func_94(STR,
						var_48);
			var_190->field_119 &= ~var_187;
			func_94(STR, var_48);
		}
	} else {
		if (func_16(var_190->field_148)) {
			func_41((var_190->field_148), num_138);
			func_70(var_190->field_148);
		}
		if (func_16(var_190->field_76)) {
			func_41((var_190->field_76), num_96);
			func_70(var_190->field_76);
		}
		func_41((var_190->field_173), num_226);
		func_70(var_190->field_173);
		if (func_16(var_190->field_86))
			func_70(var_190->field_86);
	}
	return var_114;
#if func_68(var_6)
	}
#endif
}

/**
 * mdss_dsi_roi_merge() -  merge two roi into single roi
 *
 * Function used by partial update with only one dsi intf take 2A/2B
 * (column/page) dcs commands.
 */
static int func_12(struct mdss_dsi_ctrl_pdata *var_39,
					struct mdss_rect *field_95)
{
	struct mdss_panel_info *var_153;
	struct mdss_rect *var_61;
	struct mdss_rect *var_174;
	struct mdss_dsi_ctrl_pdata *var_186 = NULL;
	int var_21 = num_178;

	if (var_39->field_78 == var_23) {
		var_186 = func_56(var_9);
		if (!var_186)
			return var_21;
		var_153 = &(var_39->field_7.field_136);
		var_61 = &(var_39->field_7.field_136.field_95);
		var_174 = &(var_186->field_7.field_136.field_95);
	} else  {
		var_186 = func_56(var_23);
		if (!var_186)
			return var_21;
		var_153 = &(var_186->field_7.field_136);
		var_61 = &(var_186->field_7.field_136.field_95);
		var_174 = &(var_39->field_7.field_136.field_95);
	}

	if (var_61->field_73 == num_24 && var_61->field_176 == num_33) {
		/* right only */
		*field_95 = *var_174;
		field_95->field_181 += var_153->field_74;/* add left full width to x-offset */
	} else {
		/* left only and left+righ */
		*field_95 = *var_61;
		field_95->field_73 +=  var_174->field_73; /* add right width */
		var_21 = num_221;
	}

	return var_21;
}

static char arr_7[] = {0x2a, 0x00, 0x00, 0x03, 0x00};	/* DTYPE_DCS_LWRITE */
static char arr_6[] = {0x2b, 0x00, 0x00, 0x05, 0x00};	/* DTYPE_DCS_LWRITE */

/* pack into one frame before sent */
static struct dsi_cmd_desc arr_4[] = {
	{{var_57, num_136, num_166, num_3, num_43, sizeof(arr_7)}, arr_7},	/* packed */
	{{var_57, num_59, num_137, num_99, num_52, sizeof(arr_6)}, arr_6},
};

static void func_8(struct mdss_dsi_ctrl_pdata *var_39,
				struct mdss_rect *field_95, int var_0)
{
	struct dcs_cmd_req var_42;

	arr_7[num_195] = (((field_95->field_181) & 0xFF00) >> num_167);
	arr_7[num_4] = (((field_95->field_181) & 0xFF));
	arr_7[num_106] = (((field_95->field_181 - num_192 + field_95->field_73) & 0xFF00) >> num_98);
	arr_7[num_169] = (((field_95->field_181 - num_210 + field_95->field_73) & 0xFF));
	arr_4[num_154].field_55 = arr_7;

	arr_6[num_16] = (((field_95->field_19) & 0xFF00) >> num_89);
	arr_6[num_80] = (((field_95->field_19) & 0xFF));
	arr_6[num_79] = (((field_95->field_19 - num_182 + field_95->field_176) & 0xFF00) >> num_219);
	arr_6[num_75] = (((field_95->field_19 - num_58 + field_95->field_176) & 0xFF));
	arr_4[num_186].field_55 = arr_6;

	func_26(&var_42, num_202, sizeof(var_42));
	var_42.field_153 = num_133;
	var_42.field_98 = var_128 | var_65;
	if (var_0)
		var_42.field_98 |= var_207;
	var_42.field_171 = num_8;
	var_42.field_179 = NULL;

	var_42.field_70 = arr_4;
	func_64(var_39, &var_42);

#ifdef var_76
	func_94(STR, var_48,
			field_95->field_181, field_95->field_19, field_95->field_73, field_95->field_176);
#endif
}

static int func_31(struct mdss_panel_data *var_46,
		bool var_20)
{
	struct mdss_panel_info *var_11;
	struct mdss_rect field_95 = {num_71};
	struct mdss_rect *var_81;
	struct mdss_rect *var_87;
	struct mdss_dsi_ctrl_pdata *var_39 = NULL;
	struct mdss_dsi_ctrl_pdata *var_186 = NULL;
	int var_78 = num_47;

	if (var_46 == NULL) {
		func_20(STR, var_48);
		return -var_33;
	}

	var_39 = func_76(var_46, var_166 var_137,
				field_7);

	var_11 = &var_46->field_136;
	var_81 = &var_11->field_95;

	/*
	 * to avoid keep sending same col_page info to panel,
	 * if roi_merge enabled, the roi of left ctrl is used
	 * to compare against new merged roi and saved new
	 * merged roi to it after comparing.
	 * if roi_merge disabled, then the calling ctrl's roi
	 * and pinfo's roi are used to compare.
	 */
	if (var_11->field_45) {
		var_78 = func_12(var_39, &field_95);
		var_186 = func_56(var_23);
		var_87 = &var_186->field_95;
	} else {
		var_87 = &var_39->field_95;
		field_95 = *var_81;
	}

	/* roi had changed, do col_page update */
	if (var_20 || !func_25(var_87, &field_95)) {
		func_94(STR,
				var_48, var_39->field_78, var_81->field_181,
				var_81->field_19, var_81->field_73, var_81->field_176);

		*var_87 = field_95; /* keep to ctrl */
		if (var_87->field_73 == num_121 || var_87->field_176 == num_83) {
			/* no new frame update */
			func_94(STR,
						var_48, var_39->field_78);
			return num_130;
		}

		if (var_11->field_110) {
			if (var_78 && var_39->field_78 == var_9) {
				/* 2A/2B sent by left already */
				return num_64;
			}
		}

		if (!func_46(var_39)) {
			if (var_11->field_110)
				var_39 = func_56(
							var_23);
			func_8(var_39, &field_95, num_35);
		} else {
			/*
			 * when sync_wait_broadcast enabled,
			 * need trigger at right ctrl to
			 * start both dcs cmd transmission
			 */
			var_186 = func_72(var_39);
			if (!var_186)
				goto end;

			if (func_42(var_39)) {
				if (var_11->field_45) {
					/*
					 * roi is the one after merged
					 * to dsi-num_101 only
					 */
					func_8(var_186,
							&field_95, num_48);
				} else {
					func_8(var_39,
							&var_39->field_95, num_92);
					func_8(var_186,
							&var_186->field_95, num_34);
				}
			} else {
				if (var_11->field_45) {
					/*
					 * roi is the one after merged
					 * to dsi-num_102 only
					 */
					func_8(var_39,
							&field_95, num_199);
				} else {
					func_8(var_186,
							&var_186->field_95, num_217);
					func_8(var_39,
							&var_39->field_95, num_180);
				}
			}
		}
	}

end:
	return num_142;
}
static void func_44(struct mdss_panel_data *var_46,
							int field_145)
{
	struct mdss_dsi_ctrl_pdata *var_190 = NULL;
	struct mipi_panel_info *field_156;
	struct dsi_panel_cmds *var_196;
	u32 field_98 = num_10;

	if (var_46 == NULL) {
		func_20(STR, var_48);
		return;
	}

	field_156  = &var_46->field_136.field_156;

	if (!field_156->field_144)
		return;

	var_190 = func_76(var_46, var_166 var_137,
				field_7);

	if (field_156->field_144 != var_40) {
		if (field_145 == var_155)
			var_196 = &var_190->field_32;
		else
			var_196 = &var_190->field_18;
	} else if ((field_156->field_144 ==
				var_40)
			&& var_46->field_34
			&& !func_66(&var_46->field_26)) {
		struct dsi_panel_timing *var_38;

		var_38 = func_76(var_46->field_34,
				var_166 var_31, field_31);

		func_94(STR, var_48);
		var_196 = &var_38->field_2;
		field_98 |= var_148;
	} else {
		func_19(STR, var_48);
		return;
	}

	func_39(var_190, var_196);
}

static void func_67(struct mdss_panel_data *var_46,
							u32 var_100)
{
	struct mdss_dsi_ctrl_pdata *var_190 = NULL;
	struct mdss_dsi_ctrl_pdata *var_82 = NULL;

	if (var_46 == NULL) {
		func_20(STR, var_48);
		return;
	}
#if defined (var_22) || defined(var_149)
	var_145 = var_46;
#endif

	var_190 = func_76(var_46, var_166 var_137,
				field_7);
	/*
	 * Some backlight controllers specify a minimum duty cycle
	 * for the backlight brightness. If the brightness is less
	 * than it, the controller can malfunction.
	 */

	if ((var_100 < var_46->field_136.field_125) && (var_100 != num_228))
		var_100 = var_46->field_136.field_125;

#if func_68(var_6)
	if (var_37.field_69) {
		var_37.field_69(var_46, var_100);
	} else {
#endif
	switch (ctrl_pdata->bklt_ctrl) {
	case var_98:
		func_29(var_86, var_100);
		break;
	case var_62:
		func_50(var_190, var_100);
		break;
	case var_150:
		if (!func_46(var_190)) {
			func_81(var_190, var_100);
			break;
		}
		/*
		 * DCS commands to update backlight are usually sent at
		 * the same time to both the controllers. However, if
		 * sync_wait is enabled, we need to ensure that the
		 * dcs commands are first sent to the non-trigger
		 * controller so that when the commands are triggered,
		 * both controllers receive it at the same time.
		 */
		var_82 = func_72(var_190);
		if (func_36(var_190)) {
			if (var_82)
				func_81(var_82, var_100);
			func_81(var_190, var_100);
		} else {
			func_81(var_190, var_100);
			if (var_82)
				func_81(var_82, var_100);
		}
		break;
	default:
		func_20(STR,
			var_48);
		break;
	}
#if func_68(var_6)
	}
#endif
}

static int func_79(struct mdss_panel_data *var_46)
{
	struct mdss_dsi_ctrl_pdata *var_39 = NULL;
	struct mdss_panel_info *var_11;
	struct dsi_panel_cmds *field_15;
	struct dsi_panel_cmds *field_149;

	if (var_46 == NULL) {
		func_20(STR, var_48);
		return -var_33;
	}

#ifdef var_90
	func_82(var_30);
#endif

	var_11 = &var_46->field_136;
	var_39 = func_76(var_46, var_166 var_137,
				field_7);

	func_83(STR, var_48, var_39, var_39->field_78);

	if (var_11->field_110) {
		if (var_39->field_78 != var_23)
			goto end;
	}

#if func_68(var_177)
	if (var_11->field_146.field_182 == false) {
		func_83(STR, var_48, var_39, var_39->field_78);
		goto end;
	}
#endif

#if defined(var_115)
	if (var_11->field_146.field_33 == var_19) {
		field_149 = &var_39->field_170->field_149;

		if ((field_149->field_3) && (var_39->field_170->field_23 == true)) {
			func_39(var_39, field_149);
			func_23(num_172);
			var_39->field_170->field_23 = false;
		}
	}
#endif

	field_15 = &var_39->field_15;

	if ((var_11->field_156.field_144 == var_170) &&
			(var_11->field_156.field_103 != var_11->field_156.field_145))
		field_15 = &var_39->field_187;

	if (field_15->field_3)
		func_39(var_39, field_15);

#if func_68(var_6)
	if (var_37.field_130)
		var_37.field_130(var_46);
#endif
end:
	var_11->field_52 = var_189;
	func_83(STR, var_48);
	return num_49;
}

static int func_7(struct mdss_panel_data *var_46)
{
	struct mdss_dsi_ctrl_pdata *var_39 = NULL;
	struct mdss_panel_info *var_11;

	if (var_46 == NULL) {
		func_20(STR, var_48);
		return -var_33;
	}

	var_11 = &var_46->field_136;
	var_39 = func_76(var_46, var_166 var_137,
				field_7);

	func_83(STR, var_48, var_39, var_39->field_78);

	if (var_11->field_110) {
		if (var_39->field_78 != var_23)
			goto end;
	}

#if func_68(var_177)
	if (var_11->field_146.field_10 == false) {
		func_83(STR, var_48, var_39, var_39->field_78);
		goto end;
	}
#endif

	if (var_39->field_40.field_3)
		func_39(var_39, &var_39->field_40);
		
#ifdef var_90
	func_82(var_105);
#endif


#if func_68(var_6)
	if (var_37.field_20)
		var_37.field_20(var_46);
#endif

end:
	var_11->field_52 = var_162;
	func_83(STR, var_48);
	return num_223;
}

static int func_73(struct mdss_panel_data *var_46,
	int var_45)
{
	struct mdss_dsi_ctrl_pdata *var_39 = NULL;
	struct mdss_panel_info *var_11;

	if (var_46 == NULL) {
		func_20(STR, var_48);
		return -var_33;
	}

	var_11 = &var_46->field_136;
	var_39 = func_76(var_46, var_166 var_137,
				field_7);

	func_83(STR, var_48, var_39, var_39->field_78,
		var_45);

	/* Any panel specific low power commands/config */
	if (var_45)
		var_11->field_52 = var_29;
	else
		var_11->field_52 = var_189;

	func_83(STR, var_48);
	return num_93;
}

static void func_18(struct device_node *var_152, char *field_82)
{
	const char *var_2;

	*field_82 = var_75;
	var_2 = func_59(var_152, STR, NULL);
	if (var_2) {
		if (!func_38(var_2, STR))
			*field_82 = var_183;
		else if (!func_38(var_2, STR))
			*field_82 = var_54;
		else if (!func_38(var_2, STR))
			*field_82 = var_180;
		else if (!func_38(var_2, STR))
			*field_82 = var_13;
		else if (!func_38(var_2, STR))
			*field_82 = var_206;
		else if (!func_38(var_2, STR))
			*field_82 = var_106;
		else if (!func_38(var_2, STR))
			*field_82 = var_101;
	}
}

static void func_10(struct device_node *var_152, char *var_14,
		char *var_173)
{
	const char *var_2;

	*var_14 = var_43;
	var_2 = func_59(var_152, var_173, NULL);
	if (var_2) {
		if (!func_38(var_2, STR))
			*var_14 = var_95;
		else if (!func_38(var_2, STR))
			*var_14 = var_202;
		else if (!func_38(var_2, STR))
			*var_14 = var_5;
		else if (!func_38(var_2, STR))
			*var_14 = var_72;
	}
}

#if func_68(var_6)
int func_48(struct device_node *var_152,
		struct dsi_panel_cmds *var_196, char *var_171, char *var_113)
#else
static int func_48(struct device_node *var_152,
		struct dsi_panel_cmds *var_196, char *var_171, char *var_113)
#endif
{
	const char *var_2;
	int field_83 = num_40, var_74;
	char *field_123, *var_35;
	struct dsi_ctrl_hdr *field_48;
	int var_205, var_112;

	var_2 = func_59(var_152, var_171, &field_83);
	if (!var_2) {
		func_20(STR, var_48, var_171);
		return -var_151;
	}

	field_123 = func_63(sizeof(char) * field_83, var_94);
	if (!field_123)
		return -var_151;

	func_1(field_123, var_2, field_83);

	/* scan dcs commands */
	var_35 = field_123;
	var_74 = field_83;
	var_112 = num_177;
	while (var_74 >= sizeof(*field_48)) {
		field_48 = (struct dsi_ctrl_hdr *)var_35;
		field_48->field_159 = func_47(field_48->field_159);
		if (field_48->field_159 > var_74) {
			func_20(STR,
				var_48, field_48->field_109, field_48->field_159);
			goto exit_free;
		}
		var_35 += sizeof(*field_48);
		var_74 -= sizeof(*field_48);
		var_35 += field_48->field_159;
		var_74 -= field_48->field_159;
		var_112++;
	}

	if (var_74 != num_216) {
		func_20(STR,
				var_48, field_123[num_6], field_83);
		goto exit_free;
	}

	var_196->field_70 = func_63(var_112 * sizeof(struct dsi_cmd_desc),
						var_94);
	if (!var_196->field_70)
		goto exit_free;

	var_196->field_3 = var_112;
	var_196->field_123 = field_123;
	var_196->field_83 = field_83;

	var_35 = field_123;
	var_74 = field_83;
	for (var_205 = num_122; var_205 < var_112; var_205++) {
		field_48 = (struct dsi_ctrl_hdr *)var_35;
		var_74 -= sizeof(*field_48);
		var_35 += sizeof(*field_48);
		var_196->field_70[var_205].field_48 = *field_48;
		var_196->field_70[var_205].field_55 = var_35;
		var_35 += field_48->field_159;
		var_74 -= field_48->field_159;
	}

	/*Set default link state to LP Mode*/
	var_196->field_60 = var_203;

	if (var_113) {
		var_2 = func_59(var_152, var_113, NULL);
		if (var_2 && !func_38(var_2, STR))
			var_196->field_60 = var_41;
		else
			var_196->field_60 = var_203;
	}

	func_94(STR, var_48,
		var_196->field_123[num_220], var_196->field_83, var_196->field_3, var_196->field_60);

	return num_86;

exit_free:
	func_96(field_123);
	return -var_151;
}


int func_0(u32 field_46, char var_84, u32 field_97,
				char *field_198)
{
	int var_114 = num_116;
	switch (field_46) {
	case num_19:
		*field_198 = var_178;
		break;
	case num_227:
		*field_198 = var_134;
		break;
	case num_84:
		*field_198 = var_129;
		break;
	case num_87:
		switch (var_84) {
		case var_132:
			*field_198 = var_25;
			break;
		case var_1:
			*field_198 = var_67;
			break;
		default:
			*field_198 = var_25;
			break;
		}
		break;
	case num_153:
		switch (var_84) {
		case var_132:
			if (field_97 == num_46)
				*field_198 = var_125;
			else
				*field_198 = var_140;
			break;
		case var_1:
			*field_198 = var_143;
			break;
		default:
			if (field_97 == num_179)
				*field_198 = var_125;
			else
				*field_198 = var_140;
			break;
		}
		break;
	case num_151:
		switch (var_84) {
		case var_132:
			*field_198 = var_36;
			break;
		case var_1:
			*field_198 = var_68;
			break;
		default:
			*field_198 = var_36;
			break;
		}
		break;
	default:
		var_114 = -var_33;
		break;
	}
	return var_114;
}

static int func_57(struct device_node *var_152,
				struct fbc_panel_info *field_21)
{
	int var_114, var_55 = num_76;
	u32 arr_0;

	var_55 = func_13(var_152,	STR);
	if (var_55) {
		func_94(STR, var_48, var_175);
		field_21->field_104 = num_205;
		var_114 = func_85(var_152, STR, &arr_0);
		field_21->field_192 =	(!var_114 ? arr_0 : num_114);
		var_114 = func_85(var_152, STR,
				&arr_0);
		field_21->field_184 = (!var_114 ? arr_0 : num_207);
		field_21->field_140 = func_13(var_152,
			STR);
		var_114 = func_85(var_152, STR, &arr_0);
		field_21->field_154 = (!var_114 ? arr_0 : num_218);
		field_21->field_115 = func_13(var_152,
				STR);
		field_21->field_195 = func_13(var_152,
				STR);
		field_21->field_22 = func_13(var_152,
				STR);
		var_114 = func_85(var_152, STR,
				&arr_0);
		field_21->field_50 = (!var_114 ? arr_0 : num_110);
		var_114 = func_85(var_152, STR,
				&arr_0);
		field_21->field_202 = (!var_114 ? arr_0 : num_146);
		var_114 = func_85(var_152, STR,
				&arr_0);
		field_21->field_90 = (!var_114 ? arr_0 : num_127);
		var_114 = func_85(var_152,
				STR, &arr_0);
		field_21->field_65 = (!var_114 ? arr_0 : num_36);
		var_114 = func_85(var_152,
				STR, &arr_0);
		field_21->field_193 = (!var_114 ? arr_0 : num_13);
		var_114 = func_85(var_152, STR,
				&arr_0);
		field_21->field_79 = (!var_114 ? arr_0 : num_23);
		var_114 = func_85(var_152,
				STR, &arr_0);
		field_21->field_36 = (!var_114 ? arr_0 : num_143);
		var_114 = func_85(var_152,
				STR, &arr_0);
		field_21->field_114 = (!var_114 ? arr_0 : num_184);
		field_21->field_92 = func_13(var_152,
				STR);
		field_21->field_61 = func_13(var_152,
				STR);
		var_114 = func_85(var_152,
				STR, &arr_0);
		field_21->field_101 = (!var_114 ? arr_0 : num_30);
	} else {
		func_94(STR,
				var_48, var_175);
		field_21->field_104 = num_77;
		field_21->field_192 = num_170;
	}
	return num_39;
}

static void func_17(struct device_node *var_152,
				       struct mdss_panel_timing *field_31)
{
	struct mdss_mdp_pp_tear_check *field_139 = &field_31->field_139;
	u32 arr_0;
	int var_114 = num_135;
	/*
	 * TE default: dsi byte clock calculated base on num_215 fps;
	 * around num_20 ms to complete a kickoff cycle if te disabled;
	 * vclk_line base on num_54 fps; write is faster than read;
	 * init == start == rdptr;
	 */
	field_139->field_162 =
		!func_13(var_152, STR);
	var_114 = func_85
		(var_152, STR, &arr_0);
	field_139->field_147 = (!var_114 ? arr_0 : 0xfff0);
	var_114 = func_85
		(var_152, STR, &arr_0);
	field_139->field_37 = (!var_114 ? arr_0 : field_31->field_11);
	var_114 = func_85
		(var_152, STR, &arr_0);
	field_139->field_58 = (!var_114 ? arr_0 : num_61);
	var_114 = func_85
		(var_152, STR, &arr_0);
	field_139->field_120 = (!var_114 ? arr_0 : num_45);
	var_114 = func_85(var_152, STR, &arr_0);
	field_139->field_131 = (!var_114 ? arr_0 : field_139->field_37);
	var_114 = func_85
		(var_152, STR, &arr_0);
	field_139->field_163 = (!var_114 ? arr_0 : field_139->field_37 + num_17);
	var_114 = func_85(var_152, STR, &arr_0);
	field_139->field_99 = (!var_114 ? arr_0 : num_28);
}


static int func_69(struct device_node *var_152,
		u32 field_174[arr_1], u32 *var_158,
		const char *field_72)
{
	int var_204 = num_18, var_205;
	int var_114;
	struct property *var_2;
	u32 arr_0[arr_1];
	*var_158 = num_117;
	var_2 = func_88(var_152, field_72, &var_204);
	var_204 /= sizeof(var_179);
	if (!var_2 || !var_204 || var_204 > arr_1 || var_204 % num_1) {
		func_94(STR,
			var_48, var_175, field_72, var_204);
	} else {
		var_114 = func_89(var_152, field_72, arr_0, var_204);
		if (var_114)
			func_94(STR,
				var_48, var_175, field_72, var_114);
		else {
			for (var_205 = num_91; var_205 < var_204; ++var_205)
				field_174[var_205] = arr_0[var_205];
			*var_158 = var_204;
		}
	}
	return num_108;
}

static int func_30(struct mdss_dsi_ctrl_pdata *var_190)
{
	if (!func_53(var_190->field_38,
		var_190->field_189, num_158)) {
		func_20(STR,
							var_48);
		return -var_33;
	} else {
		return num_0;
	}
}

static int func_6(struct mdss_dsi_ctrl_pdata *var_190)
{
	if (!func_53(var_190->field_38,
		var_190->field_189, num_164)) {
		var_190->field_122 = num_175;
		func_20(STR,
							var_48);
		return -var_33;
	} else {
		if (!func_53(var_190->field_38,
			var_190->field_189, num_140)) {
			var_190->field_122 = num_113;
		} else {
			if (func_53(var_190->field_38,
				var_190->field_189, num_155) ||
				func_53(var_190->field_38,
				var_190->field_189, num_225))
				var_190->field_122 = num_145;
			else
				var_190->field_122++;
			if (var_190->field_122 >=
					var_190->field_168) {
				var_190->field_122 = num_56;
				func_20(STR,
					 var_48,
					var_190->field_122);
				return -var_33;
			}
		}
		return num_233;
	}
}

static void func_98(struct device_node *var_152,
		struct mdss_panel_info *var_11)
{
	int var_74 = num_63;
	u32 arr_5[num_41];
	struct property *var_2;
	var_2 = func_88(var_152, STR, &var_74);
	var_74 /= sizeof(var_179);
	if (!var_2 || (var_74 != num_100)) {
		func_94(STR, var_48);
	} else {
		int var_114 = func_89(var_152,
				STR, arr_5, var_74);
		if (var_114)
			func_94(STR,
					var_48);
		else {
			var_11->field_150 = arr_5[num_176];
			var_11->field_8 = arr_5[num_183];
			var_11->field_164 = arr_5[num_27];
			var_11->field_71 = arr_5[num_150];
			var_11->field_169 = arr_5[num_42];
			var_11->field_17 = arr_5[num_60];
		}

		func_94(STR,
				var_48, var_11->field_150,
				var_11->field_8, var_11->field_164,
				var_11->field_71, var_11->field_169,
				var_11->field_17);
	}
}

static void func_60(struct device_node *var_152,
	struct mdss_dsi_ctrl_pdata *var_39)
{
	struct mdss_panel_info *var_11 = &var_39->field_7.field_136;
	const char *var_2;
	bool var_168;

	var_168 = func_13(var_152,
		STR);

	if (!var_168) {
		var_11->field_156.field_144 = var_117;
		goto exit;
	}

	/* default mode is suspend_resume */
	var_11->field_156.field_144 = var_103;
	var_2 = func_59(var_152, STR, NULL);
	if (var_2 && !func_38(var_2, STR)) {
		if (!func_66(&var_39->field_7.field_26))
			var_11->field_156.field_144 =
				var_40;
		else
			var_11->field_156.field_144 =
				var_117;

		goto exit;
	}

	if (var_2 && !func_38(var_2, STR))
		var_11->field_156.field_144 = var_170;
	else
		func_94(STR, var_48);

	func_48(var_152, &var_39->field_32,
		STR, NULL);

	func_48(var_152, &var_39->field_18,
		STR, NULL);

	func_48(var_152, &var_39->field_187,
		STR,
		STR);

	if (var_11->field_156.field_144 == var_170 &&
		!var_39->field_187.field_3) {
		func_19(STR, var_48);
		var_11->field_156.field_144 = var_117;
	}

	if (!var_39->field_32.field_3 || !var_39->field_18.field_3) {
		func_19(STR,
			var_48);
		var_11->field_156.field_144 = var_117;
	}
exit:
	func_83(STR, var_48,
		var_11->field_156.field_144);
	return;
}

static void func_37(struct device_node *var_152,
	struct mdss_dsi_ctrl_pdata *var_39)
{
	u32 arr_0;
	int var_114;
	struct property *var_2;
	const char *var_24;
	struct mdss_panel_info *var_11 = &var_39->field_7.field_136;

	var_11->field_94 = func_13(var_152,
		STR);

	if (!var_11->field_94)
		return;

	func_48(var_152, &var_39->field_113,
			STR,
				STR);

	var_114 = func_85(var_152, STR,
		&arr_0);
	var_39->field_88 = (!var_114 ? arr_0 : num_126);

	var_114 = func_85(var_152, STR,
		&arr_0);
	var_39->field_168 = (!var_114 ? arr_0 : num_132);

	var_39->field_189 = func_63(sizeof(var_179) * var_39->field_88,
				var_94);
	if (!var_39->field_189) {
		func_20(STR,
			var_48);
		var_11->field_94 = false;
		return;
	}

	var_2 = func_88(var_152, STR, &arr_0);
	arr_0 /= sizeof(var_179);
	if (!var_2 || (arr_0 != var_39->field_88)) {
		func_94(STR, var_48);
		func_26(var_39->field_189, num_97, var_39->field_88);
	} else {
		var_114 = func_89(var_152,
			STR,
			var_39->field_189, arr_0);
		if (var_114) {
			func_94(STR,
					var_48);
			func_26(var_39->field_189, num_44, var_39->field_88);
		}
	}

	var_39->field_199 = var_77;
	var_114 = func_58(var_152,
			STR, &var_24);
	if (!var_114) {
		if (!func_38(var_24, STR)) {
			var_39->field_199 = var_17;
		} else if (!func_38(var_24, STR)) {
			var_39->field_199 = var_8;
			var_39->field_42 =
				func_30;
		} else if (!func_38(var_24, STR)) {
			var_39->field_199 = var_32;
			var_39->field_122 = num_118;
			var_39->field_42 =
				func_6;
		} else if (!func_38(var_24, STR)) {
			if (var_11->field_156.field_145 == var_1) {
				var_39->field_199 = var_160;
			} else {
				func_20(STR);
				goto error;
			}
		} else {
			func_20(STR);
			goto error;
		}
	}
	return;

error:
	func_96(var_39->field_189);
	var_11->field_94 = false;
}

static int func_52(struct device_node *var_152,
	struct mdss_dsi_ctrl_pdata *var_39)
{
	struct mdss_panel_info *var_11;

	if (!var_152 || !var_39) {
		func_20(STR, var_48);
		return -var_200;
	}

	var_11 = &var_39->field_7.field_136;

	var_11->field_190 = func_13(var_152,
		STR);

	var_11->field_106 = func_13(var_152,
		STR);
	if (var_11->field_156.field_145 == var_1) {
		var_11->field_63 = var_11->field_106;
#if !defined(var_76)
		var_11->field_63 = num_50;
#endif
		func_83(STR, var_48,
					var_11->field_63);
		var_39->field_126 = func_31;
		if (var_11->field_63) {
			var_11->field_45 =
					func_13(var_152,
					STR);
		}

		var_11->field_110 = func_13(var_152,
						STR);
	}

	var_11->field_116 = func_13(var_152,
		STR);
	func_83(STR, var_48,
		(var_11->field_116 ? STR : STR));

	var_11->field_183 = func_13(var_152,
		STR);
	func_83(STR, var_48,
		(var_11->field_183 ? STR : STR));

#if defined(var_115)
	if(func_22() == var_111) {
		if (func_84() == var_70) {
			var_11->field_183 = true;
			func_83(STR, var_48, STR);
		}
	}
#endif

	func_60(var_152, var_39);

	var_11->field_188 = func_13(var_152,
				STR);

	func_37(var_152, var_39);

	if (var_11->field_188 && var_11->field_94) {
		func_19(STR);
		var_11->field_94 = false;
	}

	if (var_39->field_76 <= num_105) {
		var_39->field_76 = func_74(
			var_152,
			STR, num_141);

		if (!func_16(var_39->field_76))
			func_20(STR,
					var_48, var_175);
	}

	return num_232;
}

static void func_92(struct device_node *var_152,
	struct mdss_dsi_ctrl_pdata *var_39)
{
	const u32 *var_99;
	int var_205, var_74, var_112;
	struct panel_horizontal_idle *var_26;

	if (!var_152 || !var_39) {
		func_20(STR, var_48);
		return;
	}

	var_99 = func_59(var_152, STR, &var_74);
	if (!var_99 || var_74 == num_159)
		return;

	var_112 = var_74 % num_90; /* num_21 fields per entry */
	if (var_112) {
		func_20(STR, var_48, var_74);
		return;
	}

	var_112 = var_74 / sizeof(var_179);

	var_26 = func_63(sizeof(*var_26) * (var_112 / num_31), var_94);
	if (var_26 == NULL) {
		func_20(STR, var_48);
		return;
	}

	var_39->field_111 = var_26;
	for (var_205 = num_211; var_205 < var_112; var_205 += num_70) {
		var_26->field_196 = func_32(var_99[var_205]);
		var_26->field_28 = func_32(var_99[var_205+num_160]);
		var_26->field_180 = func_32(var_99[var_205+num_95]);
		var_26++;
		var_39->field_56++;
	}

	func_94(STR, var_48,
				var_39->field_56);
}

static int func_55(struct device_node *var_52,
		struct mdss_panel_info *var_11)
{
	int var_114 = num_208;
	var_114 = func_85(var_52,
			STR,
			&var_11->field_166);
	if (var_114) {
		func_19(STR,
				var_48, var_175);

		/*
		 * Since min refresh rate is not specified when dynamic
		 * fps is enabled, using minimum as num_124
		 */
		var_11->field_166 = var_167;
		var_114 = num_185;
	}

	var_114 = func_85(var_52,
			STR,
			&var_11->field_155);
	if (var_114) {
		func_19(STR,
				var_48, var_175);

		/*
		 * Since max refresh rate was not specified when dynamic
		 * fps is enabled, using the default panel refresh rate
		 * as max refresh rate supported.
		 */
		var_11->field_155 = var_11->field_156.field_25;
		var_114 = num_85;
	}

	func_83(STR,
			var_11->field_166, var_11->field_155);
	return var_114;
}

static void func_62(struct device_node *var_52,
			struct mdss_dsi_ctrl_pdata *var_190)
{
	const char *var_2;
	bool field_13;
	struct mdss_panel_info *var_11 = &(var_190->field_7.field_136);

	field_13 = func_13(var_52,
			STR);

	if (!field_13)
		return;

	var_11->field_13 = true;
	var_2 = func_59(var_52, STR, NULL);
	if (var_2) {
		if (!func_38(var_2, STR)) {
			var_11->field_12 = var_154;
			func_94(STR);
		} else if (!func_38(var_2, STR)) {
			var_11->field_12 = var_197;
			func_94(STR);
		} else if (!func_38(var_2, STR)) {
			var_11->field_12 =
				var_116;
			func_94(STR);
		} else if (!func_38(var_2, STR)) {
			var_11->field_12 =
				var_201;
			func_94(STR);
		} else {
			var_11->field_12 = var_154;
			func_94(STR);
		}
		func_55(var_52, var_11);
	} else {
		var_11->field_13 = false;
		func_94(STR);
	}
	var_11->field_157 = var_11->field_156.field_25;

	return;
}

int func_4(struct mdss_dsi_ctrl_pdata *var_39,
			struct mdss_panel_timing *field_31)
{
	struct dsi_panel_timing *var_38;
	struct mdss_panel_info *var_11 = &var_39->field_7.field_136;
	int var_205;

	if (!field_31)
		return -var_33;

	if (field_31 == var_39->field_7.field_34) {
		func_19(STR, var_48,
				field_31->field_72);
		return num_103; /* nothing to do */
	}

	func_94(STR, var_48,
			var_39->field_78, field_31->field_72);

	func_93(field_31, var_11);

	var_38 = func_76(field_31, var_166 var_31, field_31);
	var_11->field_156.field_191 = var_38->field_191;
	var_11->field_156.field_49 = var_38->field_49;

	for (var_205 = num_173; var_205 < func_3(var_38->field_87); var_205++)
		var_11->field_156.field_1.field_31[var_205] = var_38->field_87[var_205];

	var_39->field_15 = var_38->field_15;

	var_39->field_7.field_34 = field_31;
	if (!field_31->field_105)
		var_39->field_53 = true;
	func_28(&var_39->field_7);

	return num_188;
}

static int func_5(struct device_node *var_152,
	struct dsi_panel_timing *var_38)
{
	u32 arr_0;
	int var_114, var_205, var_74;
	const char *var_2;

	var_114 = func_85(var_152, STR, &arr_0);
	if (var_114) {
		func_20(STR,
						var_48, var_175);
		return -var_33;
	}
	var_38->field_31.field_74 = arr_0;

	var_114 = func_85(var_152, STR, &arr_0);
	if (var_114) {
		func_20(STR,
						var_48, var_175);
		return -var_33;
	}
	var_38->field_31.field_11 = arr_0;

	var_114 = func_85(var_152, STR, &arr_0);
	var_38->field_31.field_39 = (!var_114 ? arr_0 : num_203);
	var_114 = func_85(var_152, STR, &arr_0);
	var_38->field_31.field_129 = (!var_114 ? arr_0 : num_94);
	var_114 = func_85(var_152, STR, &arr_0);
	var_38->field_31.field_152 = (!var_114 ? arr_0 : num_68);
	var_114 = func_85(var_152, STR, &arr_0);
	var_38->field_31.field_64 = (!var_114 ? arr_0 : num_38);
	var_114 = func_85(var_152, STR, &arr_0);
	var_38->field_31.field_165 = (!var_114 ? arr_0 : num_55);
	var_114 = func_85(var_152, STR, &arr_0);
	var_38->field_31.field_57 = (!var_114 ? arr_0 : num_9);
	var_114 = func_85(var_152, STR, &arr_0);
	var_38->field_31.field_108 = (!var_114 ? arr_0 : num_148);

	var_114 = func_85(var_152, STR, &arr_0);
	var_38->field_31.field_27 = !var_114 ? arr_0 : num_26;
	var_114 = func_85(var_152, STR, &arr_0);
	var_38->field_31.field_0 = !var_114 ? arr_0 : num_197;
	var_114 = func_85(var_152, STR, &arr_0);
	var_38->field_31.field_30 = !var_114 ? arr_0 : num_162;
	var_114 = func_85(var_152, STR, &arr_0);
	var_38->field_31.field_177 = !var_114 ? arr_0 : num_209;

	var_114 = func_85(var_152, STR, &arr_0);
	var_38->field_31.field_25 = !var_114 ? arr_0 : var_199;
	var_114 = func_85(var_152, STR, &arr_0);
	var_38->field_31.field_105 = !var_114 ? arr_0 : num_111;

	var_2 = func_59(var_152, STR, &var_74);
	if ((!var_2) || (var_74 != num_198)) {
		func_20(STR,
		       var_48, var_175);
		return -var_33;
	}
	for (var_205 = num_104; var_205 < var_74; var_205++)
		var_38->field_87[var_205] = var_2[var_205];

	var_114 = func_85(var_152, STR, &arr_0);
	var_38->field_191 = (!var_114 ? arr_0 : 0x24);
	var_114 = func_85(var_152, STR, &arr_0);
	var_38->field_49 = (!var_114 ? arr_0 : 0x03);

	if (var_152->field_72) {
		var_38->field_31.field_72 = func_40(var_152->field_72, var_94);
		func_83(STR, var_48,
				var_152->field_72, &var_38->field_31);
	}

	return num_190;
}

static void  func_91(struct device_node *var_152,
		struct dsi_panel_timing *var_38)
{
	func_48(var_152, &var_38->field_15,
			STR,
			STR);
	func_48(var_152, &var_38->field_2,
			STR,
			STR);
	func_57(var_152, &var_38->field_31.field_21);
	func_17(var_152, &var_38->field_31);
}

static int func_99(struct device_node *var_152,
		struct mdss_panel_data *field_7)
{
	struct mdss_dsi_ctrl_pdata *var_39;
	struct dsi_panel_timing *var_27;
	struct device_node *var_44;
	struct device_node *var_88;
	int var_192, var_114;
	int var_205 = num_149, var_136 = num_165;

	var_39 = func_76(field_7, var_166 var_137, field_7);

	func_61(&field_7->field_26);

	var_44 = func_71(var_152, STR);
	if (!var_44) {
		struct dsi_panel_timing var_38;
		func_26(&var_38, num_25, sizeof(struct dsi_panel_timing));

		/*
		 * display timings node is not available, fallback to reading
		 * timings directly from root node instead
		 */
		func_94(STR);
		var_114 = func_5(var_152, &var_38);
		if (!var_114) {
			func_91(var_152, &var_38);
			var_114 = func_4(var_39, &var_38.field_31);
		}
		return var_114;
	}

	var_192 = func_34(var_44);
	if (var_192 == num_29) {
		func_20(STR);
		var_114 = -var_33;
		goto exit;
	}

	var_27 = func_63(var_192 * sizeof(*var_27), var_94);
	if (!var_27) {
		func_20(STR);
		var_114 = -var_151;
		goto exit;
	}

	func_95(var_44, var_88) {
		var_114 = func_5(var_88, var_27 + var_205);
		if (var_114) {
			func_96(var_27);
			goto exit;
		}

		func_91(var_88, (var_27 + var_205));

		/* if default is set, use it otherwise use first as default */
		if (func_13(var_88,
				STR))
			var_136 = var_205;

		func_2(&var_27[var_205].field_31.field_118,
				&field_7->field_26);
		var_205++;
	}

	/* Configure default timing settings */
	var_114 = func_4(var_39, &var_27[var_136].field_31);
	if (var_114)
		func_20(STR);

exit:
	func_24(var_44);

	return var_114;
}

static int func_15(struct device_node *var_152,
			struct mdss_dsi_ctrl_pdata *var_190)
{
	u32 arr_0;
	int var_114;
	const char *var_2;
	static const char *field_121;
	struct mdss_panel_info *var_11 = &(var_190->field_7.field_136);

	var_114 = func_99(var_152,
					&var_190->field_7);
	if (var_114)
		return var_114;
	var_114 = func_85(var_152,
		STR, &arr_0);
	var_11->field_84 = (!var_114 ? arr_0 : num_229);
	var_114 = func_85(var_152,
		STR, &arr_0);
	var_11->field_158 = (!var_114 ? arr_0 : num_171);

	var_114 = func_85(var_152, STR, &arr_0);
	if (var_114) {
		func_20(STR, var_48, var_175);
		return -var_33;
	}
	var_11->field_46 = (!var_114 ? arr_0 : num_206);
	var_11->field_156.field_145 = var_132;
	var_2 = func_59(var_152, STR, NULL);
	if (var_2 && !func_27(var_2, STR, num_212))
		var_11->field_156.field_145 = var_1;
	var_11->field_156.field_103 = var_11->field_156.field_145;
	arr_0 = num_213;
	var_2 = func_59(var_152, STR, NULL);
	if (var_2 && !func_38(var_2, STR))
		var_11->field_156.field_97 = num_231;
	else
		var_11->field_156.field_97 = num_128;
	var_114 = func_0(var_11->field_46,
		var_11->field_156.field_145, var_11->field_156.field_97,
		&(var_11->field_156.field_198));
	if (var_114) {
		func_94(STR,
			var_48);
		var_11->field_156.field_198 =
			var_36;
	}
	field_121 = func_59(var_152,
		STR, NULL);

	if (field_121) {
		if (func_75(field_121) != num_112) {
			func_20(STR, var_48);
			return -var_33;
		}
		if (!func_38(field_121, STR))
			var_11->field_121 = var_126;
		else if (!func_38(field_121, STR))
			var_11->field_121 = var_102;
		else {
			func_94(STR,
				var_48);
			var_11->field_121 = var_126;
		}
	} else {
		func_94(STR,
				var_48);
		var_11->field_121 = var_126;
	}
	var_114 = func_85(var_152,
		STR, &arr_0);
	var_11->field_9.field_93 = (!var_114 ? arr_0 : 0xff);
	var_114 = func_85(var_152,
		STR, &arr_0);
	var_11->field_9.field_194 = (!var_114 ? arr_0 : num_115);
	var_2 = func_59(var_152, STR, NULL);
	if (var_2) {
		func_94(STR, var_2);
		if (!func_38(var_2, STR))
			var_11->field_142 = var_64;
		else if (!func_38(var_2, STR))
			var_11->field_142 = var_118;
		else if (!func_38(var_2, STR))
			var_11->field_142 = var_122;
	}
#if !func_68(var_6)
	var_190->field_197 = var_110;
	var_2 = func_59(var_152, STR, NULL);
	if (var_2) {
		if (!func_27(var_2, STR, num_67)) {
			func_51(STR,
				&var_86);
			func_94(STR,
				var_48);
			var_190->field_197 = var_98;
		} else if (!func_27(var_2, STR, num_222)) {
			var_190->field_197 = var_62;
			var_190->field_67 = func_13(var_152,
					STR);
			var_114 = func_85(var_152,
				STR, &arr_0);
			if (var_114) {
				func_20(STR,
						var_48, var_175);
				return -var_33;
			}
			var_190->field_29 = arr_0;
			if (var_190->field_67) {
				var_190->field_132 = func_49(var_152, NULL);
				if (func_35(var_190->field_132)) {
					func_20(STR,
								var_48);
					var_190->field_132 = NULL;
					return -var_33;
				}
			} else {
				var_114 = func_85(var_152,
					STR,
								 &arr_0);
				if (var_114) {
					func_20(STR,
							var_48, var_175);
					return -var_33;
				}
				var_190->field_186 = arr_0;
				arr_0 = func_74(var_152,
					STR, num_32);
				var_190->field_85 = arr_0;
				func_94(STR,
								 var_48);
			}
		} else if (!func_27(var_2, STR, num_157)) {
			var_190->field_197 = var_150;
			func_94(STR,
								var_48);
		}
	}
#endif
	var_114 = func_85(var_152, STR, &arr_0);
	var_11->field_14 = (!var_114 ? arr_0 : var_49);
	var_114 = func_85(var_152, STR, &arr_0);
	var_11->field_125 = (!var_114 ? arr_0 : num_69);
	var_114 = func_85(var_152, STR, &arr_0);
	var_11->field_200 = (!var_114 ? arr_0 : num_2);
	var_190->field_143 = var_11->field_200;
#if func_68(var_6)
	if (var_37.field_178)
		var_114 = var_37.field_178(var_152, var_190);
	if(var_114)
		goto error;
#endif
	var_114 = func_85(var_152, STR, &arr_0);
	var_11->field_156.field_138 = (!var_114 ? arr_0 : num_5);

	var_11->field_156.field_167 = func_13(var_152,
		STR);
	var_11->field_156.field_160 = func_13(var_152,
		STR);

	var_114 = func_85(var_152,
		STR, &arr_0);
	var_11->field_156.field_59 = (!var_114 ? arr_0 : false);

	var_11->field_156.field_89 = func_13(var_152,
		STR);
	var_11->field_156.field_35 = func_13(var_152,
		STR);
	var_11->field_156.field_54 = func_13(var_152,
		STR);
	var_11->field_156.field_24 = func_13(var_152,
		STR);
	var_11->field_156.field_185 = func_13(var_152,
		STR);
	var_11->field_156.field_175 = func_13(
		var_152, STR);
	var_11->field_156.field_172 = var_71;
	var_2 = func_59(var_152, STR, NULL);
	if (var_2) {
		if (!func_38(var_2, STR))
			var_11->field_156.field_172 = var_53;
		else if (!func_38(var_2, STR))
			var_11->field_156.field_172 = var_47;
	}
	var_114 = func_85(var_152,
		STR, &arr_0);
	var_11->field_156.field_80 =
			(!var_114 ? arr_0 : num_81);
	var_114 = func_85(var_152,
		STR, &arr_0);
	var_11->field_156.field_135 =
			(!var_114 ? arr_0 : 0x3c);
	var_114 = func_85(var_152,
		STR, &arr_0);
	var_11->field_156.field_41 =
			(!var_114 ? arr_0 : 0x2c);
	var_114 = func_85(var_152,
		STR, &arr_0);
	var_11->field_156.field_44 =
			(!var_114 ? arr_0 : num_82);
	var_114 = func_85(var_152, STR, &arr_0);
	var_11->field_156.field_127 = (!var_114 ? arr_0 : num_73);
	var_11->field_156.field_112 = var_104;
	var_2 = func_59(var_152, STR, NULL);
	if (var_2) {
		if (!func_38(var_2, STR))
			var_11->field_156.field_112 = var_165;
		else if (!func_38(var_2, STR))
			var_11->field_156.field_112 = var_73;
		else if (!func_38(var_2, STR))
			var_11->field_156.field_112 = var_135;
		else if (!func_38(var_2, STR))
			var_11->field_156.field_112 = var_92;
		else if (!func_38(var_2, STR))
			var_11->field_156.field_112 = var_164;
	}
	var_11->field_156.field_47 = func_13(var_152,
		STR);
	var_11->field_156.field_62 = func_13(var_152,
		STR);
	var_11->field_156.field_137 = func_13(var_152,
		STR);
	var_11->field_156.field_133 = func_13(var_152,
		STR);

#if defined(var_163)
	var_114 = func_85(var_152, STR, &arr_0);
	var_11->field_156.field_201 = (!var_114 ? arr_0 : num_134);
#endif

	var_11->field_156.field_51 = func_13(var_152,
		STR);
	var_11->field_156.field_43 = func_13(var_152,
		STR);

	var_114 = func_85(var_152, STR, &arr_0);
	var_11->field_156.field_16 = (!var_114 ? arr_0 : num_15);

	var_2 = func_59(var_152, STR, NULL);
	if (var_2) {
		if (!func_38(var_2, STR))
			var_11->field_6 = var_127;
		else if (!func_38(var_2, STR))
			var_11->field_6 = var_141;
	} else {
		var_11->field_6 = var_93;
	}

	var_114 = func_85(var_152, STR, &arr_0);
	var_11->field_96 = (!var_114 ? arr_0 : var_107);

	var_11->field_156.field_128 = func_13(var_152,
					STR);
	var_114 = func_85(var_152, STR, &arr_0);
	var_11->field_156.field_134 = (!var_114 ? arr_0 : num_224);

	func_98(var_152, var_11);

	func_10(var_152, &(var_11->field_156.field_66),
		STR);

	func_10(var_152, &(var_11->field_156.field_124),
		STR);

	func_18(var_152, &(var_11->field_156.field_82));

	func_69(var_152, var_11->field_174, &(var_11->field_107),
		STR);

	func_48(var_152, &var_190->field_40,
		STR, STR);

	var_11->field_156.field_201 = func_13(var_152,
		STR);

	var_114 = func_52(var_152, var_190);
	if (var_114) {
		func_20(STR, var_48);
		goto error;
	}

	func_92(var_152, var_190);

	func_62(var_152, var_190);

	return num_65;

error:
	return -var_33;
}

int func_90(struct device_node *var_159,
	struct mdss_dsi_ctrl_pdata *var_190,
	bool var_182)
{
	int var_114 = num_78;
	static const char *field_4;
	struct mdss_panel_info *var_11;

	if (!var_159 || !var_190) {
		func_20(STR, var_48);
		return -var_200;
	}

	var_11 = &var_190->field_7.field_136;

	func_94(STR, var_48, var_175);
	var_11->field_4[num_119] = '\num_11';
	field_4 = func_59(var_159, STR, NULL);
	if (!field_4) {
		func_83(STR,
						var_48, var_175);
	} else {
		func_83(STR, var_48, field_4);
		func_43(&var_11->field_4[num_193], field_4, var_146);
#if func_68(var_6)
		if (var_37.field_117)
			var_114 = var_37.field_117(var_159,var_190,var_182);
#endif
	}
	var_114 = func_15(var_159, var_190);
	if (var_114) {
		func_20(STR, var_48, var_175);
		return var_114;
	}

	if (!var_182)
		var_11->field_190 = false;
	func_83(STR, var_48,
		var_11->field_190 ? STR : STR);

	var_11->field_75 = false;
	var_11->field_100 = false;
	var_11->field_151 = false;

	var_190->field_68 = func_79;
	var_190->field_141 = func_7;
	var_190->field_77 = func_73;
	var_190->field_7.field_81 = func_67;
	var_190->field_91 = func_44;

	return num_22;
}

