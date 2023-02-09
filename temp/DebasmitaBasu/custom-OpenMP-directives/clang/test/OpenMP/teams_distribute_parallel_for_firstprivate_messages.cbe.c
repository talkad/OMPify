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
struct l_struct_class_OC_S4;
struct l_struct_class_OC_S5;
struct l_struct_class_OC_S6;
struct l_array_5_struct_AC_l_struct_class_OC_S2;
struct l_array_5_struct_AC_l_struct_class_OC_S3;
struct l_array_56_uint8_t;
struct l_array_58_uint8_t;
struct l_array_59_uint8_t;
struct l_array_62_uint8_t;
struct l_array_63_uint8_t;
struct l_array_87_uint8_t;
struct l_array_61_uint8_t;
struct l_array_72_uint8_t;
struct l_array_66_uint8_t;
struct l_array_60_uint8_t;
struct l_array_64_uint8_t;
struct l_array_65_uint8_t;
struct l_array_71_uint8_t;
struct l_array_81_uint8_t;
struct l_array_5_uint32_t;

/* Function definitions */
typedef void l_fptr_5(struct l_struct_class_OC_S3*);

typedef void l_fptr_4(struct l_struct_class_OC_S2*);

typedef uint8_t* l_fptr_12(uint8_t*, uint32_t, uint64_t);

typedef void l_fptr_8(struct l_struct_class_OC_S4*, uint32_t);

typedef void l_fptr_7(uint8_t*, uint8_t, uint64_t, bool);

typedef void l_fptr_9(struct l_struct_class_OC_S5*, uint32_t);

typedef void l_fptr_3(void);

typedef void l_fptr_1(void);

typedef void l_fptr_11(uint8_t*);

typedef uint32_t l_fptr_6(uint32_t, uint8_t**);

typedef bool l_fptr_2(uint32_t);

typedef void l_fptr_10(struct l_struct_class_OC_S6*);


/* Types Definitions */
struct l_struct_class_OC_S2 {
  uint32_t field0;
};
struct l_struct_class_OC_S3 {
  uint32_t field0;
};
struct l_struct_class_OC_S4 {
  uint32_t field0;
};
struct l_struct_class_OC_S5 {
  uint32_t field0;
};
struct l_struct_class_OC_S6 {
  uint32_t field0;
};
struct l_array_5_struct_AC_l_struct_class_OC_S2 {
  struct l_struct_class_OC_S2 array[5];
};
struct l_array_5_struct_AC_l_struct_class_OC_S3 {
  struct l_struct_class_OC_S3 array[5];
};
struct l_array_56_uint8_t {
  uint8_t array[56];
};
struct l_array_58_uint8_t {
  uint8_t array[58];
};
struct l_array_59_uint8_t {
  uint8_t array[59];
};
struct l_array_62_uint8_t {
  uint8_t array[62];
};
struct l_array_63_uint8_t {
  uint8_t array[63];
};
struct l_array_87_uint8_t {
  uint8_t array[87];
};
struct l_array_61_uint8_t {
  uint8_t array[61];
};
struct l_array_72_uint8_t {
  uint8_t array[72];
};
struct l_array_66_uint8_t {
  uint8_t array[66];
};
struct l_array_60_uint8_t {
  uint8_t array[60];
};
struct l_array_64_uint8_t {
  uint8_t array[64];
};
struct l_array_65_uint8_t {
  uint8_t array[65];
};
struct l_array_71_uint8_t {
  uint8_t array[71];
};
struct l_array_81_uint8_t {
  uint8_t array[81];
};
struct l_array_5_uint32_t {
  uint32_t array[5];
};

/* External Global Variable Declarations */

/* Function Declarations */
void _Z3foov(void) __ATTRIBUTELIST__((noinline, nothrow));
bool _Z7foobooli(uint32_t) __ATTRIBUTELIST__((noinline, nothrow));
static void __cxx_global_var_init(void) __ATTRIBUTELIST__((noinline));
void _ZN2S2C2Ev(struct l_struct_class_OC_S2*) __ATTRIBUTELIST__((noinline, nothrow)) __ATTRIBUTE_WEAK__;
static void __cxx_global_var_init_OC_1(void) __ATTRIBUTELIST__((noinline));
static void __cxx_global_var_init_OC_2(void) __ATTRIBUTELIST__((noinline));
void _ZN2S3C2Ev(struct l_struct_class_OC_S3*) __ATTRIBUTELIST__((noinline, nothrow)) __ATTRIBUTE_WEAK__;
static void __cxx_global_var_init_OC_3(void) __ATTRIBUTELIST__((noinline));
static void __cxx_global_var_init_OC_4(void) __ATTRIBUTELIST__((noinline));
int main(int, char **) __ATTRIBUTELIST__((noinline));
void _ZN2S4C2Ei(struct l_struct_class_OC_S4*, uint32_t) __ATTRIBUTELIST__((noinline, nothrow)) __ATTRIBUTE_WEAK__;
void _ZN2S5C2Ei(struct l_struct_class_OC_S5*, uint32_t) __ATTRIBUTELIST__((noinline, nothrow)) __ATTRIBUTE_WEAK__;
void _ZN2S6C2Ev(struct l_struct_class_OC_S6*) __ATTRIBUTELIST__((noinline, nothrow)) __ATTRIBUTE_WEAK__;
void _Z25omp_for_pragma_talkad7420Pc(uint8_t*);
void _Z19for_loop_talkad7420v(void);
static void _GLOBAL__sub_I_teams_distribute_parallel_for_firstprivate_messages_OC_cpp(void) __ATTRIBUTELIST__((noinline));
uint8_t* memset(uint8_t*, uint32_t, uint64_t);


/* Global Variable Definitions and Initialization */
const float _ZN2S24S2scE;
static struct l_struct_class_OC_S2 _ZL1b;
static __MSALIGN__(16) struct l_array_5_struct_AC_l_struct_class_OC_S2 _ZL2ba __attribute__((aligned(16)));
static struct l_struct_class_OC_S3 _ZL1c;
static __MSALIGN__(16) struct l_array_5_struct_AC_l_struct_class_OC_S3 _ZL2ca __attribute__((aligned(16)));
struct l_struct_class_OC_S3 h;
static const struct l_array_56_uint8_t _OC_str = { "#pragma omp teams distribute parallel for firstprivate " };
static const struct l_array_58_uint8_t _OC_str_OC_5 = { "#pragma omp teams distribute parallel for firstprivate ( " };
static const struct l_array_59_uint8_t _OC_str_OC_6 = { "#pragma omp teams distribute parallel for firstprivate () " };
static const struct l_array_62_uint8_t _OC_str_OC_7 = { "#pragma omp teams distribute parallel for firstprivate (argc " };
static const struct l_array_63_uint8_t _OC_str_OC_8 = { "#pragma omp teams distribute parallel for firstprivate (argc, " };
static const struct l_array_87_uint8_t _OC_str_OC_9 = { "#pragma omp teams distribute parallel for firstprivate (argc > 0 ? argv[1] : argv[2]) " };
static const struct l_array_62_uint8_t _OC_str_OC_10 = { "#pragma omp teams distribute parallel for firstprivate (argc)" };
static const struct l_array_61_uint8_t _OC_str_OC_11 = { "#pragma omp teams distribute parallel for firstprivate (s1) " };
static const struct l_array_72_uint8_t _OC_str_OC_12 = { "#pragma omp teams distribute parallel for firstprivate (a, b, c, d, f) " };
static const struct l_array_66_uint8_t _OC_str_OC_13 = { "#pragma omp teams distribute parallel for firstprivate (argv[1]) " };
static const struct l_array_59_uint8_t _OC_str_OC_14 = { "#pragma omp teams distribute parallel for firstprivate(ba)" };
static const struct l_array_60_uint8_t _OC_str_OC_15 = { "#pragma omp teams distribute parallel for firstprivate(ca) " };
static const struct l_array_59_uint8_t _OC_str_OC_16 = { "#pragma omp teams distribute parallel for firstprivate(da)" };
static const struct l_array_64_uint8_t _OC_str_OC_17 = { "#pragma omp teams distribute parallel for firstprivate(s2::s2s)" };
static const struct l_array_65_uint8_t _OC_str_OC_18 = { "#pragma omp teams distribute parallel for firstprivate(s2::s2sc)" };
static const struct l_array_59_uint8_t _OC_str_OC_19 = { "#pragma omp teams distribute parallel for firstprivate(h) " };
static const struct l_array_71_uint8_t _OC_str_OC_20 = { "#pragma omp teams distribute parallel for private(i), firstprivate(i) " };
static const struct l_array_58_uint8_t _OC_str_OC_21 = { "#pragma omp teams distribute parallel for firstprivate(i)" };
static const struct l_array_59_uint8_t _OC_str_OC_22 = { "#pragma omp teams distribute parallel for firstprivate(i) " };
static const struct l_array_58_uint8_t _OC_str_OC_23 = { "#pragma omp teams distribute parallel for firstprivate(j)" };
static const struct l_array_81_uint8_t _OC_str_OC_24 = { "#pragma omp teams distribute parallel for lastprivate(argc), firstprivate(argc) " };


/* LLVM Intrinsic Builtin Function Bodies */
static __forceinline uint32_t llvm_add_u32(uint32_t a, uint32_t b) {
  uint32_t r = a + b;
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
#line 6 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_firstprivate_messages.cpp"
  ;
#line 7 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_firstprivate_messages.cpp"
  _3 = _2;
  return (((_3 != 0u)&1));
}


static void __cxx_global_var_init(void) {
#line 22 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_firstprivate_messages.cpp"
  _ZN2S2C2Ev((&_ZL1b));
}


void _ZN2S2C2Ev(struct l_struct_class_OC_S2* _4) {
  struct l_struct_class_OC_S2* _5;    /* Address-exposed local */
  struct l_struct_class_OC_S2* _6;

  _5 = _4;
#line 0 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_firstprivate_messages.cpp"
  ;
  _6 = _5;
#line 14 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_firstprivate_messages.cpp"
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
#line 23 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_firstprivate_messages.cpp"
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


static void __cxx_global_var_init_OC_2(void) {
#line 33 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_firstprivate_messages.cpp"
  _ZN2S3C2Ev((&_ZL1c));
}


void _ZN2S3C2Ev(struct l_struct_class_OC_S3* _11) {
  struct l_struct_class_OC_S3* _12;    /* Address-exposed local */
  struct l_struct_class_OC_S3* _13;

  _12 = _11;
#line 0 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_firstprivate_messages.cpp"
  ;
  _13 = _12;
#line 28 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_firstprivate_messages.cpp"
  *((&_13->field0)) = 0;
}


static void __cxx_global_var_init_OC_3(void) {
  struct l_struct_class_OC_S3* _14;
  struct l_struct_class_OC_S3* _14__PHI_TEMPORARY;
  struct l_struct_class_OC_S3* _15;

  _14__PHI_TEMPORARY = ((&_ZL2ca.array[((int32_t)0)]));   /* for PHI node */
  goto _16;

  do {     /* Syntactic loop '' to make GCC happy */
_16:
#line 34 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_firstprivate_messages.cpp"
  _14 = _14__PHI_TEMPORARY;
  _ZN2S3C2Ev(_14);
  _15 = (&_14[((int64_t)1)]);
  if ((((_15 == ((&((&_ZL2ca.array[((int32_t)0)]))[((int64_t)5)])))&1))) {
    goto _17;
  } else {
    _14__PHI_TEMPORARY = _15;   /* for PHI node */
    goto _16;
  }

  } while (1); /* end of syntactic loop '' */
_17:
  return;
}


static void __cxx_global_var_init_OC_4(void) {
#line 60 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_firstprivate_messages.cpp"
  _ZN2S3C2Ev((&h));
}


int main(int argc, char ** argv) {
  uint32_t _18 = (uint32_t)argc;
  uint8_t** _19 = (uint8_t**)argv;
  uint32_t _20;    /* Address-exposed local */
  uint32_t _21;    /* Address-exposed local */
  uint8_t** _22;    /* Address-exposed local */
  uint32_t _23;    /* Address-exposed local */
  __MSALIGN__(16) struct l_array_5_uint32_t _24 __attribute__((aligned(16)));    /* Address-exposed local */
  struct l_struct_class_OC_S4 _25;    /* Address-exposed local */
  struct l_struct_class_OC_S5 _26;    /* Address-exposed local */
  struct l_struct_class_OC_S6 _27;    /* Address-exposed local */
  uint32_t _28;    /* Address-exposed local */
  uint32_t* _29;    /* Address-exposed local */
  uint8_t* _30;
  uint32_t _31;
  uint32_t _32;
  uint32_t _33;
  uint32_t _34;
  uint32_t _35;
  uint32_t _36;
  uint32_t _37;
  uint32_t _38;
  uint32_t _39;
  uint32_t _40;
  uint32_t _41;
  uint32_t _42;
  uint32_t _43;
  uint32_t _44;
  uint32_t _45;
  uint32_t _46;
  uint32_t _47;
  uint32_t _48;
  uint32_t _49;
  uint32_t _50;
  uint32_t _51;
  uint32_t _52;
  uint32_t _53;
  uint32_t _54;
  uint32_t _55;
  uint32_t _56;
  uint32_t _57;
  uint32_t _58;
  uint32_t _59;
  uint32_t _60;
  uint32_t _61;
  uint32_t _62;
  uint32_t _63;
  uint32_t _64;
  uint32_t _65;
  uint32_t _66;
  uint32_t _67;
  uint32_t _68;
  uint32_t _69;
  uint32_t _70;
  uint32_t _71;
  uint32_t _72;
  uint32_t _73;
  uint32_t _74;
  uint32_t _75;
  uint32_t _76;
  uint32_t _77;
  uint32_t _78;
  uint32_t _79;
  uint32_t _80;
  uint32_t _81;
  uint32_t* _82;
  uint32_t* _83;
  uint32_t _84;
  uint32_t _85;
  uint32_t* _86;
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

  _20 = 0;
  _21 = _18;
#line 62 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_firstprivate_messages.cpp"
  ;
  _22 = _19;
  ;
#line 63 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_firstprivate_messages.cpp"
  ;
  _23 = 5;
#line 64 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_firstprivate_messages.cpp"
  ;
  _30 = memset((((uint8_t*)(&_24))), 0, 20);
#line 66 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_firstprivate_messages.cpp"
  ;
  _ZN2S4C2Ei((&_25), 4);
#line 67 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_firstprivate_messages.cpp"
  ;
  _ZN2S5C2Ei((&_26), 5);
#line 68 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_firstprivate_messages.cpp"
  ;
  _ZN2S6C2Ev((&_27));
#line 69 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_firstprivate_messages.cpp"
  ;
#line 70 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_firstprivate_messages.cpp"
  ;
  _29 = (&_28);
#line 72 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_firstprivate_messages.cpp"
  _Z25omp_for_pragma_talkad7420Pc(((&_OC_str.array[((int64_t)0)])));
#line 73 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_firstprivate_messages.cpp"
  _Z19for_loop_talkad7420v();
#line 74 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_firstprivate_messages.cpp"
  _28 = 0;
  goto _97;

  do {     /* Syntactic loop '' to make GCC happy */
_97:
  _31 = _28;
  _32 = _21;
  if ((((((int32_t)_31) < ((int32_t)_32))&1))) {
    goto _98;
  } else {
    goto _99;
  }

_98:
  _Z3foov();
  goto _100;

_100:
  _33 = _28;
  _28 = (llvm_add_u32(_33, 1));
  goto _97;

  } while (1); /* end of syntactic loop '' */
_99:
#line 78 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_firstprivate_messages.cpp"
  _Z25omp_for_pragma_talkad7420Pc(((&_OC_str_OC_5.array[((int64_t)0)])));
#line 79 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_firstprivate_messages.cpp"
  _Z19for_loop_talkad7420v();
#line 80 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_firstprivate_messages.cpp"
  _28 = 0;
  goto _101;

  do {     /* Syntactic loop '' to make GCC happy */
_101:
  _34 = _28;
  _35 = _21;
  if ((((((int32_t)_34) < ((int32_t)_35))&1))) {
    goto _102;
  } else {
    goto _103;
  }

_102:
  _Z3foov();
  goto _104;

_104:
  _36 = _28;
  _28 = (llvm_add_u32(_36, 1));
  goto _101;

  } while (1); /* end of syntactic loop '' */
_103:
#line 84 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_firstprivate_messages.cpp"
  _Z25omp_for_pragma_talkad7420Pc(((&_OC_str_OC_6.array[((int64_t)0)])));
#line 85 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_firstprivate_messages.cpp"
  _Z19for_loop_talkad7420v();
#line 86 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_firstprivate_messages.cpp"
  _28 = 0;
  goto _105;

  do {     /* Syntactic loop '' to make GCC happy */
_105:
  _37 = _28;
  _38 = _21;
  if ((((((int32_t)_37) < ((int32_t)_38))&1))) {
    goto _106;
  } else {
    goto _107;
  }

_106:
  _Z3foov();
  goto _108;

_108:
  _39 = _28;
  _28 = (llvm_add_u32(_39, 1));
  goto _105;

  } while (1); /* end of syntactic loop '' */
_107:
#line 90 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_firstprivate_messages.cpp"
  _Z25omp_for_pragma_talkad7420Pc(((&_OC_str_OC_7.array[((int64_t)0)])));
#line 91 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_firstprivate_messages.cpp"
  _Z19for_loop_talkad7420v();
#line 92 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_firstprivate_messages.cpp"
  _28 = 0;
  goto _109;

  do {     /* Syntactic loop '' to make GCC happy */
_109:
  _40 = _28;
  _41 = _21;
  if ((((((int32_t)_40) < ((int32_t)_41))&1))) {
    goto _110;
  } else {
    goto _111;
  }

_110:
  _Z3foov();
  goto _112;

_112:
  _42 = _28;
  _28 = (llvm_add_u32(_42, 1));
  goto _109;

  } while (1); /* end of syntactic loop '' */
_111:
#line 96 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_firstprivate_messages.cpp"
  _Z25omp_for_pragma_talkad7420Pc(((&_OC_str_OC_8.array[((int64_t)0)])));
#line 97 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_firstprivate_messages.cpp"
  _Z19for_loop_talkad7420v();
#line 98 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_firstprivate_messages.cpp"
  _28 = 0;
  goto _113;

  do {     /* Syntactic loop '' to make GCC happy */
_113:
  _43 = _28;
  _44 = _21;
  if ((((((int32_t)_43) < ((int32_t)_44))&1))) {
    goto _114;
  } else {
    goto _115;
  }

_114:
  _Z3foov();
  goto _116;

_116:
  _45 = _28;
  _28 = (llvm_add_u32(_45, 1));
  goto _113;

  } while (1); /* end of syntactic loop '' */
_115:
#line 102 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_firstprivate_messages.cpp"
  _Z25omp_for_pragma_talkad7420Pc(((&_OC_str_OC_9.array[((int64_t)0)])));
#line 103 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_firstprivate_messages.cpp"
  _Z19for_loop_talkad7420v();
#line 104 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_firstprivate_messages.cpp"
  _28 = 0;
  goto _117;

  do {     /* Syntactic loop '' to make GCC happy */
_117:
  _46 = _28;
  _47 = _21;
  if ((((((int32_t)_46) < ((int32_t)_47))&1))) {
    goto _118;
  } else {
    goto _119;
  }

_118:
  _Z3foov();
  goto _120;

_120:
  _48 = _28;
  _28 = (llvm_add_u32(_48, 1));
  goto _117;

  } while (1); /* end of syntactic loop '' */
_119:
#line 108 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_firstprivate_messages.cpp"
  _Z25omp_for_pragma_talkad7420Pc(((&_OC_str_OC_10.array[((int64_t)0)])));
#line 109 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_firstprivate_messages.cpp"
  _Z19for_loop_talkad7420v();
#line 110 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_firstprivate_messages.cpp"
  _28 = 0;
  goto _121;

  do {     /* Syntactic loop '' to make GCC happy */
_121:
  _49 = _28;
  _50 = _21;
  if ((((((int32_t)_49) < ((int32_t)_50))&1))) {
    goto _122;
  } else {
    goto _123;
  }

_122:
  _Z3foov();
  goto _124;

_124:
  _51 = _28;
  _28 = (llvm_add_u32(_51, 1));
  goto _121;

  } while (1); /* end of syntactic loop '' */
_123:
#line 114 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_firstprivate_messages.cpp"
  _Z25omp_for_pragma_talkad7420Pc(((&_OC_str_OC_11.array[((int64_t)0)])));
#line 115 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_firstprivate_messages.cpp"
  _Z19for_loop_talkad7420v();
#line 116 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_firstprivate_messages.cpp"
  _28 = 0;
  goto _125;

  do {     /* Syntactic loop '' to make GCC happy */
_125:
  _52 = _28;
  _53 = _21;
  if ((((((int32_t)_52) < ((int32_t)_53))&1))) {
    goto _126;
  } else {
    goto _127;
  }

_126:
  _Z3foov();
  goto _128;

_128:
  _54 = _28;
  _28 = (llvm_add_u32(_54, 1));
  goto _125;

  } while (1); /* end of syntactic loop '' */
_127:
#line 120 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_firstprivate_messages.cpp"
  _Z25omp_for_pragma_talkad7420Pc(((&_OC_str_OC_12.array[((int64_t)0)])));
#line 121 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_firstprivate_messages.cpp"
  _Z19for_loop_talkad7420v();
#line 122 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_firstprivate_messages.cpp"
  _28 = 0;
  goto _129;

  do {     /* Syntactic loop '' to make GCC happy */
_129:
  _55 = _28;
  _56 = _21;
  if ((((((int32_t)_55) < ((int32_t)_56))&1))) {
    goto _130;
  } else {
    goto _131;
  }

_130:
  _Z3foov();
  goto _132;

_132:
  _57 = _28;
  _28 = (llvm_add_u32(_57, 1));
  goto _129;

  } while (1); /* end of syntactic loop '' */
_131:
#line 126 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_firstprivate_messages.cpp"
  _Z25omp_for_pragma_talkad7420Pc(((&_OC_str_OC_13.array[((int64_t)0)])));
#line 127 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_firstprivate_messages.cpp"
  _Z19for_loop_talkad7420v();
#line 128 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_firstprivate_messages.cpp"
  _28 = 0;
  goto _133;

  do {     /* Syntactic loop '' to make GCC happy */
_133:
  _58 = _28;
  _59 = _21;
  if ((((((int32_t)_58) < ((int32_t)_59))&1))) {
    goto _134;
  } else {
    goto _135;
  }

_134:
  _Z3foov();
  goto _136;

_136:
  _60 = _28;
  _28 = (llvm_add_u32(_60, 1));
  goto _133;

  } while (1); /* end of syntactic loop '' */
_135:
#line 132 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_firstprivate_messages.cpp"
  _Z25omp_for_pragma_talkad7420Pc(((&_OC_str_OC_14.array[((int64_t)0)])));
#line 133 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_firstprivate_messages.cpp"
  _Z19for_loop_talkad7420v();
#line 134 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_firstprivate_messages.cpp"
  _28 = 0;
  goto _137;

  do {     /* Syntactic loop '' to make GCC happy */
_137:
  _61 = _28;
  _62 = _21;
  if ((((((int32_t)_61) < ((int32_t)_62))&1))) {
    goto _138;
  } else {
    goto _139;
  }

_138:
  _Z3foov();
  goto _140;

_140:
  _63 = _28;
  _28 = (llvm_add_u32(_63, 1));
  goto _137;

  } while (1); /* end of syntactic loop '' */
_139:
#line 138 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_firstprivate_messages.cpp"
  _Z25omp_for_pragma_talkad7420Pc(((&_OC_str_OC_15.array[((int64_t)0)])));
#line 139 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_firstprivate_messages.cpp"
  _Z19for_loop_talkad7420v();
#line 140 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_firstprivate_messages.cpp"
  _28 = 0;
  goto _141;

  do {     /* Syntactic loop '' to make GCC happy */
_141:
  _64 = _28;
  _65 = _21;
  if ((((((int32_t)_64) < ((int32_t)_65))&1))) {
    goto _142;
  } else {
    goto _143;
  }

_142:
  _Z3foov();
  goto _144;

_144:
  _66 = _28;
  _28 = (llvm_add_u32(_66, 1));
  goto _141;

  } while (1); /* end of syntactic loop '' */
_143:
#line 144 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_firstprivate_messages.cpp"
  _Z25omp_for_pragma_talkad7420Pc(((&_OC_str_OC_16.array[((int64_t)0)])));
#line 145 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_firstprivate_messages.cpp"
  _Z19for_loop_talkad7420v();
#line 146 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_firstprivate_messages.cpp"
  _28 = 0;
  goto _145;

  do {     /* Syntactic loop '' to make GCC happy */
_145:
  _67 = _28;
  _68 = _21;
  if ((((((int32_t)_67) < ((int32_t)_68))&1))) {
    goto _146;
  } else {
    goto _147;
  }

_146:
  _Z3foov();
  goto _148;

_148:
  _69 = _28;
  _28 = (llvm_add_u32(_69, 1));
  goto _145;

  } while (1); /* end of syntactic loop '' */
_147:
#line 150 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_firstprivate_messages.cpp"
  _Z25omp_for_pragma_talkad7420Pc(((&_OC_str_OC_17.array[((int64_t)0)])));
#line 151 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_firstprivate_messages.cpp"
  _Z19for_loop_talkad7420v();
#line 152 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_firstprivate_messages.cpp"
  _28 = 0;
  goto _149;

  do {     /* Syntactic loop '' to make GCC happy */
_149:
  _70 = _28;
  _71 = _21;
  if ((((((int32_t)_70) < ((int32_t)_71))&1))) {
    goto _150;
  } else {
    goto _151;
  }

_150:
  _Z3foov();
  goto _152;

_152:
  _72 = _28;
  _28 = (llvm_add_u32(_72, 1));
  goto _149;

  } while (1); /* end of syntactic loop '' */
_151:
#line 156 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_firstprivate_messages.cpp"
  _Z25omp_for_pragma_talkad7420Pc(((&_OC_str_OC_18.array[((int64_t)0)])));
#line 157 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_firstprivate_messages.cpp"
  _Z19for_loop_talkad7420v();
#line 158 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_firstprivate_messages.cpp"
  _28 = 0;
  goto _153;

  do {     /* Syntactic loop '' to make GCC happy */
_153:
  _73 = _28;
  _74 = _21;
  if ((((((int32_t)_73) < ((int32_t)_74))&1))) {
    goto _154;
  } else {
    goto _155;
  }

_154:
  _Z3foov();
  goto _156;

_156:
  _75 = _28;
  _28 = (llvm_add_u32(_75, 1));
  goto _153;

  } while (1); /* end of syntactic loop '' */
_155:
#line 162 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_firstprivate_messages.cpp"
  _Z25omp_for_pragma_talkad7420Pc(((&_OC_str_OC_19.array[((int64_t)0)])));
#line 163 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_firstprivate_messages.cpp"
  _Z19for_loop_talkad7420v();
#line 164 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_firstprivate_messages.cpp"
  _28 = 0;
  goto _157;

  do {     /* Syntactic loop '' to make GCC happy */
_157:
  _76 = _28;
  _77 = _21;
  if ((((((int32_t)_76) < ((int32_t)_77))&1))) {
    goto _158;
  } else {
    goto _159;
  }

_158:
  _Z3foov();
  goto _160;

_160:
  _78 = _28;
  _28 = (llvm_add_u32(_78, 1));
  goto _157;

  } while (1); /* end of syntactic loop '' */
_159:
#line 168 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_firstprivate_messages.cpp"
  _Z25omp_for_pragma_talkad7420Pc(((&_OC_str_OC_20.array[((int64_t)0)])));
#line 169 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_firstprivate_messages.cpp"
  _Z19for_loop_talkad7420v();
#line 170 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_firstprivate_messages.cpp"
  _28 = 0;
  goto _161;

  do {     /* Syntactic loop '' to make GCC happy */
_161:
  _79 = _28;
  _80 = _21;
  if ((((((int32_t)_79) < ((int32_t)_80))&1))) {
    goto _162;
  } else {
    goto _163;
  }

_162:
  _Z3foov();
  goto _164;

_164:
  _81 = _28;
  _28 = (llvm_add_u32(_81, 1));
  goto _161;

  } while (1); /* end of syntactic loop '' */
_163:
#line 174 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_firstprivate_messages.cpp"
  _Z25omp_for_pragma_talkad7420Pc(((&_OC_str_OC_21.array[((int64_t)0)])));
#line 175 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_firstprivate_messages.cpp"
  _Z19for_loop_talkad7420v();
#line 176 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_firstprivate_messages.cpp"
  _82 = _29;
  *_82 = 0;
  goto _165;

  do {     /* Syntactic loop '' to make GCC happy */
_165:
  _83 = _29;
  _84 = *_83;
  _85 = _21;
  if ((((((int32_t)_84) < ((int32_t)_85))&1))) {
    goto _166;
  } else {
    goto _167;
  }

_166:
  _Z3foov();
  goto _168;

_168:
  _86 = _29;
  _87 = *_86;
  *_86 = (llvm_add_u32(_87, 1));
  goto _165;

  } while (1); /* end of syntactic loop '' */
_167:
#line 180 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_firstprivate_messages.cpp"
  _Z25omp_for_pragma_talkad7420Pc(((&_OC_str_OC_22.array[((int64_t)0)])));
#line 181 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_firstprivate_messages.cpp"
  _Z19for_loop_talkad7420v();
#line 182 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_firstprivate_messages.cpp"
  _28 = 0;
  goto _169;

  do {     /* Syntactic loop '' to make GCC happy */
_169:
  _88 = _28;
  _89 = _21;
  if ((((((int32_t)_88) < ((int32_t)_89))&1))) {
    goto _170;
  } else {
    goto _171;
  }

_170:
  _Z3foov();
  goto _172;

_172:
  _90 = _28;
  _28 = (llvm_add_u32(_90, 1));
  goto _169;

  } while (1); /* end of syntactic loop '' */
_171:
#line 186 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_firstprivate_messages.cpp"
  _Z25omp_for_pragma_talkad7420Pc(((&_OC_str_OC_23.array[((int64_t)0)])));
#line 187 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_firstprivate_messages.cpp"
  _Z19for_loop_talkad7420v();
#line 188 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_firstprivate_messages.cpp"
  _28 = 0;
  goto _173;

  do {     /* Syntactic loop '' to make GCC happy */
_173:
  _91 = _28;
  _92 = _21;
  if ((((((int32_t)_91) < ((int32_t)_92))&1))) {
    goto _174;
  } else {
    goto _175;
  }

_174:
  _Z3foov();
  goto _176;

_176:
  _93 = _28;
  _28 = (llvm_add_u32(_93, 1));
  goto _173;

  } while (1); /* end of syntactic loop '' */
_175:
#line 192 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_firstprivate_messages.cpp"
  _Z25omp_for_pragma_talkad7420Pc(((&_OC_str_OC_24.array[((int64_t)0)])));
#line 193 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_firstprivate_messages.cpp"
  _Z19for_loop_talkad7420v();
#line 194 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_firstprivate_messages.cpp"
  _28 = 0;
  goto _177;

  do {     /* Syntactic loop '' to make GCC happy */
_177:
  _94 = _28;
  _95 = _21;
  if ((((((int32_t)_94) < ((int32_t)_95))&1))) {
    goto _178;
  } else {
    goto _179;
  }

_178:
  _Z3foov();
  goto _180;

_180:
  _96 = _28;
  _28 = (llvm_add_u32(_96, 1));
  goto _177;

  } while (1); /* end of syntactic loop '' */
_179:
  return 0;
}


void _ZN2S4C2Ei(struct l_struct_class_OC_S4* _181, uint32_t _182) {
  struct l_struct_class_OC_S4* _183;    /* Address-exposed local */
  uint32_t _184;    /* Address-exposed local */
  struct l_struct_class_OC_S4* _185;
  uint32_t _186;

  _183 = _181;
#line 0 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_firstprivate_messages.cpp"
  ;
  _184 = _182;
#line 41 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_firstprivate_messages.cpp"
  ;
  _185 = _183;
  _186 = _184;
  *((&_185->field0)) = _186;
}


void _ZN2S5C2Ei(struct l_struct_class_OC_S5* _187, uint32_t _188) {
  struct l_struct_class_OC_S5* _189;    /* Address-exposed local */
  uint32_t _190;    /* Address-exposed local */
  struct l_struct_class_OC_S5* _191;
  uint32_t _192;

  _189 = _187;
#line 0 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_firstprivate_messages.cpp"
  ;
  _190 = _188;
#line 51 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_firstprivate_messages.cpp"
  ;
  _191 = _189;
  _192 = _190;
  *((&_191->field0)) = _192;
}


void _ZN2S6C2Ev(struct l_struct_class_OC_S6* _193) {
  struct l_struct_class_OC_S6* _194;    /* Address-exposed local */
  struct l_struct_class_OC_S6* _195;

  _194 = _193;
#line 0 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_firstprivate_messages.cpp"
  ;
  _195 = _194;
#line 57 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_firstprivate_messages.cpp"
  *((&_195->field0)) = 0;
}


static void _GLOBAL__sub_I_teams_distribute_parallel_for_firstprivate_messages_OC_cpp(void) {
#line 0 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_firstprivate_messages.cpp"
  __cxx_global_var_init();
  __cxx_global_var_init_OC_1();
  __cxx_global_var_init_OC_2();
  __cxx_global_var_init_OC_3();
  __cxx_global_var_init_OC_4();
}

