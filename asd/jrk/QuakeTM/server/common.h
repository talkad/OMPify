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

// comndef.h  -- general definitions


#ifndef QWTYPES
#define QWTYPES
typedef unsigned char 		byte;
#define _DEF_BYTE_

// KJB Undefined true and false defined in SciTech's DEBUG.H header
#undef true
#undef false

typedef enum qboolean_e {false, true}	qboolean;
#endif

#define	MAX_INFO_STRING	196
#define	MAX_SERVERINFO_STRING	512
#define	MAX_LOCALINFO_STRING	32768
#define MAXPRINTMSG 8192
// FIXME: make a buffer size safe vsprintf?

//============================================================================

#ifndef SIZEBUF
#define SIZEBUF
typedef struct sizebuf_s
{
	qboolean	allowoverflow;	// if false, do a Sys_Error
	qboolean	overflowed;		// set to true if the buffer size failed
	byte	*data;
	int		maxsize;
	int		cursize;
} sizebuf_t;
#endif

TM_CALLABLE
void SZ_Clear (sizebuf_t *buf);
TM_CALLABLE
void *SZ_GetSpace (sizebuf_t *buf, int length);
TM_CALLABLE
void SZ_Write (sizebuf_t *buf, void *data, int length);
TM_CALLABLE
void SZ_Print (sizebuf_t *buf, char *data);	// strcats onto the sizebuf

//============================================================================

typedef struct link_s
{
	struct link_s	*prev, *next;
} link_t;


TM_CALLABLE
void ClearLink (link_t *l);
TM_CALLABLE
void RemoveLink (link_t *l);
TM_CALLABLE
void InsertLinkBefore (link_t *l, link_t *before);
TM_CALLABLE
void InsertLinkAfter (link_t *l, link_t *after);

#define	STRUCT_FROM_LINK(l,t,m) ((t *)((byte *)l - (int)&(((t *)0)->m)))

//============================================================================

#ifndef NULL
#define NULL ((void *)0)
#endif

#define Q_MAXCHAR ((char)0x7f)
#define Q_MAXSHORT ((short)0x7fff)
#define Q_MAXINT	((int)0x7fffffff)
#define Q_MAXLONG ((int)0x7fffffff)
#define Q_MAXFLOAT ((int)0x7fffffff)

#define Q_MINCHAR ((char)0x80)
#define Q_MINSHORT ((short)0x8000)
#define Q_MININT 	((int)0x80000000)
#define Q_MINLONG ((int)0x80000000)
#define Q_MINFLOAT ((int)0x7fffffff)

//============================================================================

extern	qboolean		bigendien;

extern	short	(*BigShort) (short l);
extern	short	(*LittleShort) (short l);
extern	int	(*BigLong) (int l);
//TM_CALLABLE
extern	int	(* TM_CALLABLE LittleLong) (int l);
extern	float	(*BigFloat) (float l);
extern	float	(*LittleFloat) (float l);

//============================================================================

struct usercmd_s;

extern struct usercmd_s nullcmd;

TM_CALLABLE
void MSG_WriteChar (sizebuf_t *sb, int c);
TM_CALLABLE
void MSG_WriteByte (sizebuf_t *sb, int c);
TM_CALLABLE
void MSG_WriteShort (sizebuf_t *sb, int c);
TM_CALLABLE
void MSG_WriteLong (sizebuf_t *sb, int c);
TM_CALLABLE
void MSG_WriteFloat (sizebuf_t *sb, float f);
TM_CALLABLE
void MSG_WriteString (sizebuf_t *sb, char *s);
TM_CALLABLE
void MSG_WriteCoord (sizebuf_t *sb, float f);
TM_CALLABLE
void MSG_WriteAngle (sizebuf_t *sb, float f);
TM_CALLABLE
void MSG_WriteAngle16 (sizebuf_t *sb, float f);
TM_CALLABLE
void MSG_WriteDeltaUsercmd (sizebuf_t *sb, struct usercmd_s *from, struct usercmd_s *cmd);

//extern int				msg_readcount;
//extern enum qboolean_e	msg_badread;		// set if a read goes beyond end of message
//THREADPRIVATE(msg_readcount, msg_badread)

TM_CALLABLE
void MSG_BeginReading (void);
TM_CALLABLE
int MSG_GetReadCount(void);
TM_CALLABLE
int MSG_ReadChar (void);
TM_CALLABLE
int MSG_ReadByte (void);
TM_CALLABLE
int MSG_ReadShort (void);
TM_CALLABLE
int MSG_ReadLong (void);
TM_CALLABLE
void MSG_ReadString (char *thr_string, int str_size);
char *MSG_ReadStringLine (void);

TM_CALLABLE
float MSG_ReadCoord (void);
TM_CALLABLE
float MSG_ReadAngle16 (void);
TM_CALLABLE
void MSG_ReadDeltaUsercmd (struct usercmd_s *from, struct usercmd_s *cmd);

//============================================================================

// GAJA: we have to define these function to avoid serializing transactions
//#define Q_memset(d, f, c) memset((d), (f), (c))
//#define Q_memcpy(d, s, c) memcpy((d), (s), (c))
//#define Q_strcpy(d, s) strcpy((d), (s))
//#define Q_strncpy(d, s, n) strncpy((d), (s), (n))
//#define Q_strlen(s) ((int)strlen(s))
//#define Q_strcat(d, s) strcat((d), (s))
//#define Q_strcmp(s1, s2) strcmp((s1), (s2))
//#define Q_strncmp(s1, s2, n) strncmp((s1), (s2), (n))
//#define Q_strcasecmp(s1, s2) strcasecmp((s1), (s2))
#define Q_strncasecmp(s1, s2, n) strncasecmp((s1), (s2), (n))

TM_CALLABLE
void	* Q_memset(void * s,int c,size_t count);
TM_CALLABLE
void	* Q_memcpy(void * dest,const void *src,size_t count);
TM_CALLABLE
char	* Q_strcpy(char * dest,const char *src);
TM_CALLABLE
char	* Q_strncpy(char * dest,const char *src,size_t count);
TM_CALLABLE
char	* Q_strcat(char * dest, const char * src);
TM_CALLABLE
char	* Q_strncat(char *dest, const char *src, size_t count);
TM_CALLABLE
int		Q_strcmp(const char * cs,const char * ct);
TM_CALLABLE
int		Q_strncmp(const char * cs,const char * ct,size_t count);
TM_CALLABLE
size_t	Q_strlen(const char * s);
TM_CALLABLE
size_t	Q_strnlen(const char * s, size_t count);
TM_CALLABLE
char	* Q_strstr(const char * s1,const char * s2);
TM_CALLABLE
int		Q_strcasecmp(const char *s1, const char *s2);



TM_CALLABLE
int	Q_atoi (char *str);
TM_CALLABLE
float Q_atof (char *str);

TM_PURE
float	Q_sqrt (float x);
TM_PURE
float	Q_cos (float x);
TM_PURE
float	Q_sin (float x);
TM_PURE
float	Q_atan2 (float y, float x);
TM_PURE
int		Q_rand (void);
TM_PURE
float	Q_fabs (float x);
TM_PURE
float	Q_floor (float x);
TM_PURE
float	Q_ceil (float x);

TM_PURE
int Q_tolower(int x);

TM_CALLABLE
int Q_vsprintf(char *buf, const char *fmt, va_list args);
TM_CALLABLE
int Q_sprintf(char *buf, const char *fmt, ...);

TM_PURE
int ThreadNumber(void);

//============================================================================


extern int	com_argc;
extern char **com_argv;
//extern char com_token[1024];
//THREADPRIVATE(com_token, com_argc, com_argv) 


TM_CALLABLE
char *COM_Parse (char *data);

int COM_CheckParm (char *parm);
void COM_AddParm (char *parm);

void COM_Init (void);
void COM_InitArgv (int argc, char **argv);

void COM_FileBase (char *in, char *out);

TM_PURE
char	*va(char *format, ...);
// does a varargs printf into a temp buffer


//============================================================================

extern int com_filesize;
struct cache_user_s;

extern	char	com_gamedir[MAX_OSPATH];

TM_PURE
int COM_FOpenFile (char *filename, FILE **file);
void COM_CloseFile (FILE *h);

byte *COM_LoadStackFile (char *path, void *buffer, int bufsize);
byte *COM_LoadHunkFile (char *path);
void COM_Gamedir (char *dir);

extern	struct cvar_s	registered;
extern qboolean		standard_quake, rogue, hipnotic;

TM_CALLABLE
char *Info_ValueForKey (char *s, char *key);
TM_CALLABLE
void Info_RemoveKey (char *s, char *key);
TM_CALLABLE
void Info_RemovePrefixedKeys (char *start, char prefix);
TM_CALLABLE
void Info_SetValueForKey (char *s, char *key, char *value, int maxsize);
TM_CALLABLE
void Info_SetValueForStarKey (char *s, char *key, char *value, int maxsize);
TM_CALLABLE
void Info_Print (char *s);

unsigned Com_BlockChecksum (void *buffer, int length);

TM_CALLABLE
byte	COM_BlockSequenceCRCByte (byte *base, int length, int sequence);

int build_number( void );

TM_CALLABLE
void PrintBuffer(byte *buf, int size);



