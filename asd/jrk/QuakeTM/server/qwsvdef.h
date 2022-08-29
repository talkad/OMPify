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

// quakedef.h -- primary header for server

//define	PARANOID			// speed sapping error checking

#include <math.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>
#include <ctype.h>
#include <stdint.h>

// GAJA: macros for transactions
#include "tm_macros.h"

#include "bothdefs.h"

#include "common.h"
#include "bspfile.h"
#include "sys.h"
#include "zone.h"
#include "mathlib.h"

#include "cvar.h"
#include "net.h"
#include "protocol.h"
#include "cmd.h"
#include "model.h"
#include "progs.h"

#include "server.h"
#include "world.h"
#include "pmove.h"

// GAJA: header for transactional IO handling
#include "tm_io.h"



//=============================================================================

// the host system specifies the base of the directory tree, the
// command line parms passed to the program, and the amount of memory
// available for the program to use

typedef struct quakeparms_s
{
	char	*basedir;
	char	*cachedir;		// for development over ISDN lines
	int		argc;
	char	**argv;
	void	*membase;
	int		memsize;
} quakeparms_t;


//=============================================================================

//
// host
//
extern	quakeparms_t host_parms;
extern  int NoThreads;  //GAJA: Variable to hold number of threads to run in parallel

extern	cvar_t		sys_nostdout;
extern	cvar_t		developer;

extern	qboolean	host_initialized;		// true if into command execution
extern	double		realtime;			// not bounded in any way, changed at
										// start of every frame, never reset

// ---------------------------------------
#ifndef HOST_FRAMETIME
#define HOST_FRAMETIME
	extern	double		host_frametime;
	THREADPRIVATE(host_frametime)
#endif

// ---------------------------------------


TM_PURE
void SV_Error (char *error, ...);
void SV_Init (quakeparms_t *parms);

TM_CALLABLE
void Con_Printf (char *fmt, ...);
TM_CALLABLE
void Con_DPrintf (char *fmt, ...);

// moved from crc.h
TM_CALLABLE
void CRC_Init(unsigned short *crcvalue);
TM_CALLABLE
unsigned short CRC_Block (byte *start, int count);

