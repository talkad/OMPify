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

// net_main.c

// ==================================================================================
// GAJA: define structures here to be able to use threadprivate directive

#ifndef QWTYPES
#define QWTYPES
typedef unsigned char 		byte;
#define _DEF_BYTE_

// KJB Undefined true and false defined in SciTech's DEBUG.H header
#undef true
#undef false

typedef enum qboolean_e {false, true}	qboolean;
#endif

#ifndef QWNETADR
#define QWNETADR
typedef struct netadr_s
{
	byte	ip[4];
	unsigned short	port;
	unsigned short	pad;
} netadr_t;
#endif

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

// ==================================================================================

// THREADPRIVATE ----------------------------------------------------------

#define NET_VARS
netadr_t	net_from;
sizebuf_t	net_message;
#pragma omp threadprivate(net_from, net_message)

// -------------------------------------------------------------------------

#include "qwsvdef.h"


// GAJA added to avoid assembler implementations for system library functions
#ifdef __GNUC__
#undef __GNUC__
#endif

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/param.h>
#include <sys/ioctl.h>
#include <sys/uio.h>
#include <arpa/inet.h>
#include <errno.h>


#if defined(sun)
#include <unistd.h>
#endif

#ifdef sun
#include <sys/filio.h>
#endif

#ifdef NeXT
#include <libc.h>
#endif

netadr_t	net_local_adr;
int			net_socket;			// non blocking, for receives
int			net_send_socket;	// blocking, for sends

byte		net_message_buffer[MAX_UDP_PACKET];
THREADPRIVATE(net_message_buffer)

int gethostname (char *, int);
int close (int);

int packetssent = 0;

//=============================================================================

TM_CALLABLE
void NetadrToSockadr (netadr_t *a, struct sockaddr_in *s)
{
	Q_memset (s, 0, sizeof(*s));
	s->sin_family = AF_INET;

	*(int *)&s->sin_addr = *(int *)&a->ip;
	s->sin_port = a->port;
}

void SockadrToNetadr (struct sockaddr_in *s, netadr_t *a)
{
	*(int *)&a->ip = *(int *)&s->sin_addr;
	a->port = s->sin_port;
}

TM_CALLABLE
qboolean	NET_CompareBaseAdr (netadr_t a, netadr_t b)
{
	if (a.ip[0] == b.ip[0] && a.ip[1] == b.ip[1] && a.ip[2] == b.ip[2] && a.ip[3] == b.ip[3])
		return true;
	return false;
}


TM_CALLABLE
qboolean	NET_CompareAdr (netadr_t a, netadr_t b)
{
	if (a.ip[0] == b.ip[0] && a.ip[1] == b.ip[1] && a.ip[2] == b.ip[2] && a.ip[3] == b.ip[3] && a.port == b.port)
		return true;
	return false;
}


TM_PURE
char	*NET_AdrToString (netadr_t a)
{
	static	char	s[64];
	THREADPRIVATE(s)	

	sprintf (s, "%i.%i.%i.%i:%i", a.ip[0], a.ip[1], a.ip[2], a.ip[3], ntohs(a.port));

	return s;
}

TM_PURE
char	*NET_BaseAdrToString (netadr_t a)
{
	static	char	s[64];
	THREADPRIVATE(s);
	
	sprintf (s, "%i.%i.%i.%i", a.ip[0], a.ip[1], a.ip[2], a.ip[3]);

	return s;
}

/*
=============
NET_StringToAdr

idnewt
idnewt:28000
192.246.40.70
192.246.40.70:28000
=============
*/
qboolean	NET_StringToAdr (char *s, netadr_t *a)
{
	struct hostent	*h;
	struct sockaddr_in sadr;
	char	*colon;
	char	copy[128];
	
	
	memset (&sadr, 0, sizeof(sadr));
	sadr.sin_family = AF_INET;
	
	sadr.sin_port = 0;

	strcpy (copy, s);
	// strip off a trailing :port if present
	for (colon = copy ; *colon ; colon++)
		if (*colon == ':')
		{
			*colon = 0;
			sadr.sin_port = htons(atoi(colon+1));	
		}
	
	if (copy[0] >= '0' && copy[0] <= '9')
	{
		*(int *)&sadr.sin_addr = inet_addr(copy);
	}
	else
	{
		if (! (h = gethostbyname(copy)) )
			return false;
		*(int *)&sadr.sin_addr = *(int *)h->h_addr_list[0];
	}
	
	SockadrToNetadr (&sadr, a);

	return true;
}


/*
=============
NET_GetPacket
=============
*/
qboolean NET_GetPacket (void)
{
	int 	ret;
	struct sockaddr_in	from;
	int		fromlen;

	fromlen = sizeof(from);

	ret = recvfrom (net_socket, net_message_buffer, MAX_UDP_PACKET * sizeof(byte), 0, (struct sockaddr *)&from, (socklen_t *)&fromlen);

	if (ret == -1) {
		if (errno == EWOULDBLOCK)
			return false;
		if (errno == ECONNREFUSED)
			return false;
		Sys_Printf ("NET_GetPacket: %s\n", strerror(errno));
		return false;
	}

	DEBUGPRINT(READ_PACKETS,HIGH) PrintBuffer(net_message_buffer, ret);

	net_message.cursize = ret;
	SockadrToNetadr (&from, &net_from);

	return (ret == 0) ? false : true;
}


/*
==============
NET_SendPacket
==============
*/
TM_CALLABLE
void NET_SendPacket (int length, void *data, netadr_t to)
{
	int ret;
	struct sockaddr_in	addr;

	// GAJA
	// if we are in transaction don't send the packet. Instead buffer it for sending after transaction commits
	if (inTransaction==true)
	{
		AddPacket2List(length, data, to);
		return;
	}

	NetadrToSockadr (&to, &addr);

	DEBUGPRINT(GENERAL, MEDIUM) PRINT("\nSending message to ip=%d.%d.%d.%d:%d\t\tsize = %d\n", to.ip[0], to.ip[1], to.ip[2], to.ip[3], to.port, length);


	DEBUGPRINT(GENERAL, HIGH) PrintBuffer(data, length);

	packetssent++;
	
	ret = sendto (net_socket, data, length, 0, (struct sockaddr *)&addr, sizeof(addr) );
	if (ret == -1) {
		if (errno == EWOULDBLOCK)
			return;
		if (errno == ECONNREFUSED)
			return;
		Sys_Printf ("NET_SendPacket: %s\n", strerror(errno));
	}
}

/*
==============
UDP_OpenSocket
==============
*/
int UDP_OpenSocket (int port)
{
	int newsocket;
	struct sockaddr_in address;
	qboolean _true = true;
	int i;
  
  	newsocket=1;

	if ((newsocket = socket (PF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
		Sys_Error ("UDP_OpenSocket: socket:", strerror(errno));
	if (ioctl (newsocket, FIONBIO, (char *)&_true) == -1)
		Sys_Error ("UDP_OpenSocket: ioctl FIONBIO:", strerror(errno));

	address.sin_family = AF_INET;

	//ZOID -- check for interface binding option
	if ((i = COM_CheckParm("-ip")) != 0 && i < com_argc) {
		address.sin_addr.s_addr = inet_addr(com_argv[i+1]);
		Con_Printf("Binding to IP Interface Address of %s\n",
				inet_ntoa(address.sin_addr));
	} else
		address.sin_addr.s_addr = INADDR_ANY;

	if (port == PORT_ANY)
		address.sin_port = 0;
	else
		address.sin_port = htons((short)port);

	if( bind (newsocket, (void *)&address, sizeof(address)) == -1)
		Sys_Error ("UDP_OpenSocket: bind: %s", strerror(errno));

	return newsocket;
}

/*
=====================
NET_GetLocalAddress
=====================
*/
void NET_GetLocalAddress (void)
{
	char	buff[MAXHOSTNAMELEN];
	struct sockaddr_in	address;
	int		namelen;

	gethostname(buff, MAXHOSTNAMELEN);
	buff[MAXHOSTNAMELEN-1] = 0;

	NET_StringToAdr (buff, &net_local_adr);

	namelen = sizeof(address);

  	if (getsockname (net_socket, (struct sockaddr *)&address, (socklen_t *)&namelen) == -1)
		Sys_Error ("NET_Init: getsockname:", strerror(errno));
	net_local_adr.port = address.sin_port;

	Con_Printf("IP address %s\n", NET_AdrToString (net_local_adr) );
}

/*
====================
NET_Init
====================
*/
void NET_Init (int port)
{
	//
	// open the single socket to be used for all communications
	//
	net_socket = UDP_OpenSocket (port);

	//
	// init the message buffer
	//
	net_message.maxsize = MAX_UDP_PACKET * sizeof(byte); // GAJA: SIZEOF sizeof(net_message_buffer)
	net_message.data = net_message_buffer;

	//
	// determine my name & address
	//
	NET_GetLocalAddress ();

	Con_Printf("UDP Initialized\n");
}


/*
====================
NET_Message_Init
====================
*/
// GAJA: Added this function
// Called to initialize net_message on the beggining of a task in SV_ReadPackets
void NET_Message_Init (sizebuf_t *net_msg, void *msg_buff, int msg_length)
{
	net_msg->data = net_message_buffer;
	memcpy(&net_message_buffer, msg_buff, msg_length);
}

void CopyBuffer(void *buff)
{
	memcpy(buff, &net_message_buffer, MAX_UDP_PACKET * sizeof(byte)); // GAJA: SIZEOF sizeof(net_message_buffer)
}


/*
====================
NET_Shutdown
====================
*/
void	NET_Shutdown (void)
{
	close (net_socket);
}

