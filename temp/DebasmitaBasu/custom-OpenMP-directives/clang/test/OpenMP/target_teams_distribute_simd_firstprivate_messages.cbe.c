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
struct l_array_5_uint32_t;

/* Function definitions */
typedef void l_fptr_9(struct l_struct_class_OC_S5*, uint32_t);

typedef void l_fptr_10(struct l_struct_class_OC_S6*);

typedef uint8_t* l_fptr_11(uint8_t*, uint32_t, uint64_t);

typedef void l_fptr_1(void);

typedef bool l_fptr_2(uint32_t);

typedef void l_fptr_3(void);

typedef void l_fptr_7(uint8_t*, uint8_t, uint64_t, bool);

typedef void l_fptr_5(struct l_struct_class_OC_S3*);

typedef void l_fptr_4(struct l_struct_class_OC_S2*);

typedef void l_fptr_8(struct l_struct_class_OC_S4*, uint32_t);

typedef uint32_t l_fptr_6(uint32_t, uint8_t**);


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
void _Z19for_loop_talkad7420v(void);
static void _GLOBAL__sub_I_target_teams_distribute_simd_firstprivate_messages_OC_cpp(void) __ATTRIBUTELIST__((noinline));
uint8_t* memset(uint8_t*, uint32_t, uint64_t);


/* Global Variable Definitions and Initialization */
const float _ZN2S24S2scE;
static struct l_struct_class_OC_S2 _ZL1b;
static __MSALIGN__(16) struct l_array_5_struct_AC_l_struct_class_OC_S2 _ZL2ba __attribute__((aligned(16)));
static struct l_struct_class_OC_S3 _ZL1c;
static __MSALIGN__(16) struct l_array_5_struct_AC_l_struct_class_OC_S3 _ZL2ca __attribute__((aligned(16)));
struct l_struct_class_OC_S3 h;


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
#line 6 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_simd_firstprivate_messages.cpp"
  ;
#line 7 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_simd_firstprivate_messages.cpp"
  _3 = _2;
  return (((_3 != 0u)&1));
}


static void __cxx_global_var_init(void) {
#line 22 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_simd_firstprivate_messages.cpp"
  _ZN2S2C2Ev((&_ZL1b));
}


void _ZN2S2C2Ev(struct l_struct_class_OC_S2* _4) {
  struct l_struct_class_OC_S2* _5;    /* Address-exposed local */
  struct l_struct_class_OC_S2* _6;

  _5 = _4;
#line 0 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_simd_firstprivate_messages.cpp"
  ;
  _6 = _5;
#line 14 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_simd_firstprivate_messages.cpp"
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
#line 23 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_simd_firstprivate_messages.cpp"
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
#line 33 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_simd_firstprivate_messages.cpp"
  _ZN2S3C2Ev((&_ZL1c));
}


void _ZN2S3C2Ev(struct l_struct_class_OC_S3* _11) {
  struct l_struct_class_OC_S3* _12;    /* Address-exposed local */
  struct l_struct_class_OC_S3* _13;

  _12 = _11;
#line 0 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_simd_firstprivate_messages.cpp"
  ;
  _13 = _12;
#line 28 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_simd_firstprivate_messages.cpp"
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
#line 34 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_simd_firstprivate_messages.cpp"
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
#line 60 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_simd_firstprivate_messages.cpp"
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
  uint32_t _97;
  uint32_t _98;
  uint32_t _99;

  _20 = 0;
  _21 = _18;
#line 62 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_simd_firstprivate_messages.cpp"
  ;
  _22 = _19;
  ;
#line 63 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_simd_firstprivate_messages.cpp"
  ;
  _23 = 5;
#line 64 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_simd_firstprivate_messages.cpp"
  ;
  _30 = memset((((uint8_t*)(&_24))), 0, 20);
#line 66 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_simd_firstprivate_messages.cpp"
  ;
  _ZN2S4C2Ei((&_25), 4);
#line 67 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_simd_firstprivate_messages.cpp"
  ;
  _ZN2S5C2Ei((&_26), 5);
#line 68 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_simd_firstprivate_messages.cpp"
  ;
  _ZN2S6C2Ev((&_27));
#line 69 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_simd_firstprivate_messages.cpp"
  ;
#line 70 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_simd_firstprivate_messages.cpp"
  ;
  _29 = (&_28);
#line 72 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_simd_firstprivate_messages.cpp"
  _Z19for_loop_talkad7420v();
#line 73 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_simd_firstprivate_messages.cpp"
  _28 = 0;
  goto _100;

  do {     /* Syntactic loop '' to make GCC happy */
_100:
  _31 = _28;
  _32 = _21;
  if ((((((int32_t)_31) < ((int32_t)_32))&1))) {
    goto _101;
  } else {
    goto _102;
  }

_101:
  _Z3foov();
  goto _103;

_103:
  _33 = _28;
  _28 = (llvm_add_u32(_33, 1));
  goto _100;

  } while (1); /* end of syntactic loop '' */
_102:
#line 77 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_simd_firstprivate_messages.cpp"
  _Z19for_loop_talkad7420v();
#line 78 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_simd_firstprivate_messages.cpp"
  _28 = 0;
  goto _104;

  do {     /* Syntactic loop '' to make GCC happy */
_104:
  _34 = _28;
  _35 = _21;
  if ((((((int32_t)_34) < ((int32_t)_35))&1))) {
    goto _105;
  } else {
    goto _106;
  }

_105:
  _Z3foov();
  goto _107;

_107:
  _36 = _28;
  _28 = (llvm_add_u32(_36, 1));
  goto _104;

  } while (1); /* end of syntactic loop '' */
_106:
#line 82 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_simd_firstprivate_messages.cpp"
  _Z19for_loop_talkad7420v();
#line 83 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_simd_firstprivate_messages.cpp"
  _28 = 0;
  goto _108;

  do {     /* Syntactic loop '' to make GCC happy */
_108:
  _37 = _28;
  _38 = _21;
  if ((((((int32_t)_37) < ((int32_t)_38))&1))) {
    goto _109;
  } else {
    goto _110;
  }

_109:
  _Z3foov();
  goto _111;

_111:
  _39 = _28;
  _28 = (llvm_add_u32(_39, 1));
  goto _108;

  } while (1); /* end of syntactic loop '' */
_110:
#line 87 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_simd_firstprivate_messages.cpp"
  _Z19for_loop_talkad7420v();
#line 88 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_simd_firstprivate_messages.cpp"
  _28 = 0;
  goto _112;

  do {     /* Syntactic loop '' to make GCC happy */
_112:
  _40 = _28;
  _41 = _21;
  if ((((((int32_t)_40) < ((int32_t)_41))&1))) {
    goto _113;
  } else {
    goto _114;
  }

_113:
  _Z3foov();
  goto _115;

_115:
  _42 = _28;
  _28 = (llvm_add_u32(_42, 1));
  goto _112;

  } while (1); /* end of syntactic loop '' */
_114:
#line 92 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_simd_firstprivate_messages.cpp"
  _Z19for_loop_talkad7420v();
#line 93 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_simd_firstprivate_messages.cpp"
  _28 = 0;
  goto _116;

  do {     /* Syntactic loop '' to make GCC happy */
_116:
  _43 = _28;
  _44 = _21;
  if ((((((int32_t)_43) < ((int32_t)_44))&1))) {
    goto _117;
  } else {
    goto _118;
  }

_117:
  _Z3foov();
  goto _119;

_119:
  _45 = _28;
  _28 = (llvm_add_u32(_45, 1));
  goto _116;

  } while (1); /* end of syntactic loop '' */
_118:
#line 97 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_simd_firstprivate_messages.cpp"
  _Z19for_loop_talkad7420v();
#line 98 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_simd_firstprivate_messages.cpp"
  _28 = 0;
  goto _120;

  do {     /* Syntactic loop '' to make GCC happy */
_120:
  _46 = _28;
  _47 = _21;
  if ((((((int32_t)_46) < ((int32_t)_47))&1))) {
    goto _121;
  } else {
    goto _122;
  }

_121:
  _Z3foov();
  goto _123;

_123:
  _48 = _28;
  _28 = (llvm_add_u32(_48, 1));
  goto _120;

  } while (1); /* end of syntactic loop '' */
_122:
#line 102 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_simd_firstprivate_messages.cpp"
  _Z19for_loop_talkad7420v();
#line 103 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_simd_firstprivate_messages.cpp"
  _28 = 0;
  goto _124;

  do {     /* Syntactic loop '' to make GCC happy */
_124:
  _49 = _28;
  _50 = _21;
  if ((((((int32_t)_49) < ((int32_t)_50))&1))) {
    goto _125;
  } else {
    goto _126;
  }

_125:
  _Z3foov();
  goto _127;

_127:
  _51 = _28;
  _28 = (llvm_add_u32(_51, 1));
  goto _124;

  } while (1); /* end of syntactic loop '' */
_126:
#line 107 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_simd_firstprivate_messages.cpp"
  _Z19for_loop_talkad7420v();
#line 108 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_simd_firstprivate_messages.cpp"
  _28 = 0;
  goto _128;

  do {     /* Syntactic loop '' to make GCC happy */
_128:
  _52 = _28;
  _53 = _21;
  if ((((((int32_t)_52) < ((int32_t)_53))&1))) {
    goto _129;
  } else {
    goto _130;
  }

_129:
  _Z3foov();
  goto _131;

_131:
  _54 = _28;
  _28 = (llvm_add_u32(_54, 1));
  goto _128;

  } while (1); /* end of syntactic loop '' */
_130:
#line 112 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_simd_firstprivate_messages.cpp"
  _Z19for_loop_talkad7420v();
#line 113 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_simd_firstprivate_messages.cpp"
  _28 = 0;
  goto _132;

  do {     /* Syntactic loop '' to make GCC happy */
_132:
  _55 = _28;
  _56 = _21;
  if ((((((int32_t)_55) < ((int32_t)_56))&1))) {
    goto _133;
  } else {
    goto _134;
  }

_133:
  _Z3foov();
  goto _135;

_135:
  _57 = _28;
  _28 = (llvm_add_u32(_57, 1));
  goto _132;

  } while (1); /* end of syntactic loop '' */
_134:
#line 117 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_simd_firstprivate_messages.cpp"
  _Z19for_loop_talkad7420v();
#line 118 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_simd_firstprivate_messages.cpp"
  _28 = 0;
  goto _136;

  do {     /* Syntactic loop '' to make GCC happy */
_136:
  _58 = _28;
  _59 = _21;
  if ((((((int32_t)_58) < ((int32_t)_59))&1))) {
    goto _137;
  } else {
    goto _138;
  }

_137:
  _Z3foov();
  goto _139;

_139:
  _60 = _28;
  _28 = (llvm_add_u32(_60, 1));
  goto _136;

  } while (1); /* end of syntactic loop '' */
_138:
#line 122 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_simd_firstprivate_messages.cpp"
  _Z19for_loop_talkad7420v();
#line 123 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_simd_firstprivate_messages.cpp"
  _28 = 0;
  goto _140;

  do {     /* Syntactic loop '' to make GCC happy */
_140:
  _61 = _28;
  _62 = _21;
  if ((((((int32_t)_61) < ((int32_t)_62))&1))) {
    goto _141;
  } else {
    goto _142;
  }

_141:
  _Z3foov();
  goto _143;

_143:
  _63 = _28;
  _28 = (llvm_add_u32(_63, 1));
  goto _140;

  } while (1); /* end of syntactic loop '' */
_142:
#line 127 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_simd_firstprivate_messages.cpp"
  _Z19for_loop_talkad7420v();
#line 128 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_simd_firstprivate_messages.cpp"
  _28 = 0;
  goto _144;

  do {     /* Syntactic loop '' to make GCC happy */
_144:
  _64 = _28;
  _65 = _21;
  if ((((((int32_t)_64) < ((int32_t)_65))&1))) {
    goto _145;
  } else {
    goto _146;
  }

_145:
  _Z3foov();
  goto _147;

_147:
  _66 = _28;
  _28 = (llvm_add_u32(_66, 1));
  goto _144;

  } while (1); /* end of syntactic loop '' */
_146:
#line 132 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_simd_firstprivate_messages.cpp"
  _Z19for_loop_talkad7420v();
#line 133 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_simd_firstprivate_messages.cpp"
  _28 = 0;
  goto _148;

  do {     /* Syntactic loop '' to make GCC happy */
_148:
  _67 = _28;
  _68 = _21;
  if ((((((int32_t)_67) < ((int32_t)_68))&1))) {
    goto _149;
  } else {
    goto _150;
  }

_149:
  _Z3foov();
  goto _151;

_151:
  _69 = _28;
  _28 = (llvm_add_u32(_69, 1));
  goto _148;

  } while (1); /* end of syntactic loop '' */
_150:
#line 137 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_simd_firstprivate_messages.cpp"
  _Z19for_loop_talkad7420v();
#line 138 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_simd_firstprivate_messages.cpp"
  _28 = 0;
  goto _152;

  do {     /* Syntactic loop '' to make GCC happy */
_152:
  _70 = _28;
  _71 = _21;
  if ((((((int32_t)_70) < ((int32_t)_71))&1))) {
    goto _153;
  } else {
    goto _154;
  }

_153:
  _Z3foov();
  goto _155;

_155:
  _72 = _28;
  _28 = (llvm_add_u32(_72, 1));
  goto _152;

  } while (1); /* end of syntactic loop '' */
_154:
#line 142 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_simd_firstprivate_messages.cpp"
  _Z19for_loop_talkad7420v();
#line 143 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_simd_firstprivate_messages.cpp"
  _28 = 0;
  goto _156;

  do {     /* Syntactic loop '' to make GCC happy */
_156:
  _73 = _28;
  _74 = _21;
  if ((((((int32_t)_73) < ((int32_t)_74))&1))) {
    goto _157;
  } else {
    goto _158;
  }

_157:
  _Z3foov();
  goto _159;

_159:
  _75 = _28;
  _28 = (llvm_add_u32(_75, 1));
  goto _156;

  } while (1); /* end of syntactic loop '' */
_158:
#line 147 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_simd_firstprivate_messages.cpp"
  _Z19for_loop_talkad7420v();
#line 148 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_simd_firstprivate_messages.cpp"
  _28 = 0;
  goto _160;

  do {     /* Syntactic loop '' to make GCC happy */
_160:
  _76 = _28;
  _77 = _21;
  if ((((((int32_t)_76) < ((int32_t)_77))&1))) {
    goto _161;
  } else {
    goto _162;
  }

_161:
  _Z3foov();
  goto _163;

_163:
  _78 = _28;
  _28 = (llvm_add_u32(_78, 1));
  goto _160;

  } while (1); /* end of syntactic loop '' */
_162:
#line 152 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_simd_firstprivate_messages.cpp"
  _Z19for_loop_talkad7420v();
#line 153 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_simd_firstprivate_messages.cpp"
  _28 = 0;
  goto _164;

  do {     /* Syntactic loop '' to make GCC happy */
_164:
  _79 = _28;
  _80 = _21;
  if ((((((int32_t)_79) < ((int32_t)_80))&1))) {
    goto _165;
  } else {
    goto _166;
  }

_165:
  _Z3foov();
  goto _167;

_167:
  _81 = _28;
  _28 = (llvm_add_u32(_81, 1));
  goto _164;

  } while (1); /* end of syntactic loop '' */
_166:
#line 157 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_simd_firstprivate_messages.cpp"
  _Z19for_loop_talkad7420v();
#line 158 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_simd_firstprivate_messages.cpp"
  _82 = _29;
  *_82 = 0;
  goto _168;

  do {     /* Syntactic loop '' to make GCC happy */
_168:
  _83 = _29;
  _84 = *_83;
  _85 = _21;
  if ((((((int32_t)_84) < ((int32_t)_85))&1))) {
    goto _169;
  } else {
    goto _170;
  }

_169:
  _Z3foov();
  goto _171;

_171:
  _86 = _29;
  _87 = *_86;
  *_86 = (llvm_add_u32(_87, 1));
  goto _168;

  } while (1); /* end of syntactic loop '' */
_170:
#line 162 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_simd_firstprivate_messages.cpp"
  _Z19for_loop_talkad7420v();
#line 163 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_simd_firstprivate_messages.cpp"
  _28 = 0;
  goto _172;

  do {     /* Syntactic loop '' to make GCC happy */
_172:
  _88 = _28;
  _89 = _21;
  if ((((((int32_t)_88) < ((int32_t)_89))&1))) {
    goto _173;
  } else {
    goto _174;
  }

_173:
  _Z3foov();
  goto _175;

_175:
  _90 = _28;
  _28 = (llvm_add_u32(_90, 1));
  goto _172;

  } while (1); /* end of syntactic loop '' */
_174:
#line 167 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_simd_firstprivate_messages.cpp"
  _Z19for_loop_talkad7420v();
#line 168 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_simd_firstprivate_messages.cpp"
  _28 = 0;
  goto _176;

  do {     /* Syntactic loop '' to make GCC happy */
_176:
  _91 = _28;
  _92 = _21;
  if ((((((int32_t)_91) < ((int32_t)_92))&1))) {
    goto _177;
  } else {
    goto _178;
  }

_177:
  _Z3foov();
  goto _179;

_179:
  _93 = _28;
  _28 = (llvm_add_u32(_93, 1));
  goto _176;

  } while (1); /* end of syntactic loop '' */
_178:
#line 172 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_simd_firstprivate_messages.cpp"
  _Z19for_loop_talkad7420v();
#line 173 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_simd_firstprivate_messages.cpp"
  _28 = 0;
  goto _180;

  do {     /* Syntactic loop '' to make GCC happy */
_180:
  _94 = _28;
  _95 = _21;
  if ((((((int32_t)_94) < ((int32_t)_95))&1))) {
    goto _181;
  } else {
    goto _182;
  }

_181:
  _Z3foov();
  goto _183;

_183:
  _96 = _28;
  _28 = (llvm_add_u32(_96, 1));
  goto _180;

  } while (1); /* end of syntactic loop '' */
_182:
#line 177 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_simd_firstprivate_messages.cpp"
  _Z19for_loop_talkad7420v();
#line 178 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_simd_firstprivate_messages.cpp"
  _28 = 0;
  goto _184;

  do {     /* Syntactic loop '' to make GCC happy */
_184:
  _97 = _28;
  _98 = _21;
  if ((((((int32_t)_97) < ((int32_t)_98))&1))) {
    goto _185;
  } else {
    goto _186;
  }

_185:
  _Z3foov();
  goto _187;

_187:
  _99 = _28;
  _28 = (llvm_add_u32(_99, 1));
  goto _184;

  } while (1); /* end of syntactic loop '' */
_186:
  return 0;
}


void _ZN2S4C2Ei(struct l_struct_class_OC_S4* _188, uint32_t _189) {
  struct l_struct_class_OC_S4* _190;    /* Address-exposed local */
  uint32_t _191;    /* Address-exposed local */
  struct l_struct_class_OC_S4* _192;
  uint32_t _193;

  _190 = _188;
#line 0 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_simd_firstprivate_messages.cpp"
  ;
  _191 = _189;
#line 41 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_simd_firstprivate_messages.cpp"
  ;
  _192 = _190;
  _193 = _191;
  *((&_192->field0)) = _193;
}


void _ZN2S5C2Ei(struct l_struct_class_OC_S5* _194, uint32_t _195) {
  struct l_struct_class_OC_S5* _196;    /* Address-exposed local */
  uint32_t _197;    /* Address-exposed local */
  struct l_struct_class_OC_S5* _198;
  uint32_t _199;

  _196 = _194;
#line 0 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_simd_firstprivate_messages.cpp"
  ;
  _197 = _195;
#line 51 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_simd_firstprivate_messages.cpp"
  ;
  _198 = _196;
  _199 = _197;
  *((&_198->field0)) = _199;
}


void _ZN2S6C2Ev(struct l_struct_class_OC_S6* _200) {
  struct l_struct_class_OC_S6* _201;    /* Address-exposed local */
  struct l_struct_class_OC_S6* _202;

  _201 = _200;
#line 0 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_simd_firstprivate_messages.cpp"
  ;
  _202 = _201;
#line 57 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_simd_firstprivate_messages.cpp"
  *((&_202->field0)) = 0;
}


static void _GLOBAL__sub_I_target_teams_distribute_simd_firstprivate_messages_OC_cpp(void) {
#line 0 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_distribute_simd_firstprivate_messages.cpp"
  __cxx_global_var_init();
  __cxx_global_var_init_OC_1();
  __cxx_global_var_init_OC_2();
  __cxx_global_var_init_OC_3();
  __cxx_global_var_init_OC_4();
}

