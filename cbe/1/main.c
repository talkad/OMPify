/* Provide Declarations */
#include <stdint.h>
#ifndef __cplusplus
typedef unsigned char bool;
#endif

/* get a declaration for alloca */
#if defined(__CYGWIN__) || defined(__MINGW32__)
#define  alloca(x) __builtin_alloca((x))
#define _alloca(x) __builtin_alloca((x))
#elif defined(__APPLE__)
extern void *__builtin_alloca(unsigned long);
#define alloca(x) __builtin_alloca(x)
#define longjmp _longjmp
#define setjmp _setjmp
#elif defined(__sun__)
#if defined(__sparcv9)
extern void *__builtin_alloca(unsigned long);
#else
extern void *__builtin_alloca(unsigned int);
#endif
#define alloca(x) __builtin_alloca(x)
#elif defined(__FreeBSD__) || defined(__NetBSD__) || defined(__OpenBSD__) || defined(__DragonFly__) || defined(__arm__)
#define alloca(x) __builtin_alloca(x)
#elif defined(_MSC_VER)
#define alloca(x) _alloca(x)
#else
#include <alloca.h>
#endif

#ifndef _MSC_VER
#define __forceinline __attribute__((always_inline)) inline
#endif

#if defined(__GNUC__)
#define  __ATTRIBUTELIST__(x) __attribute__(x)
#else
#define  __ATTRIBUTELIST__(x)  
#endif

#ifdef _MSC_VER  /* Can only support "linkonce" vars with GCC */
#define __attribute__(X)
#endif



/* Global Declarations */

/* Types Declarations */
struct l_array_25_uint8_t;

/* Function definitions */
typedef uint32_t l_fptr_3(void);

typedef uint8_t* l_fptr_4(void);

typedef void l_fptr_2(void);

typedef void l_fptr_1(uint8_t*);


/* Types Definitions */
struct l_array_25_uint8_t {
  uint8_t array[25];
};

/* External Global Variable Declarations */

/* Function Declarations */
void _Z10omp_pragmaPc(uint8_t*) __ATTRIBUTELIST__((noinline, nothrow));
int main(void) __ATTRIBUTELIST__((noinline, nothrow));


/* Global Variable Definitions and Initialization */
static const struct l_array_25_uint8_t _OC_str = { "#pragma omp parallel for" };


/* LLVM Intrinsic Builtin Function Bodies */
static __forceinline uint32_t llvm_add_u32(uint32_t a, uint32_t b) {
  uint32_t r = a + b;
  return r;
}


/* Function Bodies */

void _Z10omp_pragmaPc(uint8_t* _1) {
  uint8_t* _2;    /* Address-exposed local */

  _2 = _1;
#line 4 "/home/talkad/CBE/Aidandos/high_performance_computing/assignment4/miniapp_openmp/aa.cpp"
  ;
}


int main(void) {
  uint32_t _3;    /* Address-exposed local */
  uint32_t _4;    /* Address-exposed local */
  uint32_t _5;    /* Address-exposed local */
  uint8_t* _6;    /* Address-exposed local */
  uint64_t _7;    /* Address-exposed local */
  uint32_t _8;    /* Address-exposed local */
  uint32_t _9;    /* Address-exposed local */
  uint32_t _10;    /* Address-exposed local */
  uint32_t _11;
  uint64_t _12;
  uint32_t* _13;
  uint32_t _14;
  uint32_t _15;
  uint32_t _16;
  uint32_t _17;
  uint32_t _18;
  uint32_t _19;
  uint32_t _20;
  uint32_t _21;
  uint32_t _22;
  uint8_t* _23;
  uint32_t _24;

  _3 = 0;
#line 8 "/home/talkad/CBE/Aidandos/high_performance_computing/assignment4/miniapp_openmp/aa.cpp"
  ;
#line 9 "/home/talkad/CBE/Aidandos/high_performance_computing/assignment4/miniapp_openmp/aa.cpp"
  ;
  _5 = 10000;
#line 10 "/home/talkad/CBE/Aidandos/high_performance_computing/assignment4/miniapp_openmp/aa.cpp"
  _11 = _5;
  _12 = ((uint64_t)(uint32_t)_11);
  _6 = ((uint8_t*)/*NULL*/0);
  _13 = (uint32_t*) alloca(sizeof(uint32_t) * (_12));
  _7 = _12;
#line 0 "/home/talkad/CBE/Aidandos/high_performance_computing/assignment4/miniapp_openmp/aa.cpp"
  ;
#line 10 "/home/talkad/CBE/Aidandos/high_performance_computing/assignment4/miniapp_openmp/aa.cpp"
  ;
#line 12 "/home/talkad/CBE/Aidandos/high_performance_computing/assignment4/miniapp_openmp/aa.cpp"
  ;
  _8 = 0;
#line 14 "/home/talkad/CBE/Aidandos/high_performance_computing/assignment4/miniapp_openmp/aa.cpp"
  _Z10omp_pragmaPc(((&_OC_str.array[((int64_t)0)])));
#line 15 "/home/talkad/CBE/Aidandos/high_performance_computing/assignment4/miniapp_openmp/aa.cpp"
  ;
  _9 = 0;
  goto _25;

  do {     /* Syntactic loop '' to make GCC happy */
_25:
  _14 = _9;
  _15 = _5;
  if ((((((int32_t)_14) < ((int32_t)_15))&1))) {
    goto _26;
  } else {
    goto _27;
  }

_26:
#line 16 "/home/talkad/CBE/Aidandos/high_performance_computing/assignment4/miniapp_openmp/aa.cpp"
  ;
  _10 = 0;
  goto _28;

  do {     /* Syntactic loop '' to make GCC happy */
_28:
  _16 = _10;
  _17 = _5;
  if ((((((int32_t)_16) < ((int32_t)_17))&1))) {
    goto _29;
  } else {
    goto _30;
  }

_29:
#line 17 "/home/talkad/CBE/Aidandos/high_performance_computing/assignment4/miniapp_openmp/aa.cpp"
  _18 = _10;
  _19 = *((&_13[((int64_t)(((int64_t)(int32_t)_18)))]));
  _20 = _8;
  _8 = (llvm_add_u32(_20, _19));
  goto _31;

_31:
#line 16 "/home/talkad/CBE/Aidandos/high_performance_computing/assignment4/miniapp_openmp/aa.cpp"
  _21 = _10;
  _10 = (llvm_add_u32(_21, 1));
  goto _28;

  } while (1); /* end of syntactic loop '' */
_30:
  goto _32;

_32:
#line 15 "/home/talkad/CBE/Aidandos/high_performance_computing/assignment4/miniapp_openmp/aa.cpp"
  _22 = _9;
  _9 = (llvm_add_u32(_22, 1));
  goto _25;

  } while (1); /* end of syntactic loop '' */
_27:
#line 18 "/home/talkad/CBE/Aidandos/high_performance_computing/assignment4/miniapp_openmp/aa.cpp"
  _23 = _6;
  _24 = _3;
  return _24;
}

