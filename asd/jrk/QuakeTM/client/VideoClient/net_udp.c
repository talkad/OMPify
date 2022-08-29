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
// net_main.c

#include "quakedef.h"

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

netadr_t	net_from;
sizebuf_t	net_message;
int			net_socket;			// non blocking, for receives
int			net_send_socket;	// blocking, for sends

#define	MAX_UDP_PACKET	8192
byte		net_message_buffer[MAX_UDP_PACKET];

#define MAX_TRACE_MESSAGES 1024
byte    trace_message[MAX_TRACE_MESSAGES][128];		// GAJA: Messages read from a trace file.
int     trace_message_length[MAX_TRACE_MESSAGES];	// GAJA: length of the current trace message
int     current_message=0;							// GAJA: Next message to send from a trace
int     trace_msg_count;							// GAJA: How many messages were read from a file
int     telefragged = 0;


int gethostname (char *, int);
int close (int);

//=============================================================================
// GAJA: Read and Write trace file 
//=============================================================================
void ReadTraceFile(char *filename)
{
	int		msg_index = 0;
	int		msg_size;
	FILE	*tracefile = fopen (filename,"r");

	if (tracefile)
	{
		while (!feof(tracefile))
		{
			fread( &msg_size, sizeof(int), 1, tracefile);   // read the size of message
			trace_message_length[msg_index] = msg_size;
			if (msg_size)
			{
				fread(trace_message[msg_index], 1, msg_size, tracefile);  // read actual message
				msg_index++;
			}
		}
	}
	else
	Sys_Error ("ReadTraceFile: Couldn't open trace file %s", filename);

	trace_msg_count = msg_index-1;
	current_message=0;
}


void SaveTraceFile(char *filename)
{
	FILE	*tracefile = fopen (filename,"w");
	int		i;

	if (tracefile)
	{
		for (i=0; i<=current_message; i++)
		{
			fwrite(&trace_message_length[i], sizeof(int), 1, tracefile);
			fwrite(trace_message[i], 1, trace_message_length[i], tracefile);
		}
		fclose(tracefile);
	}
	else
		Sys_Error ("SaveTraceFile: Couldn't open trace file %s", filename);

	current_message=0;
}



//=============================================================================

void NetadrToSockadr (netadr_t *a, struct sockaddr_in *s)
{
	memset (s, 0, sizeof(*s));
	s->sin_family = AF_INET;

	*(int *)&s->sin_addr = *(int *)&a->ip;
	s->sin_port = a->port;
}

void SockadrToNetadr (struct sockaddr_in *s, netadr_t *a)
{
	*(int *)&a->ip = *(int *)&s->sin_addr;
	a->port = s->sin_port;
}

qboolean	NET_CompareBaseAdr (netadr_t a, netadr_t b)
{
	if (a.ip[0] == b.ip[0] && a.ip[1] == b.ip[1] && a.ip[2] == b.ip[2] && a.ip[3] == b.ip[3])
		return true;
	return false;
}


qboolean	NET_CompareAdr (netadr_t a, netadr_t b)
{
	if (a.ip[0] == b.ip[0] && a.ip[1] == b.ip[1] && a.ip[2] == b.ip[2] && a.ip[3] == b.ip[3] && a.port == b.port)
		return true;
	return false;
}

char	*NET_AdrToString (netadr_t a)
{
	static	char	s[64];
	
	sprintf (s, "%i.%i.%i.%i:%i", a.ip[0], a.ip[1], a.ip[2], a.ip[3], ntohs(a.port));

	return s;
}

char	*NET_BaseAdrToString (netadr_t a)
{
	static	char	s[64];
	
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
			return 0;
		*(int *)&sadr.sin_addr = *(int *)h->h_addr_list[0];
	}
	
	SockadrToNetadr (&sadr, a);

	return true;
}

// Returns true if we can't bind the address locally--in other words, 
// the IP is NOT one of our interfaces.
qboolean NET_IsClientLegal(netadr_t *adr)
{
	struct sockaddr_in sadr;
	int newsocket;

#if 0
	if (adr->ip[0] == 127)
		return false; // no local connections period

	NetadrToSockadr (adr, &sadr);

	if ((newsocket = socket (PF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
		Sys_Error ("NET_IsClientLegal: socket:", strerror(errno));

	sadr.sin_port = 0;

	if( bind (newsocket, (void *)&sadr, sizeof(sadr)) == -1) 
	{
		// It is not a local address
		close(newsocket);
		return true;
	}
	close(newsocket);
	return false;
#else
	return true;
#endif
}


//=============================================================================

qboolean NET_GetPacket (void)
{
	int 	ret;
	struct sockaddr_in	from;
	int		fromlen;

	fromlen = sizeof(from);
	ret = recvfrom (net_socket, net_message_buffer, sizeof(net_message_buffer), 0, (struct sockaddr *)&from, &fromlen);
	if (ret == -1) {
		if (errno == EWOULDBLOCK)
			return false;
		if (errno == ECONNREFUSED)
			return false;
		Sys_Printf ("NET_GetPacket: %s\n", strerror(errno));
		return false;
	}

	net_message.cursize = ret;
	SockadrToNetadr (&from, &net_from);

	// GAJA: Print received message
	// =============================================
    printf("\nGAJA: Receive \tsize=%d\n", ret);
    int i;
    byte x;
    char rec_string[2048];
    for (i=0;i<ret;i++)
    {
		x=net_message_buffer[i];
		printf("%02hhx", x);
		if (((i+1)%2) == 0) printf(" ");
    }
    printf("\n");
    for (i=0;i<ret;i++)
    {
		x=net_message_buffer[i];
		if (x==0)
			sprintf(rec_string, "%s ", rec_string);
		else 
			sprintf(rec_string, "%s%c", rec_string, x);
    }
    printf("%s\n\n", rec_string);

	return ret;
}

//=============================================================================

void NET_SendPacket (int length, void *data, netadr_t to)
{
	int ret;
	struct sockaddr_in	addr;
	byte    trace_data[128];  // GAJA

	NetadrToSockadr (&to, &addr);

	// GAJA: If we run a trace change client message: take first 8 bytes of the original message and the rest from the saved trace
	if (trace_state == read_trace)
	{
		// 8 bytes for the header from the original message
		memcpy(trace_data, data, 10);  
		// and the rest from the trace message
		memcpy(trace_data + 10, trace_message[current_message] + 10, trace_message_length[current_message] - 10);   
		data = trace_data;
		length = trace_message_length[current_message];
		current_message++;
		if (current_message >= trace_msg_count || current_message > (MAX_TRACE_MESSAGES-1))
		{
			// kill yourself
			printf("DEBUG: NET_SendPacket read   CommitSiucide\n");
			CommitSiucide();
		}
	}

	// GAJA: If we are recording a trace, write message to a buffer
	if (trace_state == write_trace)
	{
		memcpy(trace_message[current_message], data, length);
		trace_message_length[current_message] = length;
		current_message++;
		if (current_message > (MAX_TRACE_MESSAGES-2))
		{
			// insert command to kill yourself
			printf("DEBUG: NET_SendPacket write   CommitSiucide\n");
			CommitSiucide();
		}
	}


	ret = sendto (net_socket, data, length, 0, (struct sockaddr *)&addr, sizeof(addr) );
	if (ret == -1) {
		if (errno == EWOULDBLOCK)
			return;
		if (errno == ECONNREFUSED)
			return;
		Sys_Printf ("NET_SendPacket: %s\n", strerror(errno));
	}


	// GAJA: Print sent message
	// =============================================
    printf("\nGAJA: Send \tsize=%d\n", ret);
    int i;
    byte *x;
    for (i=0;i<length;i++)
    {
		x=data+i;
		if (*x==0)
			printf(" ");
		else 
			printf("%c", *x);
    }
    printf("\n\n");
	// =============================================

}

//=============================================================================

int UDP_OpenSocket (int port)
{
	int newsocket;
	struct sockaddr_in address;
	qboolean _true = true;
	int i;

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
// 	if( bind (newsocket, (void *)&address, sizeof(address)) == -1)
// 		Sys_Error ("UDP_OpenSocket: bind: %s", strerror(errno));

	// GAJA: Changed to pick up ports in order. Enable running multiple clients to run on the same machine
	while ( bind (newsocket, (void *)&address, sizeof(address)) == -1)
	{
		port++;
		address.sin_port = htons((short)port);
	}

	return newsocket;
}


void NET_GetLocalAddress (void)
{
	char	buff[MAXHOSTNAMELEN];
	struct sockaddr_in	address;
	int		namelen;

	gethostname(buff, MAXHOSTNAMELEN);
	buff[MAXHOSTNAMELEN-1] = 0;

	NET_StringToAdr (buff, &net_local_adr);

	namelen = sizeof(address);
	if (getsockname (net_socket, (struct sockaddr *)&address, &namelen) == -1)
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
	net_message.maxsize = sizeof(net_message_buffer);
	net_message.data = net_message_buffer;

	//
	// determine my name & address
	//
	NET_GetLocalAddress ();

	Con_Printf("UDP Initialized\n");
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

