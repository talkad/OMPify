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


#include "qwsvdef.h"

#define	RETURN_EDICT(e) (((int *)pr_globals)[OFS_RETURN] = EDICT_TO_PROG(e))
#define	RETURN_STRING(s) (((int *)pr_globals)[OFS_RETURN] = PR_SetString(s))

#define MSG_LEVELS 			4		// there are 4 message levels in progs
#define TEMP_MSG_LENGTH		200

// THREADPRIVATE -------------------------------------------------

char        bprint_msg[MSG_LEVELS][TEMP_MSG_LENGTH];             
char        sprint_msg[MAX_CLIENTS][MSG_LEVELS][TEMP_MSG_LENGTH];
byte        cl_msg_buff[MAX_CLIENTS][MAX_MSGLEN];
byte        signon_msg_buff[MAX_DATAGRAM];
byte        sv_datagram_msg_buff[MAX_DATAGRAM];     
byte        sv_rel_datagram_msg_buff[MAX_MSGLEN];   
byte        multicast_msg_buff[MAX_MSGLEN]; 
THREADPRIVATE(bprint_msg, sprint_msg, cl_msg_buff, signon_msg_buff, sv_datagram_msg_buff, sv_rel_datagram_msg_buff, multicast_msg_buff)

sizebuf_t   cl_msg[MAX_CLIENTS];
sizebuf_t   signon_msg;             // MSG_INIT
sizebuf_t   sv_datagram_msg;        // MSG_BROADCAST
sizebuf_t   sv_rel_datagram_msg;    // MSG_ALL
sizebuf_t   multicast_msg;          // MSG_MULTICAST 
THREADPRIVATE(cl_msg, signon_msg, sv_datagram_msg, sv_rel_datagram_msg, multicast_msg)

extern int				pr_argc;
extern dfunction_t*		pr_xfunction;
THREADPRIVATE(pr_argc, pr_xfunction)

// ----------------------------------------------------------------


// GAJA Function declarations

TM_CALLABLE
void PF_Fixme (void);
TM_CALLABLE
void PF_makevectors (void);
TM_CALLABLE
void PF_setorigin (void);
TM_CALLABLE
void PF_setmodel (void);
TM_CALLABLE
void PF_setsize (void);
TM_CALLABLE
void PF_break (void);
TM_CALLABLE
void PF_random (void);
TM_CALLABLE
void PF_sound (void);
TM_CALLABLE
void PF_normalize (void);
TM_PURE
void PF_error (void);
TM_PURE
void PF_objerror (void);
TM_CALLABLE
void PF_vlen (void);
TM_CALLABLE
void PF_vectoyaw (void);
TM_CALLABLE
void PF_Spawn (void);
TM_CALLABLE
void PF_Remove (void);
TM_CALLABLE
void PF_Find (void);
TM_CALLABLE
void PF_traceline (void);
TM_CALLABLE
void PF_checkclient (void);
TM_CALLABLE
int PF_newcheckclient (int check);
TM_CALLABLE
void PF_precache_sound (void);
TM_CALLABLE
void PF_precache_model (void);
TM_CALLABLE
void PF_stuffcmd (void);
TM_CALLABLE
void PF_findradius (void);
TM_CALLABLE
void PF_bprint (void);
TM_CALLABLE
void PF_sprint (void);
TM_CALLABLE
void PF_dprint (void);
TM_CALLABLE
void PF_ftos (void);
TM_CALLABLE
void PF_vtos (void);
TM_PURE
void PF_coredump (void);
TM_CALLABLE
void PF_traceon (void);
TM_CALLABLE
void PF_traceoff (void);
TM_PURE
void PF_eprint (void);
TM_CALLABLE
void PF_walkmove (void);
TM_CALLABLE
void PF_droptofloor (void);
TM_CALLABLE
void PF_lightstyle (void);
TM_CALLABLE
void PF_checkbottom (void);
TM_CALLABLE
void PF_pointcontents (void);
TM_CALLABLE
void PF_fabs (void);
TM_CALLABLE
void PF_aim (void);
TM_CALLABLE
void PF_cvar (void);
TM_CALLABLE
void PF_localcmd (void);
TM_CALLABLE
void PF_nextent (void);
TM_CALLABLE
void PF_changeyaw (void);
TM_CALLABLE
void PF_vectoangles (void);

TM_CALLABLE
void PF_WriteByte (void);
TM_CALLABLE
void PF_WriteChar (void);
TM_CALLABLE
void PF_WriteShort (void);
TM_CALLABLE
void PF_WriteLong (void);
TM_CALLABLE
void PF_WriteAngle (void);
TM_CALLABLE
void PF_WriteCoord (void);
TM_CALLABLE
void PF_WriteString (void);
TM_CALLABLE
void PF_WriteEntity (void);

TM_CALLABLE
void PF_precache_file (void);
TM_CALLABLE
void PF_makestatic (void);
TM_CALLABLE
void PF_changelevel (void);
TM_CALLABLE
void PF_cvar_set (void);
TM_CALLABLE
void PF_centerprint (void);
TM_CALLABLE
void PF_ambientsound (void);

TM_CALLABLE
void PF_setspawnparms (void);
TM_PURE
void PF_logfrag (void);
TM_CALLABLE
void PF_infokey (void);
TM_CALLABLE
void PF_stof (void);
TM_CALLABLE
void PF_multicast (void);


TM_CALLABLE
char *PF_VarString (int	first);
TM_CALLABLE
void PR_CheckEmptyString (char *s);
TM_CALLABLE
static client_t *Write_GetClient(void);
TM_CALLABLE
sizebuf_t *WriteDest (void);

TM_CALLABLE
void ClearTempBuffers(void);
TM_CALLABLE
void WriteTempBuffers(void);


/*
=================
ClearTempBuffers
=================
*/
TM_CALLABLE
void ClearTempBuffers(void)
{
	int i;
    client_t *cl;

    Q_memset(bprint_msg, 0, MSG_LEVELS * TEMP_MSG_LENGTH);

	for(i=0, cl = svs.clients ; i<MAX_CLIENTS; i++, cl++)
	{
        if (cl->state == cs_free)
            continue;
           
        Q_memset(sprint_msg[i], 0, MSG_LEVELS * TEMP_MSG_LENGTH);

        cl_msg[i].data = cl_msg_buff[i];
        cl_msg[i].maxsize = sizeof(cl_msg_buff[i]);
        SZ_Clear (&cl_msg[i]);
    }

    // Init sizebuf pointers
    signon_msg.data = signon_msg_buff;
    signon_msg.maxsize = sizeof(signon_msg_buff);
    sv_datagram_msg.data = sv_datagram_msg_buff;
    sv_datagram_msg.maxsize = sizeof(sv_datagram_msg_buff);
    sv_rel_datagram_msg.data = sv_rel_datagram_msg_buff;
    sv_rel_datagram_msg.maxsize = sizeof(sv_rel_datagram_msg_buff);
    multicast_msg.data = multicast_msg_buff;
    multicast_msg.maxsize = sizeof(multicast_msg_buff);

    SZ_Clear (&signon_msg);
	SZ_Clear (&sv_datagram_msg);
	SZ_Clear (&sv_rel_datagram_msg);
	SZ_Clear (&multicast_msg);
}

/*
=================
WriteTempBuffers

Writes temporary buffers to real destrination buffers
This is where the locks are acquired
=================
*/
TM_CALLABLE
void WriteTempBuffers(void)
{
	int i, j;
	client_t *cl;

	// bprint
	for(j=3; j>=0; j--)
		if (bprint_msg[j][0] != 0) 
			SV_BroadcastPrintf( j , "%s", bprint_msg[j]);	// Start from the highest level

	// sprint
	for(i=0 ; i<MAX_CLIENTS; i++)
	{
		for(j=0; j<4; j++)
			if (sprint_msg[i][j][0] != 0) 
				SV_ClientPrintf(&svs.clients[i], j,  "%s", sprint_msg[i][j]);
	}

	// client reliable
	for(i=0 ; i<MAX_CLIENTS; i++)
	{
		if (cl_msg[i].cursize > 0)
		{
			cl = &svs.clients[i];

			CRITICAL_MESSAGE(cl->msg_lock, CLIENT_MSG_LOCK, cl->clientId, {
				ClientReliableCheckBlock(cl, cl_msg[i].cursize);
                //printf("Write to client buffer\tsize= %d\n", cl_msg[i].cursize);
				if (cl->num_backbuf) 
				{
					SZ_Write(&cl->backbuf , cl_msg[i].data, cl_msg[i].cursize);
					ClientReliable_FinishWrite(cl);
				}
				else
					SZ_Write(&cl->netchan.message , cl_msg[i].data, cl_msg[i].cursize);
			});
		}
	}

	// MSG_INIT
	CRITICAL_MESSAGE(sv_signon_lock, SV_SIGNON_LOCK, 0, {
		SZ_Write(&sv.signon, signon_msg.data, signon_msg.cursize);
	});

	// MSG_BROADCAST
	CRITICAL_MESSAGE(sv_datagram_lock, SV_DATAGRAM_LOCK, 0, {
		SZ_Write(&sv.datagram, sv_datagram_msg.data, sv_datagram_msg.cursize);
	});

	// MSG_ALL
	CRITICAL_MESSAGE(sv_reldatagram_lock, SV_RELDATAGRAM_LOCK, 0, { 
		SZ_Write(&sv.reliable_datagram, sv_rel_datagram_msg.data, sv_rel_datagram_msg.cursize);
	}); 

	// MSG_MULTICAST
	CRITICAL_MESSAGE(sv_multicast_lock, SV_MULTICAST_LOCK, 0, {
		SZ_Write(&sv.multicast, multicast_msg.data, multicast_msg.cursize);	
	});

   	//ClearTempBuffers();
}


/*
===============================================================================

						BUILT-IN FUNCTIONS

===============================================================================
*/
 
// THREADPRIVATE STATIC ---------------------------------------------

static char thr_out[256];
THREADPRIVATE(thr_out)

// ------------------------------------------------------------------

TM_CALLABLE
char *PF_VarString (int	first)
{
	int		i;
	//static char out[256];	// GAJA: STATIC
	
	thr_out[0] = 0;
	for (i=first ; i<pr_argc ; i++)
	{
		Q_strcat (thr_out, G_STRING((OFS_PARM0+i*3)));
	}
	return thr_out;
}


/*
=================
PF_errror

This is a TERMINAL error, which will kill off the entire server.
Dumps self.

error(value)
=================
*/
TM_PURE
void PF_error (void)
{
	char	*s;
	edict_t	*ed;
	
	s = PF_VarString(0);
	Con_Printf ("======SERVER ERROR in %s:\n%s\n", PR_GetString(pr_xfunction->s_name) ,s);
	ed = PROG_TO_EDICT(pr_global_struct->self);
	ED_Print (ed);

	SV_Error ("Program error");
}

/*
=================
PF_objerror

Dumps out self, then an error message.  The program is aborted and self is
removed, but the level can continue.

objerror(value)
=================
*/
TM_PURE
void PF_objerror (void)
{
	char	*s;
	edict_t	*ed;
	
	s = PF_VarString(0);
	Con_Printf ("======OBJECT ERROR in %s:\n%s\n", PR_GetString(pr_xfunction->s_name),s);
	ed = PROG_TO_EDICT(pr_global_struct->self);
	ED_Print (ed);
	ED_Free (ed);
	
	SV_Error ("Program error");
}



/*
==============
PF_makevectors

Writes new values for v_forward, v_up, and v_right based on angles
makevectors(vector)
==============
*/
TM_CALLABLE
void PF_makevectors (void)
{
	AngleVectors (G_VECTOR(OFS_PARM0), pr_global_struct->v_forward, pr_global_struct->v_right, pr_global_struct->v_up);
}

/*
=================
PF_setorigin

This is the only valid way to move an object without using the physics of the world 
(setting velocity and waiting).  Directly changing origin will not set internal links correctly, 
so clipping would be messed up.  This should be called when an object is spawned, and then 
only if it is teleported.

setorigin (entity, origin)
=================
*/
TM_CALLABLE
void PF_setorigin (void)
{
	edict_t	*e;
	float	*org;
	
	e = G_EDICT(OFS_PARM0);
	org = G_VECTOR(OFS_PARM1);
	VectorCopy (org, e->v.origin);
	SV_LinkEdict (e, false);
}


/*
=================
PF_setsize

the size box is rotated by the current angle

setsize (entity, minvector, maxvector)
=================
*/
TM_CALLABLE
void PF_setsize (void)
{
	edict_t	*e;
	float	*min, *max;
	
	e = G_EDICT(OFS_PARM0);
	min = G_VECTOR(OFS_PARM1);
	max = G_VECTOR(OFS_PARM2);
	VectorCopy (min, e->v.mins);
	VectorCopy (max, e->v.maxs);
	VectorSubtract (max, min, e->v.size);
	SV_LinkEdict (e, false);
}


/*
=================
PF_setmodel

setmodel(entity, model)
Also sets size, mins, and maxs for inline bmodels
=================
*/
TM_CALLABLE
void PF_setmodel (void)
{
	edict_t	*e;
	char	*m, **check;
	int		i;
	model_t	*mod;

	e = G_EDICT(OFS_PARM0);
	m = G_STRING(OFS_PARM1);

// check to see if model was properly precached
	for (i=0, check = sv.model_precache ; *check ; i++, check++)
		if (!Q_strcmp(*check, m))
			break;

	if (!*check)
		PR_RunError ("no precache: %s\n", m);
		
	e->v.model = PR_SetString(m);
	e->v.modelindex = i;

// if it is an inline model, get the size information for it
	if (m[0] == '*')
	{
		mod = Mod_ForName (m, true);
		VectorCopy (mod->mins, e->v.mins);
		VectorCopy (mod->maxs, e->v.maxs);
		VectorSubtract (mod->maxs, mod->mins, e->v.size);
		SV_LinkEdict (e, false);
	}

}

/*
=================
PF_bprint

broadcast print to everyone on server

bprint(value)
=================
*/
TM_CALLABLE
void PF_bprint (void)
{
	char		*s;
	int			level;
	
	level = G_FLOAT(OFS_PARM0);

	s = PF_VarString(1);

	// GAJA: 
	if (FrameStage & DOPARALLEL) 
	{	
		sprintf( bprint_msg[level], "%s%s" , bprint_msg[level],  s);
		return;
	}	

	SV_BroadcastPrintf (level, "%s", s);
}

/*
=================
PF_sprint

single print to a specific client

sprint(clientent, value)
=================
*/
TM_CALLABLE
void PF_sprint (void)
{
	char		*s;
	client_t	*client;
	int			entnum;
	int			level;
	
	entnum = G_EDICTNUM(OFS_PARM0);
	level = G_FLOAT(OFS_PARM1);

	s = PF_VarString(2);
	
	if (entnum < 1 || entnum > MAX_CLIENTS)
	{
		Con_Printf ("tried to sprint to a non-client\n");
		return;
	}

	// GAJA: 
	if (FrameStage & DOPARALLEL) 
	{	
		sprintf( sprint_msg[entnum-1][level], "%s%s" , sprint_msg[entnum-1][level], s);
		return;
	}
		
	client = &svs.clients[entnum-1];
	
	SV_ClientPrintf (client, level, "%s", s);
}


/*
=================
PF_centerprint

single print to a specific client

centerprint(clientent, value)
=================
*/
TM_CALLABLE
void PF_centerprint (void)
{
	char		*s;
	int			entnum;
	client_t	*cl;
	
	entnum = G_EDICTNUM(OFS_PARM0);
	s = PF_VarString(1);
	
	if (entnum < 1 || entnum > MAX_CLIENTS)
	{
		Con_Printf ("tried to sprint to a non-client\n");
		return;
	}
		
	cl = &svs.clients[entnum-1];

	CRITICAL_MESSAGE(cl->msg_lock, CLIENT_MSG_LOCK, cl->clientId, {
		ClientReliableWrite_Begin (cl, svc_centerprint, 2 + Q_strlen(s));
		ClientReliableWrite_String (cl, s);
	});
}


/*
=================
PF_normalize

vector normalize(vector)
=================
*/
TM_CALLABLE
void PF_normalize (void)
{
	float	*value1;
	vec3_t	newvalue;
	float	new;
	
	value1 = G_VECTOR(OFS_PARM0);

	new = value1[0] * value1[0] + value1[1] * value1[1] + value1[2]*value1[2];
	new = Q_sqrt(new);
	
	if (new == 0)
		newvalue[0] = newvalue[1] = newvalue[2] = 0;
	else
	{
		new = 1/new;
		newvalue[0] = value1[0] * new;
		newvalue[1] = value1[1] * new;
		newvalue[2] = value1[2] * new;
	}
	
	VectorCopy (newvalue, G_VECTOR(OFS_RETURN));	
}

/*
=================
PF_vlen

scalar vlen(vector)
=================
*/
TM_CALLABLE
void PF_vlen (void)
{
	float	*value1;
	float	new;
	
	value1 = G_VECTOR(OFS_PARM0);

	new = value1[0] * value1[0] + value1[1] * value1[1] + value1[2]*value1[2];
	new = Q_sqrt(new);
	
	G_FLOAT(OFS_RETURN) = new;
}

/*
=================
PF_vectoyaw

float vectoyaw(vector)
=================
*/
TM_CALLABLE
void PF_vectoyaw (void)
{
	float	*value1;
	float	yaw;
	
	value1 = G_VECTOR(OFS_PARM0);

	if (value1[1] == 0 && value1[0] == 0)
		yaw = 0;
	else
	{
		yaw = (int) (Q_atan2(value1[1], value1[0]) * 180 / M_PI);
		if (yaw < 0)
			yaw += 360;
	}

	G_FLOAT(OFS_RETURN) = yaw;
}


/*
=================
PF_vectoangles

vector vectoangles(vector)
=================
*/
TM_CALLABLE
void PF_vectoangles (void)
{
	float	*value1;
	float	forward;
	float	yaw, pitch;
	
	value1 = G_VECTOR(OFS_PARM0);

	if (value1[1] == 0 && value1[0] == 0)
	{
		yaw = 0;
		if (value1[2] > 0)
			pitch = 90;
		else
			pitch = 270;
	}
	else
	{
		yaw = (int) (Q_atan2(value1[1], value1[0]) * 180 / M_PI);
		if (yaw < 0)
			yaw += 360;

		forward = Q_sqrt (value1[0]*value1[0] + value1[1]*value1[1]);
		pitch = (int) (Q_atan2(value1[2], forward) * 180 / M_PI);
		if (pitch < 0)
			pitch += 360;
	}

	G_FLOAT(OFS_RETURN+0) = pitch;
	G_FLOAT(OFS_RETURN+1) = yaw;
	G_FLOAT(OFS_RETURN+2) = 0;
}

/*
=================
PF_Random

Returns a number from 0<= num < 1

random()
=================
*/
TM_CALLABLE
void PF_random (void)
{
	float		num;
		
	num = (Q_rand ()&0x7fff) / ((float)0x7fff);
	
	G_FLOAT(OFS_RETURN) = num;
}


/*
=================
PF_ambientsound

=================
*/
TM_CALLABLE
void PF_ambientsound (void)
{
	char		**check;
	char		*samp;
	float		*pos;
	float 		vol, attenuation;
	int			i, soundnum;

	pos = G_VECTOR (OFS_PARM0);			
	samp = G_STRING(OFS_PARM1);
	vol = G_FLOAT(OFS_PARM2);
	attenuation = G_FLOAT(OFS_PARM3);
	
// check to see if samp was properly precached
	for (soundnum=0, check = sv.sound_precache ; *check ; check++, soundnum++)
		if (!Q_strcmp(*check,samp))
			break;
			
	if (!*check)
	{
		Con_Printf ("no precache: %s\n", samp);
		return;
	}

// add an svc_spawnambient command to the level signon packet

	CRITICAL_MESSAGE(sv_signon_lock, SV_SIGNON_LOCK, 0, {
		MSG_WriteByte (&sv.signon,svc_spawnstaticsound);
		for (i=0 ; i<3 ; i++)
			MSG_WriteCoord(&sv.signon, pos[i]);

		MSG_WriteByte (&sv.signon, soundnum);

		MSG_WriteByte (&sv.signon, vol*255);
		MSG_WriteByte (&sv.signon, attenuation*64);
	});

}

/*
=================
PF_sound

Each entity can have eight independant sound sources, like voice,
weapon, feet, etc.

Channel 0 is an auto-allocate channel, the others override anything
allready running on that entity/channel pair.

An attenuation of 0 will play full volume everywhere in the level.
Larger attenuations will drop off.

=================
*/
TM_CALLABLE
void PF_sound (void)
{
	char		*sample;
	int			channel;
	edict_t		*entity;
	int 		volume;
	float attenuation;
		
	entity = G_EDICT(OFS_PARM0);
	channel = G_FLOAT(OFS_PARM1);
	sample = G_STRING(OFS_PARM2);
	volume = G_FLOAT(OFS_PARM3) * 255;
	attenuation = G_FLOAT(OFS_PARM4);
	
	SV_StartSound (entity, channel, sample, volume, attenuation);
}

/*
=================
PF_break

break()
=================
*/
TM_CALLABLE
void PF_break (void)
{
	Con_Printf ("break statement\n");
	*(int *)-4 = 0;	// dump to debugger
	//	PR_RunError ("break statement");
}

/*
=================
PF_traceline

Used for use tracing and shot targeting
Traces are blocked by bbox and exact bsp entityes, and also slide box entities
if the tryents flag is set.

traceline (vector1, vector2, tryents)
=================
*/
TM_CALLABLE
void PF_traceline (void)
{
	float	*v1, *v2;
	trace_t	trace;
	int		nomonsters;
	edict_t	*ent;

	v1 = G_VECTOR(OFS_PARM0);
	v2 = G_VECTOR(OFS_PARM1);
	nomonsters = G_FLOAT(OFS_PARM2);
	ent = G_EDICT(OFS_PARM3);

	trace = SV_Move (v1, vec3_origin, vec3_origin, v2, nomonsters, ent);

	pr_global_struct->trace_allsolid = trace.allsolid;
	pr_global_struct->trace_startsolid = trace.startsolid;
	pr_global_struct->trace_fraction = trace.fraction;
	pr_global_struct->trace_inwater = trace.inwater;
	pr_global_struct->trace_inopen = trace.inopen;
	VectorCopy (trace.endpos, pr_global_struct->trace_endpos);
	VectorCopy (trace.plane.normal, pr_global_struct->trace_plane_normal);
	pr_global_struct->trace_plane_dist =  trace.plane.dist;	
	if (trace.ent)
		pr_global_struct->trace_ent = EDICT_TO_PROG(trace.ent);
	else
		pr_global_struct->trace_ent = EDICT_TO_PROG(sv.edicts);
}



//============================================================================

byte	checkpvs[MAX_MAP_LEAFS/8];

TM_CALLABLE
int PF_newcheckclient (int check)
{
	int		i;
	byte	*pvs;
	edict_t	*ent;
	mleaf_t	*leaf;
	vec3_t	org;

// cycle to the next one

	if (check < 1)
		check = 1;
	if (check > MAX_CLIENTS)
		check = MAX_CLIENTS;

	if (check == MAX_CLIENTS)
		i = 1;
	else
		i = check + 1;

	for ( ;  ; i++)
	{
		if (i == MAX_CLIENTS+1)
			i = 1;

		ent = EDICT_NUM(i);

		if (i == check)
			break;	// didn't find anything else

		if (ent->free)
			continue;
		if (ent->v.health <= 0)
			continue;
		if ((int)ent->v.flags & FL_NOTARGET)
			continue;

	// anything that is a client, or has a client as an enemy
		break;
	}

// get the PVS for the entity
	VectorAdd (ent->v.origin, ent->v.view_ofs, org);
	leaf = Mod_PointInLeaf (org, sv.worldmodel);
	pvs = Mod_LeafPVS (leaf, sv.worldmodel);
	Q_memcpy (checkpvs, pvs, (sv.worldmodel->numleafs+7)>>3 );

	return i;
}

/*
=================
PF_checkclient

Returns a client (or object that has a client enemy) that would be a
valid target.

If there are more than one valid options, they are cycled each frame

If (self.origin + self.viewofs) is not in the PVS of the current target,
it is not returned at all.

name checkclient ()
=================
*/
#define	MAX_CHECK	16
int c_invis, c_notvis;

TM_CALLABLE
void PF_checkclient (void)
{
	edict_t	*ent, *self;
	mleaf_t	*leaf;
	int		l;
	vec3_t	view;
	
// find a new check if on a new frame
	if (sv.time - sv.lastchecktime >= 0.1)
	{
		sv.lastcheck = PF_newcheckclient (sv.lastcheck);
		sv.lastchecktime = sv.time;
	}

// return check if it might be visible	
	ent = EDICT_NUM(sv.lastcheck);
	if (ent->free || ent->v.health <= 0)
	{
		RETURN_EDICT(sv.edicts);
		return;
	}

// if current entity can't possibly see the check entity, return 0
	self = PROG_TO_EDICT(pr_global_struct->self);
	VectorAdd (self->v.origin, self->v.view_ofs, view);
	leaf = Mod_PointInLeaf (view, sv.worldmodel);
	l = (leaf - sv.worldmodel->leafs) - 1;
	if ( (l<0) || !(checkpvs[l>>3] & (1<<(l&7)) ) )
	{
		c_notvis++;
		RETURN_EDICT(sv.edicts);
		return;
	}

// might be able to see it
	c_invis++;
	RETURN_EDICT(ent);
}

//============================================================================


/*
=================
PF_stuffcmd

Sends text over to the client's execution buffer

stuffcmd (clientent, value)
=================
*/
TM_CALLABLE
void PF_stuffcmd (void)
{
	int		entnum;
	char	*str;
	client_t	*cl;
	
	entnum = G_EDICTNUM(OFS_PARM0);
	if (entnum < 1 || entnum > MAX_CLIENTS)
		PR_RunError ("Parm 0 not a client");
	str = G_STRING(OFS_PARM1);	
	
	cl = &svs.clients[entnum-1];

	if (Q_strcmp(str, "disconnect\n") == 0) {
		// so long and thanks for all the fish
		cl->drop = true;
		return;
	}

	// GAJA: 
	if (FrameStage & DOPARALLEL) 
	{	
		// We don't expect this to overflow so we skip checking
		MSG_WriteByte(&cl_msg[entnum-1], svc_stufftext);
		MSG_WriteString(&cl_msg[entnum-1], str);
		return;
	}

	ClientReliableWrite_Begin (cl, svc_stufftext, 2 + Q_strlen(str));
	ClientReliableWrite_String (cl, str);

}

/*
=================
PF_localcmd

Sends text over to the client's execution buffer

localcmd (string)
=================
*/
TM_CALLABLE
void PF_localcmd (void)
{
	char	*str;
	
	str = G_STRING(OFS_PARM0);	
	Cbuf_AddText (str);
}

/*
=================
PF_cvar

float cvar (string)
=================
*/
TM_CALLABLE
void PF_cvar (void)
{
	char	*str;
	
	str = G_STRING(OFS_PARM0);
	
	G_FLOAT(OFS_RETURN) = Cvar_VariableValue (str);
}

/*
=================
PF_cvar_set

float cvar (string)
=================
*/
TM_CALLABLE
void PF_cvar_set (void)
{
	char	*var, *val;
	
	var = G_STRING(OFS_PARM0);
	val = G_STRING(OFS_PARM1);
	
	Cvar_Set (var, val);
}

/*
=================
PF_findradius

Returns a chain of entities that have origins within a spherical area

findradius (origin, radius)
=================
*/
TM_CALLABLE
void PF_findradius (void)
{
	edict_t	*ent, *chain;
	float	rad;
	float	*org;
	vec3_t	eorg;
	int		i, j;

	chain = (edict_t *)sv.edicts;
	
	org = G_VECTOR(OFS_PARM0);
	rad = G_FLOAT(OFS_PARM1);

	ent = NEXT_EDICT(sv.edicts);
	for (i=1 ; i<sv.num_edicts ; i++, ent = NEXT_EDICT(ent))
	{
		if (ent->free)
			continue;
		if (ent->v.solid == SOLID_NOT)
			continue;
		for (j=0 ; j<3 ; j++)
			eorg[j] = org[j] - (ent->v.origin[j] + (ent->v.mins[j] + ent->v.maxs[j])*0.5);			
		if (Length(eorg) > rad)
			continue;
			
		ent->v.chain = EDICT_TO_PROG(chain);
		chain = ent;
	}

	RETURN_EDICT(chain);
}


/*
=========
PF_dprint
=========
*/
TM_CALLABLE
void PF_dprint (void)
{
	Con_Printf ("%s",PF_VarString(0));
}

char	pr_string_temp[128];


TM_CALLABLE
void PF_ftos (void)
{
	float	v;
	v = G_FLOAT(OFS_PARM0);
	
	if (v == (int)v)
		Q_sprintf (pr_string_temp, "%d",(int)v);  
	else
		Q_sprintf (pr_string_temp, "%5.1f",v);  
	G_INT(OFS_RETURN) = PR_SetString(pr_string_temp);
}

TM_CALLABLE
void PF_fabs (void)
{
	float	v;
	v = G_FLOAT(OFS_PARM0);
	G_FLOAT(OFS_RETURN) = Q_fabs(v);
}


TM_CALLABLE
void PF_vtos (void)
{
	Q_sprintf (pr_string_temp, "'%5.1f %5.1f %5.1f'", G_VECTOR(OFS_PARM0)[0], G_VECTOR(OFS_PARM0)[1], G_VECTOR(OFS_PARM0)[2]);  
	G_INT(OFS_RETURN) = PR_SetString(pr_string_temp);
}

TM_CALLABLE
void PF_Spawn (void)
{
	edict_t	*ed;
	ed = ED_Alloc();
	RETURN_EDICT(ed);
}

TM_CALLABLE
void PF_Remove (void)
{
	edict_t	*ed;
	
	ed = G_EDICT(OFS_PARM0);
	ED_Free (ed);
}


// entity (entity start, .string field, string match) find = #5;
TM_CALLABLE
void PF_Find (void)
{
	int		e;	
	int		f;
	char	*s, *t;
	edict_t	*ed;
	
	e = G_EDICTNUM(OFS_PARM0);
	f = G_INT(OFS_PARM1);
	s = G_STRING(OFS_PARM2);
	if (!s)
		PR_RunError ("PF_Find: bad search string");
		
	for (e++ ; e < sv.num_edicts ; e++)
	{
		ed = EDICT_NUM(e);
		if (ed->free)
			continue;
		t = E_STRING(ed,f);
		if (!t)
			continue;
		if (!Q_strcmp(t,s))
		{
			RETURN_EDICT(ed);
			return;
		}
	}
	
	RETURN_EDICT(sv.edicts);
}

TM_CALLABLE
void PR_CheckEmptyString (char *s)
{
	if (s[0] <= ' ')
		PR_RunError ("Bad string");
}

TM_CALLABLE
void PF_precache_file (void)
{	// precache_file is only used to copy files with qcc, it does nothing
	G_INT(OFS_RETURN) = G_INT(OFS_PARM0);
}

TM_CALLABLE
void PF_precache_sound (void)
{
	char	*s;
	int		i;
	
	if (sv.state != ss_loading)
		PR_RunError ("PF_Precache_*: Precache can only be done in spawn functions");
		
	s = G_STRING(OFS_PARM0);
	G_INT(OFS_RETURN) = G_INT(OFS_PARM0);
	PR_CheckEmptyString (s);
	
	for (i=0 ; i<MAX_SOUNDS ; i++)
	{
		if (!sv.sound_precache[i])
		{
			sv.sound_precache[i] = s;
			return;
		}
		if (!Q_strcmp(sv.sound_precache[i], s))
			return;
	}
	PR_RunError ("PF_precache_sound: overflow");
}

TM_CALLABLE
void PF_precache_model (void)
{
	char	*s;
	int		i;
	
	if (sv.state != ss_loading)
		PR_RunError ("PF_Precache_*: Precache can only be done in spawn functions");
		
	s = G_STRING(OFS_PARM0);
	G_INT(OFS_RETURN) = G_INT(OFS_PARM0);
	PR_CheckEmptyString (s);

	for (i=0 ; i<MAX_MODELS ; i++)
	{
		if (!sv.model_precache[i])
		{
			sv.model_precache[i] = s;
			return;
		}
		if (!Q_strcmp(sv.model_precache[i], s))
			return;
	}
	PR_RunError ("PF_precache_model: overflow");
}


TM_PURE
void PF_coredump (void)
{
	ED_PrintEdicts ();
}

TM_CALLABLE
void PF_traceon (void)
{
	pr_trace = true;
}

TM_CALLABLE
void PF_traceoff (void)
{
	pr_trace = false;
}

TM_PURE
void PF_eprint (void)
{
	ED_PrintNum (G_EDICTNUM(OFS_PARM0));
}

/*
===============
PF_walkmove

float(float yaw, float dist) walkmove
===============
*/
TM_CALLABLE
void PF_walkmove (void)
{
	edict_t	*ent;
	float	yaw, dist;
	vec3_t	move;
	dfunction_t	*oldf;
	int 	oldself;
	
	ent = PROG_TO_EDICT(pr_global_struct->self);
	yaw = G_FLOAT(OFS_PARM0);
	dist = G_FLOAT(OFS_PARM1);
	
	if ( !( (int)ent->v.flags & (FL_ONGROUND|FL_FLY|FL_SWIM) ) )
	{
		G_FLOAT(OFS_RETURN) = 0;
		return;
	}

	yaw = yaw*M_PI*2 / 360;
	
	move[0] = Q_cos(yaw)*dist;
	move[1] = Q_sin(yaw)*dist;
	move[2] = 0;

// save program state, because SV_movestep may call other progs
	oldf = pr_xfunction;
	oldself = pr_global_struct->self;

	// GAJA: Write the buffers to prepare for the next call to ProgramExecute
	WriteTempBuffers();
	ClearTempBuffers();
	
	G_FLOAT(OFS_RETURN) = SV_movestep(ent, move, true);
	
	
// restore program state
	pr_xfunction = oldf;
	pr_global_struct->self = oldself;
}

/*
===============
PF_droptofloor

void() droptofloor
===============
*/
TM_CALLABLE
void PF_droptofloor (void)
{
	edict_t		*ent;
	vec3_t		end;
	trace_t		trace;
	
	ent = PROG_TO_EDICT(pr_global_struct->self);

	VectorCopy (ent->v.origin, end);
	end[2] -= 256;
	
	trace = SV_Move (ent->v.origin, ent->v.mins, ent->v.maxs, end, false, ent);

	if (trace.fraction == 1 || trace.allsolid)
		G_FLOAT(OFS_RETURN) = 0;
	else
	{
		VectorCopy (trace.endpos, ent->v.origin);
		SV_LinkEdict (ent, false);
		ent->v.flags = (int)ent->v.flags | FL_ONGROUND;
		ent->v.groundentity = EDICT_TO_PROG(trace.ent);
		G_FLOAT(OFS_RETURN) = 1;
	}
}

/*
===============
PF_lightstyle

void(float style, string value) lightstyle
===============
*/
TM_CALLABLE
void PF_lightstyle (void)
{
	int		style;
	char	*val;
	client_t	*client;
	int			j;
	
	style = G_FLOAT(OFS_PARM0);
	val = G_STRING(OFS_PARM1);

// change the string in sv
	sv.lightstyles[style] = val;
	
// send message to all clients on this server
	if (sv.state != ss_active)
		return;
	
	for (j=0, client = svs.clients ; j<MAX_CLIENTS ; j++, client++)
		if ( client->state == cs_spawned )
		{
			CRITICAL_MESSAGE(client->msg_lock, CLIENT_MSG_LOCK, client->clientId, {
				ClientReliableWrite_Begin (client, svc_lightstyle, Q_strlen(val)+3);
				ClientReliableWrite_Char (client, style);
				ClientReliableWrite_String (client, val);
			});
		}
}

TM_CALLABLE
void PF_rint (void)
{
	float	f;
	f = G_FLOAT(OFS_PARM0);
	if (f > 0)
		G_FLOAT(OFS_RETURN) = (int)(f + 0.5);
	else
		G_FLOAT(OFS_RETURN) = (int)(f - 0.5);
}

TM_CALLABLE
void PF_floor (void)
{
	G_FLOAT(OFS_RETURN) = Q_floor(G_FLOAT(OFS_PARM0));
}

TM_CALLABLE
void PF_ceil (void)
{
	G_FLOAT(OFS_RETURN) = Q_ceil(G_FLOAT(OFS_PARM0));
}


/*
=============
PF_checkbottom
=============
*/
TM_CALLABLE
void PF_checkbottom (void)
{
	edict_t	*ent;
	
	ent = G_EDICT(OFS_PARM0);

	G_FLOAT(OFS_RETURN) = SV_CheckBottom (ent);
}

/*
=============
PF_pointcontents
=============
*/
TM_CALLABLE
void PF_pointcontents (void)
{
	float	*v;
	
	v = G_VECTOR(OFS_PARM0);

	G_FLOAT(OFS_RETURN) = SV_PointContents (v);	
}

/*
=============
PF_nextent

entity nextent(entity)
=============
*/
TM_CALLABLE
void PF_nextent (void)
{
	int		i;
	edict_t	*ent;
	
	i = G_EDICTNUM(OFS_PARM0);
	while (1)
	{
		i++;
		if (i == sv.num_edicts)
		{
			RETURN_EDICT(sv.edicts);
			return;
		}
		ent = EDICT_NUM(i);
		if (!ent->free)
		{
			RETURN_EDICT(ent);
			return;
		}
	}
}

/*
=============
PF_aim

Pick a vector for the player to shoot along
vector aim(entity, missilespeed)
=============
*/
//cvar_t	sv_aim = {"sv_aim", "0.93"};
cvar_t	sv_aim = {"sv_aim", "2"};

TM_CALLABLE
void PF_aim (void)
{
	edict_t	*ent, *check, *bestent;
	vec3_t	start, dir, end, bestdir;
	int		i, j;
	trace_t	tr;
	float	dist, bestdist;
	char	*noaim;

	ent = G_EDICT(OFS_PARM0);

	VectorCopy (ent->v.origin, start);
	start[2] += 20;

// noaim option
	i = NUM_FOR_EDICT(ent);
	if (i>0 && i<MAX_CLIENTS)
	{
		noaim = Info_ValueForKey (svs.clients[i-1].userinfo, "noaim");
		if (Q_atoi(noaim) > 0)
		{
			VectorCopy (pr_global_struct->v_forward, G_VECTOR(OFS_RETURN));
			return;
		}
	}

// try sending a trace straight
	VectorCopy (pr_global_struct->v_forward, dir);
	VectorMA (start, 2048, dir, end);
	tr = SV_Move (start, vec3_origin, vec3_origin, end, false, ent);
	if (tr.ent && tr.ent->v.takedamage == DAMAGE_AIM
	&& (!teamplay.value || ent->v.team <=0 || ent->v.team != tr.ent->v.team) )
	{
		VectorCopy (pr_global_struct->v_forward, G_VECTOR(OFS_RETURN));
		return;
	}


// try all possible entities
	VectorCopy (dir, bestdir);
	bestdist = sv_aim.value;
	bestent = NULL;
	
	check = NEXT_EDICT(sv.edicts);
	for (i=1 ; i<sv.num_edicts ; i++, check = NEXT_EDICT(check) )
	{
		if (check->v.takedamage != DAMAGE_AIM)
			continue;
		if (check == ent)
			continue;
		if (teamplay.value && ent->v.team > 0 && ent->v.team == check->v.team)
			continue;	// don't aim at teammate
		for (j=0 ; j<3 ; j++)
			end[j] = check->v.origin[j]
			+ 0.5*(check->v.mins[j] + check->v.maxs[j]);
		VectorSubtract (end, start, dir);
		VectorNormalize (dir);
		dist = DotProduct (dir, pr_global_struct->v_forward);
		if (dist < bestdist)
			continue;	// to far to turn
		tr = SV_Move (start, vec3_origin, vec3_origin, end, false, ent);
		if (tr.ent == check)
		{	// can shoot at this one
			bestdist = dist;
			bestent = check;
		}
	}
	
	if (bestent)
	{
		VectorSubtract (bestent->v.origin, ent->v.origin, dir);
		dist = DotProduct (dir, pr_global_struct->v_forward);
		VectorScale (pr_global_struct->v_forward, dist, end);
		end[2] = dir[2];
		VectorNormalize (end);
		VectorCopy (end, G_VECTOR(OFS_RETURN));	
	}
	else
	{
		VectorCopy (bestdir, G_VECTOR(OFS_RETURN));
	}
}

/*
==============
PF_changeyaw

This was a major timewaster in progs, so it was converted to C
==============
*/
TM_CALLABLE
void PF_changeyaw (void)
{
	edict_t		*ent;
	float		ideal, current, move, speed;
	
	ent = PROG_TO_EDICT(pr_global_struct->self);
	current = anglemod( ent->v.angles[1] );
	ideal = ent->v.ideal_yaw;
	speed = ent->v.yaw_speed;
	
	if (current == ideal)
		return;
	move = ideal - current;
	if (ideal > current)
	{
		if (move >= 180)
			move = move - 360;
	}
	else
	{
		if (move <= -180)
			move = move + 360;
	}
	if (move > 0)
	{
		if (move > speed)
			move = speed;
	}
	else
	{
		if (move < -speed)
			move = -speed;
	}
	
	ent->v.angles[1] = anglemod (current + move);
}

/*
===============================================================================

MESSAGE WRITING

===============================================================================
*/

#define	MSG_BROADCAST	0		// unreliable to all
#define	MSG_ONE			1		// reliable to one (msg_entity)
#define	MSG_ALL			2		// reliable to all
#define	MSG_INIT		3		// write to the init string
#define	MSG_MULTICAST	4		// for multicast()


TM_CALLABLE
sizebuf_t *WriteDest (void)
{
// WARNING CLEANUP	
//	int		entnum;
	int		dest;
//	edict_t	*ent;

	dest = G_FLOAT(OFS_PARM0);
	switch (dest)
	{
	case MSG_BROADCAST:
		if (FrameStage & DOPARALLEL) 
			return &sv_datagram_msg;
		else
			return &sv.datagram;
	
	case MSG_ONE:
		SV_Error("Shouldn't be at MSG_ONE");
		
	case MSG_ALL:
		if (FrameStage & DOPARALLEL) 
			return &sv_rel_datagram_msg;
		else
			return &sv.reliable_datagram;
	
	case MSG_INIT:
		if (sv.state != ss_loading)
			PR_RunError ("PF_Write_*: MSG_INIT can only be written in spawn functions");
		if (FrameStage & DOPARALLEL) 
			return &signon_msg;
		else
			return &sv.signon;

	case MSG_MULTICAST:
		if (FrameStage & DOPARALLEL) 
			return &multicast_msg;
		else
			return &sv.multicast;

	default:
		PR_RunError ("WriteDest: bad destination");
		break;
	}
	
	return NULL;
}


TM_CALLABLE
static int GetEntNum(void)
{
	return NUM_FOR_EDICT(PROG_TO_EDICT(pr_global_struct->msg_entity));
}

TM_CALLABLE
static client_t *Write_GetClient(void)
{
	int		entnum;
	edict_t	*ent;

	ent = PROG_TO_EDICT(pr_global_struct->msg_entity);
	entnum = NUM_FOR_EDICT(ent);
	if (entnum < 1 || entnum > MAX_CLIENTS)
		PR_RunError ("WriteDest: not a client");
	return &svs.clients[entnum-1];
}


TM_CALLABLE
void PF_WriteByte (void)
{
	sizebuf_t *buf;

	if (G_FLOAT(OFS_PARM0) == MSG_ONE) {
		client_t *cl = Write_GetClient();

		// GAJA:
		if (FrameStage & DOPARALLEL) 
			MSG_WriteByte(&cl_msg[GetEntNum() - 1], G_FLOAT(OFS_PARM1));
		else
		{
			ClientReliableCheckBlock(cl, 1);
			ClientReliableWrite_Byte(cl, G_FLOAT(OFS_PARM1));
		}
	} 
	else
	{
		buf = WriteDest();
		MSG_WriteByte (buf, G_FLOAT(OFS_PARM1));
	}
}

TM_CALLABLE
void PF_WriteChar (void)
{
	sizebuf_t *buf;

	if (G_FLOAT(OFS_PARM0) == MSG_ONE) {
		client_t *cl = Write_GetClient();
		// GAJA:
		if (FrameStage & DOPARALLEL) 
			MSG_WriteChar(&cl_msg[GetEntNum() - 1], G_FLOAT(OFS_PARM1));
		else
		{
			ClientReliableCheckBlock(cl, 1);
			ClientReliableWrite_Char(cl, G_FLOAT(OFS_PARM1));
		}
	} 
	else
	{
		buf = WriteDest();
		MSG_WriteChar (buf, G_FLOAT(OFS_PARM1));
	}
}

TM_CALLABLE
void PF_WriteShort (void)
{
	sizebuf_t *buf;

	if (G_FLOAT(OFS_PARM0) == MSG_ONE) {
		client_t *cl = Write_GetClient();
		// GAJA:
		if (FrameStage & DOPARALLEL) 
			MSG_WriteShort(&cl_msg[GetEntNum() - 1], G_FLOAT(OFS_PARM1));
		else
		{
			ClientReliableCheckBlock(cl, 2);
			ClientReliableWrite_Short(cl, G_FLOAT(OFS_PARM1));
		}
	} 
	else
	{
		buf = WriteDest();
		MSG_WriteShort (buf, G_FLOAT(OFS_PARM1));
	}
}

TM_CALLABLE
void PF_WriteLong (void)
{
	sizebuf_t *buf;

	if (G_FLOAT(OFS_PARM0) == MSG_ONE) {
		client_t *cl = Write_GetClient();
		// GAJA:
		if (FrameStage & DOPARALLEL) 
			MSG_WriteLong(&cl_msg[GetEntNum() - 1], G_FLOAT(OFS_PARM1));
		else
		{
			ClientReliableCheckBlock(cl, 4);
			ClientReliableWrite_Long(cl, G_FLOAT(OFS_PARM1));
		}
	} 
	else
	{
		buf = WriteDest();
		MSG_WriteLong (buf, G_FLOAT(OFS_PARM1));
	}
}

TM_CALLABLE
void PF_WriteAngle (void)
{
	sizebuf_t *buf;

	if (G_FLOAT(OFS_PARM0) == MSG_ONE) {
		client_t *cl = Write_GetClient();
		// GAJA:
		if (FrameStage & DOPARALLEL) 
			MSG_WriteAngle(&cl_msg[GetEntNum() - 1], G_FLOAT(OFS_PARM1));
		else
		{
			ClientReliableCheckBlock(cl, 1);
			ClientReliableWrite_Angle(cl, G_FLOAT(OFS_PARM1));
		}
	} 
	else
	{
		buf = WriteDest();
		MSG_WriteAngle (buf, G_FLOAT(OFS_PARM1));
	}
}

TM_CALLABLE
void PF_WriteCoord (void)
{
	sizebuf_t *buf;

	if (G_FLOAT(OFS_PARM0) == MSG_ONE) {
		client_t *cl = Write_GetClient();
		// GAJA:
		if (FrameStage & DOPARALLEL) 
			MSG_WriteCoord(&cl_msg[GetEntNum() - 1], G_FLOAT(OFS_PARM1));
		else
		{
			ClientReliableCheckBlock(cl, 2);
			ClientReliableWrite_Coord(cl, G_FLOAT(OFS_PARM1));
		}
	} 
	else
	{
		buf = WriteDest();
		MSG_WriteCoord (buf, G_FLOAT(OFS_PARM1));
	}
}

TM_CALLABLE
void PF_WriteString (void)
{
	sizebuf_t *buf;

	if (G_FLOAT(OFS_PARM0) == MSG_ONE) {
		client_t *cl = Write_GetClient();
		// GAJA:
		if (FrameStage & DOPARALLEL) 
			MSG_WriteString(&cl_msg[GetEntNum() - 1], G_STRING(OFS_PARM1));
		else
		{
			ClientReliableCheckBlock(cl, 1+Q_strlen(G_STRING(OFS_PARM1)));
			ClientReliableWrite_String(cl, G_STRING(OFS_PARM1));
		}
	} 
	else
	{
		buf = WriteDest();
		MSG_WriteString (buf, G_STRING(OFS_PARM1));
	}
}

TM_CALLABLE
void PF_WriteEntity (void)
{
	sizebuf_t *buf;

	if (G_FLOAT(OFS_PARM0) == MSG_ONE) {
		client_t *cl = Write_GetClient();
		// GAJA:
		if (FrameStage & DOPARALLEL) 
			MSG_WriteShort(&cl_msg[GetEntNum() - 1], G_EDICTNUM(OFS_PARM1));
		else
		{
			ClientReliableCheckBlock(cl, 2);
			ClientReliableWrite_Short(cl, G_EDICTNUM(OFS_PARM1));
		}
	} 
	else
	{
		buf = WriteDest();
		MSG_WriteShort (buf, G_EDICTNUM(OFS_PARM1));
	}
}

//=============================================================================

//TM_CALLABLE
//int SV_ModelIndex (char *name);

TM_CALLABLE
void PF_makestatic (void)
{
	edict_t	*ent;
	int		i;
	
	ent = G_EDICT(OFS_PARM0);

	CRITICAL_MESSAGE(sv_signon_lock, SV_SIGNON_LOCK, 0, {
		MSG_WriteByte (&sv.signon,svc_spawnstatic);

		MSG_WriteByte (&sv.signon, SV_ModelIndex(PR_GetString(ent->v.model)));

		MSG_WriteByte (&sv.signon, ent->v.frame);
		MSG_WriteByte (&sv.signon, ent->v.colormap);
		MSG_WriteByte (&sv.signon, ent->v.skin);
		for (i=0 ; i<3 ; i++)
		{
			MSG_WriteCoord(&sv.signon, ent->v.origin[i]);
			MSG_WriteAngle(&sv.signon, ent->v.angles[i]);
		}
	});

// throw the entity away now
	ED_Free (ent);
}

//=============================================================================

/*
==============
PF_setspawnparms
==============
*/
TM_CALLABLE
void PF_setspawnparms (void)
{
	edict_t	*ent;
	int		i;
	client_t	*client;

	ent = G_EDICT(OFS_PARM0);
	i = NUM_FOR_EDICT(ent);
	if (i < 1 || i > MAX_CLIENTS)
		PR_RunError ("Entity is not a client");

	// copy spawn parms out of the client_t
	client = svs.clients + (i-1);

	for (i=0 ; i< NUM_SPAWN_PARMS ; i++)
		(&pr_global_struct->parm1)[i] = client->spawn_parms[i];
}

/*
==============
PF_changelevel
==============
*/
TM_CALLABLE
void PF_changelevel (void)
{
	char	*s;
	static	int	last_spawncount;

// make sure we don't issue two changelevels
	if (svs.spawncount == last_spawncount)
		return;
	last_spawncount = svs.spawncount;
	
	s = G_STRING(OFS_PARM0);
	Cbuf_AddText (va("map %s\n",s));
}


/*
==============
PF_logfrag

logfrag (killer, killee)
==============
*/
// GAJA: since we disabled this function in TM we can mark it as TM_PURE
TM_PURE
void PF_logfrag (void)
{

	// GAJA
	// if we are in transaction don't allow this request
	if (inTransaction==true)
	{
		Con_Printf("Disabled in TM version of server\n");
		Sys_Printf("Client request for NextDownload rejected because TM\n");
		return;
	}

	edict_t	*ent1, *ent2;
	int		e1, e2;
	char	*s;

	ent1 = G_EDICT(OFS_PARM0);
	ent2 = G_EDICT(OFS_PARM1);

	e1 = NUM_FOR_EDICT(ent1);
	e2 = NUM_FOR_EDICT(ent2);
	
	if (e1 < 1 || e1 > MAX_CLIENTS
	|| e2 < 1 || e2 > MAX_CLIENTS)
		return;
	
	s = va("\\%s\\%s\\\n",svs.clients[e1-1].name, svs.clients[e2-1].name);

	SZ_Print (&svs.log[svs.logsequence&1], s);
	if (sv_fraglogfile) {
		fprintf (sv_fraglogfile, s);
		fflush (sv_fraglogfile);
	}
}


/*
==============
PF_infokey

string(entity e, string key) infokey
==============
*/
//  
// THREADPRIVATE STATIC ----------------------------------------------

static char	thr_ov[256];
THREADPRIVATE(thr_ov)

// -------------------------------------------------------------------


TM_CALLABLE
void PF_infokey (void)
{
	edict_t	*e;
	int		e1;
	char	*value;
	char	*key;
	//static	char ov[256];	// GAJA

	e = G_EDICT(OFS_PARM0);
	e1 = NUM_FOR_EDICT(e);
	key = G_STRING(OFS_PARM1);

	if (e1 == 0) {
		if ((value = Info_ValueForKey (svs.info, key)) == NULL ||
				   !*value)
			value = Info_ValueForKey(localinfo, key);
	} else if (e1 <= MAX_CLIENTS) {
		if (!Q_strcmp(key, "ip"))
			value = Q_strcpy(thr_ov, NET_BaseAdrToString (svs.clients[e1-1].netchan.remote_address));
		else if (!Q_strcmp(key, "ping")) {
			int ping = SV_CalcPing (&svs.clients[e1-1]);
			Q_sprintf(thr_ov, "%d", ping);  
			value = thr_ov;
		} else
			value = Info_ValueForKey (svs.clients[e1-1].userinfo, key);
	} else
		value = "";

		RETURN_STRING(value);
}

/*
==============
PF_stof

float(string s) stof
==============
*/
TM_CALLABLE
void PF_stof (void)
{
	char	*s;

	s = G_STRING(OFS_PARM0);

	G_FLOAT(OFS_RETURN) = Q_atof(s);
}


/*
==============
PF_multicast

void(vector where, float set) multicast
==============
*/
TM_CALLABLE
void PF_multicast (void)
{
	float	*o;
	int		to;

	o = G_VECTOR(OFS_PARM0);
	to = G_FLOAT(OFS_PARM1);

	// GAJA: Write private multicast message before multicasting it 
	CRITICAL_MESSAGE(sv_multicast_lock, SV_MULTICAST_LOCK, 0, {
		SZ_Write(&sv.multicast, multicast_msg.data, multicast_msg.cursize);	

		//multicast_msg.data = multicast_msg_buff;
		//multicast_msg.maxsize = sizeof(multicast_msg_buff);
		SZ_Clear (&multicast_msg);

		SV_Multicast (o, to);
	});
}

TM_CALLABLE
void PF_Fixme (void)
{
	PR_RunError ("unimplemented bulitin");
}


//TM_CALLABLE
builtin_t pr_builtin[] =
{
	PF_Fixme,
 	PF_makevectors,	// void(entity e)	makevectors 		= #1;
 	PF_setorigin,	// void(entity e, vector o) setorigin	= #2;
 	PF_setmodel,	// void(entity e, string m) setmodel	= #3;
 	PF_setsize,	// void(entity e, vector min, vector max) setsize = #4;
 	PF_Fixme,	// void(entity e, vector min, vector max) setabssize = #5;
 	PF_break,	// void() break						= #6;
 	PF_random,	// float() random						= #7;
 	PF_sound,	// void(entity e, float chan, string samp) sound = #8;
 	PF_normalize,	// vector(vector v) normalize			= #9;
 	PF_error,	// void(string e) error				= #10;
 	PF_objerror,	// void(string e) objerror				= #11;
 	PF_vlen,	// float(vector v) vlen				= #12;
 	PF_vectoyaw,	// float(vector v) vectoyaw		= #13;
 	PF_Spawn,	// entity() spawn						= #14;
 	PF_Remove,	// void(entity e) remove				= #15;
 	PF_traceline,	// float(vector v1, vector v2, float tryents) traceline = #16;
 	PF_checkclient,	// entity() clientlist					= #17;
 	PF_Find,	// entity(entity start, .string fld, string match) find = #18;
 	PF_precache_sound,	// void(string s) precache_sound		= #19;
 	PF_precache_model,	// void(string s) precache_model		= #20;
 	PF_stuffcmd,	// void(entity client, string s)stuffcmd = #21;
 	PF_findradius,	// entity(vector org, float rad) findradius = #22;
 	PF_bprint,	// void(string s) bprint				= #23;
 	PF_sprint,	// void(entity client, string s) sprint = #24;
 	PF_dprint,	// void(string s) dprint				= #25;
 	PF_ftos,	// void(string s) ftos				= #26;
 	PF_vtos,	// void(string s) vtos				= #27;
 	PF_coredump,
 	PF_traceon,
 	PF_traceoff,
 	PF_eprint,	// void(entity e) debug print an entire entity
 	PF_walkmove, // float(float yaw, float dist) walkmove
 	PF_Fixme, // float(float yaw, float dist) walkmove
 	PF_droptofloor,
 	PF_lightstyle,
 	PF_rint,
 	PF_floor,
 	PF_ceil,
 	PF_Fixme,
 	PF_checkbottom,
 	PF_pointcontents,
 	PF_Fixme,
 	PF_fabs,
 	PF_aim,
 	PF_cvar,
 	PF_localcmd,
 	PF_nextent,
 	PF_Fixme,
 	PF_changeyaw,
 	PF_Fixme,
 	PF_vectoangles,
 
 	PF_WriteByte,
 	PF_WriteChar,
 	PF_WriteShort,
 	PF_WriteLong,
 	PF_WriteCoord,
 	PF_WriteAngle,
 	PF_WriteString,
 	PF_WriteEntity,
 
 	PF_Fixme,
 	PF_Fixme,
 	PF_Fixme,
 	PF_Fixme,
 	PF_Fixme,
 	PF_Fixme,
 	PF_Fixme,
 
 	SV_MoveToGoal,
 	PF_precache_file,
 	PF_makestatic,
 
 	PF_changelevel,
 	PF_Fixme,
 
 	PF_cvar_set,
 	PF_centerprint,
 
 	PF_ambientsound,
 
 	PF_precache_model,
 	PF_precache_sound,		// precache_sound2 is different only for qcc
 	PF_precache_file,
 
 	PF_setspawnparms,
 
 	PF_logfrag,
 
 	PF_infokey,
 	PF_stof,
 	PF_multicast
};

//GAJA: Maybe TM_CALLABLE
builtin_t *pr_builtins = pr_builtin;
int pr_numbuiltins = sizeof(pr_builtin)/sizeof(pr_builtin[0]);

