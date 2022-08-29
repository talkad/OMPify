/*
QuakeTM
Transactional implementation of the original Quake game
Copyright (C) 2008 Barcelona Supercomputing Center
Author: Vladimir Gajinov
*/

#ifndef QWTYPES
#define QWTYPES
typedef unsigned char 		byte;
#define _DEF_BYTE_

#undef true
#undef false

typedef enum qboolean_e {false, true}	qboolean;
#endif

// THREADPRIVATE ----------------------------------------------------------

#define INTRANSACTION
qboolean inTransaction;       // per client flag
#pragma omp threadprivate(inTransaction)

// -------------------------------------------------------------------------


#include "qwsvdef.h"


// THREADPRIVATE ----------------------------------------------------------

TM_packets_t	packets;
TM_messages_t	messages;
TM_files_t		files;
THREADPRIVATE( packets, messages, files )

// --------------------------------------------------------------------


void TM_IOInit (void)
{
	packets.count = 0;
	messages.count = 0;
	messages.pos = 0;
	files.count = 0;
}


/*
=====================
TM packet handling functions
=====================
*/

TM_CALLABLE
void AddPacket2List (int length, void *data, netadr_t to)
{
	int pos = packets.count;

	if (pos < MAX_PACKETS)
	{
		Q_memcpy(&packets.data[pos], data, length);
		packets.lenght[pos] = length;
		packets.to[pos] = to;
		packets.count++;
	}
}


void SendAllTMPackets (void)
{
	int i;

	if (packets.count > 0)
		DEBUGPRINT(GENERAL, HIGH) 
			printf ("Thread: %d\t\tIn SendAllTMPackets ... packet : %d\n", omp_get_thread_num(), packets.count);

	for (i=0 ; i<packets.count ; i++)
	{
		NET_SendPacket(packets.lenght[i], packets.data[i], packets.to[i]);
	}

	// clear the state of the buffer
	packets.count = 0;
}


/*
=====================
TM messages handling functions
=====================
*/

TM_CALLABLE
void AddMsg2List(qboolean IsSysMsg, char *msg)
{
	int		cnt = messages.count;
	int		msglen = Q_strlen(msg)+1;

	if ( cnt < MAX_MESSAGES)
	{
		if ((messages.pos + msglen) < 2*MAXPRINTMSG)
		{
			Q_strcpy(&messages.msg[messages.pos], msg);
			messages.lenght[cnt] = msglen;
			messages.msg_type[cnt] = IsSysMsg;
			messages.count++;
			messages.pos += msglen;
		}
	}
}


void SendAllTMMessages (void)
{
	int i;
	int pos = 0;
	char temp[MAXPRINTMSG];

	if (messages.count > 0)
		DEBUGPRINT(GENERAL, HIGH) 
			printf ("Thread: %d\t\tIn SendAllTMMessages ... count : %d\n", omp_get_thread_num(), messages.count);

	for ( i=0 ; i<messages.count ; i++)
	{
		strncpy(temp, &messages.msg[pos], messages.lenght[i]);
		if (messages.msg_type[i] == true) 
		{
			Sys_Printf(temp);
		}
		else
		{
			Con_Printf(temp);
		}
		pos += messages.lenght[i];
	}

	// clear the state of buffer
	messages.count = 0;
	messages.pos = 0;
}



/*
=====================
TM file handling functions
=====================
*/

// TM_fclose is called only if the client is droped and when the upload or download was active.
// This means that we don't actually need to defer file operation, but we do it to avoid irrevocable transaction
TM_CALLABLE
void TM_fclose (FILE *File)
{
#if defined(DO_TM)
		AddFile2List (File);
#else
		fclose(File);
#endif
}


TM_CALLABLE
void AddFile2List (FILE *File)
{
	if (files.count < MAX_FILES)
		files.File[files.count] = File;
}


void CloseAllTMFiles (void)
{
	if (files.count > 0)
		DEBUGPRINT(GENERAL, HIGH) 
			printf ("Thread: %d\t\tIn CloseAllTMFiles ... files : %d\n", omp_get_thread_num(), files.count);

	int i;
	for ( i=0 ; i< files.count ; i++ )
		fclose(files.File[i]);

	files.count = 0;
}



/*
=====================
FlushTMOutput

Flushes all IO generated during transaction
=====================
*/

void FlushTMOutput()
{
	// get FlushIO lock
	#pragma omp critical (FlushIO) //(FlushTMOutput)	
	{
		CloseAllTMFiles ();
		SendAllTMPackets ();
		SendAllTMMessages ();
	}
}

