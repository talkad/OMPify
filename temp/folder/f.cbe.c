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
typedef void l_fptr_2(void);

typedef uint32_t l_fptr_1(void);

typedef uint8_t* l_fptr_3(void);

typedef void l_fptr_4(uint8_t*);


/* Types Definitions */
struct l_array_25_uint8_t {
  uint8_t array[25];
};

/* External Global Variable Declarations */

/* Function Declarations */
int main(void) __ATTRIBUTELIST__((noinline));
void _Z21omp_pragma_talkad7420Pc(uint8_t*);


/* Global Variable Definitions and Initialization */
static const struct l_array_25_uint8_t _OC_str = { "#pragma omp parallel for" };


/* LLVM Intrinsic Builtin Function Bodies */
static __forceinline uint32_t llvm_add_u32(uint32_t a, uint32_t b) {
  uint32_t r = a + b;
  return r;
}


/* Function Bodies */

int main(void) {
  uint32_t _1;    /* Address-exposed local */
  uint32_t _2;    /* Address-exposed local */
  uint32_t _3;    /* Address-exposed local */
  uint8_t* _4;    /* Address-exposed local */
  uint64_t _5;    /* Address-exposed local */
  uint32_t _6;    /* Address-exposed local */
  uint32_t _7;    /* Address-exposed local */
  uint32_t _8;    /* Address-exposed local */
  uint32_t _9;
  uint64_t _10;
  uint32_t* _11;
  uint32_t _12;
  uint32_t _13;
  uint32_t _14;
  uint32_t _15;
  uint32_t _16;
  uint32_t _17;
  uint32_t _18;
  uint32_t _19;
  uint32_t _20;
  uint8_t* _21;
  uint32_t _22;

  _1 = 0;
#line 7 "/home/talkad/OpenMPdb/temp/folder/f.cpp"
  ;
#line 8 "/home/talkad/OpenMPdb/temp/folder/f.cpp"
  ;
  _3 = 10000;
#line 9 "/home/talkad/OpenMPdb/temp/folder/f.cpp"
  _9 = _3;
  _10 = ((uint64_t)(uint32_t)_9);
  _4 = ((uint8_t*)/*NULL*/0);
  _11 = (uint32_t*) alloca(sizeof(uint32_t) * (_10));
  _5 = _10;
#line 0 "/home/talkad/OpenMPdb/temp/folder/f.cpp"
  ;
#line 9 "/home/talkad/OpenMPdb/temp/folder/f.cpp"
  ;
#line 10 "/home/talkad/OpenMPdb/temp/folder/f.cpp"
  ;
  _6 = 0;
#line 11 "/home/talkad/OpenMPdb/temp/folder/f.cpp"
  _Z21omp_pragma_talkad7420Pc(((&_OC_str.array[((int64_t)0)])));
#line 12 "/home/talkad/OpenMPdb/temp/folder/f.cpp"
  ;
  _7 = 0;
  goto _23;

  do {     /* Syntactic loop '' to make GCC happy */
_23:
  _12 = _7;
  _13 = _3;
  if ((((((int32_t)_12) < ((int32_t)_13))&1))) {
    goto _24;
  } else {
    goto _25;
  }

_24:
#line 14 "/home/talkad/OpenMPdb/temp/folder/f.cpp"
  _Z21omp_pragma_talkad7420Pc(((&_OC_str.array[((int64_t)0)])));
#line 15 "/home/talkad/OpenMPdb/temp/folder/f.cpp"
  ;
  _8 = 0;
  goto _26;

  do {     /* Syntactic loop '' to make GCC happy */
_26:
  _14 = _8;
  _15 = _3;
  if ((((((int32_t)_14) < ((int32_t)_15))&1))) {
    goto _27;
  } else {
    goto _28;
  }

_27:
#line 17 "/home/talkad/OpenMPdb/temp/folder/f.cpp"
  _16 = _8;
  _17 = *((&_11[((int64_t)(((int64_t)(int32_t)_16)))]));
  _18 = _6;
  _6 = (llvm_add_u32(_18, _17));
  goto _29;

_29:
#line 15 "/home/talkad/OpenMPdb/temp/folder/f.cpp"
  _19 = _8;
  _8 = (llvm_add_u32(_19, 1));
  goto _26;

  } while (1); /* end of syntactic loop '' */
_28:
  goto _30;

_30:
#line 12 "/home/talkad/OpenMPdb/temp/folder/f.cpp"
  _20 = _7;
  _7 = (llvm_add_u32(_20, 1));
  goto _23;

  } while (1); /* end of syntactic loop '' */
_25:
#line 20 "/home/talkad/OpenMPdb/temp/folder/f.cpp"
  _6 = 1;
#line 21 "/home/talkad/OpenMPdb/temp/folder/f.cpp"
  _21 = _4;
  _22 = _1;
  return _22;
}

