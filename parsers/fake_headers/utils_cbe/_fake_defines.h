
#define DECLARE_LOCK(x)
#define THREADPRIVATE(x)
#define DECLARE_NEST_LOCK(x)
#define SALT_CMP_OFF(a, b)
#define DBGVEC(a,b,c)
#define AIR_CAST(a, b)
#define SALT_CMP_SIZE(a,b,c,d)
#define QSORT(a,b,c,d)
#define SHA256_ALGORITHM_NAME
#define SHA512_ALGORITHM_NAME
#define MD4_ALGORITHM_NAME
#define SHA1_ALGORITHM_NAME
#define ARCH_BITS_STR
#define CRC32_C_ALGORITHM_NAME
#define DES_BS_ALGORITHM_NAME
#define INFO 1

#define FMT1 "hello"
#define FSTR64U "hello"
#define PRG_NAME "hello"
#define SPLATT_PF_IDX "hello"
#define ANSI_RED "hello"
#define RED "hello"
#define RESET "hello"
#define BOLDRED "hello"
#define RESET_COLOR "hello"
#define MPI_COMM_WORLD 1
#define NUMNODES 1
#define INIT 0
#define COUNT 0
#define CRUSH 0
#define XFORM 0
#define SORT 0
#define MPI_MSG 0
#define NVAR 100
#define WS 10
#define NDIM 200
#define PLOOP for(int ip = 0; ip < NVAR; ip++)
#define ZLOOP for(int ip = 0; ip < NVAR; ip++)
#define DLOOP1 for (int mu = 0; mu < NDIM; mu++)
#define DLOOP2 for (int mu = 0; mu < NDIM; mu++)	\
               for (int nu = 0; nu < NDIM; nu++)
#define REPETITIONS 1000 
#define LOOPCOUNT 1000

// #ifndef __GNUC_PREREQ
// # define __GNUC_PREREQ(ma, mi) 0
// #endif




// #ifndef __GLIBC_USE
// # define __GLIBC_USE(ma) 0
// #endif