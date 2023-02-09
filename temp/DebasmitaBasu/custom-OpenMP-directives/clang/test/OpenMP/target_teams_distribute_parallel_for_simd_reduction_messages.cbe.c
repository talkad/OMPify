/* Provide Declarations */
#include <stdint.h>
#ifndef __cplusplus
typedef unsigned char bool;
#endif

#ifndef _MSC_VER
#define __forceinline __attribute__((always_inline)) inline
#endif

#if defined(__GNUC__) && defined(__APPLE_CC__)
#define __ATTRIBUTE_WEAK__
#elif defined(__GNUC__)
#define __ATTRIBUTE_WEAK__ __attribute__((weak))
#else
#define __ATTRIBUTE_WEAK__
#endif

#if defined(__GNUC__)
#define  __ATTRIBUTELIST__(x) __attribute__(x)
#else
#define  __ATTRIBUTELIST__(x)  
#endif

#ifdef _MSC_VER  /* Can only support "linkonce" vars with GCC */
#define __attribute__(X)
#endif

#ifdef _MSC_VER
#define __MSALIGN__(X) __declspec(align(X))
#else
#define __MSALIGN__(X)
#endif



/* Global Declarations */

/* Types Declarations */
struct l_struct_class_OC_S2;
struct l_struct_class_OC_S3;
struct l_struct_class_OC_S6;
struct l_struct_class_OC_S4;
struct l_struct_class_OC_S5;
struct l_array_5_struct_AC_l_struct_class_OC_S2;
struct l_array_5_struct_AC_l_struct_class_OC_S3;
struct l_array_65_uint8_t;
struct l_array_67_uint8_t;
struct l_array_66_uint8_t;
struct l_array_68_uint8_t;
struct l_array_76_uint8_t;
struct l_array_75_uint8_t;
struct l_array_100_uint8_t;
struct l_array_73_uint8_t;
struct l_array_84_uint8_t;
struct l_array_86_uint8_t;
struct l_array_79_uint8_t;
struct l_array_80_uint8_t;
struct l_array_81_uint8_t;
struct l_array_72_uint8_t;
struct l_array_102_uint8_t;
struct l_array_90_uint8_t;
struct l_array_74_uint8_t;
struct l_array_87_uint8_t;
struct l_array_77_uint8_t;
struct l_array_5_uint32_t;
struct l_array_5_float;

/* Function definitions */
typedef uint32_t l_fptr_9(uint32_t, uint8_t**);

typedef uint8_t* l_fptr_16(uint8_t*, uint32_t, uint64_t);

typedef void l_fptr_4(struct l_struct_class_OC_S2*);

typedef bool l_fptr_2(uint32_t);

typedef uint32_t l_fptr_14(uint32_t);

typedef void l_fptr_10(uint8_t*, uint8_t, uint64_t, bool);

typedef void l_fptr_1(void);

typedef void l_fptr_3(void);

typedef struct l_struct_class_OC_S4* l_fptr_7(struct l_struct_class_OC_S4*, struct l_struct_class_OC_S4*);

typedef void l_fptr_12(struct l_struct_class_OC_S5*, uint32_t);

typedef void l_fptr_6(struct l_struct_class_OC_S3*);

typedef void l_fptr_8(struct l_struct_class_OC_S6*);

typedef void l_fptr_13(uint8_t*);

typedef float l_fptr_15(float);

typedef void l_fptr_11(struct l_struct_class_OC_S4*, uint32_t);

typedef uint32_t l_fptr_5(struct l_struct_class_OC_S3*, struct l_struct_class_OC_S3*);


/* Types Definitions */
struct l_struct_class_OC_S2 {
  uint32_t field0;
};
struct l_struct_class_OC_S3 {
  uint32_t field0;
  uint32_t field1;
};
struct l_struct_class_OC_S6 {
  uint32_t field0;
};
struct l_struct_class_OC_S4 {
  uint32_t field0;
};
struct l_struct_class_OC_S5 {
  uint32_t field0;
};
struct l_array_5_struct_AC_l_struct_class_OC_S2 {
  struct l_struct_class_OC_S2 array[5];
};
struct l_array_5_struct_AC_l_struct_class_OC_S3 {
  struct l_struct_class_OC_S3 array[5];
};
struct l_array_65_uint8_t {
  uint8_t array[65];
};
struct l_array_67_uint8_t {
  uint8_t array[67];
};
struct l_array_66_uint8_t {
  uint8_t array[66];
};
struct l_array_68_uint8_t {
  uint8_t array[68];
};
struct l_array_76_uint8_t {
  uint8_t array[76];
};
struct l_array_75_uint8_t {
  uint8_t array[75];
};
struct l_array_100_uint8_t {
  uint8_t array[100];
};
struct l_array_73_uint8_t {
  uint8_t array[73];
};
struct l_array_84_uint8_t {
  uint8_t array[84];
};
struct l_array_86_uint8_t {
  uint8_t array[86];
};
struct l_array_79_uint8_t {
  uint8_t array[79];
};
struct l_array_80_uint8_t {
  uint8_t array[80];
};
struct l_array_81_uint8_t {
  uint8_t array[81];
};
struct l_array_72_uint8_t {
  uint8_t array[72];
};
struct l_array_102_uint8_t {
  uint8_t array[102];
};
struct l_array_90_uint8_t {
  uint8_t array[90];
};
struct l_array_74_uint8_t {
  uint8_t array[74];
};
struct l_array_87_uint8_t {
  uint8_t array[87];
};
struct l_array_77_uint8_t {
  uint8_t array[77];
};
struct l_array_5_uint32_t {
  uint32_t array[5];
};
struct l_array_5_float {
  float array[5];
};

/* External Global Variable Declarations */

/* Function Declarations */
void _Z3foov(void) __ATTRIBUTELIST__((noinline, nothrow));
bool _Z7foobooli(uint32_t) __ATTRIBUTELIST__((noinline, nothrow));
static void __cxx_global_var_init(void) __ATTRIBUTELIST__((noinline));
void _ZN2S2C2Ev(struct l_struct_class_OC_S2*) __ATTRIBUTELIST__((noinline, nothrow)) __ATTRIBUTE_WEAK__;
static void __cxx_global_var_init_OC_1(void) __ATTRIBUTELIST__((noinline));
uint32_t _ZplRK2S3S1_(struct l_struct_class_OC_S3*, struct l_struct_class_OC_S3*) __ATTRIBUTELIST__((noinline, nothrow));
static void __cxx_global_var_init_OC_2(void) __ATTRIBUTELIST__((noinline));
void _ZN2S3C2Ev(struct l_struct_class_OC_S3*) __ATTRIBUTELIST__((noinline, nothrow)) __ATTRIBUTE_WEAK__;
static void __cxx_global_var_init_OC_3(void) __ATTRIBUTELIST__((noinline));
struct l_struct_class_OC_S4* _ZaNR2S4S0_(struct l_struct_class_OC_S4*, struct l_struct_class_OC_S4*) __ATTRIBUTELIST__((noinline, nothrow));
static void __cxx_global_var_init_OC_4(void) __ATTRIBUTELIST__((noinline));
void _ZN2S6C2Ev(struct l_struct_class_OC_S6*) __ATTRIBUTELIST__((noinline, nothrow)) __ATTRIBUTE_WEAK__;
static void __cxx_global_var_init_OC_5(void) __ATTRIBUTELIST__((noinline));
static void __cxx_global_var_init_OC_6(void) __ATTRIBUTELIST__((noinline));
int main(int, char **) __ATTRIBUTELIST__((noinline));
void _ZN2S4C2Ei(struct l_struct_class_OC_S4*, uint32_t) __ATTRIBUTELIST__((noinline, nothrow)) __ATTRIBUTE_WEAK__;
void _ZN2S5C2Ei(struct l_struct_class_OC_S5*, uint32_t) __ATTRIBUTELIST__((noinline, nothrow)) __ATTRIBUTE_WEAK__;
void _Z25omp_for_pragma_talkad7420Pc(uint8_t*);
void _Z19for_loop_talkad7420v(void);
uint32_t _Z5tmainIiET_S0_(uint32_t) __ATTRIBUTELIST__((noinline)) __ATTRIBUTE_WEAK__;
float _Z5tmainIfET_S0_(float) __ATTRIBUTELIST__((noinline)) __ATTRIBUTE_WEAK__;
static void _GLOBAL__sub_I_target_teams_distribute_parallel_for_simd_reduction_messages_OC_cpp(void) __ATTRIBUTELIST__((noinline));
uint8_t* memset(uint8_t*, uint32_t, uint64_t);


/* Global Variable Definitions and Initialization */
const float _ZN2S24S2scE;
struct l_struct_class_OC_S2 b;
static __MSALIGN__(16) struct l_array_5_struct_AC_l_struct_class_OC_S2 _ZL2ba __attribute__((aligned(16)));
struct l_struct_class_OC_S3 c;
static __MSALIGN__(16) struct l_array_5_struct_AC_l_struct_class_OC_S3 _ZL2ca __attribute__((aligned(16)));
struct l_struct_class_OC_S6 o;
struct l_struct_class_OC_S3 h;
struct l_struct_class_OC_S3 k;
double _ZN1A1xE;
static const struct l_array_65_uint8_t _OC_str = { "#pragma omp target teams distribute parallel for simd reduction " };
static const struct l_array_67_uint8_t _OC_str_OC_7 = { "#pragma omp target teams distribute parallel for simd reduction + " };
static const struct l_array_66_uint8_t _OC_str_OC_8 = { "#pragma omp target teams distribute parallel for simd reduction( " };
static const struct l_array_67_uint8_t _OC_str_OC_9 = { "#pragma omp target teams distribute parallel for simd reduction(- " };
static const struct l_array_67_uint8_t _OC_str_OC_10 = { "#pragma omp target teams distribute parallel for simd reduction() " };
static const struct l_array_68_uint8_t _OC_str_OC_11 = { "#pragma omp target teams distribute parallel for simd reduction(*) " };
static const struct l_array_76_uint8_t _OC_str_OC_12 = { "#pragma omp target teams distribute parallel for simd reduction(foo : argc " };
static const struct l_array_75_uint8_t _OC_str_OC_13 = { "#pragma omp target teams distribute parallel for simd reduction(| : argc, " };
static const struct l_array_100_uint8_t _OC_str_OC_14 = { "#pragma omp target teams distribute parallel for simd reduction(|| : argc > 0 ? argv[1] : argv[2]) " };
static const struct l_array_75_uint8_t _OC_str_OC_15 = { "#pragma omp target teams distribute parallel for simd reduction(~ : argc) " };
static const struct l_array_75_uint8_t _OC_str_OC_16 = { "#pragma omp target teams distribute parallel for simd reduction(&& : argc)" };
static const struct l_array_73_uint8_t _OC_str_OC_17 = { "#pragma omp target teams distribute parallel for simd reduction(^ : s1) " };
static const struct l_array_84_uint8_t _OC_str_OC_18 = { "#pragma omp target teams distribute parallel for simd reduction(+ : a, b, c, d, f) " };
static const struct l_array_86_uint8_t _OC_str_OC_19 = { "#pragma omp target teams distribute parallel for simd reduction(min : a, b, c, d, f) " };
static const struct l_array_76_uint8_t _OC_str_OC_20 = { "#pragma omp target teams distribute parallel for simd reduction(max : h.b) " };
static const struct l_array_73_uint8_t _OC_str_OC_21 = { "#pragma omp target teams distribute parallel for simd reduction(+ : ba) " };
static const struct l_array_73_uint8_t _OC_str_OC_22 = { "#pragma omp target teams distribute parallel for simd reduction(* : ca) " };
static const struct l_array_73_uint8_t _OC_str_OC_23 = { "#pragma omp target teams distribute parallel for simd reduction(- : da) " };
static const struct l_array_73_uint8_t _OC_str_OC_24 = { "#pragma omp target teams distribute parallel for simd reduction(^ : fl) " };
static const struct l_array_79_uint8_t _OC_str_OC_25 = { "#pragma omp target teams distribute parallel for simd reduction(&& : s2::s2s) " };
static const struct l_array_80_uint8_t _OC_str_OC_26 = { "#pragma omp target teams distribute parallel for simd reduction(&& : s2::s2sc) " };
static const struct l_array_75_uint8_t _OC_str_OC_27 = { "#pragma omp target teams distribute parallel for simd reduction(& : e, g) " };
static const struct l_array_81_uint8_t _OC_str_OC_28 = { "#pragma omp target teams distribute parallel for simd reduction(+ : h, k, b::x) " };
static const struct l_array_72_uint8_t _OC_str_OC_29 = { "#pragma omp target teams distribute parallel for simd reduction(+ : o) " };
static const struct l_array_102_uint8_t _OC_str_OC_30 = { "#pragma omp target teams distribute parallel for simd private(i), reduction(+ : j), reduction(+ : q) " };
static const struct l_array_90_uint8_t _OC_str_OC_31 = { "#pragma omp target teams distribute parallel for simd reduction(+ : p), reduction(+ : p) " };
static const struct l_array_72_uint8_t _OC_str_OC_32 = { "#pragma omp target teams distribute parallel for simd reduction(+ : r) " };
static const struct l_array_74_uint8_t _OC_str_OC_33 = { "#pragma omp target teams distribute parallel for simd reduction(max : j) " };
static const struct l_array_72_uint8_t _OC_str_OC_34 = { "#pragma omp target teams distribute parallel for simd reduction(+ : fl)" };
static uint32_t _ZZ4mainE1m;
static const struct l_array_72_uint8_t _OC_str_OC_35 = { "#pragma omp target teams distribute parallel for simd reduction(+ : m) " };
static const struct l_array_74_uint8_t _OC_str_OC_36 = { "#pragma omp target teams distribute parallel for simd reduction(& : argc " };
static const struct l_array_87_uint8_t _OC_str_OC_37 = { "#pragma omp target teams distribute parallel for simd reduction(|| : argc ? i : argc) " };
static const struct l_array_77_uint8_t _OC_str_OC_38 = { "#pragma omp target teams distribute parallel for simd reduction(foo : argc) " };
static const struct l_array_72_uint8_t _OC_str_OC_39 = { "#pragma omp target teams distribute parallel for simd reduction(^ : t) " };
static const struct l_array_75_uint8_t _OC_str_OC_40 = { "#pragma omp target teams distribute parallel for simd reduction(+ : h, k) " };


/* LLVM Intrinsic Builtin Function Bodies */
static __forceinline uint32_t llvm_add_u32(uint32_t a, uint32_t b) {
  uint32_t r = a + b;
  return r;
}
static __forceinline float llvm_fadd_f32(float a, float b) {
  float r = a + b;
  return r;
}


/* Function Bodies */

void _Z3foov(void) {
  return;
}


bool _Z7foobooli(uint32_t _1) {
  uint32_t _2;    /* Address-exposed local */
  uint32_t _3;

  _2 = _1;
#line 6 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  ;
#line 7 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _3 = _2;
  return (((_3 != 0u)&1));
}


static void __cxx_global_var_init(void) {
#line 24 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _ZN2S2C2Ev((&b));
}


void _ZN2S2C2Ev(struct l_struct_class_OC_S2* _4) {
  struct l_struct_class_OC_S2* _5;    /* Address-exposed local */
  struct l_struct_class_OC_S2* _6;

  _5 = _4;
#line 0 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  ;
  _6 = _5;
#line 16 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  *((&_6->field0)) = 0;
}


static void __cxx_global_var_init_OC_1(void) {
  struct l_struct_class_OC_S2* _7;
  struct l_struct_class_OC_S2* _7__PHI_TEMPORARY;
  struct l_struct_class_OC_S2* _8;

  _7__PHI_TEMPORARY = ((&_ZL2ba.array[((int32_t)0)]));   /* for PHI node */
  goto _9;

  do {     /* Syntactic loop '' to make GCC happy */
_9:
#line 25 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _7 = _7__PHI_TEMPORARY;
  _ZN2S2C2Ev(_7);
  _8 = (&_7[((int64_t)1)]);
  if ((((_8 == ((&((&_ZL2ba.array[((int32_t)0)]))[((int64_t)5)])))&1))) {
    goto _10;
  } else {
    _7__PHI_TEMPORARY = _8;   /* for PHI node */
    goto _9;
  }

  } while (1); /* end of syntactic loop '' */
_10:
  return;
}


uint32_t _ZplRK2S3S1_(struct l_struct_class_OC_S3* _11, struct l_struct_class_OC_S3* _12) {
  struct l_struct_class_OC_S3* _13;    /* Address-exposed local */
  struct l_struct_class_OC_S3* _14;    /* Address-exposed local */

  _13 = _11;
#line 37 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  ;
  _14 = _12;
  ;
  return 5;
}


static void __cxx_global_var_init_OC_2(void) {
#line 39 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _ZN2S3C2Ev((&c));
}


void _ZN2S3C2Ev(struct l_struct_class_OC_S3* _15) {
  struct l_struct_class_OC_S3* _16;    /* Address-exposed local */
  struct l_struct_class_OC_S3* _17;

  _16 = _15;
#line 0 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  ;
  _17 = _16;
#line 30 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  *((&_17->field0)) = 0;
}


static void __cxx_global_var_init_OC_3(void) {
  struct l_struct_class_OC_S3* _18;
  struct l_struct_class_OC_S3* _18__PHI_TEMPORARY;
  struct l_struct_class_OC_S3* _19;

  _18__PHI_TEMPORARY = ((&_ZL2ca.array[((int32_t)0)]));   /* for PHI node */
  goto _20;

  do {     /* Syntactic loop '' to make GCC happy */
_20:
#line 40 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _18 = _18__PHI_TEMPORARY;
  _ZN2S3C2Ev(_18);
  _19 = (&_18[((int64_t)1)]);
  if ((((_19 == ((&((&_ZL2ca.array[((int32_t)0)]))[((int64_t)5)])))&1))) {
    goto _21;
  } else {
    _18__PHI_TEMPORARY = _19;   /* for PHI node */
    goto _20;
  }

  } while (1); /* end of syntactic loop '' */
_21:
  return;
}


struct l_struct_class_OC_S4* _ZaNR2S4S0_(struct l_struct_class_OC_S4* _22, struct l_struct_class_OC_S4* _23) {
  struct l_struct_class_OC_S4* _24;    /* Address-exposed local */
  struct l_struct_class_OC_S4* _25;    /* Address-exposed local */
  struct l_struct_class_OC_S4* _26;

  _24 = _22;
#line 52 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  ;
  _25 = _23;
  ;
  _26 = _24;
  return _26;
}


static void __cxx_global_var_init_OC_4(void) {
#line 74 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _ZN2S6C2Ev((&o));
}


void _ZN2S6C2Ev(struct l_struct_class_OC_S6* _27) {
  struct l_struct_class_OC_S6* _28;    /* Address-exposed local */
  struct l_struct_class_OC_S6* _29;

  _28 = _27;
#line 0 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  ;
  _29 = _28;
#line 70 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  *((&_29->field0)) = 6;
}


static void __cxx_global_var_init_OC_5(void) {
#line 75 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _ZN2S3C2Ev((&h));
}


static void __cxx_global_var_init_OC_6(void) {
  _ZN2S3C2Ev((&k));
}


int main(int argc, char ** argv) {
  uint32_t _30 = (uint32_t)argc;
  uint8_t** _31 = (uint8_t**)argv;
  uint32_t _32;    /* Address-exposed local */
  uint32_t _33;    /* Address-exposed local */
  uint8_t** _34;    /* Address-exposed local */
  uint32_t _35;    /* Address-exposed local */
  __MSALIGN__(16) struct l_array_5_uint32_t _36 __attribute__((aligned(16)));    /* Address-exposed local */
  __MSALIGN__(16) struct l_array_5_uint32_t _37 __attribute__((aligned(16)));    /* Address-exposed local */
  struct l_struct_class_OC_S4 _38;    /* Address-exposed local */
  struct l_struct_class_OC_S5 _39;    /* Address-exposed local */
  uint32_t _40;    /* Address-exposed local */
  uint32_t* _41;    /* Address-exposed local */
  struct l_struct_class_OC_S3* _42;    /* Address-exposed local */
  uint32_t* _43;    /* Address-exposed local */
  uint32_t* _44;    /* Address-exposed local */
  float _45;    /* Address-exposed local */
  uint32_t _46;    /* Address-exposed local */
  uint32_t _47;    /* Address-exposed local */
  uint32_t _48;    /* Address-exposed local */
  uint32_t _49;    /* Address-exposed local */
  uint32_t _50;    /* Address-exposed local */
  uint32_t _51;    /* Address-exposed local */
  uint32_t _52;    /* Address-exposed local */
  uint32_t _53;    /* Address-exposed local */
  uint32_t _54;    /* Address-exposed local */
  uint32_t _55;    /* Address-exposed local */
  uint32_t _56;    /* Address-exposed local */
  uint32_t _57;    /* Address-exposed local */
  uint32_t _58;    /* Address-exposed local */
  uint32_t _59;    /* Address-exposed local */
  uint32_t _60;    /* Address-exposed local */
  uint32_t _61;    /* Address-exposed local */
  uint32_t _62;    /* Address-exposed local */
  uint32_t _63;    /* Address-exposed local */
  uint32_t _64;    /* Address-exposed local */
  uint32_t _65;    /* Address-exposed local */
  uint32_t _66;    /* Address-exposed local */
  uint32_t _67;    /* Address-exposed local */
  uint32_t _68;    /* Address-exposed local */
  uint32_t _69;    /* Address-exposed local */
  uint32_t _70;    /* Address-exposed local */
  uint32_t _71;    /* Address-exposed local */
  uint32_t _72;    /* Address-exposed local */
  uint32_t _73;    /* Address-exposed local */
  uint32_t _74;    /* Address-exposed local */
  uint32_t _75;    /* Address-exposed local */
  uint32_t _76;    /* Address-exposed local */
  uint32_t _77;    /* Address-exposed local */
  uint8_t* _78;
  uint8_t* _79;
  uint32_t _80;
  uint32_t _81;
  uint32_t _82;
  uint32_t _83;
  uint32_t _84;
  uint32_t _85;
  uint32_t _86;
  uint32_t _87;
  uint32_t _88;
  uint32_t _89;
  uint32_t _90;
  uint32_t _91;
  uint32_t _92;
  uint32_t _93;
  uint32_t _94;
  uint32_t _95;
  uint32_t _96;
  uint32_t _97;
  uint32_t _98;
  uint32_t _99;
  uint32_t _100;
  uint32_t _101;
  uint32_t _102;
  uint32_t _103;
  uint32_t _104;
  uint32_t _105;
  uint32_t _106;
  uint32_t _107;
  uint32_t _108;
  uint32_t _109;
  uint32_t _110;
  uint32_t _111;
  uint32_t _112;
  uint32_t _113;
  uint32_t _114;
  uint32_t _115;
  uint32_t _116;
  uint32_t _117;
  uint32_t _118;
  uint32_t _119;
  uint32_t _120;
  uint32_t _121;
  uint32_t _122;
  uint32_t _123;
  uint32_t _124;
  uint32_t _125;
  uint32_t _126;
  uint32_t _127;
  uint32_t _128;
  uint32_t _129;
  uint32_t _130;
  uint32_t _131;
  uint32_t _132;
  uint32_t _133;
  uint32_t _134;
  uint32_t _135;
  uint32_t _136;
  uint32_t _137;
  uint32_t _138;
  uint32_t _139;
  uint32_t _140;
  uint32_t _141;
  uint32_t _142;
  uint32_t _143;
  uint32_t _144;
  uint32_t _145;
  uint32_t _146;
  uint32_t _147;
  float _148;
  float _149;

  _32 = 0;
  _33 = _30;
#line 252 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  ;
  _34 = _31;
  ;
#line 253 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  ;
  _35 = 5;
#line 254 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  ;
  _78 = memset((((uint8_t*)(&_36))), 0, 20);
#line 256 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  ;
  _79 = memset((((uint8_t*)(&_37))), 0, 20);
#line 258 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  ;
  _ZN2S4C2Ei((&_38), 4);
#line 259 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  ;
  _ZN2S5C2Ei((&_39), 5);
#line 260 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  ;
#line 261 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  ;
  _41 = (&_40);
#line 262 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  ;
  _42 = (&k);
#line 263 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  ;
  _80 = _40;
  _43 = ((&_36.array[((int64_t)(((int64_t)(int32_t)_80)))]));
#line 264 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  ;
  _81 = _40;
  _44 = ((&_37.array[((int64_t)(((int64_t)(int32_t)_81)))]));
#line 265 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  ;
#line 266 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z25omp_for_pragma_talkad7420Pc(((&_OC_str.array[((int64_t)0)])));
#line 267 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z19for_loop_talkad7420v();
#line 268 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  ;
  _46 = 0;
  goto _150;

  do {     /* Syntactic loop '' to make GCC happy */
_150:
  _82 = _46;
  if ((((((int32_t)_82) < ((int32_t)100u))&1))) {
    goto _151;
  } else {
    goto _152;
  }

_151:
  _Z3foov();
  goto _153;

_153:
  _83 = _46;
  _46 = (llvm_add_u32(_83, 1));
  goto _150;

  } while (1); /* end of syntactic loop '' */
_152:
#line 271 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z25omp_for_pragma_talkad7420Pc(((&_OC_str_OC_7.array[((int64_t)0)])));
#line 272 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z19for_loop_talkad7420v();
#line 273 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  ;
  _47 = 0;
  goto _154;

  do {     /* Syntactic loop '' to make GCC happy */
_154:
  _84 = _47;
  if ((((((int32_t)_84) < ((int32_t)100u))&1))) {
    goto _155;
  } else {
    goto _156;
  }

_155:
  _Z3foov();
  goto _157;

_157:
  _85 = _47;
  _47 = (llvm_add_u32(_85, 1));
  goto _154;

  } while (1); /* end of syntactic loop '' */
_156:
#line 276 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z25omp_for_pragma_talkad7420Pc(((&_OC_str_OC_8.array[((int64_t)0)])));
#line 277 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z19for_loop_talkad7420v();
#line 278 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  ;
  _48 = 0;
  goto _158;

  do {     /* Syntactic loop '' to make GCC happy */
_158:
  _86 = _48;
  if ((((((int32_t)_86) < ((int32_t)100u))&1))) {
    goto _159;
  } else {
    goto _160;
  }

_159:
  _Z3foov();
  goto _161;

_161:
  _87 = _48;
  _48 = (llvm_add_u32(_87, 1));
  goto _158;

  } while (1); /* end of syntactic loop '' */
_160:
#line 281 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z25omp_for_pragma_talkad7420Pc(((&_OC_str_OC_9.array[((int64_t)0)])));
#line 282 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z19for_loop_talkad7420v();
#line 283 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  ;
  _49 = 0;
  goto _162;

  do {     /* Syntactic loop '' to make GCC happy */
_162:
  _88 = _49;
  if ((((((int32_t)_88) < ((int32_t)100u))&1))) {
    goto _163;
  } else {
    goto _164;
  }

_163:
  _Z3foov();
  goto _165;

_165:
  _89 = _49;
  _49 = (llvm_add_u32(_89, 1));
  goto _162;

  } while (1); /* end of syntactic loop '' */
_164:
#line 286 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z25omp_for_pragma_talkad7420Pc(((&_OC_str_OC_10.array[((int64_t)0)])));
#line 287 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z19for_loop_talkad7420v();
#line 288 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  ;
  _50 = 0;
  goto _166;

  do {     /* Syntactic loop '' to make GCC happy */
_166:
  _90 = _50;
  if ((((((int32_t)_90) < ((int32_t)100u))&1))) {
    goto _167;
  } else {
    goto _168;
  }

_167:
  _Z3foov();
  goto _169;

_169:
  _91 = _50;
  _50 = (llvm_add_u32(_91, 1));
  goto _166;

  } while (1); /* end of syntactic loop '' */
_168:
#line 291 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z25omp_for_pragma_talkad7420Pc(((&_OC_str_OC_11.array[((int64_t)0)])));
#line 292 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z19for_loop_talkad7420v();
#line 293 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  ;
  _51 = 0;
  goto _170;

  do {     /* Syntactic loop '' to make GCC happy */
_170:
  _92 = _51;
  if ((((((int32_t)_92) < ((int32_t)100u))&1))) {
    goto _171;
  } else {
    goto _172;
  }

_171:
  _Z3foov();
  goto _173;

_173:
  _93 = _51;
  _51 = (llvm_add_u32(_93, 1));
  goto _170;

  } while (1); /* end of syntactic loop '' */
_172:
#line 296 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z25omp_for_pragma_talkad7420Pc(((&_OC_str_OC_10.array[((int64_t)0)])));
#line 297 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z19for_loop_talkad7420v();
#line 298 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  ;
  _52 = 0;
  goto _174;

  do {     /* Syntactic loop '' to make GCC happy */
_174:
  _94 = _52;
  if ((((((int32_t)_94) < ((int32_t)100u))&1))) {
    goto _175;
  } else {
    goto _176;
  }

_175:
  _Z3foov();
  goto _177;

_177:
  _95 = _52;
  _52 = (llvm_add_u32(_95, 1));
  goto _174;

  } while (1); /* end of syntactic loop '' */
_176:
#line 301 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z25omp_for_pragma_talkad7420Pc(((&_OC_str_OC_12.array[((int64_t)0)])));
#line 302 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z19for_loop_talkad7420v();
#line 303 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  ;
  _53 = 0;
  goto _178;

  do {     /* Syntactic loop '' to make GCC happy */
_178:
  _96 = _53;
  if ((((((int32_t)_96) < ((int32_t)100u))&1))) {
    goto _179;
  } else {
    goto _180;
  }

_179:
  _Z3foov();
  goto _181;

_181:
  _97 = _53;
  _53 = (llvm_add_u32(_97, 1));
  goto _178;

  } while (1); /* end of syntactic loop '' */
_180:
#line 306 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z25omp_for_pragma_talkad7420Pc(((&_OC_str_OC_13.array[((int64_t)0)])));
#line 307 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z19for_loop_talkad7420v();
#line 308 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  ;
  _54 = 0;
  goto _182;

  do {     /* Syntactic loop '' to make GCC happy */
_182:
  _98 = _54;
  if ((((((int32_t)_98) < ((int32_t)100u))&1))) {
    goto _183;
  } else {
    goto _184;
  }

_183:
  _Z3foov();
  goto _185;

_185:
  _99 = _54;
  _54 = (llvm_add_u32(_99, 1));
  goto _182;

  } while (1); /* end of syntactic loop '' */
_184:
#line 311 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z25omp_for_pragma_talkad7420Pc(((&_OC_str_OC_14.array[((int64_t)0)])));
#line 312 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z19for_loop_talkad7420v();
#line 313 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  ;
  _55 = 0;
  goto _186;

  do {     /* Syntactic loop '' to make GCC happy */
_186:
  _100 = _55;
  if ((((((int32_t)_100) < ((int32_t)100u))&1))) {
    goto _187;
  } else {
    goto _188;
  }

_187:
  _Z3foov();
  goto _189;

_189:
  _101 = _55;
  _55 = (llvm_add_u32(_101, 1));
  goto _186;

  } while (1); /* end of syntactic loop '' */
_188:
#line 316 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z25omp_for_pragma_talkad7420Pc(((&_OC_str_OC_15.array[((int64_t)0)])));
#line 317 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z19for_loop_talkad7420v();
#line 318 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  ;
  _56 = 0;
  goto _190;

  do {     /* Syntactic loop '' to make GCC happy */
_190:
  _102 = _56;
  if ((((((int32_t)_102) < ((int32_t)100u))&1))) {
    goto _191;
  } else {
    goto _192;
  }

_191:
  _Z3foov();
  goto _193;

_193:
  _103 = _56;
  _56 = (llvm_add_u32(_103, 1));
  goto _190;

  } while (1); /* end of syntactic loop '' */
_192:
#line 321 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z25omp_for_pragma_talkad7420Pc(((&_OC_str_OC_16.array[((int64_t)0)])));
#line 322 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z19for_loop_talkad7420v();
#line 323 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  ;
  _57 = 0;
  goto _194;

  do {     /* Syntactic loop '' to make GCC happy */
_194:
  _104 = _57;
  if ((((((int32_t)_104) < ((int32_t)100u))&1))) {
    goto _195;
  } else {
    goto _196;
  }

_195:
  _Z3foov();
  goto _197;

_197:
  _105 = _57;
  _57 = (llvm_add_u32(_105, 1));
  goto _194;

  } while (1); /* end of syntactic loop '' */
_196:
#line 326 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z25omp_for_pragma_talkad7420Pc(((&_OC_str_OC_17.array[((int64_t)0)])));
#line 327 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z19for_loop_talkad7420v();
#line 328 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  ;
  _58 = 0;
  goto _198;

  do {     /* Syntactic loop '' to make GCC happy */
_198:
  _106 = _58;
  if ((((((int32_t)_106) < ((int32_t)100u))&1))) {
    goto _199;
  } else {
    goto _200;
  }

_199:
  _Z3foov();
  goto _201;

_201:
  _107 = _58;
  _58 = (llvm_add_u32(_107, 1));
  goto _198;

  } while (1); /* end of syntactic loop '' */
_200:
#line 331 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z25omp_for_pragma_talkad7420Pc(((&_OC_str_OC_18.array[((int64_t)0)])));
#line 332 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z19for_loop_talkad7420v();
#line 333 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  ;
  _59 = 0;
  goto _202;

  do {     /* Syntactic loop '' to make GCC happy */
_202:
  _108 = _59;
  if ((((((int32_t)_108) < ((int32_t)100u))&1))) {
    goto _203;
  } else {
    goto _204;
  }

_203:
  _Z3foov();
  goto _205;

_205:
  _109 = _59;
  _59 = (llvm_add_u32(_109, 1));
  goto _202;

  } while (1); /* end of syntactic loop '' */
_204:
#line 336 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z25omp_for_pragma_talkad7420Pc(((&_OC_str_OC_19.array[((int64_t)0)])));
#line 337 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z19for_loop_talkad7420v();
#line 338 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  ;
  _60 = 0;
  goto _206;

  do {     /* Syntactic loop '' to make GCC happy */
_206:
  _110 = _60;
  if ((((((int32_t)_110) < ((int32_t)100u))&1))) {
    goto _207;
  } else {
    goto _208;
  }

_207:
  _Z3foov();
  goto _209;

_209:
  _111 = _60;
  _60 = (llvm_add_u32(_111, 1));
  goto _206;

  } while (1); /* end of syntactic loop '' */
_208:
#line 341 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z25omp_for_pragma_talkad7420Pc(((&_OC_str_OC_20.array[((int64_t)0)])));
#line 342 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z19for_loop_talkad7420v();
#line 343 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  ;
  _61 = 0;
  goto _210;

  do {     /* Syntactic loop '' to make GCC happy */
_210:
  _112 = _61;
  if ((((((int32_t)_112) < ((int32_t)100u))&1))) {
    goto _211;
  } else {
    goto _212;
  }

_211:
  _Z3foov();
  goto _213;

_213:
  _113 = _61;
  _61 = (llvm_add_u32(_113, 1));
  goto _210;

  } while (1); /* end of syntactic loop '' */
_212:
#line 346 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z25omp_for_pragma_talkad7420Pc(((&_OC_str_OC_21.array[((int64_t)0)])));
#line 347 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z19for_loop_talkad7420v();
#line 348 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  ;
  _62 = 0;
  goto _214;

  do {     /* Syntactic loop '' to make GCC happy */
_214:
  _114 = _62;
  if ((((((int32_t)_114) < ((int32_t)100u))&1))) {
    goto _215;
  } else {
    goto _216;
  }

_215:
  _Z3foov();
  goto _217;

_217:
  _115 = _62;
  _62 = (llvm_add_u32(_115, 1));
  goto _214;

  } while (1); /* end of syntactic loop '' */
_216:
#line 351 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z25omp_for_pragma_talkad7420Pc(((&_OC_str_OC_22.array[((int64_t)0)])));
#line 352 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z19for_loop_talkad7420v();
#line 353 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  ;
  _63 = 0;
  goto _218;

  do {     /* Syntactic loop '' to make GCC happy */
_218:
  _116 = _63;
  if ((((((int32_t)_116) < ((int32_t)100u))&1))) {
    goto _219;
  } else {
    goto _220;
  }

_219:
  _Z3foov();
  goto _221;

_221:
  _117 = _63;
  _63 = (llvm_add_u32(_117, 1));
  goto _218;

  } while (1); /* end of syntactic loop '' */
_220:
#line 356 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z25omp_for_pragma_talkad7420Pc(((&_OC_str_OC_23.array[((int64_t)0)])));
#line 357 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z19for_loop_talkad7420v();
#line 358 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  ;
  _64 = 0;
  goto _222;

  do {     /* Syntactic loop '' to make GCC happy */
_222:
  _118 = _64;
  if ((((((int32_t)_118) < ((int32_t)100u))&1))) {
    goto _223;
  } else {
    goto _224;
  }

_223:
  _Z3foov();
  goto _225;

_225:
  _119 = _64;
  _64 = (llvm_add_u32(_119, 1));
  goto _222;

  } while (1); /* end of syntactic loop '' */
_224:
#line 361 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z25omp_for_pragma_talkad7420Pc(((&_OC_str_OC_24.array[((int64_t)0)])));
#line 362 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z19for_loop_talkad7420v();
#line 363 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  ;
  _65 = 0;
  goto _226;

  do {     /* Syntactic loop '' to make GCC happy */
_226:
  _120 = _65;
  if ((((((int32_t)_120) < ((int32_t)100u))&1))) {
    goto _227;
  } else {
    goto _228;
  }

_227:
  _Z3foov();
  goto _229;

_229:
  _121 = _65;
  _65 = (llvm_add_u32(_121, 1));
  goto _226;

  } while (1); /* end of syntactic loop '' */
_228:
#line 366 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z25omp_for_pragma_talkad7420Pc(((&_OC_str_OC_25.array[((int64_t)0)])));
#line 367 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z19for_loop_talkad7420v();
#line 368 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  ;
  _66 = 0;
  goto _230;

  do {     /* Syntactic loop '' to make GCC happy */
_230:
  _122 = _66;
  if ((((((int32_t)_122) < ((int32_t)100u))&1))) {
    goto _231;
  } else {
    goto _232;
  }

_231:
  _Z3foov();
  goto _233;

_233:
  _123 = _66;
  _66 = (llvm_add_u32(_123, 1));
  goto _230;

  } while (1); /* end of syntactic loop '' */
_232:
#line 371 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z25omp_for_pragma_talkad7420Pc(((&_OC_str_OC_26.array[((int64_t)0)])));
#line 372 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z19for_loop_talkad7420v();
#line 373 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  ;
  _67 = 0;
  goto _234;

  do {     /* Syntactic loop '' to make GCC happy */
_234:
  _124 = _67;
  if ((((((int32_t)_124) < ((int32_t)100u))&1))) {
    goto _235;
  } else {
    goto _236;
  }

_235:
  _Z3foov();
  goto _237;

_237:
  _125 = _67;
  _67 = (llvm_add_u32(_125, 1));
  goto _234;

  } while (1); /* end of syntactic loop '' */
_236:
#line 376 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z25omp_for_pragma_talkad7420Pc(((&_OC_str_OC_27.array[((int64_t)0)])));
#line 377 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z19for_loop_talkad7420v();
#line 378 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  ;
  _68 = 0;
  goto _238;

  do {     /* Syntactic loop '' to make GCC happy */
_238:
  _126 = _68;
  if ((((((int32_t)_126) < ((int32_t)100u))&1))) {
    goto _239;
  } else {
    goto _240;
  }

_239:
  _Z3foov();
  goto _241;

_241:
  _127 = _68;
  _68 = (llvm_add_u32(_127, 1));
  goto _238;

  } while (1); /* end of syntactic loop '' */
_240:
#line 381 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z25omp_for_pragma_talkad7420Pc(((&_OC_str_OC_28.array[((int64_t)0)])));
#line 382 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z19for_loop_talkad7420v();
#line 383 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  ;
  _69 = 0;
  goto _242;

  do {     /* Syntactic loop '' to make GCC happy */
_242:
  _128 = _69;
  if ((((((int32_t)_128) < ((int32_t)100u))&1))) {
    goto _243;
  } else {
    goto _244;
  }

_243:
  _Z3foov();
  goto _245;

_245:
  _129 = _69;
  _69 = (llvm_add_u32(_129, 1));
  goto _242;

  } while (1); /* end of syntactic loop '' */
_244:
#line 386 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z25omp_for_pragma_talkad7420Pc(((&_OC_str_OC_29.array[((int64_t)0)])));
#line 387 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z19for_loop_talkad7420v();
#line 388 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  ;
  _70 = 0;
  goto _246;

  do {     /* Syntactic loop '' to make GCC happy */
_246:
  _130 = _70;
  if ((((((int32_t)_130) < ((int32_t)100u))&1))) {
    goto _247;
  } else {
    goto _248;
  }

_247:
  _Z3foov();
  goto _249;

_249:
  _131 = _70;
  _70 = (llvm_add_u32(_131, 1));
  goto _246;

  } while (1); /* end of syntactic loop '' */
_248:
#line 391 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z25omp_for_pragma_talkad7420Pc(((&_OC_str_OC_30.array[((int64_t)0)])));
#line 392 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z19for_loop_talkad7420v();
#line 393 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  ;
  _71 = 0;
  goto _250;

  do {     /* Syntactic loop '' to make GCC happy */
_250:
  _132 = _71;
  if ((((((int32_t)_132) < ((int32_t)100u))&1))) {
    goto _251;
  } else {
    goto _252;
  }

_251:
  _Z3foov();
  goto _253;

_253:
  _133 = _71;
  _71 = (llvm_add_u32(_133, 1));
  goto _250;

  } while (1); /* end of syntactic loop '' */
_252:
#line 397 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z25omp_for_pragma_talkad7420Pc(((&_OC_str_OC_31.array[((int64_t)0)])));
#line 398 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z19for_loop_talkad7420v();
#line 399 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  ;
  _72 = 0;
  goto _254;

  do {     /* Syntactic loop '' to make GCC happy */
_254:
  _134 = _72;
  if ((((((int32_t)_134) < ((int32_t)100u))&1))) {
    goto _255;
  } else {
    goto _256;
  }

_255:
  _Z3foov();
  goto _257;

_257:
  _135 = _72;
  _72 = (llvm_add_u32(_135, 1));
  goto _254;

  } while (1); /* end of syntactic loop '' */
_256:
#line 402 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z25omp_for_pragma_talkad7420Pc(((&_OC_str_OC_31.array[((int64_t)0)])));
#line 403 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z19for_loop_talkad7420v();
#line 404 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  ;
  _73 = 0;
  goto _258;

  do {     /* Syntactic loop '' to make GCC happy */
_258:
  _136 = _73;
  if ((((((int32_t)_136) < ((int32_t)100u))&1))) {
    goto _259;
  } else {
    goto _260;
  }

_259:
  _Z3foov();
  goto _261;

_261:
  _137 = _73;
  _73 = (llvm_add_u32(_137, 1));
  goto _258;

  } while (1); /* end of syntactic loop '' */
_260:
#line 407 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z25omp_for_pragma_talkad7420Pc(((&_OC_str_OC_32.array[((int64_t)0)])));
#line 408 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z19for_loop_talkad7420v();
#line 409 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  ;
  _74 = 0;
  goto _262;

  do {     /* Syntactic loop '' to make GCC happy */
_262:
  _138 = _74;
  if ((((((int32_t)_138) < ((int32_t)100u))&1))) {
    goto _263;
  } else {
    goto _264;
  }

_263:
  _Z3foov();
  goto _265;

_265:
  _139 = _74;
  _74 = (llvm_add_u32(_139, 1));
  goto _262;

  } while (1); /* end of syntactic loop '' */
_264:
#line 414 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z25omp_for_pragma_talkad7420Pc(((&_OC_str_OC_33.array[((int64_t)0)])));
#line 415 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z19for_loop_talkad7420v();
#line 416 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  ;
  _75 = 0;
  goto _266;

  do {     /* Syntactic loop '' to make GCC happy */
_266:
  _140 = _75;
  if ((((((int32_t)_140) < ((int32_t)100u))&1))) {
    goto _267;
  } else {
    goto _268;
  }

_267:
  _Z3foov();
  goto _269;

_269:
  _141 = _75;
  _75 = (llvm_add_u32(_141, 1));
  goto _266;

  } while (1); /* end of syntactic loop '' */
_268:
#line 419 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z25omp_for_pragma_talkad7420Pc(((&_OC_str_OC_34.array[((int64_t)0)])));
#line 420 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z19for_loop_talkad7420v();
#line 421 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  ;
  _76 = 0;
  goto _270;

  do {     /* Syntactic loop '' to make GCC happy */
_270:
  _142 = _76;
  if ((((((int32_t)_142) < ((int32_t)100u))&1))) {
    goto _271;
  } else {
    goto _272;
  }

_271:
  _Z3foov();
  goto _273;

_273:
  _143 = _76;
  _76 = (llvm_add_u32(_143, 1));
  goto _270;

  } while (1); /* end of syntactic loop '' */
_272:
#line 425 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z25omp_for_pragma_talkad7420Pc(((&_OC_str_OC_35.array[((int64_t)0)])));
#line 426 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z19for_loop_talkad7420v();
#line 427 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  ;
  _77 = 0;
  goto _274;

  do {     /* Syntactic loop '' to make GCC happy */
_274:
  _144 = _77;
  if ((((((int32_t)_144) < ((int32_t)100u))&1))) {
    goto _275;
  } else {
    goto _276;
  }

_275:
  _Z3foov();
  goto _277;

_277:
  _145 = _77;
  _77 = (llvm_add_u32(_145, 1));
  goto _274;

  } while (1); /* end of syntactic loop '' */
_276:
#line 430 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _146 = _33;
  _147 = _Z5tmainIiET_S0_(_146);
  _148 = _45;
  _149 = _Z5tmainIfET_S0_(_148);
  return (((int32_t)(llvm_fadd_f32((((float)(int32_t)_147)), _149))));
}


void _ZN2S4C2Ei(struct l_struct_class_OC_S4* _278, uint32_t _279) {
  struct l_struct_class_OC_S4* _280;    /* Address-exposed local */
  uint32_t _281;    /* Address-exposed local */
  struct l_struct_class_OC_S4* _282;
  uint32_t _283;

  _280 = _278;
#line 0 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  ;
  _281 = _279;
#line 49 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  ;
  _282 = _280;
  _283 = _281;
  *((&_282->field0)) = _283;
}


void _ZN2S5C2Ei(struct l_struct_class_OC_S5* _284, uint32_t _285) {
  struct l_struct_class_OC_S5* _286;    /* Address-exposed local */
  uint32_t _287;    /* Address-exposed local */
  struct l_struct_class_OC_S5* _288;
  uint32_t _289;

  _286 = _284;
#line 0 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  ;
  _287 = _285;
#line 62 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  ;
  _288 = _286;
  _289 = _287;
  *((&_288->field0)) = _289;
}


uint32_t _Z5tmainIiET_S0_(uint32_t _290) {
  uint32_t _291;    /* Address-exposed local */
  uint32_t _292;    /* Address-exposed local */
  __MSALIGN__(16) struct l_array_5_uint32_t _293 __attribute__((aligned(16)));    /* Address-exposed local */
  __MSALIGN__(16) struct l_array_5_uint32_t _294 __attribute__((aligned(16)));    /* Address-exposed local */
  uint32_t _295;    /* Address-exposed local */
  uint32_t* _296;    /* Address-exposed local */
  struct l_struct_class_OC_S3* _297;    /* Address-exposed local */
  uint32_t* _298;    /* Address-exposed local */
  uint32_t* _299;    /* Address-exposed local */
  uint32_t _300;    /* Address-exposed local */
  uint32_t _301;    /* Address-exposed local */
  uint32_t _302;    /* Address-exposed local */
  uint32_t _303;    /* Address-exposed local */
  uint32_t _304;    /* Address-exposed local */
  uint32_t _305;    /* Address-exposed local */
  uint32_t _306;    /* Address-exposed local */
  uint32_t _307;    /* Address-exposed local */
  uint32_t _308;    /* Address-exposed local */
  uint32_t _309;    /* Address-exposed local */
  uint32_t _310;    /* Address-exposed local */
  uint32_t _311;    /* Address-exposed local */
  uint32_t _312;    /* Address-exposed local */
  uint32_t _313;    /* Address-exposed local */
  uint32_t _314;    /* Address-exposed local */
  uint32_t _315;    /* Address-exposed local */
  uint32_t _316;    /* Address-exposed local */
  uint32_t _317;    /* Address-exposed local */
  uint32_t _318;    /* Address-exposed local */
  uint32_t _319;    /* Address-exposed local */
  uint32_t _320;    /* Address-exposed local */
  uint32_t _321;    /* Address-exposed local */
  uint32_t _322;    /* Address-exposed local */
  uint32_t _323;    /* Address-exposed local */
  uint32_t _324;    /* Address-exposed local */
  uint32_t _325;    /* Address-exposed local */
  uint32_t _326;    /* Address-exposed local */
  uint32_t _327;    /* Address-exposed local */
  uint32_t _328;    /* Address-exposed local */
  uint32_t _329;    /* Address-exposed local */
  uint32_t _330;    /* Address-exposed local */
  uint8_t* _331;
  uint8_t* _332;
  uint32_t _333;
  uint32_t _334;
  uint32_t _335;
  uint32_t _336;
  uint32_t _337;
  uint32_t _338;
  uint32_t _339;
  uint32_t _340;
  uint32_t _341;
  uint32_t _342;
  uint32_t _343;
  uint32_t _344;
  uint32_t _345;
  uint32_t _346;
  uint32_t _347;
  uint32_t _348;
  uint32_t _349;
  uint32_t _350;
  uint32_t _351;
  uint32_t _352;
  uint32_t _353;
  uint32_t _354;
  uint32_t _355;
  uint32_t _356;
  uint32_t _357;
  uint32_t _358;
  uint32_t _359;
  uint32_t _360;
  uint32_t _361;
  uint32_t _362;
  uint32_t _363;
  uint32_t _364;
  uint32_t _365;
  uint32_t _366;
  uint32_t _367;
  uint32_t _368;
  uint32_t _369;
  uint32_t _370;
  uint32_t _371;
  uint32_t _372;
  uint32_t _373;
  uint32_t _374;
  uint32_t _375;
  uint32_t _376;
  uint32_t _377;
  uint32_t _378;
  uint32_t _379;
  uint32_t _380;
  uint32_t _381;
  uint32_t _382;
  uint32_t _383;
  uint32_t _384;
  uint32_t _385;
  uint32_t _386;
  uint32_t _387;
  uint32_t _388;
  uint32_t _389;
  uint32_t _390;
  uint32_t _391;
  uint32_t _392;
  uint32_t _393;
  uint32_t _394;

  _291 = _290;
#line 78 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  ;
#line 79 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  ;
  _292 = 0;
#line 80 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  ;
  _331 = memset((((uint8_t*)(&_293))), 0, 20);
#line 82 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  ;
  _332 = memset((((uint8_t*)(&_294))), 0, 20);
#line 84 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  ;
#line 85 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  ;
  _296 = (&_295);
#line 86 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  ;
  _297 = (&k);
#line 87 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  ;
  _333 = _295;
  _298 = ((&_293.array[((int64_t)(((int64_t)(int32_t)_333)))]));
#line 88 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  ;
  _334 = _295;
  _299 = ((&_294.array[((int64_t)(((int64_t)(int32_t)_334)))]));
#line 89 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  ;
#line 90 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z25omp_for_pragma_talkad7420Pc(((&_OC_str.array[((int64_t)0)])));
#line 91 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z19for_loop_talkad7420v();
#line 92 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  ;
  _301 = 0;
  goto _395;

  do {     /* Syntactic loop '' to make GCC happy */
_395:
  _335 = _301;
  if ((((((int32_t)_335) < ((int32_t)100u))&1))) {
    goto _396;
  } else {
    goto _397;
  }

_396:
  _Z3foov();
  goto _398;

_398:
  _336 = _301;
  _301 = (llvm_add_u32(_336, 1));
  goto _395;

  } while (1); /* end of syntactic loop '' */
_397:
#line 95 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z25omp_for_pragma_talkad7420Pc(((&_OC_str_OC_7.array[((int64_t)0)])));
#line 96 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z19for_loop_talkad7420v();
#line 97 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  ;
  _302 = 0;
  goto _399;

  do {     /* Syntactic loop '' to make GCC happy */
_399:
  _337 = _302;
  if ((((((int32_t)_337) < ((int32_t)100u))&1))) {
    goto _400;
  } else {
    goto _401;
  }

_400:
  _Z3foov();
  goto _402;

_402:
  _338 = _302;
  _302 = (llvm_add_u32(_338, 1));
  goto _399;

  } while (1); /* end of syntactic loop '' */
_401:
#line 100 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z25omp_for_pragma_talkad7420Pc(((&_OC_str_OC_8.array[((int64_t)0)])));
#line 101 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z19for_loop_talkad7420v();
#line 102 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  ;
  _303 = 0;
  goto _403;

  do {     /* Syntactic loop '' to make GCC happy */
_403:
  _339 = _303;
  if ((((((int32_t)_339) < ((int32_t)100u))&1))) {
    goto _404;
  } else {
    goto _405;
  }

_404:
  _Z3foov();
  goto _406;

_406:
  _340 = _303;
  _303 = (llvm_add_u32(_340, 1));
  goto _403;

  } while (1); /* end of syntactic loop '' */
_405:
#line 105 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z25omp_for_pragma_talkad7420Pc(((&_OC_str_OC_9.array[((int64_t)0)])));
#line 106 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z19for_loop_talkad7420v();
#line 107 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  ;
  _304 = 0;
  goto _407;

  do {     /* Syntactic loop '' to make GCC happy */
_407:
  _341 = _304;
  if ((((((int32_t)_341) < ((int32_t)100u))&1))) {
    goto _408;
  } else {
    goto _409;
  }

_408:
  _Z3foov();
  goto _410;

_410:
  _342 = _304;
  _304 = (llvm_add_u32(_342, 1));
  goto _407;

  } while (1); /* end of syntactic loop '' */
_409:
#line 110 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z25omp_for_pragma_talkad7420Pc(((&_OC_str_OC_10.array[((int64_t)0)])));
#line 111 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z19for_loop_talkad7420v();
#line 112 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  ;
  _305 = 0;
  goto _411;

  do {     /* Syntactic loop '' to make GCC happy */
_411:
  _343 = _305;
  if ((((((int32_t)_343) < ((int32_t)100u))&1))) {
    goto _412;
  } else {
    goto _413;
  }

_412:
  _Z3foov();
  goto _414;

_414:
  _344 = _305;
  _305 = (llvm_add_u32(_344, 1));
  goto _411;

  } while (1); /* end of syntactic loop '' */
_413:
#line 115 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z25omp_for_pragma_talkad7420Pc(((&_OC_str_OC_11.array[((int64_t)0)])));
#line 116 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z19for_loop_talkad7420v();
#line 117 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  ;
  _306 = 0;
  goto _415;

  do {     /* Syntactic loop '' to make GCC happy */
_415:
  _345 = _306;
  if ((((((int32_t)_345) < ((int32_t)100u))&1))) {
    goto _416;
  } else {
    goto _417;
  }

_416:
  _Z3foov();
  goto _418;

_418:
  _346 = _306;
  _306 = (llvm_add_u32(_346, 1));
  goto _415;

  } while (1); /* end of syntactic loop '' */
_417:
#line 120 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z25omp_for_pragma_talkad7420Pc(((&_OC_str_OC_10.array[((int64_t)0)])));
#line 121 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z19for_loop_talkad7420v();
#line 122 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  ;
  _307 = 0;
  goto _419;

  do {     /* Syntactic loop '' to make GCC happy */
_419:
  _347 = _307;
  if ((((((int32_t)_347) < ((int32_t)100u))&1))) {
    goto _420;
  } else {
    goto _421;
  }

_420:
  _Z3foov();
  goto _422;

_422:
  _348 = _307;
  _307 = (llvm_add_u32(_348, 1));
  goto _419;

  } while (1); /* end of syntactic loop '' */
_421:
#line 125 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z25omp_for_pragma_talkad7420Pc(((&_OC_str_OC_36.array[((int64_t)0)])));
#line 126 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z19for_loop_talkad7420v();
#line 127 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  ;
  _308 = 0;
  goto _423;

  do {     /* Syntactic loop '' to make GCC happy */
_423:
  _349 = _308;
  if ((((((int32_t)_349) < ((int32_t)100u))&1))) {
    goto _424;
  } else {
    goto _425;
  }

_424:
  _Z3foov();
  goto _426;

_426:
  _350 = _308;
  _308 = (llvm_add_u32(_350, 1));
  goto _423;

  } while (1); /* end of syntactic loop '' */
_425:
#line 130 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z25omp_for_pragma_talkad7420Pc(((&_OC_str_OC_13.array[((int64_t)0)])));
#line 131 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z19for_loop_talkad7420v();
#line 132 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  ;
  _309 = 0;
  goto _427;

  do {     /* Syntactic loop '' to make GCC happy */
_427:
  _351 = _309;
  if ((((((int32_t)_351) < ((int32_t)100u))&1))) {
    goto _428;
  } else {
    goto _429;
  }

_428:
  _Z3foov();
  goto _430;

_430:
  _352 = _309;
  _309 = (llvm_add_u32(_352, 1));
  goto _427;

  } while (1); /* end of syntactic loop '' */
_429:
#line 135 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z25omp_for_pragma_talkad7420Pc(((&_OC_str_OC_37.array[((int64_t)0)])));
#line 136 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z19for_loop_talkad7420v();
#line 137 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  ;
  _310 = 0;
  goto _431;

  do {     /* Syntactic loop '' to make GCC happy */
_431:
  _353 = _310;
  if ((((((int32_t)_353) < ((int32_t)100u))&1))) {
    goto _432;
  } else {
    goto _433;
  }

_432:
  _Z3foov();
  goto _434;

_434:
  _354 = _310;
  _310 = (llvm_add_u32(_354, 1));
  goto _431;

  } while (1); /* end of syntactic loop '' */
_433:
#line 140 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z25omp_for_pragma_talkad7420Pc(((&_OC_str_OC_38.array[((int64_t)0)])));
#line 141 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z19for_loop_talkad7420v();
#line 142 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  ;
  _311 = 0;
  goto _435;

  do {     /* Syntactic loop '' to make GCC happy */
_435:
  _355 = _311;
  if ((((((int32_t)_355) < ((int32_t)100u))&1))) {
    goto _436;
  } else {
    goto _437;
  }

_436:
  _Z3foov();
  goto _438;

_438:
  _356 = _311;
  _311 = (llvm_add_u32(_356, 1));
  goto _435;

  } while (1); /* end of syntactic loop '' */
_437:
#line 145 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z25omp_for_pragma_talkad7420Pc(((&_OC_str_OC_16.array[((int64_t)0)])));
#line 146 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z19for_loop_talkad7420v();
#line 147 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  ;
  _312 = 0;
  goto _439;

  do {     /* Syntactic loop '' to make GCC happy */
_439:
  _357 = _312;
  if ((((((int32_t)_357) < ((int32_t)100u))&1))) {
    goto _440;
  } else {
    goto _441;
  }

_440:
  _Z3foov();
  goto _442;

_442:
  _358 = _312;
  _312 = (llvm_add_u32(_358, 1));
  goto _439;

  } while (1); /* end of syntactic loop '' */
_441:
#line 150 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z25omp_for_pragma_talkad7420Pc(((&_OC_str_OC_39.array[((int64_t)0)])));
#line 151 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z19for_loop_talkad7420v();
#line 152 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  ;
  _313 = 0;
  goto _443;

  do {     /* Syntactic loop '' to make GCC happy */
_443:
  _359 = _313;
  if ((((((int32_t)_359) < ((int32_t)100u))&1))) {
    goto _444;
  } else {
    goto _445;
  }

_444:
  _Z3foov();
  goto _446;

_446:
  _360 = _313;
  _313 = (llvm_add_u32(_360, 1));
  goto _443;

  } while (1); /* end of syntactic loop '' */
_445:
#line 155 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z25omp_for_pragma_talkad7420Pc(((&_OC_str_OC_18.array[((int64_t)0)])));
#line 156 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z19for_loop_talkad7420v();
#line 157 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  ;
  _314 = 0;
  goto _447;

  do {     /* Syntactic loop '' to make GCC happy */
_447:
  _361 = _314;
  if ((((((int32_t)_361) < ((int32_t)100u))&1))) {
    goto _448;
  } else {
    goto _449;
  }

_448:
  _Z3foov();
  goto _450;

_450:
  _362 = _314;
  _314 = (llvm_add_u32(_362, 1));
  goto _447;

  } while (1); /* end of syntactic loop '' */
_449:
#line 160 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z25omp_for_pragma_talkad7420Pc(((&_OC_str_OC_19.array[((int64_t)0)])));
#line 161 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z19for_loop_talkad7420v();
#line 162 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  ;
  _315 = 0;
  goto _451;

  do {     /* Syntactic loop '' to make GCC happy */
_451:
  _363 = _315;
  if ((((((int32_t)_363) < ((int32_t)100u))&1))) {
    goto _452;
  } else {
    goto _453;
  }

_452:
  _Z3foov();
  goto _454;

_454:
  _364 = _315;
  _315 = (llvm_add_u32(_364, 1));
  goto _451;

  } while (1); /* end of syntactic loop '' */
_453:
#line 165 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z25omp_for_pragma_talkad7420Pc(((&_OC_str_OC_20.array[((int64_t)0)])));
#line 166 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z19for_loop_talkad7420v();
#line 167 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  ;
  _316 = 0;
  goto _455;

  do {     /* Syntactic loop '' to make GCC happy */
_455:
  _365 = _316;
  if ((((((int32_t)_365) < ((int32_t)100u))&1))) {
    goto _456;
  } else {
    goto _457;
  }

_456:
  _Z3foov();
  goto _458;

_458:
  _366 = _316;
  _316 = (llvm_add_u32(_366, 1));
  goto _455;

  } while (1); /* end of syntactic loop '' */
_457:
#line 170 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z25omp_for_pragma_talkad7420Pc(((&_OC_str_OC_21.array[((int64_t)0)])));
#line 171 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z19for_loop_talkad7420v();
#line 172 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  ;
  _317 = 0;
  goto _459;

  do {     /* Syntactic loop '' to make GCC happy */
_459:
  _367 = _317;
  if ((((((int32_t)_367) < ((int32_t)100u))&1))) {
    goto _460;
  } else {
    goto _461;
  }

_460:
  _Z3foov();
  goto _462;

_462:
  _368 = _317;
  _317 = (llvm_add_u32(_368, 1));
  goto _459;

  } while (1); /* end of syntactic loop '' */
_461:
#line 175 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z25omp_for_pragma_talkad7420Pc(((&_OC_str_OC_22.array[((int64_t)0)])));
#line 176 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z19for_loop_talkad7420v();
#line 177 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  ;
  _318 = 0;
  goto _463;

  do {     /* Syntactic loop '' to make GCC happy */
_463:
  _369 = _318;
  if ((((((int32_t)_369) < ((int32_t)100u))&1))) {
    goto _464;
  } else {
    goto _465;
  }

_464:
  _Z3foov();
  goto _466;

_466:
  _370 = _318;
  _318 = (llvm_add_u32(_370, 1));
  goto _463;

  } while (1); /* end of syntactic loop '' */
_465:
#line 180 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z25omp_for_pragma_talkad7420Pc(((&_OC_str_OC_23.array[((int64_t)0)])));
#line 181 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z19for_loop_talkad7420v();
#line 182 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  ;
  _319 = 0;
  goto _467;

  do {     /* Syntactic loop '' to make GCC happy */
_467:
  _371 = _319;
  if ((((((int32_t)_371) < ((int32_t)100u))&1))) {
    goto _468;
  } else {
    goto _469;
  }

_468:
  _Z3foov();
  goto _470;

_470:
  _372 = _319;
  _319 = (llvm_add_u32(_372, 1));
  goto _467;

  } while (1); /* end of syntactic loop '' */
_469:
#line 185 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z25omp_for_pragma_talkad7420Pc(((&_OC_str_OC_24.array[((int64_t)0)])));
#line 186 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z19for_loop_talkad7420v();
#line 187 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  ;
  _320 = 0;
  goto _471;

  do {     /* Syntactic loop '' to make GCC happy */
_471:
  _373 = _320;
  if ((((((int32_t)_373) < ((int32_t)100u))&1))) {
    goto _472;
  } else {
    goto _473;
  }

_472:
  _Z3foov();
  goto _474;

_474:
  _374 = _320;
  _320 = (llvm_add_u32(_374, 1));
  goto _471;

  } while (1); /* end of syntactic loop '' */
_473:
#line 190 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z25omp_for_pragma_talkad7420Pc(((&_OC_str_OC_25.array[((int64_t)0)])));
#line 191 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z19for_loop_talkad7420v();
#line 192 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  ;
  _321 = 0;
  goto _475;

  do {     /* Syntactic loop '' to make GCC happy */
_475:
  _375 = _321;
  if ((((((int32_t)_375) < ((int32_t)100u))&1))) {
    goto _476;
  } else {
    goto _477;
  }

_476:
  _Z3foov();
  goto _478;

_478:
  _376 = _321;
  _321 = (llvm_add_u32(_376, 1));
  goto _475;

  } while (1); /* end of syntactic loop '' */
_477:
#line 195 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z25omp_for_pragma_talkad7420Pc(((&_OC_str_OC_26.array[((int64_t)0)])));
#line 196 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z19for_loop_talkad7420v();
#line 197 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  ;
  _322 = 0;
  goto _479;

  do {     /* Syntactic loop '' to make GCC happy */
_479:
  _377 = _322;
  if ((((((int32_t)_377) < ((int32_t)100u))&1))) {
    goto _480;
  } else {
    goto _481;
  }

_480:
  _Z3foov();
  goto _482;

_482:
  _378 = _322;
  _322 = (llvm_add_u32(_378, 1));
  goto _479;

  } while (1); /* end of syntactic loop '' */
_481:
#line 200 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z25omp_for_pragma_talkad7420Pc(((&_OC_str_OC_40.array[((int64_t)0)])));
#line 201 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z19for_loop_talkad7420v();
#line 202 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  ;
  _323 = 0;
  goto _483;

  do {     /* Syntactic loop '' to make GCC happy */
_483:
  _379 = _323;
  if ((((((int32_t)_379) < ((int32_t)100u))&1))) {
    goto _484;
  } else {
    goto _485;
  }

_484:
  _Z3foov();
  goto _486;

_486:
  _380 = _323;
  _323 = (llvm_add_u32(_380, 1));
  goto _483;

  } while (1); /* end of syntactic loop '' */
_485:
#line 205 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z25omp_for_pragma_talkad7420Pc(((&_OC_str_OC_29.array[((int64_t)0)])));
#line 206 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z19for_loop_talkad7420v();
#line 207 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  ;
  _324 = 0;
  goto _487;

  do {     /* Syntactic loop '' to make GCC happy */
_487:
  _381 = _324;
  if ((((((int32_t)_381) < ((int32_t)100u))&1))) {
    goto _488;
  } else {
    goto _489;
  }

_488:
  _Z3foov();
  goto _490;

_490:
  _382 = _324;
  _324 = (llvm_add_u32(_382, 1));
  goto _487;

  } while (1); /* end of syntactic loop '' */
_489:
#line 210 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z25omp_for_pragma_talkad7420Pc(((&_OC_str_OC_30.array[((int64_t)0)])));
#line 211 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z19for_loop_talkad7420v();
#line 212 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  ;
  _325 = 0;
  goto _491;

  do {     /* Syntactic loop '' to make GCC happy */
_491:
  _383 = _325;
  if ((((((int32_t)_383) < ((int32_t)100u))&1))) {
    goto _492;
  } else {
    goto _493;
  }

_492:
  _Z3foov();
  goto _494;

_494:
  _384 = _325;
  _325 = (llvm_add_u32(_384, 1));
  goto _491;

  } while (1); /* end of syntactic loop '' */
_493:
#line 216 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z25omp_for_pragma_talkad7420Pc(((&_OC_str_OC_31.array[((int64_t)0)])));
#line 217 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z19for_loop_talkad7420v();
#line 218 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  ;
  _326 = 0;
  goto _495;

  do {     /* Syntactic loop '' to make GCC happy */
_495:
  _385 = _326;
  if ((((((int32_t)_385) < ((int32_t)100u))&1))) {
    goto _496;
  } else {
    goto _497;
  }

_496:
  _Z3foov();
  goto _498;

_498:
  _386 = _326;
  _326 = (llvm_add_u32(_386, 1));
  goto _495;

  } while (1); /* end of syntactic loop '' */
_497:
#line 221 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z25omp_for_pragma_talkad7420Pc(((&_OC_str_OC_31.array[((int64_t)0)])));
#line 222 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z19for_loop_talkad7420v();
#line 223 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  ;
  _327 = 0;
  goto _499;

  do {     /* Syntactic loop '' to make GCC happy */
_499:
  _387 = _327;
  if ((((((int32_t)_387) < ((int32_t)100u))&1))) {
    goto _500;
  } else {
    goto _501;
  }

_500:
  _Z3foov();
  goto _502;

_502:
  _388 = _327;
  _327 = (llvm_add_u32(_388, 1));
  goto _499;

  } while (1); /* end of syntactic loop '' */
_501:
#line 226 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z25omp_for_pragma_talkad7420Pc(((&_OC_str_OC_32.array[((int64_t)0)])));
#line 227 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z19for_loop_talkad7420v();
#line 228 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  ;
  _328 = 0;
  goto _503;

  do {     /* Syntactic loop '' to make GCC happy */
_503:
  _389 = _328;
  if ((((((int32_t)_389) < ((int32_t)100u))&1))) {
    goto _504;
  } else {
    goto _505;
  }

_504:
  _Z3foov();
  goto _506;

_506:
  _390 = _328;
  _328 = (llvm_add_u32(_390, 1));
  goto _503;

  } while (1); /* end of syntactic loop '' */
_505:
#line 233 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z25omp_for_pragma_talkad7420Pc(((&_OC_str_OC_33.array[((int64_t)0)])));
#line 234 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z19for_loop_talkad7420v();
#line 235 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  ;
  _329 = 0;
  goto _507;

  do {     /* Syntactic loop '' to make GCC happy */
_507:
  _391 = _329;
  if ((((((int32_t)_391) < ((int32_t)100u))&1))) {
    goto _508;
  } else {
    goto _509;
  }

_508:
  _Z3foov();
  goto _510;

_510:
  _392 = _329;
  _329 = (llvm_add_u32(_392, 1));
  goto _507;

  } while (1); /* end of syntactic loop '' */
_509:
#line 238 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z25omp_for_pragma_talkad7420Pc(((&_OC_str_OC_34.array[((int64_t)0)])));
#line 239 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z19for_loop_talkad7420v();
#line 240 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  ;
  _330 = 0;
  goto _511;

  do {     /* Syntactic loop '' to make GCC happy */
_511:
  _393 = _330;
  if ((((((int32_t)_393) < ((int32_t)100u))&1))) {
    goto _512;
  } else {
    goto _513;
  }

_512:
  _Z3foov();
  goto _514;

_514:
  _394 = _330;
  _330 = (llvm_add_u32(_394, 1));
  goto _511;

  } while (1); /* end of syntactic loop '' */
_513:
  return 0;
}


float _Z5tmainIfET_S0_(float _515) {
  float _516;    /* Address-exposed local */
  float _517;    /* Address-exposed local */
  __MSALIGN__(16) struct l_array_5_float _518 __attribute__((aligned(16)));    /* Address-exposed local */
  __MSALIGN__(16) struct l_array_5_float _519 __attribute__((aligned(16)));    /* Address-exposed local */
  float _520;    /* Address-exposed local */
  float* _521;    /* Address-exposed local */
  struct l_struct_class_OC_S3* _522;    /* Address-exposed local */
  float* _523;    /* Address-exposed local */
  float* _524;    /* Address-exposed local */
  float _525;    /* Address-exposed local */
  uint32_t _526;    /* Address-exposed local */
  uint32_t _527;    /* Address-exposed local */
  uint32_t _528;    /* Address-exposed local */
  uint32_t _529;    /* Address-exposed local */
  uint32_t _530;    /* Address-exposed local */
  uint32_t _531;    /* Address-exposed local */
  uint32_t _532;    /* Address-exposed local */
  uint32_t _533;    /* Address-exposed local */
  uint32_t _534;    /* Address-exposed local */
  uint32_t _535;    /* Address-exposed local */
  uint32_t _536;    /* Address-exposed local */
  uint32_t _537;    /* Address-exposed local */
  uint32_t _538;    /* Address-exposed local */
  uint32_t _539;    /* Address-exposed local */
  uint32_t _540;    /* Address-exposed local */
  uint32_t _541;    /* Address-exposed local */
  uint32_t _542;    /* Address-exposed local */
  uint32_t _543;    /* Address-exposed local */
  uint32_t _544;    /* Address-exposed local */
  uint32_t _545;    /* Address-exposed local */
  uint32_t _546;    /* Address-exposed local */
  uint32_t _547;    /* Address-exposed local */
  uint32_t _548;    /* Address-exposed local */
  uint32_t _549;    /* Address-exposed local */
  uint32_t _550;    /* Address-exposed local */
  uint32_t _551;    /* Address-exposed local */
  uint32_t _552;    /* Address-exposed local */
  uint32_t _553;    /* Address-exposed local */
  uint32_t _554;    /* Address-exposed local */
  uint32_t _555;    /* Address-exposed local */
  uint8_t* _556;
  uint8_t* _557;
  float _558;
  float _559;
  uint32_t _560;
  uint32_t _561;
  uint32_t _562;
  uint32_t _563;
  uint32_t _564;
  uint32_t _565;
  uint32_t _566;
  uint32_t _567;
  uint32_t _568;
  uint32_t _569;
  uint32_t _570;
  uint32_t _571;
  uint32_t _572;
  uint32_t _573;
  uint32_t _574;
  uint32_t _575;
  uint32_t _576;
  uint32_t _577;
  uint32_t _578;
  uint32_t _579;
  uint32_t _580;
  uint32_t _581;
  uint32_t _582;
  uint32_t _583;
  uint32_t _584;
  uint32_t _585;
  uint32_t _586;
  uint32_t _587;
  uint32_t _588;
  uint32_t _589;
  uint32_t _590;
  uint32_t _591;
  uint32_t _592;
  uint32_t _593;
  uint32_t _594;
  uint32_t _595;
  uint32_t _596;
  uint32_t _597;
  uint32_t _598;
  uint32_t _599;
  uint32_t _600;
  uint32_t _601;
  uint32_t _602;
  uint32_t _603;
  uint32_t _604;
  uint32_t _605;
  uint32_t _606;
  uint32_t _607;
  uint32_t _608;
  uint32_t _609;
  uint32_t _610;
  uint32_t _611;
  uint32_t _612;
  uint32_t _613;
  uint32_t _614;
  uint32_t _615;
  uint32_t _616;
  uint32_t _617;
  uint32_t _618;
  uint32_t _619;

  _516 = _515;
#line 78 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  ;
#line 79 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  ;
  _517 = 0;
#line 80 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  ;
  _556 = memset((((uint8_t*)(&_518))), 0, 20);
#line 82 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  ;
  _557 = memset((((uint8_t*)(&_519))), 0, 20);
#line 84 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  ;
#line 85 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  ;
  _521 = (&_520);
#line 86 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  ;
  _522 = (&k);
#line 87 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  ;
  _558 = _520;
  _523 = ((&_518.array[((int64_t)(((int64_t)(int32_t)(((int32_t)_558)))))]));
#line 88 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  ;
  _559 = _520;
  _524 = ((&_519.array[((int64_t)(((int64_t)(int32_t)(((int32_t)_559)))))]));
#line 89 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  ;
#line 90 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z25omp_for_pragma_talkad7420Pc(((&_OC_str.array[((int64_t)0)])));
#line 91 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z19for_loop_talkad7420v();
#line 92 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  ;
  _526 = 0;
  goto _620;

  do {     /* Syntactic loop '' to make GCC happy */
_620:
  _560 = _526;
  if ((((((int32_t)_560) < ((int32_t)100u))&1))) {
    goto _621;
  } else {
    goto _622;
  }

_621:
  _Z3foov();
  goto _623;

_623:
  _561 = _526;
  _526 = (llvm_add_u32(_561, 1));
  goto _620;

  } while (1); /* end of syntactic loop '' */
_622:
#line 95 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z25omp_for_pragma_talkad7420Pc(((&_OC_str_OC_7.array[((int64_t)0)])));
#line 96 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z19for_loop_talkad7420v();
#line 97 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  ;
  _527 = 0;
  goto _624;

  do {     /* Syntactic loop '' to make GCC happy */
_624:
  _562 = _527;
  if ((((((int32_t)_562) < ((int32_t)100u))&1))) {
    goto _625;
  } else {
    goto _626;
  }

_625:
  _Z3foov();
  goto _627;

_627:
  _563 = _527;
  _527 = (llvm_add_u32(_563, 1));
  goto _624;

  } while (1); /* end of syntactic loop '' */
_626:
#line 100 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z25omp_for_pragma_talkad7420Pc(((&_OC_str_OC_8.array[((int64_t)0)])));
#line 101 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z19for_loop_talkad7420v();
#line 102 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  ;
  _528 = 0;
  goto _628;

  do {     /* Syntactic loop '' to make GCC happy */
_628:
  _564 = _528;
  if ((((((int32_t)_564) < ((int32_t)100u))&1))) {
    goto _629;
  } else {
    goto _630;
  }

_629:
  _Z3foov();
  goto _631;

_631:
  _565 = _528;
  _528 = (llvm_add_u32(_565, 1));
  goto _628;

  } while (1); /* end of syntactic loop '' */
_630:
#line 105 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z25omp_for_pragma_talkad7420Pc(((&_OC_str_OC_9.array[((int64_t)0)])));
#line 106 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z19for_loop_talkad7420v();
#line 107 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  ;
  _529 = 0;
  goto _632;

  do {     /* Syntactic loop '' to make GCC happy */
_632:
  _566 = _529;
  if ((((((int32_t)_566) < ((int32_t)100u))&1))) {
    goto _633;
  } else {
    goto _634;
  }

_633:
  _Z3foov();
  goto _635;

_635:
  _567 = _529;
  _529 = (llvm_add_u32(_567, 1));
  goto _632;

  } while (1); /* end of syntactic loop '' */
_634:
#line 110 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z25omp_for_pragma_talkad7420Pc(((&_OC_str_OC_10.array[((int64_t)0)])));
#line 111 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z19for_loop_talkad7420v();
#line 112 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  ;
  _530 = 0;
  goto _636;

  do {     /* Syntactic loop '' to make GCC happy */
_636:
  _568 = _530;
  if ((((((int32_t)_568) < ((int32_t)100u))&1))) {
    goto _637;
  } else {
    goto _638;
  }

_637:
  _Z3foov();
  goto _639;

_639:
  _569 = _530;
  _530 = (llvm_add_u32(_569, 1));
  goto _636;

  } while (1); /* end of syntactic loop '' */
_638:
#line 115 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z25omp_for_pragma_talkad7420Pc(((&_OC_str_OC_11.array[((int64_t)0)])));
#line 116 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z19for_loop_talkad7420v();
#line 117 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  ;
  _531 = 0;
  goto _640;

  do {     /* Syntactic loop '' to make GCC happy */
_640:
  _570 = _531;
  if ((((((int32_t)_570) < ((int32_t)100u))&1))) {
    goto _641;
  } else {
    goto _642;
  }

_641:
  _Z3foov();
  goto _643;

_643:
  _571 = _531;
  _531 = (llvm_add_u32(_571, 1));
  goto _640;

  } while (1); /* end of syntactic loop '' */
_642:
#line 120 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z25omp_for_pragma_talkad7420Pc(((&_OC_str_OC_10.array[((int64_t)0)])));
#line 121 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z19for_loop_talkad7420v();
#line 122 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  ;
  _532 = 0;
  goto _644;

  do {     /* Syntactic loop '' to make GCC happy */
_644:
  _572 = _532;
  if ((((((int32_t)_572) < ((int32_t)100u))&1))) {
    goto _645;
  } else {
    goto _646;
  }

_645:
  _Z3foov();
  goto _647;

_647:
  _573 = _532;
  _532 = (llvm_add_u32(_573, 1));
  goto _644;

  } while (1); /* end of syntactic loop '' */
_646:
#line 125 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z25omp_for_pragma_talkad7420Pc(((&_OC_str_OC_36.array[((int64_t)0)])));
#line 126 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z19for_loop_talkad7420v();
#line 127 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  ;
  _533 = 0;
  goto _648;

  do {     /* Syntactic loop '' to make GCC happy */
_648:
  _574 = _533;
  if ((((((int32_t)_574) < ((int32_t)100u))&1))) {
    goto _649;
  } else {
    goto _650;
  }

_649:
  _Z3foov();
  goto _651;

_651:
  _575 = _533;
  _533 = (llvm_add_u32(_575, 1));
  goto _648;

  } while (1); /* end of syntactic loop '' */
_650:
#line 130 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z25omp_for_pragma_talkad7420Pc(((&_OC_str_OC_13.array[((int64_t)0)])));
#line 131 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z19for_loop_talkad7420v();
#line 132 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  ;
  _534 = 0;
  goto _652;

  do {     /* Syntactic loop '' to make GCC happy */
_652:
  _576 = _534;
  if ((((((int32_t)_576) < ((int32_t)100u))&1))) {
    goto _653;
  } else {
    goto _654;
  }

_653:
  _Z3foov();
  goto _655;

_655:
  _577 = _534;
  _534 = (llvm_add_u32(_577, 1));
  goto _652;

  } while (1); /* end of syntactic loop '' */
_654:
#line 135 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z25omp_for_pragma_talkad7420Pc(((&_OC_str_OC_37.array[((int64_t)0)])));
#line 136 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z19for_loop_talkad7420v();
#line 137 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  ;
  _535 = 0;
  goto _656;

  do {     /* Syntactic loop '' to make GCC happy */
_656:
  _578 = _535;
  if ((((((int32_t)_578) < ((int32_t)100u))&1))) {
    goto _657;
  } else {
    goto _658;
  }

_657:
  _Z3foov();
  goto _659;

_659:
  _579 = _535;
  _535 = (llvm_add_u32(_579, 1));
  goto _656;

  } while (1); /* end of syntactic loop '' */
_658:
#line 140 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z25omp_for_pragma_talkad7420Pc(((&_OC_str_OC_38.array[((int64_t)0)])));
#line 141 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z19for_loop_talkad7420v();
#line 142 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  ;
  _536 = 0;
  goto _660;

  do {     /* Syntactic loop '' to make GCC happy */
_660:
  _580 = _536;
  if ((((((int32_t)_580) < ((int32_t)100u))&1))) {
    goto _661;
  } else {
    goto _662;
  }

_661:
  _Z3foov();
  goto _663;

_663:
  _581 = _536;
  _536 = (llvm_add_u32(_581, 1));
  goto _660;

  } while (1); /* end of syntactic loop '' */
_662:
#line 145 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z25omp_for_pragma_talkad7420Pc(((&_OC_str_OC_16.array[((int64_t)0)])));
#line 146 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z19for_loop_talkad7420v();
#line 147 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  ;
  _537 = 0;
  goto _664;

  do {     /* Syntactic loop '' to make GCC happy */
_664:
  _582 = _537;
  if ((((((int32_t)_582) < ((int32_t)100u))&1))) {
    goto _665;
  } else {
    goto _666;
  }

_665:
  _Z3foov();
  goto _667;

_667:
  _583 = _537;
  _537 = (llvm_add_u32(_583, 1));
  goto _664;

  } while (1); /* end of syntactic loop '' */
_666:
#line 150 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z25omp_for_pragma_talkad7420Pc(((&_OC_str_OC_39.array[((int64_t)0)])));
#line 151 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z19for_loop_talkad7420v();
#line 152 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  ;
  _538 = 0;
  goto _668;

  do {     /* Syntactic loop '' to make GCC happy */
_668:
  _584 = _538;
  if ((((((int32_t)_584) < ((int32_t)100u))&1))) {
    goto _669;
  } else {
    goto _670;
  }

_669:
  _Z3foov();
  goto _671;

_671:
  _585 = _538;
  _538 = (llvm_add_u32(_585, 1));
  goto _668;

  } while (1); /* end of syntactic loop '' */
_670:
#line 155 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z25omp_for_pragma_talkad7420Pc(((&_OC_str_OC_18.array[((int64_t)0)])));
#line 156 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z19for_loop_talkad7420v();
#line 157 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  ;
  _539 = 0;
  goto _672;

  do {     /* Syntactic loop '' to make GCC happy */
_672:
  _586 = _539;
  if ((((((int32_t)_586) < ((int32_t)100u))&1))) {
    goto _673;
  } else {
    goto _674;
  }

_673:
  _Z3foov();
  goto _675;

_675:
  _587 = _539;
  _539 = (llvm_add_u32(_587, 1));
  goto _672;

  } while (1); /* end of syntactic loop '' */
_674:
#line 160 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z25omp_for_pragma_talkad7420Pc(((&_OC_str_OC_19.array[((int64_t)0)])));
#line 161 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z19for_loop_talkad7420v();
#line 162 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  ;
  _540 = 0;
  goto _676;

  do {     /* Syntactic loop '' to make GCC happy */
_676:
  _588 = _540;
  if ((((((int32_t)_588) < ((int32_t)100u))&1))) {
    goto _677;
  } else {
    goto _678;
  }

_677:
  _Z3foov();
  goto _679;

_679:
  _589 = _540;
  _540 = (llvm_add_u32(_589, 1));
  goto _676;

  } while (1); /* end of syntactic loop '' */
_678:
#line 165 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z25omp_for_pragma_talkad7420Pc(((&_OC_str_OC_20.array[((int64_t)0)])));
#line 166 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z19for_loop_talkad7420v();
#line 167 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  ;
  _541 = 0;
  goto _680;

  do {     /* Syntactic loop '' to make GCC happy */
_680:
  _590 = _541;
  if ((((((int32_t)_590) < ((int32_t)100u))&1))) {
    goto _681;
  } else {
    goto _682;
  }

_681:
  _Z3foov();
  goto _683;

_683:
  _591 = _541;
  _541 = (llvm_add_u32(_591, 1));
  goto _680;

  } while (1); /* end of syntactic loop '' */
_682:
#line 170 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z25omp_for_pragma_talkad7420Pc(((&_OC_str_OC_21.array[((int64_t)0)])));
#line 171 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z19for_loop_talkad7420v();
#line 172 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  ;
  _542 = 0;
  goto _684;

  do {     /* Syntactic loop '' to make GCC happy */
_684:
  _592 = _542;
  if ((((((int32_t)_592) < ((int32_t)100u))&1))) {
    goto _685;
  } else {
    goto _686;
  }

_685:
  _Z3foov();
  goto _687;

_687:
  _593 = _542;
  _542 = (llvm_add_u32(_593, 1));
  goto _684;

  } while (1); /* end of syntactic loop '' */
_686:
#line 175 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z25omp_for_pragma_talkad7420Pc(((&_OC_str_OC_22.array[((int64_t)0)])));
#line 176 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z19for_loop_talkad7420v();
#line 177 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  ;
  _543 = 0;
  goto _688;

  do {     /* Syntactic loop '' to make GCC happy */
_688:
  _594 = _543;
  if ((((((int32_t)_594) < ((int32_t)100u))&1))) {
    goto _689;
  } else {
    goto _690;
  }

_689:
  _Z3foov();
  goto _691;

_691:
  _595 = _543;
  _543 = (llvm_add_u32(_595, 1));
  goto _688;

  } while (1); /* end of syntactic loop '' */
_690:
#line 180 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z25omp_for_pragma_talkad7420Pc(((&_OC_str_OC_23.array[((int64_t)0)])));
#line 181 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z19for_loop_talkad7420v();
#line 182 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  ;
  _544 = 0;
  goto _692;

  do {     /* Syntactic loop '' to make GCC happy */
_692:
  _596 = _544;
  if ((((((int32_t)_596) < ((int32_t)100u))&1))) {
    goto _693;
  } else {
    goto _694;
  }

_693:
  _Z3foov();
  goto _695;

_695:
  _597 = _544;
  _544 = (llvm_add_u32(_597, 1));
  goto _692;

  } while (1); /* end of syntactic loop '' */
_694:
#line 185 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z25omp_for_pragma_talkad7420Pc(((&_OC_str_OC_24.array[((int64_t)0)])));
#line 186 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z19for_loop_talkad7420v();
#line 187 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  ;
  _545 = 0;
  goto _696;

  do {     /* Syntactic loop '' to make GCC happy */
_696:
  _598 = _545;
  if ((((((int32_t)_598) < ((int32_t)100u))&1))) {
    goto _697;
  } else {
    goto _698;
  }

_697:
  _Z3foov();
  goto _699;

_699:
  _599 = _545;
  _545 = (llvm_add_u32(_599, 1));
  goto _696;

  } while (1); /* end of syntactic loop '' */
_698:
#line 190 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z25omp_for_pragma_talkad7420Pc(((&_OC_str_OC_25.array[((int64_t)0)])));
#line 191 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z19for_loop_talkad7420v();
#line 192 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  ;
  _546 = 0;
  goto _700;

  do {     /* Syntactic loop '' to make GCC happy */
_700:
  _600 = _546;
  if ((((((int32_t)_600) < ((int32_t)100u))&1))) {
    goto _701;
  } else {
    goto _702;
  }

_701:
  _Z3foov();
  goto _703;

_703:
  _601 = _546;
  _546 = (llvm_add_u32(_601, 1));
  goto _700;

  } while (1); /* end of syntactic loop '' */
_702:
#line 195 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z25omp_for_pragma_talkad7420Pc(((&_OC_str_OC_26.array[((int64_t)0)])));
#line 196 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z19for_loop_talkad7420v();
#line 197 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  ;
  _547 = 0;
  goto _704;

  do {     /* Syntactic loop '' to make GCC happy */
_704:
  _602 = _547;
  if ((((((int32_t)_602) < ((int32_t)100u))&1))) {
    goto _705;
  } else {
    goto _706;
  }

_705:
  _Z3foov();
  goto _707;

_707:
  _603 = _547;
  _547 = (llvm_add_u32(_603, 1));
  goto _704;

  } while (1); /* end of syntactic loop '' */
_706:
#line 200 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z25omp_for_pragma_talkad7420Pc(((&_OC_str_OC_40.array[((int64_t)0)])));
#line 201 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z19for_loop_talkad7420v();
#line 202 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  ;
  _548 = 0;
  goto _708;

  do {     /* Syntactic loop '' to make GCC happy */
_708:
  _604 = _548;
  if ((((((int32_t)_604) < ((int32_t)100u))&1))) {
    goto _709;
  } else {
    goto _710;
  }

_709:
  _Z3foov();
  goto _711;

_711:
  _605 = _548;
  _548 = (llvm_add_u32(_605, 1));
  goto _708;

  } while (1); /* end of syntactic loop '' */
_710:
#line 205 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z25omp_for_pragma_talkad7420Pc(((&_OC_str_OC_29.array[((int64_t)0)])));
#line 206 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z19for_loop_talkad7420v();
#line 207 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  ;
  _549 = 0;
  goto _712;

  do {     /* Syntactic loop '' to make GCC happy */
_712:
  _606 = _549;
  if ((((((int32_t)_606) < ((int32_t)100u))&1))) {
    goto _713;
  } else {
    goto _714;
  }

_713:
  _Z3foov();
  goto _715;

_715:
  _607 = _549;
  _549 = (llvm_add_u32(_607, 1));
  goto _712;

  } while (1); /* end of syntactic loop '' */
_714:
#line 210 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z25omp_for_pragma_talkad7420Pc(((&_OC_str_OC_30.array[((int64_t)0)])));
#line 211 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z19for_loop_talkad7420v();
#line 212 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  ;
  _550 = 0;
  goto _716;

  do {     /* Syntactic loop '' to make GCC happy */
_716:
  _608 = _550;
  if ((((((int32_t)_608) < ((int32_t)100u))&1))) {
    goto _717;
  } else {
    goto _718;
  }

_717:
  _Z3foov();
  goto _719;

_719:
  _609 = _550;
  _550 = (llvm_add_u32(_609, 1));
  goto _716;

  } while (1); /* end of syntactic loop '' */
_718:
#line 216 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z25omp_for_pragma_talkad7420Pc(((&_OC_str_OC_31.array[((int64_t)0)])));
#line 217 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z19for_loop_talkad7420v();
#line 218 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  ;
  _551 = 0;
  goto _720;

  do {     /* Syntactic loop '' to make GCC happy */
_720:
  _610 = _551;
  if ((((((int32_t)_610) < ((int32_t)100u))&1))) {
    goto _721;
  } else {
    goto _722;
  }

_721:
  _Z3foov();
  goto _723;

_723:
  _611 = _551;
  _551 = (llvm_add_u32(_611, 1));
  goto _720;

  } while (1); /* end of syntactic loop '' */
_722:
#line 221 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z25omp_for_pragma_talkad7420Pc(((&_OC_str_OC_31.array[((int64_t)0)])));
#line 222 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z19for_loop_talkad7420v();
#line 223 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  ;
  _552 = 0;
  goto _724;

  do {     /* Syntactic loop '' to make GCC happy */
_724:
  _612 = _552;
  if ((((((int32_t)_612) < ((int32_t)100u))&1))) {
    goto _725;
  } else {
    goto _726;
  }

_725:
  _Z3foov();
  goto _727;

_727:
  _613 = _552;
  _552 = (llvm_add_u32(_613, 1));
  goto _724;

  } while (1); /* end of syntactic loop '' */
_726:
#line 226 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z25omp_for_pragma_talkad7420Pc(((&_OC_str_OC_32.array[((int64_t)0)])));
#line 227 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z19for_loop_talkad7420v();
#line 228 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  ;
  _553 = 0;
  goto _728;

  do {     /* Syntactic loop '' to make GCC happy */
_728:
  _614 = _553;
  if ((((((int32_t)_614) < ((int32_t)100u))&1))) {
    goto _729;
  } else {
    goto _730;
  }

_729:
  _Z3foov();
  goto _731;

_731:
  _615 = _553;
  _553 = (llvm_add_u32(_615, 1));
  goto _728;

  } while (1); /* end of syntactic loop '' */
_730:
#line 233 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z25omp_for_pragma_talkad7420Pc(((&_OC_str_OC_33.array[((int64_t)0)])));
#line 234 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z19for_loop_talkad7420v();
#line 235 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  ;
  _554 = 0;
  goto _732;

  do {     /* Syntactic loop '' to make GCC happy */
_732:
  _616 = _554;
  if ((((((int32_t)_616) < ((int32_t)100u))&1))) {
    goto _733;
  } else {
    goto _734;
  }

_733:
  _Z3foov();
  goto _735;

_735:
  _617 = _554;
  _554 = (llvm_add_u32(_617, 1));
  goto _732;

  } while (1); /* end of syntactic loop '' */
_734:
#line 238 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z25omp_for_pragma_talkad7420Pc(((&_OC_str_OC_34.array[((int64_t)0)])));
#line 239 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  _Z19for_loop_talkad7420v();
#line 240 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  ;
  _555 = 0;
  goto _736;

  do {     /* Syntactic loop '' to make GCC happy */
_736:
  _618 = _555;
  if ((((((int32_t)_618) < ((int32_t)100u))&1))) {
    goto _737;
  } else {
    goto _738;
  }

_737:
  _Z3foov();
  goto _739;

_739:
  _619 = _555;
  _555 = (llvm_add_u32(_619, 1));
  goto _736;

  } while (1); /* end of syntactic loop '' */
_738:
  return 0;
}


static void _GLOBAL__sub_I_target_teams_distribute_parallel_for_simd_reduction_messages_OC_cpp(void) {
#line 0 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_parallel_for_simd_reduction_messages.cpp"
  __cxx_global_var_init();
  __cxx_global_var_init_OC_1();
  __cxx_global_var_init_OC_2();
  __cxx_global_var_init_OC_3();
  __cxx_global_var_init_OC_4();
  __cxx_global_var_init_OC_5();
  __cxx_global_var_init_OC_6();
}

