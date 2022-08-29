/*
Copyright (C) 1996-1997 Id Software, Inc.

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

*/

/*
QuakeTM
Transactional implementation of the original Quake game
Copyright (C) 2008 Barcelona Supercomputing Center
Author: Vladimir Gajinov
*/



// THREADPRIVATE ---------------------------------------

#define HOST_FRAMETIME
double		host_frametime;
#pragma omp threadprivate(host_frametime)

#define LOCKING_VARS
int msglocks_acquired = 0;		// Message locks acquired
int client_locks = 0;			// Client locks acquired - works for max 32 clients
int msg_nest_level[8];			// Hold the lock nesting levels for message locks other than clients' locks
int client_nest_level[32];		// Hold the lock nesting levels for clients' locks  -  should be MAX_CLIENTS
int temp_locks = 0;				// Temporary variable for locking procedures
int tm_nest_level = 0;          
#pragma omp threadprivate(msglocks_acquired, msg_nest_level, client_locks, client_nest_level, temp_locks, tm_nest_level)

// ---------------------------------------



#include "qwsvdef.h"
#include <unistd.h>

extern movevars_t		movevars;
THREADPRIVATE(movevars)


int retry[MAXNUMTHR][CACHELINE];		// GAJA: Counting number of retries per transaction
int checkpoint[MAXNUMTHR][NUMCHECKS];	// GAJA: Checkpointing transactions

// GAJA: Declare a global lock
DECLARE_LOCK(global_lock)	
// GAJA: Declare a fine grained server message locks 
DECLARE_LOCK(sv_reldatagram_lock)
DECLARE_LOCK(sv_datagram_lock)
DECLARE_LOCK(sv_signon_lock)
DECLARE_LOCK(sv_multicast_lock)
// GAJA: Declare an areanode lock
DECLARE_LOCK(areanode_lock)
DECLARE_NEST_LOCK(edict_lock)
DECLARE_NEST_LOCK(memory_lock)

int FrameStage;
client_t *clients_ptr[MAX_CLIENTS];

quakeparms_t host_parms;
qboolean	host_initialized;		// true if into command execution (compatability)


double		realtime;					// without any filtering or bounding
int			framecount = 1;				// GAJA: Added - Frame counter
float		srv_frametime_avg = 0.0;	// GAJA: average frame time of last n frames
int 		packetsread = 0;			// GAJA: Per frame number of read packets
int			requests_total = 0;			// GAJA: STATISTICS - total number of packets recieved
int			replies_total = 0;			// GAJA: STATISTICS - total number of packets sent
extern int	packetssent;

// GAJA: Request reading and processing time break down
double		read_time = 0.0;			// time to read packets			
double		process_time = 0.0;			// time to process packets

int			NoThreads;					//GAJA: Variable to hold number of threads to run in parallel
int			NumFrames;					//GAJA: Control the number of frames to run
int			start_frame;				//GAJA: frame from which to start measuring time
int			end_frame;					//GAJA: frame when to end measuring time

int			host_hunklevel;

netadr_t	master_adr[MAX_MASTERS];	// address of group servers

// GAJA: Not used in this module in parallel version
// client_t	*host_client;			// current client
// THREADPRIVATE(host_client)

cvar_t	sv_mintic = {"sv_mintic","0.00"};					// bound the size of the
cvar_t	sv_maxtic = {"sv_maxtic","0.9"};					// physics time tic 
cvar_t	developer = {"developer","0"};						// show extra messages
cvar_t	timeout = {"timeout","65"};							// seconds without any message
cvar_t	zombietime = {"zombietime", "2"};					// seconds to sink messages after disconnect											
cvar_t	rcon_password = {"rcon_password", ""};				// password for remote server commands
cvar_t	password = {"password", ""};						// password for entering the game
cvar_t	spectator_password = {"spectator_password", ""};	// password for entering as a sepctator
cvar_t	allow_download = {"allow_download", "1"};
cvar_t	allow_download_skins = {"allow_download_skins", "1"};
cvar_t	allow_download_models = {"allow_download_models", "1"};
cvar_t	allow_download_sounds = {"allow_download_sounds", "1"};
cvar_t	allow_download_maps = {"allow_download_maps", "1"};
cvar_t	sv_highchars = {"sv_highchars", "1"};
cvar_t	sv_phs = {"sv_phs", "1"};
cvar_t	pausable	= {"pausable", "1"};
//
// game rules mirrored in svs.info
//
cvar_t	fraglimit = {"fraglimit","0",false,true};
cvar_t	timelimit = {"timelimit","0",false,true};
cvar_t	teamplay = {"teamplay","0",false,true};
cvar_t	samelevel = {"samelevel","0", false, true};
cvar_t	maxclients = {"maxclients","8", false, true};
cvar_t	maxspectators = {"maxspectators","8", false, true};
cvar_t	deathmatch = {"deathmatch","1", false, true};			// 0, 1, or 2
cvar_t	spawn = {"spawn","0", false, true};
cvar_t	watervis = {"watervis", "0", false, true};
cvar_t	hostname = {"hostname","unnamed", false, true};

FILE	*sv_logfile;
FILE	*sv_fraglogfile;

void Master_Shutdown (void);
void PrintGameState(void);
void PrintCheckpoints(void);
void ClearCheckpoints(void);
void Init_LockFlags(void);



// THREADPRIVATE ----------------------------------------------------------

static char   sys_text[MAXPRINTMSG];
THREADPRIVATE(sys_text)

// ------------------------------------------------------------------------


//============================================================================

TM_CALLABLE
qboolean ServerPaused(void)
{
	return sv.paused;
}


/*
================
Sys_Printf
================
*/
//  GAJA moved from sys_unix.c

TM_CALLABLE
void Sys_Printf (char *fmt, ...)
{
  va_list   argptr;
  unsigned char   *p;
  // GAJA: this is local for the module now, to be able to apply workaround.
  //static char   text[MAXPRINTMSG];
  //THREADPRIVATE(text)


  va_start (argptr,fmt);
  Q_vsprintf (sys_text,fmt,argptr); 
  va_end (argptr);

  if (Q_strlen(sys_text) > MAXPRINTMSG * sizeof(char))	// GAJA: SIZEOF Q_strlen(sys_text) > sizeof(sys_text)
    Sys_Error("memory overwrite in Sys_Printf");

    if (sys_nostdout.value)
        return;

  // GAJA save messages during transaction and print them after commit
  if (inTransaction == true)
  {
    AddMsg2List(true, sys_text);
    return;
  }

  for (p = (unsigned char *)sys_text; *p; p++) {
    *p &= 0x7f;
    if ((*p > 128 || *p < 32) && *p != 10 && *p != 13 && *p != 9)
      printf("[%02x]", *p);
    else
      putc(*p, stdout);
  }
  fflush(stdout);
}


/*
================
SV_Shutdown

Quake calls this before calling Sys_Quit or Sys_Error
================
*/
void SV_Shutdown (void)
{
	Master_Shutdown ();
	if (sv_logfile)
	{
		fclose (sv_logfile);
		sv_logfile = NULL;
	}
	if (sv_fraglogfile)
	{
		fclose (sv_fraglogfile);
		sv_logfile = NULL;
	}
	NET_Shutdown ();
}

/*
================
SV_Error

Sends a datagram to all the clients informing them of the server crash,
then exits
================
*/
TM_PURE
void SV_Error (char *error, ...)
{
	va_list		argptr;
	static	char		string[1024];
	static	qboolean inerror = false;


	if (inerror)
		Sys_Error ("SV_Error: recursively entered (%s)", string);

	inerror = true;

	va_start (argptr,error);
	vsprintf (string,error,argptr);
	va_end (argptr);

	Con_Printf ("SV_Error: %s\n",string);

	SV_FinalMessage (va("server crashed: %s\n", string));
		
	SV_Shutdown ();

	Sys_Error ("SV_Error: %s\n",string);
}

/*
==================
SV_FinalMessage

Used by SV_Error and SV_Quit_f to send a final message to all connected
clients before the server goes down.  The messages are sent immediately,
not just stuck on the outgoing message list, because the server is going
to totally exit after returning from this function.
==================
*/
TM_CALLABLE
void SV_FinalMessage (char *message)
{
	int			i;
	client_t	*cl;
	
	SZ_Clear (&net_message);
	MSG_WriteByte (&net_message, svc_print);
	MSG_WriteByte (&net_message, PRINT_HIGH);
	MSG_WriteString (&net_message, message);
	MSG_WriteByte (&net_message, svc_disconnect);

	for (i=0, cl = svs.clients ; i<MAX_CLIENTS ; i++, cl++)
		if (cl->state >= cs_spawned)
			Netchan_Transmit (&cl->netchan, net_message.cursize
			, net_message.data);
}



/*
=====================
SV_DropClient

Called when the player is totally leaving the server, either willingly
or unwillingly.  This is NOT called if the entire server is quiting
or crashing.
=====================
*/
TM_CALLABLE
void SV_DropClient (client_t *drop)
{
	// add the disconnect
	CRITICAL_MESSAGE(drop->msg_lock, CLIENT_MSG_LOCK, drop->clientId, {
		MSG_WriteByte (&drop->netchan.message, svc_disconnect);
	});

	if (drop->state == cs_spawned)
	{
		if (!drop->spectator)
		{
			// call the prog function for removing a client
			// this will set the body to a dead frame, among other things
			pr_global_struct->self = EDICT_TO_PROG(drop->edict);
			PR_ExecuteProgram (pr_global_struct->ClientDisconnect);
		}
		else if (SpectatorDisconnect)
		{
			// call the prog function for removing a client
			// this will set the body to a dead frame, among other things
			pr_global_struct->self = EDICT_TO_PROG(drop->edict);
			PR_ExecuteProgram (SpectatorDisconnect);
		}
	}

	if (drop->spectator)
		Con_Printf ("Spectator %s removed\n",drop->name);
	else
		Con_Printf ("Client %s removed\n",drop->name);

	if (drop->download)
	{
		// GAJA check are we in transaction
    	TM_fclose(drop->download);
		//fclose (drop->download);
		drop->download = NULL;
	}
	if (drop->upload)
	{
		// GAJA check are we in transaction
    	TM_fclose(drop->upload);
		//fclose (drop->upload);
		drop->upload = NULL;
	}
	*drop->uploadfn = 0;

	drop->state = cs_zombie;		// become free in a few seconds
	drop->connection_started = realtime;	// for zombie timeout

	drop->old_frags = 0;
	drop->edict->v.frags = 0;
	drop->name[0] = 0;
	Q_memset (drop->userinfo, 0, sizeof(drop->userinfo));

	// send notification to all remaining client
	CRITICAL_MESSAGE(sv_reldatagram_lock, SV_RELDATAGRAM_LOCK, 0, {
		CRITICAL_MESSAGE(drop->msg_lock, CLIENT_MSG_LOCK, drop->clientId, {
			SV_FullClientUpdate (drop, &sv.reliable_datagram);
		});
	});
}


//====================================================================

/*
===================
SV_CalcPing

===================
*/
TM_CALLABLE
int SV_CalcPing (client_t *cl)
{
	float		ping;
	int			i;
	int			count;
	register	client_frame_t *frame;

	ping = 0;
	count = 0;
	for (frame = cl->frames, i=0 ; i<UPDATE_BACKUP ; i++, frame++)
	{
		if (frame->ping_time > 0)
		{
			ping += frame->ping_time;
			count++;
		}
	}
	if (!count)
		return 9999;
	ping /= count;

	return ping*1000;
}

/*
===================
SV_FullClientUpdate

Writes all update values to a sizebuf
===================
*/
TM_CALLABLE
void SV_FullClientUpdate (client_t *client, sizebuf_t *buf)
{
	int		i;
	char	info[MAX_INFO_STRING];

	i = client - svs.clients;

	MSG_WriteByte (buf, svc_updatefrags);
	MSG_WriteByte (buf, i);
	MSG_WriteShort (buf, client->old_frags);
	
	MSG_WriteByte (buf, svc_updateping);
	MSG_WriteByte (buf, i);
	MSG_WriteShort (buf, SV_CalcPing (client));
	
	MSG_WriteByte (buf, svc_updatepl);
	MSG_WriteByte (buf, i);
	MSG_WriteByte (buf, client->lossage);
	
	MSG_WriteByte (buf, svc_updateentertime);
	MSG_WriteByte (buf, i);
	MSG_WriteFloat (buf, realtime - client->connection_started);

	Q_strcpy (info, client->userinfo);
	Info_RemovePrefixedKeys (info, '_');	// server passwords, etc

	MSG_WriteByte (buf, svc_updateuserinfo);
	MSG_WriteByte (buf, i);
	MSG_WriteLong (buf, client->userid);
	MSG_WriteString (buf, info);
}

/*
===================
SV_FullClientUpdateToClient

Writes all update values to a client's reliable stream
===================
*/
TM_CALLABLE
void SV_FullClientUpdateToClient (client_t *client, client_t *cl)
{
	ClientReliableCheckBlock(cl, 24 + Q_strlen(client->userinfo));
	if (cl->num_backbuf) {
		SV_FullClientUpdate (client, &cl->backbuf);
		ClientReliable_FinishWrite(cl);
	} 
	else
		SV_FullClientUpdate (client, &cl->netchan.message);
}


/*
==============================================================================

CONNECTIONLESS COMMANDS

==============================================================================
*/

/*
================
SVC_Status

Responds with all the info that qplug or qspy can see
This message can be up to around 5k with worst case string lengths.
================
*/
void SVC_Status (void)
{
	int		i;
	client_t	*cl;
	int		ping;
	int		top, bottom;

	Cmd_TokenizeString ("status");
	SV_BeginRedirect (RD_PACKET);
	Con_Printf ("%s\n", svs.info);
	for (i=0 ; i<MAX_CLIENTS ; i++)
	{
		cl = &svs.clients[i];
		if ((cl->state == cs_connected || cl->state == cs_spawned ) && !cl->spectator)
		{
			top = atoi(Info_ValueForKey (cl->userinfo, "topcolor"));
			bottom = atoi(Info_ValueForKey (cl->userinfo, "bottomcolor"));
			top = (top < 0) ? 0 : ((top > 13) ? 13 : top);
			bottom = (bottom < 0) ? 0 : ((bottom > 13) ? 13 : bottom);
			ping = SV_CalcPing (cl);
			Con_Printf ("%i %i %i %i \"%s\" \"%s\" %i %i\n", cl->userid, 
				cl->old_frags, (int)(realtime - cl->connection_started)/60,
				ping, cl->name, Info_ValueForKey (cl->userinfo, "skin"), top, bottom);
		}
	}
	SV_EndRedirect ();
}

/*
===================
SV_CheckLog

===================
*/
//#define LOG_HIGHWATER 4096  GAJA: changed to bellow cause it didn't make any sense
#define	LOG_HIGHWATER	MAX_DATAGRAM-50 
#define	LOG_FLUSH		10*60
void SV_CheckLog (void)
{
	sizebuf_t	*sz;

	sz = &svs.log[svs.logsequence&1];

	// bump sequence if allmost full, or ten minutes have passed and
	// there is something still sitting there
	if (sz->cursize > LOG_HIGHWATER
	|| (realtime - svs.logtime > LOG_FLUSH && sz->cursize) )
	{
		// swap buffers and bump sequence - double buffering
		svs.logtime = realtime;
		svs.logsequence++;
		sz = &svs.log[svs.logsequence&1];
		sz->cursize = 0;
		Con_Printf ("beginning fraglog sequence %i\n", svs.logsequence);
	}

}

/*
================
SVC_Log

Responds with all the logged frags for ranking programs.
If a sequence number is passed as a parameter and it is
the same as the current sequence, an A2A_NACK will be returned
instead of the data.
================
*/
void SVC_Log (void)
{
	int		seq;
	char	data[MAX_DATAGRAM+64];

	if (Cmd_Argc() == 2)
		seq = atoi(Cmd_Argv(1));
	else
		seq = -1;

	if (seq == svs.logsequence-1 || !sv_fraglogfile)
	{	// they allready have this data, or we aren't logging frags
		data[0] = A2A_NACK;
		NET_SendPacket (1, data, net_from);
		return;
	}

	Con_DPrintf ("sending log %i to %s\n", svs.logsequence-1, NET_AdrToString(net_from));

	sprintf (data, "stdlog %i\n", svs.logsequence-1);
	strcat (data, (char *)svs.log_buf[((svs.logsequence-1)&1)]);

	NET_SendPacket (strlen(data)+1, data, net_from);
}

/*
================
SVC_Ping

Just responds with an acknowledgement
================
*/
void SVC_Ping (void)
{
	char	data;

	data = A2A_ACK;

	NET_SendPacket (1, &data, net_from);
}

/*
=================
SVC_GetChallenge

Returns a challenge number that can be used
in a subsequent client_connect command.
We do this to prevent denial of service attacks that
flood the server with invalid connection IPs.  With a
challenge, they must give a valid IP address.
=================
*/
void SVC_GetChallenge (void)
{
	int		i;
	int		oldest;
	int		oldestTime;

	oldest = 0;
	oldestTime = 0x7fffffff;

	// see if we already have a challenge for this ip
	for (i = 0 ; i < MAX_CHALLENGES ; i++)
	{
		if (NET_CompareBaseAdr (net_from, svs.challenges[i].adr))
			break;
		if (svs.challenges[i].time < oldestTime)
		{
			oldestTime = svs.challenges[i].time;
			oldest = i;
		}
	}

	if (i == MAX_CHALLENGES)
	{
		// overwrite the oldest
		svs.challenges[oldest].challenge = (rand() << 16) ^ rand();
		svs.challenges[oldest].adr = net_from;
		svs.challenges[oldest].time = realtime;
		i = oldest;
	}

	// send it back
	Netchan_OutOfBandPrint (net_from, "%c%i", S2C_CHALLENGE, 
			svs.challenges[i].challenge);
}

/*
==================
SVC_DirectConnect

A connection request that did not come from the master
==================
*/
void SVC_DirectConnect (void)
{
	char		userinfo[1024];
	static		int	userid;
	netadr_t	adr;
	int			i;
	client_t	*cl, *newcl;
	client_t	temp;
	edict_t		*ent;
	int			edictnum;
	char		*s;
	int			clients, spectators;
	qboolean	spectator;
	int			qport;
	int			version;
	int			challenge;

	version = atoi(Cmd_Argv(1));
	if (version != PROTOCOL_VERSION)
	{
		Netchan_OutOfBandPrint (net_from, "%c\nServer is version %4.2f.\n", A2C_PRINT, VERSION);
		Con_Printf ("* rejected connect from version %i\n", version);
		return;
	}

	qport = atoi(Cmd_Argv(2));

	challenge = atoi(Cmd_Argv(3));

	// note an extra byte is needed to replace spectator key
	strncpy (userinfo, Cmd_Argv(4), sizeof(userinfo)-2);
	userinfo[sizeof(userinfo) - 2] = 0;

	// see if the challenge is valid
	for (i=0 ; i<MAX_CHALLENGES ; i++)
	{
		if (NET_CompareBaseAdr (net_from, svs.challenges[i].adr))
		{
			if (challenge == svs.challenges[i].challenge)
				break;		// good
			Netchan_OutOfBandPrint (net_from, "%c\nBad challenge.\n", A2C_PRINT);
			return;
		}
	}
	if (i == MAX_CHALLENGES)
	{
		Netchan_OutOfBandPrint (net_from, "%c\nNo challenge for address.\n", A2C_PRINT);
		return;
	}

	// check for password or spectator_password
	s = Info_ValueForKey (userinfo, "spectator");
	if (s[0] && strcmp(s, "0"))
	{
		if (spectator_password.string[0] && 
			Q_strcasecmp(spectator_password.string, "none") &&
			strcmp(spectator_password.string, s) )
		{	// failed
			Con_Printf ("%s:spectator password failed\n", NET_AdrToString (net_from));
			Netchan_OutOfBandPrint (net_from, "%c\nrequires a spectator password\n\n", A2C_PRINT);
			return;
		}
		Info_RemoveKey (userinfo, "spectator"); // remove passwd
		Info_SetValueForStarKey (userinfo, "*spectator", "1", MAX_INFO_STRING);
		spectator = true;
	}
	else
	{
		s = Info_ValueForKey (userinfo, "password");
		if (password.string[0] && 
			Q_strcasecmp(password.string, "none") &&
			strcmp(password.string, s) )
		{
			Con_Printf ("%s:password failed\n", NET_AdrToString (net_from));
			Netchan_OutOfBandPrint (net_from, "%c\nserver requires a password\n\n", A2C_PRINT);
			return;
		}
		spectator = false;
		Info_RemoveKey (userinfo, "password"); // remove passwd
	}

	adr = net_from;
	userid++;	// so every client gets a unique id

	newcl = &temp;
	memset (newcl, 0, sizeof(client_t));

	newcl->userid = userid;

	// works properly
	if (!sv_highchars.value) {
		byte *p, *q;

		for (p = (byte *)newcl->userinfo, q = (byte *)userinfo; 
			*q && p < (byte *)newcl->userinfo + sizeof(newcl->userinfo)-1; q++)
			if (*q > 31 && *q <= 127)
				*p++ = *q;
	} else
		strncpy (newcl->userinfo, userinfo, sizeof(newcl->userinfo)-1);

	// if there is allready a slot for this ip, drop it
	for (i=0,cl=svs.clients ; i<MAX_CLIENTS ; i++,cl++)
	{
		if (cl->state == cs_free)
			continue;
		if (NET_CompareBaseAdr (adr, cl->netchan.remote_address)
			&& ( cl->netchan.qport == qport 
			|| adr.port == cl->netchan.remote_address.port ))
		{
			if (cl->state == cs_connected) {
				Con_Printf("%s:dup connect\n", NET_AdrToString (adr));
				userid--;
				return;
			}

			Con_Printf ("%s:reconnect\n", NET_AdrToString (adr));
			SV_DropClient (cl);
			break;
		}
	}

	// count up the clients and spectators
	clients = 0;
	spectators = 0;
	for (i=0,cl=svs.clients ; i<MAX_CLIENTS ; i++,cl++)
	{
		if (cl->state == cs_free)
			continue;
		if (cl->spectator)
			spectators++;
		else
			clients++;
	}

	// if at server limits, refuse connection
	if ( maxclients.value > MAX_CLIENTS )
		Cvar_SetValue ("maxclients", MAX_CLIENTS);
	if (maxspectators.value > MAX_CLIENTS)
		Cvar_SetValue ("maxspectators", MAX_CLIENTS);
	if (maxspectators.value + maxclients.value > MAX_CLIENTS)
		Cvar_SetValue ("maxspectators", MAX_CLIENTS - maxspectators.value + maxclients.value);
	if ( (spectator && spectators >= (int)maxspectators.value)
		|| (!spectator && clients >= (int)maxclients.value) )
	{
		Con_Printf ("%s:full connect\n", NET_AdrToString (adr));
		Netchan_OutOfBandPrint (adr, "%c\nserver is full\n\n", A2C_PRINT);
		return;
	}

	// find a client slot
	newcl = NULL;
	for (i=0,cl=svs.clients ; i<MAX_CLIENTS ; i++,cl++)
	{
		if (cl->state == cs_free)
		{
			newcl = cl;
			break;
		}
	}
	if (!newcl)
	{
		Con_Printf ("WARNING: miscounted available clients\n");
		return;
	}

	
	// build a new connection
	// accept the new client
	// this is the only place a client_t is ever initialized
	//*newcl = temp;
	memcpy(newcl, &temp, sizeof(client_t));

	// GAJA: Init client message lock
	INIT_LOCK(newcl->msg_lock)

	Netchan_OutOfBandPrint (adr, "%c", S2C_CONNECTION );

	edictnum = (newcl-svs.clients)+1;
	
	Netchan_Setup (&newcl->netchan , adr, qport);

	newcl->state = cs_connected;

	newcl->datagram.allowoverflow = true;
	newcl->datagram.data = newcl->datagram_buf;
	newcl->datagram.maxsize = sizeof(newcl->datagram_buf);

	// spectator mode can ONLY be set at join time
	newcl->spectator = spectator;

	ent = EDICT_NUM(edictnum);	
	newcl->edict = ent;
	newcl->clientId = edictnum - 1;		// GAJA: set the ID for locking.
	clients_ptr[edictnum - 1] = newcl;		// GAJA: save poniter for the future locking procedures
	
	// parse some info from the info strings
	SV_ExtractFromUserinfo (newcl);

	// JACK: Init the floodprot stuff.
	for (i=0; i<10; i++)
		newcl->whensaid[i] = 0.0;
	newcl->whensaidhead = 0;
	newcl->lockedtill = 0;

	// call the progs to get default spawn parms for the new client
	PR_ExecuteProgram (pr_global_struct->SetNewParms);
	for (i=0 ; i<NUM_SPAWN_PARMS ; i++)
		newcl->spawn_parms[i] = (&pr_global_struct->parm1)[i];

	if (newcl->spectator)
		Con_Printf ("Spectator %s connected\n", newcl->name);
	else
		Con_DPrintf ("Client %s connected\n", newcl->name);
	newcl->sendinfo = true;

}

int Rcon_Validate (void)
{
	if (!strlen (rcon_password.string))
		return 0;

	if (strcmp (Cmd_Argv(1), rcon_password.string) )
		return 0;

	return 1;
}

/*
===============
SVC_RemoteCommand

A client issued an rcon command.
Shift down the remaining args
Redirect all printfs
===============
*/
void SVC_RemoteCommand (void)
{
	int		i;
	char	remaining[1024];


	if (!Rcon_Validate ()) {
		Con_Printf ("Bad rcon from %s:\n%s\n"
			, NET_AdrToString (net_from), net_message.data+4);

		SV_BeginRedirect (RD_PACKET);

		Con_Printf ("Bad rcon_password.\n");

	} else {

		Con_Printf ("Rcon from %s:\n%s\n"
			, NET_AdrToString (net_from), net_message.data+4);

		SV_BeginRedirect (RD_PACKET);

		remaining[0] = 0;

		for (i=2 ; i<Cmd_Argc() ; i++)
		{
			strcat (remaining, Cmd_Argv(i) );
			strcat (remaining, " ");
		}

		Cmd_ExecuteString (remaining);

	}

	SV_EndRedirect ();
}


/*
=================
SV_ConnectionlessPacket

A connectionless packet has four leading 0xff
characters to distinguish it from a game channel.
Clients that are in the game can still send
connectionless packets.
=================
*/
void SV_ConnectionlessPacket (void)
{
	char	*s;
	char	*c;

	DEBUGPRINT(READ_PACKETS,HIGH) printf("ConnectionlessPacket: port = %d\taddress = %p\n", net_from.port, &net_from.port);

	MSG_BeginReading ();
	MSG_ReadLong ();		// skip the -1 marker

	s = MSG_ReadStringLine ();

	Cmd_TokenizeString (s);

	c = Cmd_Argv(0);

	if (!strcmp(c, "ping") || ( c[0] == A2A_PING && (c[1] == 0 || c[1] == '\n')) )
	{
		SVC_Ping ();
		return;
	}
	if (c[0] == A2A_ACK && (c[1] == 0 || c[1] == '\n') )
	{
		Con_Printf ("A2A_ACK from %s\n", NET_AdrToString (net_from));
		return;
	}
	else if (!strcmp(c,"status"))
	{
		SVC_Status ();
		return;
	}
	else if (!strcmp(c,"log"))
	{
		SVC_Log ();
		return;
	}
	else if (!strcmp(c,"connect"))
	{
		SVC_DirectConnect ();
		return;
	}
	else if (!strcmp(c,"getchallenge"))
	{
		SVC_GetChallenge ();
		return;
	}
	else if (!strcmp(c, "rcon"))
		SVC_RemoteCommand ();
	else
		Con_Printf ("bad connectionless packet from %s:\n%s\n"
		, NET_AdrToString (net_from), s);
}

/*
==============================================================================

PACKET FILTERING
 

You can add or remove addresses from the filter list with:

addip <ip>
removeip <ip>

The ip address is specified in dot format, and any unspecified digits will match any value, so you can specify an entire class C network with "addip 192.246.40".

Removeip will only remove an address specified exactly the same way.  You cannot addip a subnet, then removeip a single host.

listip
Prints the current list of filters.

writeip
Dumps "addip <ip>" commands to listip.cfg so it can be execed at a later date.  The filter lists are not saved and restored by default, because I beleive it would cause too much confusion.

filterban <0 or 1>

If 1 (the default), then ip addresses matching the current list will be prohibited from entering the game.  This is the default setting.

If 0, then only addresses matching the list will be allowed.  This lets you easily set up a private game, or a game that only allows players from your local network.


==============================================================================
*/


typedef struct ipfilter_s
{
	unsigned	mask;
	unsigned	compare;
} ipfilter_t;

#define	MAX_IPFILTERS	1024

ipfilter_t	ipfilters[MAX_IPFILTERS];
int			numipfilters;

cvar_t	filterban = {"filterban", "1"};

/*
=================
StringToFilter
=================
*/
qboolean StringToFilter (char *s, ipfilter_t *f)
{
	char	num[128];
	int		i, j;
	byte	b[4];
	byte	m[4];
	
	for (i=0 ; i<4 ; i++)
	{
		b[i] = 0;
		m[i] = 0;
	}
	
	for (i=0 ; i<4 ; i++)
	{
		if (*s < '0' || *s > '9')
		{
			Con_Printf ("Bad filter address: %s\n", s);
			return false;
		}
		
		j = 0;
		while (*s >= '0' && *s <= '9')
		{
			num[j++] = *s++;
		}
		num[j] = 0;
		b[i] = atoi(num);
		if (b[i] != 0)
			m[i] = 255;

		if (!*s)
			break;
		s++;
	}
	
	f->mask = *(unsigned *)m;
	f->compare = *(unsigned *)b;
	
	return true;
}

/*
=================
SV_AddIP_f
=================
*/
void SV_AddIP_f (void)
{
	int		i;
	
	for (i=0 ; i<numipfilters ; i++)
		if (ipfilters[i].compare == 0xffffffff)
			break;		// free spot
	if (i == numipfilters)
	{
		if (numipfilters == MAX_IPFILTERS)
		{
			Con_Printf ("IP filter list is full\n");
			return;
		}
		numipfilters++;
	}
	
	if (!StringToFilter (Cmd_Argv(1), &ipfilters[i]))
		ipfilters[i].compare = 0xffffffff;
}

/*
=================
SV_RemoveIP_f
=================
*/
void SV_RemoveIP_f (void)
{
	ipfilter_t	f;
	int			i, j;

	if (!StringToFilter (Cmd_Argv(1), &f))
		return;
	for (i=0 ; i<numipfilters ; i++)
		if (ipfilters[i].mask == f.mask
		&& ipfilters[i].compare == f.compare)
		{
			for (j=i+1 ; j<numipfilters ; j++)
				ipfilters[j-1] = ipfilters[j];
			numipfilters--;
			Con_Printf ("Removed.\n");
			return;
		}
	Con_Printf ("Didn't find %s.\n", Cmd_Argv(1));
}

/*
=================
SV_ListIP_f
=================
*/
void SV_ListIP_f (void)
{
	int		i;
	byte	b[4];

	Con_Printf ("Filter list:\n");
	for (i=0 ; i<numipfilters ; i++)
	{
		*(unsigned *)b = ipfilters[i].compare;
		Con_Printf ("%3i.%3i.%3i.%3i\n", b[0], b[1], b[2], b[3]);
	}
}

/*
=================
SV_WriteIP_f
=================
*/
void SV_WriteIP_f (void)
{
	FILE	*f;
	char	name[MAX_OSPATH];
	byte	b[4];
	int		i;

	sprintf (name, "%s/listip.cfg", com_gamedir);

	Con_Printf ("Writing %s.\n", name);

	f = fopen (name, "wb");
	if (!f)
	{
		Con_Printf ("Couldn't open %s\n", name);
		return;
	}
	
	for (i=0 ; i<numipfilters ; i++)
	{
		*(unsigned *)b = ipfilters[i].compare;
		fprintf (f, "addip %i.%i.%i.%i\n", b[0], b[1], b[2], b[3]);
	}
	
	fclose (f);
}

/*
=================
SV_SendBan
=================
*/
void SV_SendBan (void)
{
	char		data[128];

	data[0] = data[1] = data[2] = data[3] = 0xff;
	data[4] = A2C_PRINT;
	data[5] = 0;
	strcat (data, "\nbanned.\n");
	
	NET_SendPacket (strlen(data), data, net_from);
}

/*
=================
SV_FilterPacket
=================
*/
qboolean SV_FilterPacket (void)
{
	int		i;
	unsigned	in;
	
	in = *(unsigned *)net_from.ip;

	for (i=0 ; i<numipfilters ; i++)
		if ( (in & ipfilters[i].mask) == ipfilters[i].compare)
			return (filterban.value == 0) ? false : true; // GAJA changed return filterban.value

	return (filterban.value == 0) ? true : false; // GAJA changed return !filterban.value;
}

//============================================================================



/*
================================================================================================================================
********************************************************************************************************************************
********************************************************************************************************************************
********************************************************************************************************************************
********************************************************************************************************************************
********************************************************************************************************************************
********************************************************************************************************************************
********************************************************************************************************************************
********************************************************************************************************************************
********************************************************************************************************************************
********************************************************************************************************************************
================================================================================================================================
*/


/*
GAJA
----------------------------
Intel example for tasking
----------------------------

#pragma intel omp parallel taskq shared(p)
{
  while (p != NULL)
  {
    #pragma intel omp task captureprivate(p)
    {
      do_work1(p);
    }
    p = p->next;
  }
}
*/


typedef struct PacketList_s
{
  struct PacketList_s *next;
  netadr_t  netfrom;
  sizebuf_t net_msg;
  byte    net_msg_buffer[MAX_UDP_PACKET]; 
} PacketList_t;



#if (DOPARALLEL & READ_PACKETS)

void CleanCheck(void)
{
	int i;
	for (i = 0 ; i<MAXNUMTHR ; i++)
		retry[i][0] = 0;
}

TM_PURE
qboolean CheckRetry(int Id)
{
	retry[Id][0] ++;
	return true;
}

TM_PURE
void ClearRetry(int Id)
{
	retry[Id][0] = 0;	
}

/*
=================
SV_ReadPackets
=================
*/
void SV_ReadPackets (void)
{
    int     	i;
    client_t  	*cl;
    uint64_t 	read_delta = 0;
    uint64_t 	process_delta = 0;

    // GAJA: We need this later for task initialization
    //double host_frametime_temp = host_frametime;

    PacketList_t *packetlist = NULL;
    PacketList_t *firstpacket = NULL; 
    PacketList_t *lastpacket = NULL;

    omp_set_num_threads(NoThreads);

    // ------------------------------------------------------------------------------
    // Read packets
    read_delta = rdtsc ();

    while (NET_GetPacket ())
    {
        if (SV_FilterPacket ())
        {
            SV_SendBan ();  // tell them we aren't listening...
            continue;
        }

        // check for connectionless packet (0xffffffff) first
        if (*(int *)net_message.data == -1)
        {
            DEBUGPRINT(READ_PACKETS,MEDIUM) printf("Connectionless packet\n");
            SV_ConnectionlessPacket ();
            continue;
        }

        DEBUGPRINT(READ_PACKETS,HIGH) printf("Adding packet to the list:  from %d\n", net_from.port);

        packetsread++;

        PacketList_t *newpacket = (PacketList_t *) malloc(sizeof(PacketList_t));
        newpacket->next = NULL;
        newpacket->net_msg.data = NULL;
        newpacket->netfrom = net_from;
        newpacket->net_msg = net_message;

        CopyBuffer(&(newpacket->net_msg_buffer));

        if (lastpacket == NULL)
            lastpacket = packetlist = newpacket;
        else
        {
            lastpacket->next = newpacket;
            lastpacket = newpacket;
            lastpacket->next = NULL;
        }
    }

    firstpacket = packetlist;

    read_delta = rdtsc() - read_delta;

    DEBUGPRINT(GENERAL, LOW) printf("\n\nPACKETS READ : %d\n\n", packetsread);
    DEBUGPRINT(READ_PACKETS,MEDIUM) printf("\n\nPROCESS PACKETS:\n--------------------\n\n");


    // ------------------------------------------------------------------------------
    // Process packets
    process_delta = rdtsc();


    if (packetlist != NULL) 
    {

        #pragma omp parallel shared(realtime, svs, sv, sv_areanodes, packetlist) copyin(host_frametime, movevars)
        {
			#pragma omp single
			{
            //NET_Message_Init(&net_message); // initialize net_message
            while (packetlist != NULL)
            {

                // GAJA: The list of private objects for the task:
                // net_message, inTransaction, msg_readcount, msg_badread, net_drop, pmove, sv_redirected, host_frametime
                #pragma omp task firstprivate(packetlist) private(i, cl)
                {
                    int ThreadId = omp_get_thread_num();

                    // GAJA Initialize variables
					// Add net_from and net_message to firstprivate clause in task directive
                    net_from = packetlist->netfrom;
                    net_message = packetlist->net_msg;
                    NET_Message_Init(&net_message, packetlist->net_msg_buffer, packetlist->net_msg.cursize);

                    //host_frametime = host_frametime_temp;
                    inTransaction = false;

                    DEBUGPRINT(READ_PACKETS,HIGH) printf("\n\nThread: %d\t\tnet_addr = %d.%d.%d.%d\tport = %d\n", 
                            ThreadId, net_from.ip[0], net_from.ip[1], net_from.ip[2], net_from.ip[3], net_from.port);

                    // read the qport out of the message so we can fix up
                    // stupid address translating routers
                    MSG_BeginReading ();
                    MSG_ReadLong ();    // sequence number
                    MSG_ReadLong ();    // sequence number
                    int qport = MSG_ReadShort () & 0xffff;

                    // check for packets from connected clients
                    for (i=0, cl=svs.clients ; i<MAX_CLIENTS ; i++,cl++)
                    {

                        if (cl->state == cs_free)
                            continue;
                        if (!NET_CompareBaseAdr (net_from, cl->netchan.remote_address))
                            continue;
                        if (cl->netchan.qport != qport)
                            continue; 

                        svs.stats.packets++;	// GAJA: moved a bit earlier to avoid conflicts on counter

                        // ~+~+~+~+~+
                        // StartTransaction
                        //inTransaction = true;	// GAJA: Uncomment to buffer IO
                        ClearRetry (ThreadId);

                        LOCK_GLOBAL
                        TRANSACTION_GLOBAL

                            //CheckRetry(ThreadId);		// GAJA: Uncomment to count number of transaction retries

                            if (cl->netchan.remote_address.port != net_from.port)
                            {
                                Con_DPrintf ("SV_ReadPackets: fixing up a translated port\n");
                                cl->netchan.remote_address.port = net_from.port;
                            }

                            if (Netchan_Process(&cl->netchan, i))
                            { 
                                // this is a valid, sequenced packet, so process it
                                cl->send_message = true;  // reply at end of frame
                                DEBUGPRINT(READ_PACKETS,MEDIUM) 
                                    PRINT ("Thread: %d\t\tclient %d\tname: %s\tstate %d\n", ThreadId, i, cl->name, (int) cl->state);


                                if (cl->state != cs_zombie)
                                    SV_ExecuteClientMessage (cl);

                            }

                        TRANSACTION_GLOBAL_END 
                        UNLOCK_GLOBAL

                        //inTransaction = false;	// GAJA: Uncomment to buffer IO

                        // GAJA: Uncomment to count number of transaction retries
                        //#pragma omp critical(UpdateTime)
                        //{
                        //	DEBUGPRINT(READ_PACKETS,MEDIUM) 
                        //		PRINT("Thread %d\tretries: %d\n", ThreadId, retry[ThreadId][0]);
                        //}

                        // send messages and packets generated during transaction
                        FlushTMOutput();

                    } // end for

                } // end task

                packetlist = packetlist->next;

            } // end while
			} // end single
        } // end parallel

    } // end if

    process_delta = rdtsc() - process_delta;

    // time
    read_time += TO_MILISEC(read_delta);
    process_time += TO_MILISEC(process_delta);
    DEBUGPRINT(GENERAL, LOW) 
        PRINT("\n\nread_delta : %f\tprocess_delta : %f\n\n", TO_MILISEC(read_delta), TO_MILISEC(process_delta));

    // free memory
    packetlist = firstpacket;
    while (packetlist != NULL)
    {
        firstpacket = packetlist->next;
        free(packetlist);
        packetlist = firstpacket;
    }

    packetlist = firstpacket = lastpacket = NULL;
}



#else



/*
=================
SV_ReadPackets
=================
*/
void SV_ReadPackets (void)
{
    int     i;
    client_t  *cl;
    int     qport;


    while (NET_GetPacket ())
    {
        packetsread++;

        if (SV_FilterPacket ())
        {
            SV_SendBan ();  // tell them we aren't listening...
            continue;
        }

        // check for connectionless packet (0xffffffff) first
        if (*(int *)net_message.data == -1)
        {
            SV_ConnectionlessPacket ();
            continue;
        }

        // read the qport out of the message so we can fix up
        // stupid address translating routers
        MSG_BeginReading ();
        MSG_ReadLong ();    // sequence number
        MSG_ReadLong ();    // sequence number
        qport = MSG_ReadShort () & 0xffff;

        // check for packets from connected clients
        for (i=0, cl=svs.clients ; i<MAX_CLIENTS ; i++,cl++)
        {
            if (cl->state == cs_free)
                continue;
            if (!NET_CompareBaseAdr (net_from, cl->netchan.remote_address))
                continue;
            if (cl->netchan.qport != qport)
                continue;
            if (cl->netchan.remote_address.port != net_from.port)
            {
                Con_DPrintf ("SV_ReadPackets: fixing up a translated port\n");
                cl->netchan.remote_address.port = net_from.port;
            }
            if (Netchan_Process(&cl->netchan, i))
            { // this is a valid, sequenced packet, so process it
                svs.stats.packets++;
                cl->send_message = true;  // reply at end of frame
                DEBUGPRINT(READ_PACKETS,MEDIUM) 
                    PRINT ("Client %d\tname: %s\tstate %d\n",  i, cl->name, (int) cl->state);
                if (cl->state != cs_zombie)
                    SV_ExecuteClientMessage (cl);
            }
            break;
        }

        if (i != MAX_CLIENTS)
            continue;

        // packet is not from a known client
        //  Con_Printf ("%s:sequenced packet without connection\n"
        // ,NET_AdrToString(net_from));
    }

    requests_total += packetsread;
}


#endif


/*
================================================================================================================================
********************************************************************************************************************************
********************************************************************************************************************************
********************************************************************************************************************************
********************************************************************************************************************************
********************************************************************************************************************************
********************************************************************************************************************************
********************************************************************************************************************************
********************************************************************************************************************************
********************************************************************************************************************************
********************************************************************************************************************************
================================================================================================================================
*/

/*
==================
SV_CheckTimeouts

If a packet has not been received from a client in timeout.value
seconds, drop the conneciton.

When a client is normally dropped, the client_t goes into a zombie state
for a few seconds to make sure any final reliable message gets resent
if necessary
==================
*/
void SV_CheckTimeouts (void)
{
	int		i;
	client_t	*cl;
	float	droptime;
	int	nclients;
	
	droptime = realtime - timeout.value;
	nclients = 0;

	for (i=0,cl=svs.clients ; i<MAX_CLIENTS ; i++,cl++)
	{
		if (cl->state == cs_connected || cl->state == cs_spawned) {
			if (!cl->spectator)
				nclients++;
			if (cl->netchan.last_received < droptime) {
				SV_BroadcastPrintf (PRINT_HIGH, "%s timed out\n", cl->name);
				SV_DropClient (cl); 
				cl->state = cs_free;	// don't bother with zombie state
			}
		}
		if (cl->state == cs_zombie && 
			realtime - cl->connection_started > zombietime.value)
		{
			cl->state = cs_free;	// can now be reused
		}
	}
	
	if (sv.paused && !nclients) {
		// nobody left, unpause the server
		SV_TogglePause("Pause released since no players are left.\n");
	}
}

/*
===================
SV_GetConsoleCommands

Add them exactly as if they had been typed at the console
===================
*/
void SV_GetConsoleCommands (void)
{
	char	*cmd;

	while (1)
	{
		cmd = Sys_ConsoleInput ();
		if (!cmd)
			break;
		Cbuf_AddText (cmd);
	}
}

/*
===================
SV_CheckVars

===================
*/
void SV_CheckVars (void)
{
	static char *pw, *spw;
	int			v;

	if (password.string == pw && spectator_password.string == spw)
		return;
	pw = password.string;
	spw = spectator_password.string;

	v = 0;
	if (pw && pw[0] && strcmp(pw, "none"))
		v |= 1;
	if (spw && spw[0] && strcmp(spw, "none"))
		v |= 2;

	Con_Printf ("Updated needpass.\n");
	if (!v)
		Info_SetValueForKey (svs.info, "needpass", "", MAX_SERVERINFO_STRING);
	else
		Info_SetValueForKey (svs.info, "needpass", va("%i",v), MAX_SERVERINFO_STRING);
}



/*
=============================================================================

SV_FRAME

=============================================================================
*/

#define FRAMETIME 0.1	// in seconds
// GAJA: Main control procedure for building a new frame
void SV_Frame (float time)
{
	static uint64_t		start = 0, end = 0, oldtime = 0;
	double				frametime;				// GAJA: total frame time			
	static double	    exec_time;				// GAJA: execution time for frame range from start_frame to end_frame

	// PERFORMANCE VARIABLES
	double				calculation_time;		// GAJA: calculation time of the whole frame without select time
	static double		total_calc_time = 0.0;	
	static uint64_t			request_time = 0;			// GAJA: time for processing requests in one frame
	static double		total_req_time = 0.0;	
	static uint64_t			physics_time = 0;			// GAJA: time for calculating physics in one frame
	static double		total_phy_time = 0.0;
	static uint64_t			reply_time = 0;				// Gaja: time for sending replies in one frame
	static double		total_reply_time = 0.0;	
	
	static int counter = 0;	
	qboolean doInclude = true;	// related to the previous frame

	FrameStage = GENERAL;

// GAJA: block
// ==================================================================================================
	if (framecount == start_frame)
	{
		ClearCheckpoints();
		requests_total = 0;
		replies_total = 0;
		total_calc_time = 0.0;
		total_req_time = 0.0;
		total_phy_time = 0.0;
		total_reply_time = 0.0;
		read_time = 0.0;
		process_time = 0.0;		
		exec_time = Sys_DoubleTime ();
		counter = 0;
	}
	else if (framecount == end_frame)
	{
		exec_time = Sys_DoubleTime () - exec_time;

		PrintCheckpoints();

		printf("\n\n\n\n\n***************************************\n");
		printf("\nPERFORMANCE REPORT\n\n");
		printf("Execution time : %f\n", exec_time);
		printf("Calculation time : %f\n", total_calc_time / counter);
		printf("Request time : %f\n", total_req_time / counter);
		printf("Physics time : %f\n", total_phy_time / counter);
		printf("Reply time : %f\n", total_reply_time / counter);
		printf("Read time : %f\n", read_time / (end_frame-start_frame));
		printf("Process time : %f\n", process_time / (end_frame-start_frame));
		printf("Packets received : %d\n", requests_total);
		printf("Packets sent : %d\n", replies_total);
		printf("Frames counted : %d\n", counter);
		printf("***************************************\n");
		Sys_Quit();
	}

	if ((NumFrames != -1) && (framecount > NumFrames)) Sys_Quit(); //GAJA stop execution after NumFrames frames
	
 
	DEBUGPRINT(GENERAL, LOW) printf("\n\n=============================================================================================\n");
	DEBUGPRINT(GENERAL, LOW) printf("/////////////////////////////////////////////////////////////////////////////////////////////\n");
	DEBUGPRINT(GENERAL, LOW) printf("      FRAME %d  \n", framecount);
	DEBUGPRINT(GENERAL, LOW) printf("/////////////////////////////////////////////////////////////////////////////////////////////\n");
	DEBUGPRINT(GENERAL, LOW) printf("=============================================================================================\n");

// ==================================================================================================

	// GAJA: Time spent for calculation of the previous frame
	calculation_time = TO_MILISEC(end - start);	// keep it in miliseconds
	
	// GAJA: DO not include peek values in performance results. 
	// If there are a lot of disbanded values try increasing the frame time. 
	// This could also mean that there is some problem in the runtime system
	if (calculation_time > FRAMETIME*1000) 
		doInclude = false;
		
	if ( (total_calc_time > 0.0) && (calculation_time > (3 * total_calc_time / counter)) )
		doInclude = false;
	
	if (doInclude)
	{
		total_calc_time += calculation_time;
		counter++;
	}
		
	while ((rdtsc() - start) < WAITTIME(floor(1000 * FRAMETIME)));   // wait for the frametime
	start = rdtsc ();
	frametime = TO_SECONDS(start - oldtime);	// GAJA: Measured frametime. Should be equal to FRAMETIME
    if (frametime > 1.0) frametime = 1.0;
	oldtime = start;
	svs.stats.idle += TO_SECONDS(start - end);

	// keep the random time dependent
	rand ();	// GAJA: Leftover form the original code

	framecount++;
	srv_frametime_avg = FRAMETIME;

	// decide the simulation time
	if (!sv.paused) {
		realtime += frametime;	//FRAMETIME;
		sv.time += frametime;	//FRAMETIME;
	}

    DEBUGPRINT(GENERAL, LOW) printf("TIME: calculation_time: %f\tframetime: %f\tsrv_frametime_avg: %f\n\n", 
            calculation_time, frametime, srv_frametime_avg);


	// check timeouts
	SV_CheckTimeouts ();

	// toggle the log buffer if full
	SV_CheckLog ();



	/*
	++++++++++++++++++++++++++++++++++
		   GAJA parallelization

	  paralelize 3 sections in order

	++++++++++++++++++++++++++++++++++
	*/


// PHYSICS
// -----------------------------------------------------------------------
	// move autonomous things around if enough time has passed
	FrameStage = PHYSICS;
	if (doInclude) 
		total_phy_time += TO_MILISEC(physics_time);
	physics_time = rdtsc ();
	if (!sv.paused)
		SV_Physics ();
	physics_time = rdtsc () - physics_time;

	DEBUGPRINT(GENERAL, MEDIUM) PrintGameState();


// REQUEST
// -----------------------------------------------------------------------
	DEBUGPRINT(GENERAL, LOW) printf("\n\nREAD PACKETS\n------------------------------\n");
	// get packets
	FrameStage = READ_PACKETS;
	if (doInclude) 
	{
		total_req_time += TO_MILISEC(request_time);
		requests_total += packetsread;
	}
	packetsread = 0;
	request_time = rdtsc ();
	SV_ReadPackets ();
	request_time = rdtsc () - request_time;


	FrameStage = GENERAL;
	// HOST CONSOLE
	// check for commands typed to the host
	SV_GetConsoleCommands ();	
	// process console commands
	Cbuf_Execute ();
	SV_CheckVars ();



// REPLIES
// -----------------------------------------------------------------------
	DEBUGPRINT(GENERAL, LOW) printf("\n\nSEND MESSAGES\n------------------------------\n\n");
	// send messages back to the clients that had packets read this frame
	FrameStage = SEND_REPLY;
	if (doInclude) 
	{
		total_reply_time += TO_MILISEC(reply_time);
		replies_total += packetssent;
	}
	packetssent = 0;
	reply_time = rdtsc ();
	SV_SendClientMessages ();
	reply_time = rdtsc () - reply_time;

	FrameStage = GENERAL;
	DEBUGPRINT(GENERAL, LOW) printf("\n\nPACKETS SENT : %d\n\n", packetssent);


// -----------------------------------------------------------------------

	DEBUGPRINT(GENERAL, LOW) printf("\nRequest time: %f\tphysics time: %f\treply time: %f\n\n", TO_MILISEC(request_time), TO_MILISEC(physics_time), TO_MILISEC(reply_time));
	
// send a heartbeat to the master if needed
	Master_Heartbeat ();

// collect timing statistics
	end = rdtsc ();
	svs.stats.active += TO_SECONDS(end-start);
	if (++svs.stats.count == STATFRAMES)
	{
		svs.stats.latched_active = svs.stats.active;
		svs.stats.latched_idle = svs.stats.idle;
		svs.stats.latched_packets = svs.stats.packets;
		svs.stats.active = 0;
		svs.stats.idle = 0;
		svs.stats.packets = 0;
		svs.stats.count = 0;
	}

}



/*
===============
SV_InitLocal
===============
*/
void SV_InitLocal (void)
{
	int		i;
	extern	cvar_t	sv_maxvelocity;
	extern	cvar_t	sv_gravity;
	extern	cvar_t	sv_aim;
	extern	cvar_t	sv_stopspeed;
	extern	cvar_t	sv_spectatormaxspeed;
	extern	cvar_t	sv_accelerate;
	extern	cvar_t	sv_airaccelerate;
	extern	cvar_t	sv_wateraccelerate;
	extern	cvar_t	sv_friction;
	extern	cvar_t	sv_waterfriction;

	SV_InitOperatorCommands	();
	SV_UserInit ();
	
	Cvar_RegisterVariable (&rcon_password);
	Cvar_RegisterVariable (&password);
	Cvar_RegisterVariable (&spectator_password);
	Cvar_RegisterVariable (&sv_mintic);
	Cvar_RegisterVariable (&sv_maxtic);
	Cvar_RegisterVariable (&fraglimit);
	Cvar_RegisterVariable (&timelimit);
	Cvar_RegisterVariable (&teamplay);
	Cvar_RegisterVariable (&samelevel);
	Cvar_RegisterVariable (&maxclients);
	Cvar_RegisterVariable (&maxspectators);
	Cvar_RegisterVariable (&hostname);
	Cvar_RegisterVariable (&deathmatch);
	Cvar_RegisterVariable (&spawn);
	Cvar_RegisterVariable (&watervis);
	Cvar_RegisterVariable (&developer);
	Cvar_RegisterVariable (&timeout);
	Cvar_RegisterVariable (&zombietime);
	Cvar_RegisterVariable (&sv_maxvelocity);
	Cvar_RegisterVariable (&sv_gravity);
	Cvar_RegisterVariable (&sv_stopspeed);
	Cvar_RegisterVariable (&sv_maxspeed);
	Cvar_RegisterVariable (&sv_spectatormaxspeed);
	Cvar_RegisterVariable (&sv_accelerate);
	Cvar_RegisterVariable (&sv_airaccelerate);
	Cvar_RegisterVariable (&sv_wateraccelerate);
	Cvar_RegisterVariable (&sv_friction);
	Cvar_RegisterVariable (&sv_waterfriction);
	Cvar_RegisterVariable (&sv_aim);
	Cvar_RegisterVariable (&filterban);
	Cvar_RegisterVariable (&allow_download);
	Cvar_RegisterVariable (&allow_download_skins);
	Cvar_RegisterVariable (&allow_download_models);
	Cvar_RegisterVariable (&allow_download_sounds);
	Cvar_RegisterVariable (&allow_download_maps);
	Cvar_RegisterVariable (&sv_highchars);
	Cvar_RegisterVariable (&sv_phs);
	Cvar_RegisterVariable (&pausable);

	Cmd_AddCommand ("addip", SV_AddIP_f);
	Cmd_AddCommand ("removeip", SV_RemoveIP_f);
	Cmd_AddCommand ("listip", SV_ListIP_f);
	Cmd_AddCommand ("writeip", SV_WriteIP_f);

	for (i=0 ; i<MAX_MODELS ; i++)
		sprintf (localmodels[i], "*%i", i);

	Info_SetValueForStarKey (svs.info, "*version", va("%4.2f", VERSION), MAX_SERVERINFO_STRING);

	// init fraglog stuff
	svs.logsequence = 1;
	svs.logtime = realtime;
	svs.log[0].data = svs.log_buf[0];
	svs.log[0].maxsize = sizeof(svs.log_buf[0]);
	svs.log[0].cursize = 0;
	svs.log[0].allowoverflow = true;
	svs.log[1].data = svs.log_buf[1];
	svs.log[1].maxsize = sizeof(svs.log_buf[1]);
	svs.log[1].cursize = 0;
	svs.log[1].allowoverflow = true;

  	// GAJA:
  	inTransaction = false;
}


//============================================================================

/*
================
Master_Heartbeat

Send a message to the master every few minutes to
let it know we are alive, and log information
================
*/
#define	HEARTBEAT_SECONDS	300
void Master_Heartbeat (void)
{
	char		string[2048];
	int			active;
	int			i;

	if (realtime - svs.last_heartbeat < HEARTBEAT_SECONDS)
		return;		// not time to send yet

	svs.last_heartbeat = realtime;

	//
	// count active users
	//
	active = 0;
	for (i=0 ; i<MAX_CLIENTS ; i++)
		if (svs.clients[i].state == cs_connected ||
		svs.clients[i].state == cs_spawned )
			active++;

	svs.heartbeat_sequence++;
	sprintf (string, "%c\n%i\n%i\n", S2M_HEARTBEAT,
		svs.heartbeat_sequence, active);


	// send to group master
	for (i=0 ; i<MAX_MASTERS ; i++)
		if (master_adr[i].port)
		{
			Con_Printf ("Sending heartbeat to %s\n", NET_AdrToString (master_adr[i]));
			NET_SendPacket (strlen(string), string, master_adr[i]);
		}
}

/*
=================
Master_Shutdown

Informs all masters that this server is going down
=================
*/
void Master_Shutdown (void)
{
	char		string[2048];
	int			i;

	sprintf (string, "%c\n", S2M_SHUTDOWN);

	// send to group master
	for (i=0 ; i<MAX_MASTERS ; i++)
		if (master_adr[i].port)
		{
			Con_Printf ("Sending heartbeat to %s\n", NET_AdrToString (master_adr[i]));
			NET_SendPacket (strlen(string), string, master_adr[i]);
		}
}

/*
=================
SV_ExtractFromUserinfo

Pull specific info from a newly changed userinfo string
into a more C freindly form.
=================
*/
TM_CALLABLE
void SV_ExtractFromUserinfo (client_t *cl)
{
	char	*val, *p, *q;
	int		i;
	client_t	*client;
	int		dupc = 1;
	char	newname[80];


	// name for C code
	val = Info_ValueForKey (cl->userinfo, "name");

	// trim user name
	Q_strncpy(newname, val, sizeof(newname) - 1);
	newname[sizeof(newname) - 1] = 0;

	for (p = newname; (*p == ' ' || *p == '\r' || *p == '\n') && *p; p++)
		;

	if (p != newname && !*p) {
		//white space only
		Q_strcpy(newname, "unnamed");
		p = newname;
	}

	if (p != newname && *p) {
		for (q = newname; *p; *q++ = *p++)
			;
		*q = 0;
	}
	for (p = newname + Q_strlen(newname) - 1; p != newname && (*p == ' ' || *p == '\r' || *p == '\n') ; p--)
		;
	p[1] = 0;

	if (Q_strcmp(val, newname)) {
		Info_SetValueForKey (cl->userinfo, "name", newname, MAX_INFO_STRING);
		val = Info_ValueForKey (cl->userinfo, "name");
	}

	if (!val[0] || !Q_strcasecmp(val, "console")) {
		Info_SetValueForKey (cl->userinfo, "name", "unnamed", MAX_INFO_STRING);
		val = Info_ValueForKey (cl->userinfo, "name");
	}

	// check to see if another user by the same name exists
	while (1) {
		for (i=0, client = svs.clients ; i<MAX_CLIENTS ; i++, client++) {
			if (client->state != cs_spawned || client == cl)
				continue;
			if (!Q_strcasecmp(client->name, val))
				break;
		}
		if (i != MAX_CLIENTS) { // dup name
			if (Q_strlen(val) > sizeof(cl->name) - 1)
				val[sizeof(cl->name) - 4] = 0;
			p = val;

			if (val[0] == '(')
			{
				if (val[2] == ')')
					p = val + 3;
				else if (val[3] == ')')
					p = val + 4;
			}

			Q_sprintf(newname, "(%d)%-.40s", dupc++, p);  
			Info_SetValueForKey (cl->userinfo, "name", newname, MAX_INFO_STRING);
			val = Info_ValueForKey (cl->userinfo, "name");
		} else
			break;
	}
	
	if (Q_strncmp(val, cl->name, Q_strlen(cl->name))) {
		if (!sv.paused) {
			if (!cl->lastnametime || realtime - cl->lastnametime > 5) {
				cl->lastnamecount = 0;
				cl->lastnametime = realtime;
			} else if (cl->lastnamecount++ > 4) {
				SV_BroadcastPrintf (PRINT_HIGH, "%s was kicked for name spam\n", cl->name);
				SV_ClientPrintf (cl, PRINT_HIGH, "You were kicked from the game for name spamming\n");
				SV_DropClient (cl); 
				return;
			}
		}
				
		if (cl->state >= cs_spawned && !cl->spectator)
			SV_BroadcastPrintf (PRINT_HIGH, "%s changed name to %s\n", cl->name, val);
	}


	Q_strncpy (cl->name, val, sizeof(cl->name)-1);	

	// rate command
	val = Info_ValueForKey (cl->userinfo, "rate");
	if (Q_strlen(val))
	{
		i = Q_atoi(val);
		if (i < 500)
			i = 500;
		if (i > 10000)
			i = 10000;
		cl->netchan.rate = 1.0/i;
	}

	// msg command
	val = Info_ValueForKey (cl->userinfo, "msg");
	if (Q_strlen(val))
	{
		cl->messagelevel = Q_atoi(val);
	}

}


//============================================================================

/*
====================
SV_InitNet
====================
*/
void SV_InitNet (void)
{
	int	port;
	int	p;

	port = PORT_SERVER;
	p = COM_CheckParm ("-port");
	if (p && p < com_argc)
	{
		port = atoi(com_argv[p+1]);
		Con_Printf ("Port: %i\n", port);
	}
	NET_Init (port);

	Netchan_Init ();

	// heartbeats will allways be sent to the id master
	svs.last_heartbeat = -99999;		// send immediately
//	NET_StringToAdr ("192.246.40.70:27000", &idmaster_adr);
}


/*
====================
SV_Init
====================
*/
void SV_GetParameters(void)
{
    //GAJA: Check for the number of threads argument
    int t_index = COM_CheckParm ("-threads");
    if (t_index > 0)
    {
        NoThreads = (int) Q_atoi(com_argv[t_index+1]);
        if (NoThreads < 1) NoThreads = 1;
    }
    else
        NoThreads = 1;
    Con_Printf ("Running with %d threads\n", NoThreads);

    //GAJA: Check for the number of frames to run
    // -1 means indefinitly
    int f_index = COM_CheckParm ("-frames");
    if (f_index > 0)
    {
        NumFrames = (int) Q_atoi(com_argv[f_index+1]);
        if (NumFrames < -1) NumFrames = -1;
        Con_Printf ("Run %d frames\n", NumFrames);
    }
    else
    { 
        NumFrames = -1;
        Con_Printf ("Run indefinitely\n");
    }

    // GAJA: Set frame markers for performance measuring
    f_index = COM_CheckParm ("-start");
    if (f_index > 0)
        start_frame = (int) Q_atoi(com_argv[f_index+1]);
    else 
        start_frame = -1;

    f_index = COM_CheckParm ("-end");
    if (f_index > 0)
        end_frame = (int) Q_atoi(com_argv[f_index+1]);
    else 
        end_frame = -1;

    if ( (start_frame > 0) || (end_frame > 0)) 
    {
        if (start_frame > end_frame)
        {
            Sys_Printf("Wrong frame markers ... exiting!!!\n");
            Sys_Quit();
        }
    }

}

//Gaja: Initialize server
void SV_Init (quakeparms_t *parms)
{
	INIT_NEST_LOCK(memory_lock)

	COM_InitArgv (parms->argc, parms->argv);
	COM_AddParm ("-game");
	COM_AddParm ("qw");

	if (COM_CheckParm ("-minmemory"))
		parms->memsize = MINIMUM_MEMORY;

	host_parms = *parms;

	if (parms->memsize < MINIMUM_MEMORY)
		SV_Error ("Only %4.1f megs of memory reported, can't execute game", parms->memsize / (float)0x100000);

	Memory_Init (parms->membase, parms->memsize);
	Cbuf_Init ();
	Cmd_Init ();	

	COM_Init ();

    //GAJA:
	SV_GetParameters();
    INIT_LOCK(global_lock)
	INIT_LOCK(sv_reldatagram_lock)
	INIT_LOCK(sv_datagram_lock)
	INIT_LOCK(sv_signon_lock)
	INIT_LOCK(sv_multicast_lock)
	INIT_LOCK(areanode_lock)	// GAJA: uncomment if global lock for locking the areanode tree
	INIT_NEST_LOCK(edict_lock)
    int i;
    for (i=0 ; i<MAX_CLIENTS; i++)
    {
        INIT_LOCK(svs.clients[i].msg_lock)
    }

    Init_LockFlags();
	
	PR_Init();
	Mod_Init ();

	SV_InitNet ();

	SV_InitLocal ();
	Sys_Init ();
	Pmove_Init ();

	TM_IOInit ();

	Hunk_AllocName (0, "-HOST_HUNKLEVEL-");
	host_hunklevel = Hunk_LowMark ();

	Cbuf_InsertText ("exec server.cfg\n");

	host_initialized = true;
	
	Con_Printf ("Exe: "__TIME__" "__DATE__"\n");
	Con_Printf ("%4.1f megabyte heap\n",parms->memsize/ (1024*1024.0));	

	Con_Printf ("\nServer Version %4.2f (Build %04d)\n\n", VERSION, build_number());

	Con_Printf ("======== QuakeWorld Initialized ========\n");


	// process command line arguments
	Cmd_StuffCmds_f ();
	Cbuf_Execute ();

	// if a map wasn't specified on the command line, spawn start.map
	if (sv.state == ss_dead)
		Cmd_ExecuteString ("map start");
	if (sv.state == ss_dead)
		SV_Error ("Couldn't spawn a server");
}


/*
====================
PrintGameState
====================
*/
//Gaja: Print the state of player entities.
void PrintGameState(void)
{
    int i;
    client_t		*cl;
    static vec3_t old_origin[MAX_CLIENTS];
    static char	flag = 0;
    char			state[150];

    //Init old_origin
    if (flag == 0)
    {
        flag = 1;
        for (i=0; i<MAX_CLIENTS ; i++) 
        {
            old_origin[i][0] = 0;
            old_origin[i][1] = 0;
            old_origin[i][2] = 0;
        }
    }


    for (i=0, cl=svs.clients ; i<MAX_CLIENTS ; i++,cl++) 
    {
        switch (cl->state)
        {
            case cs_free:
                sprintf(state, "free\t");
                break;
            case cs_zombie:
                sprintf(state, "zombie\t");
                printf("%d\t%s\n", i, state);
                break;
            case cs_connected:
                sprintf(state, "connected\t");
                sprintf(state, "%s%s\t", state, cl->name);
                printf("%d\t%s\n", i, state);
                break;
            case cs_spawned:
                sprintf(state, "spawned\t");
                sprintf(state, "%s%s\t\t", state, cl->name);
                if ((cl->edict->v.origin[0] != old_origin[i][0]) 
                        || (cl->edict->v.origin[1] != old_origin[i][1]) 
                        || (cl->edict->v.origin[2] != old_origin[i][2]))
                {
                    sprintf(state, "%smoved : ( %f , %f , %f ) ", state, old_origin[i][0], old_origin[i][1], old_origin[i][2]);
                    sprintf(state, "%s-> ( %f , %f , %f )", state, cl->edict->v.origin[0], cl->edict->v.origin[1], cl->edict->v.origin[2]);
                    old_origin[i][0] = cl->edict->v.origin[0];
                    old_origin[i][1] = cl->edict->v.origin[1];
                    old_origin[i][2] = cl->edict->v.origin[2];
                }
                printf("%d\t%s\n", i, state);
                break;
        }

    }
}


/*
==================================
GAJA:
Transaction checkpointing
==================================
*/
TM_PURE
void DoCheckpoint(int point)
{
	checkpoint[GetThreadId()][point]++;
}

void ClearCheckpoints(void)
{
	int i,j;
	for (i=0; i<MAXNUMTHR; i++)
	{
		for (j=0; j<NUMCHECKS; j++)
		{
			checkpoint[i][j] = 0;
		}
	}
}

void PrintCheckpoints(void)
{
	int i,j;
	for (i=0; i<NoThreads; i++)
	{
		printf("\nThread %d\n------------------------\n\n", i);
		for (j=0; j<NUMCHECKS; j++)
		{
			printf("%d:\t%d\n", j, checkpoint[i][j]);
		}
	}
}


/*
==================================
GAJA:
Initialize locking flags
==================================
*/
void Init_LockFlags(void)
{
	int i,j;
	
	#pragma omp parallel for schedule(static,1)
	for ( i=0 ; i<NoThreads ; i++)
	{
		for ( j=0 ; j < 8 ; j++)		
			msg_nest_level[j] = 0;
		for ( j=0 ; j < 32 ; j++)
			client_nest_level[j] = 0;
	}
}


