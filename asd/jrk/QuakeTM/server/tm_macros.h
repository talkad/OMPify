/*
QuakeTM
Transactional implementation of the original Quake game
Copyright (C) 2008 Barcelona Supercomputing Center
Author: Vladimir Gajinov
*/

/*
This module defines macros to be used instead of OpenMP pragmas and for working with transactions.
*/


/* 
=============================================
OpenMP macros
=============================================
*/
#if defined(_OPENMP)
  #include <omp.h>
  //A more scalable malloc/free
  #if defined(INTEL)
	# define malloc kmp_malloc
	# define free kmp_free
  #endif
#else
  #include <malloc.h>
#endif

/* Ensure no warnings when compiling without OpenMP, by macro-ing away the pragmas */
#if defined (_OPENMP)
  # define Pragma(args) _Pragma (#args)
  # define OMP_ATOMIC Pragma(omp atomic)
  # define PARALLEL_FOR_SHARED(...) Pragma(omp parallel for shared (__VA_ARGS__))
  # define THREADPRIVATE(...) Pragma(omp threadprivate (__VA_ARGS__))
#else
  # define OMP_ATOMIC
  # define PARALLEL_FOR_SHARED(...) 
  # define THREADPRIVATE(...)
#endif



/* 
=============================================
Synchronization macros
=============================================
*/

#if defined(DO_LOCK)
  #define DECLARE_LOCK(lock)		omp_lock_t lock;
  #define INIT_LOCK(lock)			omp_init_lock(&(lock));
  #define LOCK(lock)				omp_set_lock(&(lock));
  #define UNLOCK(lock)				omp_unset_lock(&(lock));
  #define DECLARE_NEST_LOCK(lock)	omp_nest_lock_t lock;
  #define INIT_NEST_LOCK(lock)      omp_init_nest_lock(&(lock));
  #define LOCK_NEST(lock)			omp_set_nest_lock(&(lock));
  #define UNLOCK_NEST(lock)			omp_unset_nest_lock(&(lock));
# else
  #define DECLARE_LOCK(lock)
  #define INIT_LOCK(lock)
  #define LOCK(lock)
  #define UNLOCK(lock)
  #define DECLARE_NEST_LOCK(lock)
  #define INIT_NEST_LOCK(lock)
  #define LOCK_NEST(lock)
  #define UNLOCK_NEST(lock)
#endif

#if defined(DO_TM)
  #define TRANSACTION			__tm_atomic {
  #define TRANSACTION_END		}
  #define TM_CALLABLE			__attribute__((tm_callable))
  #define TM_ONLY				__attribute__((tm_only))
  #define TM_PURE				__attribute__((tm_pure))
  #define TM_UNKNOWN			__attribute__((tm_unknown))
#else
  #define TRANSACTION
  #define TRANSACTION_END
  #define TM_CALLABLE
  #define TM_ONLY
  #define TM_PURE
  #define TM_UNKNOWN
# endif


/* 
=============================================
Granularity macros
=============================================
*/

#if defined(DO_GLOBAL)
  #define TRANSACTION_GLOBAL		TRANSACTION
  #define TRANSACTION_GLOBAL_END	TRANSACTION_END
  #define LOCK_GLOBAL				LOCK(global_lock)
  #define UNLOCK_GLOBAL				UNLOCK(global_lock)
  #if defined(DO_NESTED)
	#define TRANSACTION_COARSE		TRANSACTION
	#define TRANSACTION_COARSE_END	TRANSACTION_END
  #endif
#else
  #define TRANSACTION_GLOBAL		
  #define TRANSACTION_GLOBAL_END	
  #define LOCK_GLOBAL				
  #define UNLOCK_GLOBAL				
#endif


#if defined(DO_COARSE)
  #define TRANSACTION_COARSE		TRANSACTION
  #define TRANSACTION_COARSE_END	TRANSACTION_END
  #define LOCK_COARSE				LOCK(global_lock)
  #define UNLOCK_COARSE				UNLOCK(global_lock)
#else
  #ifndef  TRANSACTION_COARSE		
    #define TRANSACTION_COARSE
  #endif
  #ifndef  TRANSACTION_COARSE_END	
    #define TRANSACTION_COARSE_END
  #endif
  #define LOCK_COARSE				LOCK(global_lock)
  #define UNLOCK_COARSE				UNLOCK(global_lock)
#endif

#if defined(DO_FINE)	
  #define TRANSACTION_FINE			TRANSACTION
  #define TRANSACTION_FINE_END		TRANSACTION_END
  #define LOCK_FINE(lock)			LOCK(lock)
  #define UNLOCK_FINE(lock)			UNLOCK(lock)
  #define LOCK_NEST_FINE(lock)		LOCK_NEST(lock)
  #define UNLOCK_NEST_FINE(lock)	UNLOCK_NEST(lock)
#else
  #define TRANSACTION_FINE			
  #define TRANSACTION_FINE_END		
  #define LOCK_FINE(lock)			
  #define UNLOCK_FINE(lock)			
  #define LOCK_NEST_FINE(lock)		
  #define UNLOCK_NEST_FINE(lock)	
#endif


/* 
=============================================
Parallel section macros
=============================================
*/

  // Level of debug info
#define  LOW    0
#define  MEDIUM 1
#define  HIGH   2

  // debugging & parallel regions
#define  READ_PACKETS 4
#define  PHYSICS      8
#define  SEND_REPLY   16
#define  GENERAL      32


#define DEBUGPRINT(DEBUG_REGION, DEBUG_DETAIL) if (((DEBUG & (~0x03)) & DEBUG_REGION) && ((DEBUG & 0x03) >= DEBUG_DETAIL))
#if defined(DO_TM)
  #define PRINT Con_Printf
#else
  #define PRINT printf
#endif


#define CACHELINE 16				// GAJA: Cache line size / sizeof(int)
#define MAXNUMTHR 8					// GAJA: Maximum number of threads in the run
#define NUMCHECKS CACHELINE*1		// GAJA: Number of checkpoints per thread 



/*
==========================================================================================================

FINE GRAINED PARALLELIZATION MACROS

==========================================================================================================
*/

#if (defined(DO_LOCK) && defined(DO_FINE))
  //#define LOCKPRINT(...) printf(__VA_ARGS__);
  #define LOCKPRINT(...)
  //#define LOCKPRINT if (0) printf
#else 
  #define LOCKPRINT(...)
#endif

#define SV_RELDATAGRAM_LOCK		1
#define SV_DATAGRAM_LOCK		2
#define SV_SIGNON_LOCK			4
#define SV_MULTICAST_LOCK		8
#define CLIENT_MSG_LOCK			16


/*
---------------------------
LOCK_CLIENT
---------------------------
Impose locking order
*/
#define LOCK_CLIENT(lock, clientId)		/* Lock clients */													\
	temp_locks = (int) pow(2,clientId);																		\
	if (!(client_locks & temp_locks))		/* Do not acquire the same lock */								\
	{																										\
		if (~(temp_locks - 1) & client_locks)	/* we already hold the higher order lock */					\
		{																									\
			/* Unlock all higer order locks */																\
			while ( temp_locks = temp_locks << 1)															\
			{																								\
				if (temp_locks & client_locks)																\
					UNLOCK_FINE(clients_ptr[(int) log2(temp_locks)]->msg_lock)								\
			}																								\
																											\
			temp_locks = (int) pow(2,clientId);																\
			/* Lock desired lock */																			\
			LOCK_FINE(lock)																					\
																											\
			/* Lock all higer order locks */																\
			while ( temp_locks = temp_locks << 1)															\
			{																								\
				if (temp_locks & client_locks)																\
					LOCK_FINE(clients_ptr[(int) log2(temp_locks)]->msg_lock)								\
			}																								\
		}																									\
		else																								\
			LOCK_FINE(lock)																					\
		client_locks |= (int) pow(2,clientId);																\
	}																										\
	client_nest_level[clientId]++;


/*
---------------------------
LOCKMSG
---------------------------
With an assumption that other lock are never acquired while holding on of the client locks
*/
#define LOCKMSG(lock, locktype, clientId)	/* Lock ordering */												\
		if (FrameStage & DOPARALLEL) {																		\
			LOCKPRINT("Thread: %d\tLOCKING\ttype = %d\tmsglocks_acquired = %d\n", omp_get_thread_num(), locktype, msglocks_acquired);				\
			if (locktype < CLIENT_MSG_LOCK)																	\
			{																								\
				if (!(locktype & msglocks_acquired))														\
				{																							\
					LOCK_FINE(lock)																			\
					LOCKPRINT("Thread: %d\tLock acquired %d\n", omp_get_thread_num(), locktype);			\
				}																							\
				msg_nest_level[(int) log2(locktype)]++;														\
			}																								\
			else	/* locktype == CLIENT_MSG_LOCK */														\
			{																								\
				LOCK_CLIENT(lock, clientId)																\
				LOCKPRINT("Thread: %d\tLock acquired %d\n", omp_get_thread_num(), locktype);			\
			}																								\
			msglocks_acquired |= locktype;																	\
			LOCKPRINT("Thread: %d\tFinished locking \tmsglocks_acquired = %d\n", omp_get_thread_num(), msglocks_acquired);				\
		}

/*
---------------------------
UNLOCKMSG
---------------------------
*/
#define UNLOCKMSG(lock, locktype, clientId)		/* Unlocking */		\
		if (FrameStage & DOPARALLEL) {								\
			LOCKPRINT("Thread: %d\tUnlocking  type = %d\tcl_locks = %d\tmsglocks_acquired = %d\n", omp_get_thread_num(), locktype, client_locks, msglocks_acquired); 	\
																											\
			if (locktype == CLIENT_MSG_LOCK)						\
			{														\
				LOCKPRINT("Thread: %d\tnest_level = %d\n", omp_get_thread_num(), client_nest_level[clientId]);				\
				if (client_nest_level[clientId] == 1)				\
				{													\
					UNLOCK_FINE(lock)								\
					client_nest_level[clientId] = 0;				\
					client_locks ^= (int) pow(2, clientId);			\
					if (client_locks == 0)							\
						msglocks_acquired ^= locktype;				\
				}													\
				else												\
					client_nest_level[clientId]--;					\
			}														\
			else													\
			{														\
				if (msg_nest_level[(int) log2(locktype)] == 1)		\
				{													\
					UNLOCK_FINE(lock)								\
					msglocks_acquired ^= locktype;					\
				}													\
				msg_nest_level[(int) log2(locktype)]--;				\
			}														\
			LOCKPRINT("Thread: %d\tLock released %d\n", omp_get_thread_num(), locktype);					\
		}

/*
---------------------------
LOCK_AREA
---------------------------
*/
#define LOCK_AREA(lock)			if (FrameStage & DOPARALLEL) {											\
									LOCKPRINT("THREAD %d\tLocking node %p\n", GetThreadId(), lock);		\
									LOCK_NEST_FINE(lock)												\
									LOCKPRINT("THREAD %d\tNode locked %p\n", GetThreadId(), lock);		\
								}

/*
---------------------------
UNLOCK_AREA
---------------------------
*/
#define UNLOCK_AREA(lock)		if (FrameStage & DOPARALLEL) {											\
									LOCKPRINT("THREAD %d\tUnlocking node %p\n", GetThreadId(), lock);	\
									UNLOCK_NEST_FINE(lock)												\
								}



/*
---------------------------
Declare lock related stuff
---------------------------
*/
extern int FrameStage;
//#ifndef LOCKING_VARS
//#define LOCKING_VARS
extern int msglocks_acquired;
extern int client_locks;			// Client locks acquired
extern int msg_nest_level[8];		// Hold the lock nesting levels for message locks other than clients' locks
extern int client_nest_level[32];	// Hold the lock nesting levels for clients' locks  -  should be MAX_CLIENTS lenght
extern int temp_locks;				// Temporary variable for locking procedures
extern int tm_nest_level;           // Nesting level for the transactional run without runtime nesting
THREADPRIVATE(msglocks_acquired, client_locks, msg_nest_level, client_nest_level, temp_locks, tm_nest_level)
//#endif


#ifdef DO_LOCK
  extern DECLARE_LOCK(global_lock)
  extern DECLARE_LOCK(sv_reldatagram_lock)
  extern DECLARE_LOCK(sv_datagram_lock)
  extern DECLARE_LOCK(sv_signon_lock)
  extern DECLARE_LOCK(sv_multicast_lock)
  extern DECLARE_NEST_LOCK(areanode_lock)
  extern DECLARE_NEST_LOCK(edict_lock)
  extern DECLARE_NEST_LOCK(memory_lock)
#endif




/*
---------------------------
TRANSACTION_INTERNAL
---------------------------
*/
#ifdef DO_NESTED
    #define TRANSACTION_INTERNAL(CODE)			\
				TRANSACTION_FINE                \
				do { CODE } while (0);			\
				TRANSACTION_FINE_END            
#else
    #define TRANSACTION_INTERNAL(CODE)			\
		do {                                    \
			if(tm_nest_level == 0){             \
				TRANSACTION_FINE                \
				tm_nest_level ++;				\
				do { CODE } while (0);			\
				TRANSACTION_FINE_END            \
			} else {                            \
				tm_nest_level ++;               \
				do { CODE } while (0);          \
			}                                   \
			tm_nest_level--;                    \
		} while (0);
#endif





/*
---------------------------
CRITICAL_SECTION
---------------------------
*/
#if (defined(DO_LOCK) && defined(DO_FINE))
  #define CRITICAL_SECTION(lock, CODE)											\
	if (FrameStage & DOPARALLEL) {												\
			LOCKPRINT("THREAD %d\tLocking node %p\n", GetThreadId(), lock);		\
			LOCK_NEST_FINE(lock)												\
			LOCKPRINT("THREAD %d\tNode locked %p\n", GetThreadId(), lock);		\
			do { CODE } while (0);												\
			LOCKPRINT("THREAD %d\tUnlocking node %p\n", GetThreadId(), lock);	\
			UNLOCK_NEST_FINE(lock)												\
	} else																		\
			do { CODE } while (0);		
#elif (defined(DO_TM) && defined(DO_FINE))
  #define CRITICAL_SECTION(lock, CODE)											\
	if (FrameStage & DOPARALLEL) {												\
			TRANSACTION_INTERNAL({do { CODE } while (0);});						\
	} else																		\
			do { CODE } while (0);
#else
  #define CRITICAL_SECTION(lock, CODE)											\
			do { CODE } while (0);
#endif


/*
---------------------------
CRITICAL_AREA
---------------------------
*/
#if (defined(DO_LOCK) && defined(DO_FINE))
  #define CRITICAL_AREA(lock, CODE)												\
	if (FrameStage & DOPARALLEL) {												\
			LOCKPRINT("THREAD %d\tLocking node %p\n", GetThreadId(), lock);		\
			LOCK_AREA(lock)														\
			LOCKPRINT("THREAD %d\tNode locked %p\n", GetThreadId(), lock);		\
			do { CODE } while (0);												\
			LOCKPRINT("THREAD %d\tUnlocking node %p\n", GetThreadId(), lock);	\
			UNLOCK_AREA(lock)													\
	} else																		\
			do { CODE } while (0);
#elif (defined(DO_TM) && defined(DO_FINE))
  #define CRITICAL_AREA(lock, CODE)												\
	if (FrameStage & DOPARALLEL) {												\
			TRANSACTION_INTERNAL({do { CODE } while (0);});						\
	} else																		\
			do { CODE } while (0);
#else
  #define CRITICAL_AREA(lock, CODE)												\
			do { CODE } while (0);
#endif


/*
---------------------------
CRITICAL_MESSAGE
---------------------------
*/
#if (defined(DO_LOCK) && defined(DO_FINE))
  #define CRITICAL_MESSAGE(lock, locktype, clientId, CODE)						\
	if (FrameStage & DOPARALLEL) {												\
			LOCKMSG(lock, locktype, clientId)									\
			do { CODE } while (0);												\
			UNLOCKMSG(lock, locktype, clientId)									\
	} else																		\
			do { CODE } while (0);		
#elif (defined(DO_TM) && defined(DO_FINE))
  #define CRITICAL_MESSAGE(lock, locktype, clientId, CODE)						\
	if (FrameStage & DOPARALLEL) {												\
			TRANSACTION_INTERNAL({do { CODE } while (0);});						\
	} else																		\
			do { CODE } while (0);												
#else
  #define CRITICAL_MESSAGE(lock, locktype, clientId, CODE)						\
			do { CODE } while (0);												
#endif






