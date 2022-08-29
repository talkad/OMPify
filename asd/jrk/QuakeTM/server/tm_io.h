/*
QuakeTM
Transactional implementation of the original Quake game
Copyright (C) 2008 Barcelona Supercomputing Center
Author: Vladimir Gajinov
*/


// GAJA 
// Header for transactional IO handling


// THREADPRIVATE ----------------------------------------------------------

#ifndef INTRANSACTION
#define INTRANSACTION
	extern qboolean inTransaction;       // per client flag
	THREADPRIVATE(inTransaction)
#endif

// -------------------------------------------------------------------------



void TM_IOInit (void);

/*
=====================
UDP Packets handling
=====================
*/
#define		MAX_PACKETS	4
typedef struct TM_packets_s
{
	int			count;							// Number of packets

	byte		data[MAX_PACKETS][MAX_UDP_PACKET];	// Storage for data
	int			lenght[MAX_PACKETS];			// lenght of individual packet
	netadr_t	to[MAX_PACKETS];				// destination addresses
} TM_packets_t;

TM_CALLABLE
void AddPacket2List (int length, void *data, netadr_t to);
void SendAllTMPackets (void);


/*
=====================
Messages handling
=====================
*/
#define MAX_MESSAGES 16
typedef struct TM_messages_s
{
	int		count;					// number of messages 
	int		pos;					// position free block
	qboolean msg_type[MAX_MESSAGES];	// type of message : console or system message
	int		lenght[MAX_MESSAGES];	// lengths of the individual messages
	char	msg[2*MAXPRINTMSG];		// a buffer to store messages continualy
} TM_messages_t;

TM_CALLABLE
void AddMsg2List(qboolean IsSysMsg, char *msg);
void SendAllTMMessages (void);



/*
=====================
File handling 
=====================
*/
#define MAX_FILES 16
typedef struct TM_files_s
{
	int		count;
	FILE	*File[MAX_FILES]; 
} TM_files_t;


TM_CALLABLE
void TM_fclose (FILE *File);
TM_CALLABLE
void AddFile2List (FILE *File);

void CloseAllTMFiles (void);



// Flushes all IO generated during transaction
void FlushTMOutput(void);


