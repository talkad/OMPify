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

// sys.h -- non-portable functions


int	Sys_FileTime (char *path);

void Sys_mkdir (char *path);

TM_PURE
void Sys_Error (char *error, ...);
// an error will cause the entire program to exit

TM_CALLABLE
void Sys_Printf (char *fmt, ...);
// send text to the console

TM_PURE
void Sys_Quit (void);
double Sys_DoubleTime (void);

char *Sys_ConsoleInput (void);
void Sys_Init (void);

TM_PURE
int GetThreadId(void);

// GAJA: rdtsc
// ==============================================
inline uint64_t rdtsc(void);

#define RDTSC 1

// PROCSPEED is processor speed in MHz. Use it to get the time in milliseconds
// dell04
//#define PROCSPEED 3000
// dell01, dell02
#define PROCSPEED 3200
// laptop
//#define PROCSPEED 1830


//waittime is in milliseconds
#define WAITTIME(x)     PROCSPEED * 1000 * x
#define TO_MILISEC(x)   ((double)( (x) / PROCSPEED)) / 1000
#define TO_SECONDS(x)	((double)( (x) / (1000 * PROCSPEED))) / 1000




