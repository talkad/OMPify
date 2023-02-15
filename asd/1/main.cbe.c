/* Provide Declarations */
#include <stdint.h>
#ifndef __cplusplus
typedef unsigned char bool;
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
struct l_array_1_uint8_t;
struct l_array_20_uint8_t;
struct l_struct_struct_OC__IO_FILE;
struct l_array_16_uint8_t;
struct l_array_21_uint8_t;
struct l_array_27_uint8_t;
struct l_array_31_uint8_t;

/* Function definitions */
typedef void l_fptr_9(uint32_t);

typedef uint32_t l_fptr_7(struct l_struct_struct_OC__IO_FILE*, uint8_t*, ...);

typedef uint32_t l_fptr_4(int, ...);

typedef void l_fptr_6(int, ...);

typedef uint8_t* l_fptr_8(uint64_t);

typedef void l_fptr_3(void);

typedef void l_fptr_5(uint8_t*);

typedef uint32_t l_fptr_1(uint32_t, ...);

typedef uint32_t l_fptr_2(void);


/* Types Definitions */
struct l_array_1_uint8_t {
  uint8_t array[1];
};
struct l_array_20_uint8_t {
  uint8_t array[20];
};
struct l_struct_struct_OC__IO_FILE {
  uint32_t field0;
  uint8_t* field1;
  uint8_t* field2;
  uint8_t* field3;
  uint8_t* field4;
  uint8_t* field5;
  uint8_t* field6;
  uint8_t* field7;
  uint8_t* field8;
  uint8_t* field9;
  uint8_t* field10;
  uint8_t* field11;
  void* field12;
  struct l_struct_struct_OC__IO_FILE* field13;
  uint32_t field14;
  uint32_t field15;
  uint64_t field16;
  uint16_t field17;
  uint8_t field18;
  struct l_array_1_uint8_t field19;
  uint8_t* field20;
  uint64_t field21;
  void* field22;
  void* field23;
  struct l_struct_struct_OC__IO_FILE* field24;
  uint8_t* field25;
  uint64_t field26;
  uint32_t field27;
  struct l_array_20_uint8_t field28;
};
struct l_array_16_uint8_t {
  uint8_t array[16];
};
struct l_array_21_uint8_t {
  uint8_t array[21];
};
struct l_array_27_uint8_t {
  uint8_t array[27];
};
struct l_array_31_uint8_t {
  uint8_t array[31];
};

/* External Global Variable Declarations */
extern struct l_struct_struct_OC__IO_FILE* stderr;

/* Function Declarations */
uint32_t test_omp_threadprivate(void) __ATTRIBUTELIST__((noinline, nothrow));
uint32_t omp_set_dynamic(int, ...);
void omp_for_pragma_talkad7420(uint8_t*);
void for_loop_talkad7420(int, ...);
void omp_pragma_talkad7420(uint8_t*);
uint32_t fprintf(struct l_struct_struct_OC__IO_FILE*, uint8_t*, ...);
uint32_t omp_get_num_threads(int, ...);
uint8_t* malloc(uint64_t) __ATTRIBUTELIST__((nothrow));
void srand(uint32_t) __ATTRIBUTELIST__((nothrow));
uint32_t rand(void) __ATTRIBUTELIST__((nothrow));
uint32_t omp_get_thread_num(int, ...);
void free(uint8_t*) __ATTRIBUTELIST__((nothrow));
int main(void) __ATTRIBUTELIST__((noinline, nothrow));


/* Global Variable Definitions and Initialization */
static uint32_t sum0;
static const struct l_array_16_uint8_t _OC_str = { "#pragma omp for" };
static const struct l_array_21_uint8_t _OC_str_OC_1 = { "#pragma omp critical" };
static const struct l_array_27_uint8_t _OC_str_OC_2 = { " known_sum = %d, sum = %d\n" };
static uint32_t myvalue;
static const struct l_array_31_uint8_t _OC_str_OC_3 = { " myvalue = %d, data[rank]= %d\n" };


/* LLVM Intrinsic Builtin Function Bodies */
static __forceinline uint32_t llvm_add_u32(uint32_t a, uint32_t b) {
  uint32_t r = a + b;
  return r;
}
static __forceinline uint64_t llvm_mul_u64(uint64_t a, uint64_t b) {
  uint64_t r = a * b;
  return r;
}


/* Function Bodies */

uint32_t test_omp_threadprivate(void) {
  uint32_t _1;    /* Address-exposed local */
  uint32_t _2;    /* Address-exposed local */
  uint32_t _3;    /* Address-exposed local */
  uint32_t _4;    /* Address-exposed local */
  uint32_t* _5;    /* Address-exposed local */
  uint32_t _6;    /* Address-exposed local */
  uint32_t _7;    /* Address-exposed local */
  uint32_t _8;    /* Address-exposed local */
  uint32_t _9;    /* Address-exposed local */
  uint32_t _10;    /* Address-exposed local */
  uint32_t _11;
  uint32_t _12;
  uint32_t _13;
  uint32_t _14;
  uint32_t _15;
  uint32_t _16;
  uint32_t _17;
  uint32_t _18;
  uint32_t _19;
  struct l_struct_struct_OC__IO_FILE* _20;
  uint32_t _21;
  uint32_t _22;
  uint32_t _23;
  uint32_t _24;
  uint32_t _25;
  uint32_t _26;
  uint8_t* _27;
  uint32_t _28;
  uint32_t _29;
  uint32_t _30;
  uint32_t _31;
  uint32_t _32;
  uint32_t _33;
  uint32_t* _34;
  uint32_t _35;
  uint32_t _36;
  uint32_t _37;
  uint32_t _38;
  uint32_t* _39;
  uint32_t _40;
  uint32_t _41;
  uint32_t _42;
  uint32_t* _43;
  uint32_t _44;
  uint32_t _45;
  struct l_struct_struct_OC__IO_FILE* _46;
  uint32_t _47;
  uint32_t* _48;
  uint32_t _49;
  uint32_t _50;
  uint32_t _51;
  uint32_t _52;
  uint32_t* _53;
  uint32_t _54;
  uint32_t _55;
  uint32_t _56;
  bool _57;
  bool _58;
  bool _58__PHI_TEMPORARY;

#line 13 "/home/talkad/OpenMPdb/temp/flang-cavium/flang7/projects/openmp/runtime/test/threadprivate/omp_threadprivate.c"
  ;
  _1 = 0;
#line 14 "/home/talkad/OpenMPdb/temp/flang-cavium/flang7/projects/openmp/runtime/test/threadprivate/omp_threadprivate.c"
  ;
#line 15 "/home/talkad/OpenMPdb/temp/flang-cavium/flang7/projects/openmp/runtime/test/threadprivate/omp_threadprivate.c"
  ;
#line 16 "/home/talkad/OpenMPdb/temp/flang-cavium/flang7/projects/openmp/runtime/test/threadprivate/omp_threadprivate.c"
  ;
#line 17 "/home/talkad/OpenMPdb/temp/flang-cavium/flang7/projects/openmp/runtime/test/threadprivate/omp_threadprivate.c"
  ;
#line 18 "/home/talkad/OpenMPdb/temp/flang-cavium/flang7/projects/openmp/runtime/test/threadprivate/omp_threadprivate.c"
  ;
#line 19 "/home/talkad/OpenMPdb/temp/flang-cavium/flang7/projects/openmp/runtime/test/threadprivate/omp_threadprivate.c"
  ;
  _7 = 0;
#line 20 "/home/talkad/OpenMPdb/temp/flang-cavium/flang7/projects/openmp/runtime/test/threadprivate/omp_threadprivate.c"
  ;
#line 21 "/home/talkad/OpenMPdb/temp/flang-cavium/flang7/projects/openmp/runtime/test/threadprivate/omp_threadprivate.c"
  _11 = ((l_fptr_1*)(void*)omp_set_dynamic)(0);
#line 24 "/home/talkad/OpenMPdb/temp/flang-cavium/flang7/projects/openmp/runtime/test/threadprivate/omp_threadprivate.c"
  sum0 = 0;
#line 25 "/home/talkad/OpenMPdb/temp/flang-cavium/flang7/projects/openmp/runtime/test/threadprivate/omp_threadprivate.c"
  omp_for_pragma_talkad7420(((&_OC_str.array[((int64_t)0)])));
#line 26 "/home/talkad/OpenMPdb/temp/flang-cavium/flang7/projects/openmp/runtime/test/threadprivate/omp_threadprivate.c"
  for_loop_talkad7420(0 /*dummy arg*/);
#line 27 "/home/talkad/OpenMPdb/temp/flang-cavium/flang7/projects/openmp/runtime/test/threadprivate/omp_threadprivate.c"
  _3 = 1;
  goto _59;

  do {     /* Syntactic loop '' to make GCC happy */
_59:
  _12 = _3;
  if ((((((int32_t)_12) <= ((int32_t)1000u))&1))) {
    goto _60;
  } else {
    goto _61;
  }

_60:
#line 28 "/home/talkad/OpenMPdb/temp/flang-cavium/flang7/projects/openmp/runtime/test/threadprivate/omp_threadprivate.c"
  _13 = sum0;
  _14 = _3;
  sum0 = (llvm_add_u32(_13, _14));
  goto _62;

  omp_pragma_talkad7420(((&_OC_str_OC_1.array[((int64_t)0)])));

_62:
#line 27 "/home/talkad/OpenMPdb/temp/flang-cavium/flang7/projects/openmp/runtime/test/threadprivate/omp_threadprivate.c"
  _15 = _3;
  _3 = (llvm_add_u32(_15, 1));
  goto _59;

  } while (1); /* end of syntactic loop '' */
_61:
#line 30 "/home/talkad/OpenMPdb/temp/flang-cavium/flang7/projects/openmp/runtime/test/threadprivate/omp_threadprivate.c"
  omp_pragma_talkad7420(((&_OC_str_OC_1.array[((int64_t)0)])));
#line 32 "/home/talkad/OpenMPdb/temp/flang-cavium/flang7/projects/openmp/runtime/test/threadprivate/omp_threadprivate.c"
  _16 = _1;
  _17 = sum0;
  _1 = (llvm_add_u32(_16, _17));
#line 35 "/home/talkad/OpenMPdb/temp/flang-cavium/flang7/projects/openmp/runtime/test/threadprivate/omp_threadprivate.c"
  _2 = 500500;
#line 36 "/home/talkad/OpenMPdb/temp/flang-cavium/flang7/projects/openmp/runtime/test/threadprivate/omp_threadprivate.c"
  _18 = _2;
  _19 = _1;
  if ((((_18 != _19)&1))) {
    goto _63;
  } else {
    goto _64;
  }

_63:
#line 37 "/home/talkad/OpenMPdb/temp/flang-cavium/flang7/projects/openmp/runtime/test/threadprivate/omp_threadprivate.c"
  _20 = stderr;
  _21 = _2;
  _22 = _1;
  _23 = fprintf(_20, ((&_OC_str_OC_2.array[((int64_t)0)])), _21, _22);
  goto _64;

_64:
#line 39 "/home/talkad/OpenMPdb/temp/flang-cavium/flang7/projects/openmp/runtime/test/threadprivate/omp_threadprivate.c"
  _24 = ((l_fptr_1*)(void*)omp_set_dynamic)(0);
#line 44 "/home/talkad/OpenMPdb/temp/flang-cavium/flang7/projects/openmp/runtime/test/threadprivate/omp_threadprivate.c"
  _25 = omp_get_num_threads(0 /*dummy arg*/);
  _6 = _25;
#line 45 "/home/talkad/OpenMPdb/temp/flang-cavium/flang7/projects/openmp/runtime/test/threadprivate/omp_threadprivate.c"
  _26 = _6;
  _27 = malloc((llvm_mul_u64((((int64_t)(int32_t)_26)), 4)));
  _5 = (((uint32_t*)_27));
#line 48 "/home/talkad/OpenMPdb/temp/flang-cavium/flang7/projects/openmp/runtime/test/threadprivate/omp_threadprivate.c"
  srand(45);


_67:
#line 69 "/home/talkad/OpenMPdb/temp/flang-cavium/flang7/projects/openmp/runtime/test/threadprivate/omp_threadprivate.c"
  _53 = _5;
  free((((uint8_t*)_53)));
#line 70 "/home/talkad/OpenMPdb/temp/flang-cavium/flang7/projects/openmp/runtime/test/threadprivate/omp_threadprivate.c"
  _54 = _2;
  _55 = _1;
  if ((((_54 == _55)&1))) {
    goto _71;
  } else {
    _58__PHI_TEMPORARY = 0;   /* for PHI node */
    goto _72;
  }

_71:
  _56 = _7;
  _57 = ((~((((_56 != 0u)&1))))&1);
  _58__PHI_TEMPORARY = _57;   /* for PHI node */
  goto _72;

_72:
  _58 = ((_58__PHI_TEMPORARY)&1);
#line 70 "/home/talkad/OpenMPdb/temp/flang-cavium/flang7/projects/openmp/runtime/test/threadprivate/omp_threadprivate.c"
  return (((uint32_t)(bool)_58));
}


int main(void) {
  uint32_t _73;    /* Address-exposed local */
  uint32_t _74;    /* Address-exposed local */
  uint32_t _75;    /* Address-exposed local */
  uint32_t _76;
  uint32_t _77;
  uint32_t _78;
  uint32_t _79;
  uint32_t _80;

  _73 = 0;
#line 74 "/home/talkad/OpenMPdb/temp/flang-cavium/flang7/projects/openmp/runtime/test/threadprivate/omp_threadprivate.c"
  ;
#line 75 "/home/talkad/OpenMPdb/temp/flang-cavium/flang7/projects/openmp/runtime/test/threadprivate/omp_threadprivate.c"
  ;
  _75 = 0;
#line 76 "/home/talkad/OpenMPdb/temp/flang-cavium/flang7/projects/openmp/runtime/test/threadprivate/omp_threadprivate.c"
 
_83:
#line 82 "/home/talkad/OpenMPdb/temp/flang-cavium/flang7/projects/openmp/runtime/test/threadprivate/omp_threadprivate.c"
  _80 = _75;
  return _80;
}

