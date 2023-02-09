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



/* Global Declarations */

/* Types Declarations */
struct l_array_52_uint8_t;
struct l_array_54_uint8_t;
struct l_array_55_uint8_t;
struct l_array_59_uint8_t;
struct l_array_77_uint8_t;
struct l_array_100_uint8_t;
struct l_array_57_uint8_t;
struct l_array_64_uint8_t;
struct l_array_98_uint8_t;
struct l_array_56_uint8_t;
struct l_array_58_uint8_t;
struct l_array_76_uint8_t;
struct l_array_99_uint8_t;

/* Function definitions */
typedef bool l_fptr_2(uint32_t);

typedef void l_fptr_3(void);

typedef void l_fptr_1(void);

typedef void l_fptr_5(uint8_t*);

typedef uint32_t l_fptr_4(uint32_t, uint8_t**);


/* Types Definitions */
struct l_array_52_uint8_t {
  uint8_t array[52];
};
struct l_array_54_uint8_t {
  uint8_t array[54];
};
struct l_array_55_uint8_t {
  uint8_t array[55];
};
struct l_array_59_uint8_t {
  uint8_t array[59];
};
struct l_array_77_uint8_t {
  uint8_t array[77];
};
struct l_array_100_uint8_t {
  uint8_t array[100];
};
struct l_array_57_uint8_t {
  uint8_t array[57];
};
struct l_array_64_uint8_t {
  uint8_t array[64];
};
struct l_array_98_uint8_t {
  uint8_t array[98];
};
struct l_array_56_uint8_t {
  uint8_t array[56];
};
struct l_array_58_uint8_t {
  uint8_t array[58];
};
struct l_array_76_uint8_t {
  uint8_t array[76];
};
struct l_array_99_uint8_t {
  uint8_t array[99];
};

/* External Global Variable Declarations */

/* Function Declarations */
void _Z3foov(void) __ATTRIBUTELIST__((noinline, nothrow));
bool _Z7foobooli(uint32_t) __ATTRIBUTELIST__((noinline, nothrow));
int main(int, char **) __ATTRIBUTELIST__((noinline));
void _Z25omp_for_pragma_talkad7420Pc(uint8_t*);
void _Z19for_loop_talkad7420v(void);
uint32_t _Z5tmainIicLi1ELi0EET_S0_PPT0_(uint32_t, uint8_t**) __ATTRIBUTELIST__((noinline)) __ATTRIBUTE_WEAK__;


/* Global Variable Definitions and Initialization */
static const struct l_array_52_uint8_t _OC_str = { "#pragma omp teams distribute parallel for collapse " };
static const struct l_array_54_uint8_t _OC_str_OC_1 = { "#pragma omp teams distribute parallel for collapse ( " };
static const struct l_array_55_uint8_t _OC_str_OC_2 = { "#pragma omp teams distribute parallel for collapse () " };
static const struct l_array_55_uint8_t _OC_str_OC_3 = { "#pragma omp teams distribute parallel for collapse (4 " };
static const struct l_array_59_uint8_t _OC_str_OC_4 = { "#pragma omp teams distribute parallel for collapse (2+2)) " };
static const struct l_array_77_uint8_t _OC_str_OC_5 = { "#pragma omp teams distribute parallel for collapse (foobool(1) > 0 ? 1 : 2) " };
static const struct l_array_100_uint8_t _OC_str_OC_6 = { "#pragma omp teams distribute parallel for collapse (foobool(argc)), collapse (true), collapse (-5) " };
static const struct l_array_57_uint8_t _OC_str_OC_7 = { "#pragma omp teams distribute parallel for collapse (s1) " };
static const struct l_array_64_uint8_t _OC_str_OC_8 = { "#pragma omp teams distribute parallel for collapse (argv[1]=2) " };
static const struct l_array_98_uint8_t _OC_str_OC_9 = { "#pragma omp teams distribute parallel for collapse(collapse(tmain<int, char, -1, -2>(argc, argv) " };
static const struct l_array_56_uint8_t _OC_str_OC_10 = { "#pragma omp teams distribute parallel for collapse (2) " };
static const struct l_array_58_uint8_t _OC_str_OC_11 = { "#pragma omp teams distribute parallel for collapse (argc " };
static const struct l_array_56_uint8_t _OC_str_OC_12 = { "#pragma omp teams distribute parallel for collapse (st " };
static const struct l_array_57_uint8_t _OC_str_OC_13 = { "#pragma omp teams distribute parallel for collapse (1)) " };
static const struct l_array_76_uint8_t _OC_str_OC_14 = { "#pragma omp teams distribute parallel for collapse ((st > 0) ? 1 + st : 2) " };
static const struct l_array_99_uint8_t _OC_str_OC_15 = { "#pragma omp teams distribute parallel for collapse (foobool(argc)), collapse (true), collapse (-5)" };
static const struct l_array_55_uint8_t _OC_str_OC_16 = { "#pragma omp teams distribute parallel for collapse (1)" };
static const struct l_array_56_uint8_t _OC_str_OC_17 = { "#pragma omp teams distribute parallel for collapse (n) " };


/* LLVM Intrinsic Builtin Function Bodies */
static __forceinline uint32_t llvm_add_u32(uint32_t a, uint32_t b) {
  uint32_t r = a + b;
  return r;
}
static __forceinline uint32_t llvm_sub_u32(uint32_t a, uint32_t b) {
  uint32_t r = a - b;
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
#line 8 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_collapse_messages.cpp"
  ;
#line 9 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_collapse_messages.cpp"
  _3 = _2;
  return (((_3 != 0u)&1));
}


int main(int argc, char ** argv) {
  uint32_t _4 = (uint32_t)argc;
  uint8_t** _5 = (uint8_t**)argv;
  uint32_t _6;    /* Address-exposed local */
  uint32_t _7;    /* Address-exposed local */
  uint8_t** _8;    /* Address-exposed local */
  uint32_t _9;    /* Address-exposed local */
  uint32_t _10;    /* Address-exposed local */
  uint32_t _11;    /* Address-exposed local */
  uint32_t _12;    /* Address-exposed local */
  uint32_t _13;    /* Address-exposed local */
  uint32_t _14;    /* Address-exposed local */
  uint32_t _15;    /* Address-exposed local */
  uint32_t _16;    /* Address-exposed local */
  uint32_t _17;    /* Address-exposed local */
  uint32_t _18;
  uint8_t** _19;
  uint8_t* _20;
  uint32_t _21;
  uint8_t _22;
  uint8_t** _23;
  uint8_t* _24;
  uint32_t _25;
  uint8_t _26;
  uint8_t** _27;
  uint8_t* _28;
  uint32_t _29;
  uint32_t _30;
  uint32_t _31;
  uint8_t** _32;
  uint8_t* _33;
  uint32_t _34;
  uint8_t _35;
  uint8_t** _36;
  uint8_t* _37;
  uint32_t _38;
  uint8_t _39;
  uint8_t** _40;
  uint8_t* _41;
  uint32_t _42;
  uint32_t _43;
  uint32_t _44;
  uint8_t** _45;
  uint8_t* _46;
  uint32_t _47;
  uint8_t _48;
  uint8_t** _49;
  uint8_t* _50;
  uint32_t _51;
  uint8_t _52;
  uint8_t** _53;
  uint8_t* _54;
  uint32_t _55;
  uint32_t _56;
  uint32_t _57;
  uint8_t** _58;
  uint8_t* _59;
  uint32_t _60;
  uint8_t _61;
  uint8_t** _62;
  uint8_t* _63;
  uint32_t _64;
  uint8_t _65;
  uint8_t** _66;
  uint8_t* _67;
  uint32_t _68;
  uint32_t _69;
  uint32_t _70;
  uint8_t** _71;
  uint8_t* _72;
  uint32_t _73;
  uint8_t _74;
  uint8_t** _75;
  uint8_t* _76;
  uint32_t _77;
  uint8_t _78;
  uint8_t** _79;
  uint8_t* _80;
  uint32_t _81;
  uint32_t _82;
  uint32_t _83;
  uint8_t** _84;
  uint8_t* _85;
  uint32_t _86;
  uint8_t _87;
  uint8_t** _88;
  uint8_t* _89;
  uint32_t _90;
  uint8_t _91;
  uint8_t** _92;
  uint8_t* _93;
  uint32_t _94;
  uint32_t _95;
  uint32_t _96;
  uint8_t** _97;
  uint8_t* _98;
  uint32_t _99;
  uint8_t _100;
  uint8_t** _101;
  uint8_t* _102;
  uint32_t _103;
  uint8_t _104;
  uint8_t** _105;
  uint8_t* _106;
  uint32_t _107;
  uint32_t _108;
  uint32_t _109;
  uint8_t** _110;
  uint8_t* _111;
  uint32_t _112;
  uint8_t _113;
  uint8_t** _114;
  uint8_t* _115;
  uint32_t _116;
  uint8_t _117;
  uint8_t** _118;
  uint8_t* _119;
  uint32_t _120;
  uint32_t _121;
  uint32_t _122;
  uint8_t** _123;
  uint8_t* _124;
  uint32_t _125;
  uint8_t _126;
  uint8_t** _127;
  uint8_t* _128;
  uint32_t _129;
  uint8_t _130;
  uint8_t** _131;
  uint8_t* _132;
  uint32_t _133;
  uint32_t _134;
  uint32_t _135;
  uint8_t** _136;
  uint32_t _137;

  _6 = 0;
  _7 = _4;
#line 107 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_collapse_messages.cpp"
  ;
  _8 = _5;
  ;
#line 109 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_collapse_messages.cpp"
  _Z25omp_for_pragma_talkad7420Pc(((&_OC_str.array[((int64_t)0)])));
#line 110 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_collapse_messages.cpp"
  _Z19for_loop_talkad7420v();
#line 111 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_collapse_messages.cpp"
  ;
  _9 = 4;
  goto _138;

  do {     /* Syntactic loop '' to make GCC happy */
_138:
  _18 = _9;
  if ((((((int32_t)_18) < ((int32_t)12u))&1))) {
    goto _139;
  } else {
    goto _140;
  }

_139:
#line 113 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_collapse_messages.cpp"
  _19 = _8;
  _20 = *((&(*_19)));
  _21 = _9;
  _22 = *((&_20[((int64_t)(((int64_t)(int32_t)_21)))]));
  _23 = _8;
  _24 = *((&(*_23)));
  _25 = _9;
  _26 = *((&_24[((int64_t)(((int64_t)(int32_t)(llvm_sub_u32(_25, 4)))))]));
  _27 = _8;
  _28 = *((&(*_27)));
  _29 = _9;
  *((&_28[((int64_t)(((int64_t)(int32_t)_29)))])) = (((uint8_t)(llvm_sub_u32((((int32_t)(int8_t)_22)), (((int32_t)(int8_t)_26))))));
  goto _141;

_141:
#line 111 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_collapse_messages.cpp"
  _30 = _9;
  _9 = (llvm_add_u32(_30, 1));
  goto _138;

  } while (1); /* end of syntactic loop '' */
_140:
#line 116 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_collapse_messages.cpp"
  _Z25omp_for_pragma_talkad7420Pc(((&_OC_str_OC_1.array[((int64_t)0)])));
#line 117 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_collapse_messages.cpp"
  _Z19for_loop_talkad7420v();
#line 118 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_collapse_messages.cpp"
  ;
  _10 = 4;
  goto _142;

  do {     /* Syntactic loop '' to make GCC happy */
_142:
  _31 = _10;
  if ((((((int32_t)_31) < ((int32_t)12u))&1))) {
    goto _143;
  } else {
    goto _144;
  }

_143:
#line 120 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_collapse_messages.cpp"
  _32 = _8;
  _33 = *((&(*_32)));
  _34 = _10;
  _35 = *((&_33[((int64_t)(((int64_t)(int32_t)_34)))]));
  _36 = _8;
  _37 = *((&(*_36)));
  _38 = _10;
  _39 = *((&_37[((int64_t)(((int64_t)(int32_t)(llvm_sub_u32(_38, 4)))))]));
  _40 = _8;
  _41 = *((&(*_40)));
  _42 = _10;
  *((&_41[((int64_t)(((int64_t)(int32_t)_42)))])) = (((uint8_t)(llvm_sub_u32((((int32_t)(int8_t)_35)), (((int32_t)(int8_t)_39))))));
  goto _145;

_145:
#line 118 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_collapse_messages.cpp"
  _43 = _10;
  _10 = (llvm_add_u32(_43, 1));
  goto _142;

  } while (1); /* end of syntactic loop '' */
_144:
#line 123 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_collapse_messages.cpp"
  _Z25omp_for_pragma_talkad7420Pc(((&_OC_str_OC_2.array[((int64_t)0)])));
#line 124 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_collapse_messages.cpp"
  _Z19for_loop_talkad7420v();
#line 125 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_collapse_messages.cpp"
  ;
  _11 = 4;
  goto _146;

  do {     /* Syntactic loop '' to make GCC happy */
_146:
  _44 = _11;
  if ((((((int32_t)_44) < ((int32_t)12u))&1))) {
    goto _147;
  } else {
    goto _148;
  }

_147:
#line 127 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_collapse_messages.cpp"
  _45 = _8;
  _46 = *((&(*_45)));
  _47 = _11;
  _48 = *((&_46[((int64_t)(((int64_t)(int32_t)_47)))]));
  _49 = _8;
  _50 = *((&(*_49)));
  _51 = _11;
  _52 = *((&_50[((int64_t)(((int64_t)(int32_t)(llvm_sub_u32(_51, 4)))))]));
  _53 = _8;
  _54 = *((&(*_53)));
  _55 = _11;
  *((&_54[((int64_t)(((int64_t)(int32_t)_55)))])) = (((uint8_t)(llvm_sub_u32((((int32_t)(int8_t)_48)), (((int32_t)(int8_t)_52))))));
  goto _149;

_149:
#line 125 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_collapse_messages.cpp"
  _56 = _11;
  _11 = (llvm_add_u32(_56, 1));
  goto _146;

  } while (1); /* end of syntactic loop '' */
_148:
#line 130 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_collapse_messages.cpp"
  _Z25omp_for_pragma_talkad7420Pc(((&_OC_str_OC_3.array[((int64_t)0)])));
#line 131 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_collapse_messages.cpp"
  _Z19for_loop_talkad7420v();
#line 132 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_collapse_messages.cpp"
  ;
  _12 = 4;
  goto _150;

  do {     /* Syntactic loop '' to make GCC happy */
_150:
  _57 = _12;
  if ((((((int32_t)_57) < ((int32_t)12u))&1))) {
    goto _151;
  } else {
    goto _152;
  }

_151:
#line 134 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_collapse_messages.cpp"
  _58 = _8;
  _59 = *((&(*_58)));
  _60 = _12;
  _61 = *((&_59[((int64_t)(((int64_t)(int32_t)_60)))]));
  _62 = _8;
  _63 = *((&(*_62)));
  _64 = _12;
  _65 = *((&_63[((int64_t)(((int64_t)(int32_t)(llvm_sub_u32(_64, 4)))))]));
  _66 = _8;
  _67 = *((&(*_66)));
  _68 = _12;
  *((&_67[((int64_t)(((int64_t)(int32_t)_68)))])) = (((uint8_t)(llvm_sub_u32((((int32_t)(int8_t)_61)), (((int32_t)(int8_t)_65))))));
  goto _153;

_153:
#line 132 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_collapse_messages.cpp"
  _69 = _12;
  _12 = (llvm_add_u32(_69, 1));
  goto _150;

  } while (1); /* end of syntactic loop '' */
_152:
#line 137 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_collapse_messages.cpp"
  _Z25omp_for_pragma_talkad7420Pc(((&_OC_str_OC_4.array[((int64_t)0)])));
#line 138 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_collapse_messages.cpp"
  _Z19for_loop_talkad7420v();
#line 139 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_collapse_messages.cpp"
  ;
  _13 = 4;
  goto _154;

  do {     /* Syntactic loop '' to make GCC happy */
_154:
  _70 = _13;
  if ((((((int32_t)_70) < ((int32_t)12u))&1))) {
    goto _155;
  } else {
    goto _156;
  }

_155:
#line 141 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_collapse_messages.cpp"
  _71 = _8;
  _72 = *((&(*_71)));
  _73 = _13;
  _74 = *((&_72[((int64_t)(((int64_t)(int32_t)_73)))]));
  _75 = _8;
  _76 = *((&(*_75)));
  _77 = _13;
  _78 = *((&_76[((int64_t)(((int64_t)(int32_t)(llvm_sub_u32(_77, 4)))))]));
  _79 = _8;
  _80 = *((&(*_79)));
  _81 = _13;
  *((&_80[((int64_t)(((int64_t)(int32_t)_81)))])) = (((uint8_t)(llvm_sub_u32((((int32_t)(int8_t)_74)), (((int32_t)(int8_t)_78))))));
  goto _157;

_157:
#line 139 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_collapse_messages.cpp"
  _82 = _13;
  _13 = (llvm_add_u32(_82, 1));
  goto _154;

  } while (1); /* end of syntactic loop '' */
_156:
#line 146 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_collapse_messages.cpp"
  _Z25omp_for_pragma_talkad7420Pc(((&_OC_str_OC_5.array[((int64_t)0)])));
#line 147 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_collapse_messages.cpp"
  _Z19for_loop_talkad7420v();
#line 148 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_collapse_messages.cpp"
  ;
  _14 = 4;
  goto _158;

  do {     /* Syntactic loop '' to make GCC happy */
_158:
  _83 = _14;
  if ((((((int32_t)_83) < ((int32_t)12u))&1))) {
    goto _159;
  } else {
    goto _160;
  }

_159:
#line 150 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_collapse_messages.cpp"
  _84 = _8;
  _85 = *((&(*_84)));
  _86 = _14;
  _87 = *((&_85[((int64_t)(((int64_t)(int32_t)_86)))]));
  _88 = _8;
  _89 = *((&(*_88)));
  _90 = _14;
  _91 = *((&_89[((int64_t)(((int64_t)(int32_t)(llvm_sub_u32(_90, 4)))))]));
  _92 = _8;
  _93 = *((&(*_92)));
  _94 = _14;
  *((&_93[((int64_t)(((int64_t)(int32_t)_94)))])) = (((uint8_t)(llvm_sub_u32((((int32_t)(int8_t)_87)), (((int32_t)(int8_t)_91))))));
  goto _161;

_161:
#line 148 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_collapse_messages.cpp"
  _95 = _14;
  _14 = (llvm_add_u32(_95, 1));
  goto _158;

  } while (1); /* end of syntactic loop '' */
_160:
#line 155 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_collapse_messages.cpp"
  _Z25omp_for_pragma_talkad7420Pc(((&_OC_str_OC_6.array[((int64_t)0)])));
#line 156 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_collapse_messages.cpp"
  _Z19for_loop_talkad7420v();
#line 157 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_collapse_messages.cpp"
  ;
  _15 = 4;
  goto _162;

  do {     /* Syntactic loop '' to make GCC happy */
_162:
  _96 = _15;
  if ((((((int32_t)_96) < ((int32_t)12u))&1))) {
    goto _163;
  } else {
    goto _164;
  }

_163:
#line 159 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_collapse_messages.cpp"
  _97 = _8;
  _98 = *((&(*_97)));
  _99 = _15;
  _100 = *((&_98[((int64_t)(((int64_t)(int32_t)_99)))]));
  _101 = _8;
  _102 = *((&(*_101)));
  _103 = _15;
  _104 = *((&_102[((int64_t)(((int64_t)(int32_t)(llvm_sub_u32(_103, 4)))))]));
  _105 = _8;
  _106 = *((&(*_105)));
  _107 = _15;
  *((&_106[((int64_t)(((int64_t)(int32_t)_107)))])) = (((uint8_t)(llvm_sub_u32((((int32_t)(int8_t)_100)), (((int32_t)(int8_t)_104))))));
  goto _165;

_165:
#line 157 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_collapse_messages.cpp"
  _108 = _15;
  _15 = (llvm_add_u32(_108, 1));
  goto _162;

  } while (1); /* end of syntactic loop '' */
_164:
#line 162 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_collapse_messages.cpp"
  _Z25omp_for_pragma_talkad7420Pc(((&_OC_str_OC_7.array[((int64_t)0)])));
#line 163 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_collapse_messages.cpp"
  _Z19for_loop_talkad7420v();
#line 164 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_collapse_messages.cpp"
  ;
  _16 = 4;
  goto _166;

  do {     /* Syntactic loop '' to make GCC happy */
_166:
  _109 = _16;
  if ((((((int32_t)_109) < ((int32_t)12u))&1))) {
    goto _167;
  } else {
    goto _168;
  }

_167:
#line 166 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_collapse_messages.cpp"
  _110 = _8;
  _111 = *((&(*_110)));
  _112 = _16;
  _113 = *((&_111[((int64_t)(((int64_t)(int32_t)_112)))]));
  _114 = _8;
  _115 = *((&(*_114)));
  _116 = _16;
  _117 = *((&_115[((int64_t)(((int64_t)(int32_t)(llvm_sub_u32(_116, 4)))))]));
  _118 = _8;
  _119 = *((&(*_118)));
  _120 = _16;
  *((&_119[((int64_t)(((int64_t)(int32_t)_120)))])) = (((uint8_t)(llvm_sub_u32((((int32_t)(int8_t)_113)), (((int32_t)(int8_t)_117))))));
  goto _169;

_169:
#line 164 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_collapse_messages.cpp"
  _121 = _16;
  _16 = (llvm_add_u32(_121, 1));
  goto _166;

  } while (1); /* end of syntactic loop '' */
_168:
#line 172 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_collapse_messages.cpp"
  _Z25omp_for_pragma_talkad7420Pc(((&_OC_str_OC_8.array[((int64_t)0)])));
#line 173 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_collapse_messages.cpp"
  _Z19for_loop_talkad7420v();
#line 174 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_collapse_messages.cpp"
  ;
  _17 = 4;
  goto _170;

  do {     /* Syntactic loop '' to make GCC happy */
_170:
  _122 = _17;
  if ((((((int32_t)_122) < ((int32_t)12u))&1))) {
    goto _171;
  } else {
    goto _172;
  }

_171:
#line 176 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_collapse_messages.cpp"
  _123 = _8;
  _124 = *((&(*_123)));
  _125 = _17;
  _126 = *((&_124[((int64_t)(((int64_t)(int32_t)_125)))]));
  _127 = _8;
  _128 = *((&(*_127)));
  _129 = _17;
  _130 = *((&_128[((int64_t)(((int64_t)(int32_t)(llvm_sub_u32(_129, 4)))))]));
  _131 = _8;
  _132 = *((&(*_131)));
  _133 = _17;
  *((&_132[((int64_t)(((int64_t)(int32_t)_133)))])) = (((uint8_t)(llvm_sub_u32((((int32_t)(int8_t)_126)), (((int32_t)(int8_t)_130))))));
  goto _173;

_173:
#line 174 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_collapse_messages.cpp"
  _134 = _17;
  _17 = (llvm_add_u32(_134, 1));
  goto _170;

  } while (1); /* end of syntactic loop '' */
_172:
#line 179 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_collapse_messages.cpp"
  _Z25omp_for_pragma_talkad7420Pc(((&_OC_str_OC_9.array[((int64_t)0)])));
#line 180 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_collapse_messages.cpp"
  _Z3foov();
#line 182 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_collapse_messages.cpp"
  _Z25omp_for_pragma_talkad7420Pc(((&_OC_str_OC_10.array[((int64_t)0)])));
#line 183 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_collapse_messages.cpp"
  _Z3foov();
#line 184 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_collapse_messages.cpp"
  _135 = _7;
  _136 = _8;
  _137 = _Z5tmainIicLi1ELi0EET_S0_PPT0_(_135, _136);
  return _137;
}


uint32_t _Z5tmainIicLi1ELi0EET_S0_PPT0_(uint32_t _174, uint8_t** _175) {
  uint32_t _176;    /* Address-exposed local */
  uint8_t** _177;    /* Address-exposed local */
  uint32_t _178;    /* Address-exposed local */
  uint32_t _179;    /* Address-exposed local */
  uint32_t _180;    /* Address-exposed local */
  uint32_t _181;    /* Address-exposed local */
  uint32_t _182;    /* Address-exposed local */
  uint32_t _183;    /* Address-exposed local */
  uint32_t _184;    /* Address-exposed local */
  uint32_t _185;    /* Address-exposed local */
  uint32_t _186;    /* Address-exposed local */
  uint32_t _187;    /* Address-exposed local */
  uint32_t _188;    /* Address-exposed local */
  uint32_t _189;    /* Address-exposed local */
  uint32_t _190;
  uint8_t** _191;
  uint8_t* _192;
  uint32_t _193;
  uint8_t _194;
  uint8_t** _195;
  uint8_t* _196;
  uint32_t _197;
  uint8_t _198;
  uint8_t** _199;
  uint8_t* _200;
  uint32_t _201;
  uint32_t _202;
  uint32_t _203;
  uint8_t** _204;
  uint8_t* _205;
  uint32_t _206;
  uint8_t _207;
  uint8_t** _208;
  uint8_t* _209;
  uint32_t _210;
  uint8_t _211;
  uint8_t** _212;
  uint8_t* _213;
  uint32_t _214;
  uint32_t _215;
  uint32_t _216;
  uint8_t** _217;
  uint8_t* _218;
  uint32_t _219;
  uint8_t _220;
  uint8_t** _221;
  uint8_t* _222;
  uint32_t _223;
  uint8_t _224;
  uint8_t** _225;
  uint8_t* _226;
  uint32_t _227;
  uint32_t _228;
  uint32_t _229;
  uint8_t** _230;
  uint8_t* _231;
  uint32_t _232;
  uint8_t _233;
  uint8_t** _234;
  uint8_t* _235;
  uint32_t _236;
  uint8_t _237;
  uint8_t** _238;
  uint8_t* _239;
  uint32_t _240;
  uint32_t _241;
  uint32_t _242;
  uint8_t** _243;
  uint8_t* _244;
  uint32_t _245;
  uint8_t _246;
  uint8_t** _247;
  uint8_t* _248;
  uint32_t _249;
  uint8_t _250;
  uint8_t** _251;
  uint8_t* _252;
  uint32_t _253;
  uint32_t _254;
  uint32_t _255;
  uint8_t** _256;
  uint8_t* _257;
  uint32_t _258;
  uint8_t _259;
  uint8_t** _260;
  uint8_t* _261;
  uint32_t _262;
  uint8_t _263;
  uint8_t** _264;
  uint8_t* _265;
  uint32_t _266;
  uint32_t _267;
  uint32_t _268;
  uint8_t** _269;
  uint8_t* _270;
  uint32_t _271;
  uint8_t _272;
  uint8_t** _273;
  uint8_t* _274;
  uint32_t _275;
  uint8_t _276;
  uint8_t** _277;
  uint8_t* _278;
  uint32_t _279;
  uint32_t _280;
  uint32_t _281;
  uint8_t** _282;
  uint8_t* _283;
  uint32_t _284;
  uint8_t _285;
  uint8_t** _286;
  uint8_t* _287;
  uint32_t _288;
  uint8_t _289;
  uint8_t** _290;
  uint8_t* _291;
  uint32_t _292;
  uint32_t _293;
  uint32_t _294;
  uint8_t** _295;
  uint8_t* _296;
  uint32_t _297;
  uint8_t _298;
  uint8_t** _299;
  uint8_t* _300;
  uint32_t _301;
  uint8_t _302;
  uint8_t** _303;
  uint8_t* _304;
  uint32_t _305;
  uint32_t _306;
  uint32_t _307;
  uint8_t** _308;
  uint8_t* _309;
  uint32_t _310;
  uint8_t _311;
  uint8_t** _312;
  uint8_t* _313;
  uint32_t _314;
  uint8_t _315;
  uint8_t** _316;
  uint8_t* _317;
  uint32_t _318;
  uint32_t _319;
  uint32_t _320;
  uint8_t** _321;
  uint8_t* _322;
  uint32_t _323;
  uint8_t _324;
  uint8_t** _325;
  uint8_t* _326;
  uint32_t _327;
  uint8_t _328;
  uint8_t** _329;
  uint8_t* _330;
  uint32_t _331;
  uint32_t _332;
  uint32_t _333;
  uint8_t** _334;
  uint8_t* _335;
  uint32_t _336;
  uint8_t _337;
  uint8_t** _338;
  uint8_t* _339;
  uint32_t _340;
  uint8_t _341;
  uint8_t** _342;
  uint8_t* _343;
  uint32_t _344;
  uint32_t _345;
  uint32_t _346;

  _176 = _174;
#line 13 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_collapse_messages.cpp"
  ;
  _177 = _175;
  ;
#line 15 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_collapse_messages.cpp"
  _Z25omp_for_pragma_talkad7420Pc(((&_OC_str.array[((int64_t)0)])));
#line 16 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_collapse_messages.cpp"
  _Z19for_loop_talkad7420v();
#line 17 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_collapse_messages.cpp"
  ;
  _178 = 0;
  goto _347;

  do {     /* Syntactic loop '' to make GCC happy */
_347:
  _190 = _178;
  if ((((((int32_t)_190) < ((int32_t)1u))&1))) {
    goto _348;
  } else {
    goto _349;
  }

_348:
#line 19 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_collapse_messages.cpp"
  _191 = _177;
  _192 = *((&(*_191)));
  _193 = _178;
  _194 = *((&_192[((int64_t)(((int64_t)(int32_t)_193)))]));
  _195 = _177;
  _196 = *((&(*_195)));
  _197 = _178;
  _198 = *((&_196[((int64_t)(((int64_t)(int32_t)(llvm_sub_u32(_197, 0)))))]));
  _199 = _177;
  _200 = *((&(*_199)));
  _201 = _178;
  *((&_200[((int64_t)(((int64_t)(int32_t)_201)))])) = (((uint8_t)(llvm_sub_u32((((int32_t)(int8_t)_194)), (((int32_t)(int8_t)_198))))));
  goto _350;

_350:
#line 17 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_collapse_messages.cpp"
  _202 = _178;
  _178 = (llvm_add_u32(_202, 1));
  goto _347;

  } while (1); /* end of syntactic loop '' */
_349:
#line 22 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_collapse_messages.cpp"
  _Z25omp_for_pragma_talkad7420Pc(((&_OC_str_OC_1.array[((int64_t)0)])));
#line 23 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_collapse_messages.cpp"
  _Z19for_loop_talkad7420v();
#line 24 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_collapse_messages.cpp"
  ;
  _179 = 0;
  goto _351;

  do {     /* Syntactic loop '' to make GCC happy */
_351:
  _203 = _179;
  if ((((((int32_t)_203) < ((int32_t)1u))&1))) {
    goto _352;
  } else {
    goto _353;
  }

_352:
#line 26 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_collapse_messages.cpp"
  _204 = _177;
  _205 = *((&(*_204)));
  _206 = _179;
  _207 = *((&_205[((int64_t)(((int64_t)(int32_t)_206)))]));
  _208 = _177;
  _209 = *((&(*_208)));
  _210 = _179;
  _211 = *((&_209[((int64_t)(((int64_t)(int32_t)(llvm_sub_u32(_210, 0)))))]));
  _212 = _177;
  _213 = *((&(*_212)));
  _214 = _179;
  *((&_213[((int64_t)(((int64_t)(int32_t)_214)))])) = (((uint8_t)(llvm_sub_u32((((int32_t)(int8_t)_207)), (((int32_t)(int8_t)_211))))));
  goto _354;

_354:
#line 24 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_collapse_messages.cpp"
  _215 = _179;
  _179 = (llvm_add_u32(_215, 1));
  goto _351;

  } while (1); /* end of syntactic loop '' */
_353:
#line 29 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_collapse_messages.cpp"
  _Z25omp_for_pragma_talkad7420Pc(((&_OC_str_OC_2.array[((int64_t)0)])));
#line 30 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_collapse_messages.cpp"
  _Z19for_loop_talkad7420v();
#line 31 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_collapse_messages.cpp"
  ;
  _180 = 0;
  goto _355;

  do {     /* Syntactic loop '' to make GCC happy */
_355:
  _216 = _180;
  if ((((((int32_t)_216) < ((int32_t)1u))&1))) {
    goto _356;
  } else {
    goto _357;
  }

_356:
#line 33 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_collapse_messages.cpp"
  _217 = _177;
  _218 = *((&(*_217)));
  _219 = _180;
  _220 = *((&_218[((int64_t)(((int64_t)(int32_t)_219)))]));
  _221 = _177;
  _222 = *((&(*_221)));
  _223 = _180;
  _224 = *((&_222[((int64_t)(((int64_t)(int32_t)(llvm_sub_u32(_223, 0)))))]));
  _225 = _177;
  _226 = *((&(*_225)));
  _227 = _180;
  *((&_226[((int64_t)(((int64_t)(int32_t)_227)))])) = (((uint8_t)(llvm_sub_u32((((int32_t)(int8_t)_220)), (((int32_t)(int8_t)_224))))));
  goto _358;

_358:
#line 31 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_collapse_messages.cpp"
  _228 = _180;
  _180 = (llvm_add_u32(_228, 1));
  goto _355;

  } while (1); /* end of syntactic loop '' */
_357:
#line 36 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_collapse_messages.cpp"
  _Z25omp_for_pragma_talkad7420Pc(((&_OC_str_OC_11.array[((int64_t)0)])));
#line 37 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_collapse_messages.cpp"
  _Z19for_loop_talkad7420v();
#line 38 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_collapse_messages.cpp"
  ;
  _181 = 0;
  goto _359;

  do {     /* Syntactic loop '' to make GCC happy */
_359:
  _229 = _181;
  if ((((((int32_t)_229) < ((int32_t)1u))&1))) {
    goto _360;
  } else {
    goto _361;
  }

_360:
#line 40 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_collapse_messages.cpp"
  _230 = _177;
  _231 = *((&(*_230)));
  _232 = _181;
  _233 = *((&_231[((int64_t)(((int64_t)(int32_t)_232)))]));
  _234 = _177;
  _235 = *((&(*_234)));
  _236 = _181;
  _237 = *((&_235[((int64_t)(((int64_t)(int32_t)(llvm_sub_u32(_236, 0)))))]));
  _238 = _177;
  _239 = *((&(*_238)));
  _240 = _181;
  *((&_239[((int64_t)(((int64_t)(int32_t)_240)))])) = (((uint8_t)(llvm_sub_u32((((int32_t)(int8_t)_233)), (((int32_t)(int8_t)_237))))));
  goto _362;

_362:
#line 38 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_collapse_messages.cpp"
  _241 = _181;
  _181 = (llvm_add_u32(_241, 1));
  goto _359;

  } while (1); /* end of syntactic loop '' */
_361:
#line 43 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_collapse_messages.cpp"
  _Z25omp_for_pragma_talkad7420Pc(((&_OC_str_OC_12.array[((int64_t)0)])));
#line 44 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_collapse_messages.cpp"
  _Z19for_loop_talkad7420v();
#line 45 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_collapse_messages.cpp"
  ;
  _182 = 0;
  goto _363;

  do {     /* Syntactic loop '' to make GCC happy */
_363:
  _242 = _182;
  if ((((((int32_t)_242) < ((int32_t)1u))&1))) {
    goto _364;
  } else {
    goto _365;
  }

_364:
#line 47 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_collapse_messages.cpp"
  _243 = _177;
  _244 = *((&(*_243)));
  _245 = _182;
  _246 = *((&_244[((int64_t)(((int64_t)(int32_t)_245)))]));
  _247 = _177;
  _248 = *((&(*_247)));
  _249 = _182;
  _250 = *((&_248[((int64_t)(((int64_t)(int32_t)(llvm_sub_u32(_249, 0)))))]));
  _251 = _177;
  _252 = *((&(*_251)));
  _253 = _182;
  *((&_252[((int64_t)(((int64_t)(int32_t)_253)))])) = (((uint8_t)(llvm_sub_u32((((int32_t)(int8_t)_246)), (((int32_t)(int8_t)_250))))));
  goto _366;

_366:
#line 45 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_collapse_messages.cpp"
  _254 = _182;
  _182 = (llvm_add_u32(_254, 1));
  goto _363;

  } while (1); /* end of syntactic loop '' */
_365:
#line 50 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_collapse_messages.cpp"
  _Z25omp_for_pragma_talkad7420Pc(((&_OC_str_OC_13.array[((int64_t)0)])));
#line 51 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_collapse_messages.cpp"
  _Z19for_loop_talkad7420v();
#line 52 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_collapse_messages.cpp"
  ;
  _183 = 0;
  goto _367;

  do {     /* Syntactic loop '' to make GCC happy */
_367:
  _255 = _183;
  if ((((((int32_t)_255) < ((int32_t)1u))&1))) {
    goto _368;
  } else {
    goto _369;
  }

_368:
#line 54 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_collapse_messages.cpp"
  _256 = _177;
  _257 = *((&(*_256)));
  _258 = _183;
  _259 = *((&_257[((int64_t)(((int64_t)(int32_t)_258)))]));
  _260 = _177;
  _261 = *((&(*_260)));
  _262 = _183;
  _263 = *((&_261[((int64_t)(((int64_t)(int32_t)(llvm_sub_u32(_262, 0)))))]));
  _264 = _177;
  _265 = *((&(*_264)));
  _266 = _183;
  *((&_265[((int64_t)(((int64_t)(int32_t)_266)))])) = (((uint8_t)(llvm_sub_u32((((int32_t)(int8_t)_259)), (((int32_t)(int8_t)_263))))));
  goto _370;

_370:
#line 52 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_collapse_messages.cpp"
  _267 = _183;
  _183 = (llvm_add_u32(_267, 1));
  goto _367;

  } while (1); /* end of syntactic loop '' */
_369:
#line 57 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_collapse_messages.cpp"
  _Z25omp_for_pragma_talkad7420Pc(((&_OC_str_OC_14.array[((int64_t)0)])));
#line 58 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_collapse_messages.cpp"
  _Z19for_loop_talkad7420v();
#line 59 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_collapse_messages.cpp"
  ;
  _184 = 0;
  goto _371;

  do {     /* Syntactic loop '' to make GCC happy */
_371:
  _268 = _184;
  if ((((((int32_t)_268) < ((int32_t)1u))&1))) {
    goto _372;
  } else {
    goto _373;
  }

_372:
#line 61 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_collapse_messages.cpp"
  _269 = _177;
  _270 = *((&(*_269)));
  _271 = _184;
  _272 = *((&_270[((int64_t)(((int64_t)(int32_t)_271)))]));
  _273 = _177;
  _274 = *((&(*_273)));
  _275 = _184;
  _276 = *((&_274[((int64_t)(((int64_t)(int32_t)(llvm_sub_u32(_275, 0)))))]));
  _277 = _177;
  _278 = *((&(*_277)));
  _279 = _184;
  *((&_278[((int64_t)(((int64_t)(int32_t)_279)))])) = (((uint8_t)(llvm_sub_u32((((int32_t)(int8_t)_272)), (((int32_t)(int8_t)_276))))));
  goto _374;

_374:
#line 59 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_collapse_messages.cpp"
  _280 = _184;
  _184 = (llvm_add_u32(_280, 1));
  goto _371;

  } while (1); /* end of syntactic loop '' */
_373:
#line 66 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_collapse_messages.cpp"
  _Z25omp_for_pragma_talkad7420Pc(((&_OC_str_OC_15.array[((int64_t)0)])));
#line 67 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_collapse_messages.cpp"
  _Z19for_loop_talkad7420v();
#line 68 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_collapse_messages.cpp"
  ;
  _185 = 0;
  goto _375;

  do {     /* Syntactic loop '' to make GCC happy */
_375:
  _281 = _185;
  if ((((((int32_t)_281) < ((int32_t)1u))&1))) {
    goto _376;
  } else {
    goto _377;
  }

_376:
#line 70 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_collapse_messages.cpp"
  _282 = _177;
  _283 = *((&(*_282)));
  _284 = _185;
  _285 = *((&_283[((int64_t)(((int64_t)(int32_t)_284)))]));
  _286 = _177;
  _287 = *((&(*_286)));
  _288 = _185;
  _289 = *((&_287[((int64_t)(((int64_t)(int32_t)(llvm_sub_u32(_288, 0)))))]));
  _290 = _177;
  _291 = *((&(*_290)));
  _292 = _185;
  *((&_291[((int64_t)(((int64_t)(int32_t)_292)))])) = (((uint8_t)(llvm_sub_u32((((int32_t)(int8_t)_285)), (((int32_t)(int8_t)_289))))));
  goto _378;

_378:
#line 68 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_collapse_messages.cpp"
  _293 = _185;
  _185 = (llvm_add_u32(_293, 1));
  goto _375;

  } while (1); /* end of syntactic loop '' */
_377:
#line 73 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_collapse_messages.cpp"
  _Z19for_loop_talkad7420v();
#line 74 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_collapse_messages.cpp"
  ;
  _186 = 0;
  goto _379;

  do {     /* Syntactic loop '' to make GCC happy */
_379:
  _294 = _186;
  if ((((((int32_t)_294) < ((int32_t)1u))&1))) {
    goto _380;
  } else {
    goto _381;
  }

_380:
#line 76 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_collapse_messages.cpp"
  _295 = _177;
  _296 = *((&(*_295)));
  _297 = _186;
  _298 = *((&_296[((int64_t)(((int64_t)(int32_t)_297)))]));
  _299 = _177;
  _300 = *((&(*_299)));
  _301 = _186;
  _302 = *((&_300[((int64_t)(((int64_t)(int32_t)(llvm_sub_u32(_301, 0)))))]));
  _303 = _177;
  _304 = *((&(*_303)));
  _305 = _186;
  *((&_304[((int64_t)(((int64_t)(int32_t)_305)))])) = (((uint8_t)(llvm_sub_u32((((int32_t)(int8_t)_298)), (((int32_t)(int8_t)_302))))));
  goto _382;

_382:
#line 74 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_collapse_messages.cpp"
  _306 = _186;
  _186 = (llvm_add_u32(_306, 1));
  goto _379;

  } while (1); /* end of syntactic loop '' */
_381:
#line 82 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_collapse_messages.cpp"
  _Z25omp_for_pragma_talkad7420Pc(((&_OC_str_OC_8.array[((int64_t)0)])));
#line 83 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_collapse_messages.cpp"
  _Z19for_loop_talkad7420v();
#line 84 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_collapse_messages.cpp"
  ;
  _187 = 0;
  goto _383;

  do {     /* Syntactic loop '' to make GCC happy */
_383:
  _307 = _187;
  if ((((((int32_t)_307) < ((int32_t)1u))&1))) {
    goto _384;
  } else {
    goto _385;
  }

_384:
#line 86 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_collapse_messages.cpp"
  _308 = _177;
  _309 = *((&(*_308)));
  _310 = _187;
  _311 = *((&_309[((int64_t)(((int64_t)(int32_t)_310)))]));
  _312 = _177;
  _313 = *((&(*_312)));
  _314 = _187;
  _315 = *((&_313[((int64_t)(((int64_t)(int32_t)(llvm_sub_u32(_314, 0)))))]));
  _316 = _177;
  _317 = *((&(*_316)));
  _318 = _187;
  *((&_317[((int64_t)(((int64_t)(int32_t)_318)))])) = (((uint8_t)(llvm_sub_u32((((int32_t)(int8_t)_311)), (((int32_t)(int8_t)_315))))));
  goto _386;

_386:
#line 84 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_collapse_messages.cpp"
  _319 = _187;
  _187 = (llvm_add_u32(_319, 1));
  goto _383;

  } while (1); /* end of syntactic loop '' */
_385:
#line 89 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_collapse_messages.cpp"
  _Z25omp_for_pragma_talkad7420Pc(((&_OC_str_OC_16.array[((int64_t)0)])));
#line 90 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_collapse_messages.cpp"
  _Z19for_loop_talkad7420v();
#line 91 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_collapse_messages.cpp"
  ;
  _188 = 0;
  goto _387;

  do {     /* Syntactic loop '' to make GCC happy */
_387:
  _320 = _188;
  if ((((((int32_t)_320) < ((int32_t)1u))&1))) {
    goto _388;
  } else {
    goto _389;
  }

_388:
#line 93 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_collapse_messages.cpp"
  _321 = _177;
  _322 = *((&(*_321)));
  _323 = _188;
  _324 = *((&_322[((int64_t)(((int64_t)(int32_t)_323)))]));
  _325 = _177;
  _326 = *((&(*_325)));
  _327 = _188;
  _328 = *((&_326[((int64_t)(((int64_t)(int32_t)(llvm_sub_u32(_327, 0)))))]));
  _329 = _177;
  _330 = *((&(*_329)));
  _331 = _188;
  *((&_330[((int64_t)(((int64_t)(int32_t)_331)))])) = (((uint8_t)(llvm_sub_u32((((int32_t)(int8_t)_324)), (((int32_t)(int8_t)_328))))));
  goto _390;

_390:
#line 91 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_collapse_messages.cpp"
  _332 = _188;
  _188 = (llvm_add_u32(_332, 1));
  goto _387;

  } while (1); /* end of syntactic loop '' */
_389:
#line 96 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_collapse_messages.cpp"
  _Z25omp_for_pragma_talkad7420Pc(((&_OC_str_OC_17.array[((int64_t)0)])));
#line 97 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_collapse_messages.cpp"
  _Z19for_loop_talkad7420v();
#line 98 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_collapse_messages.cpp"
  ;
  _189 = 0;
  goto _391;

  do {     /* Syntactic loop '' to make GCC happy */
_391:
  _333 = _189;
  if ((((((int32_t)_333) < ((int32_t)1u))&1))) {
    goto _392;
  } else {
    goto _393;
  }

_392:
#line 100 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_collapse_messages.cpp"
  _334 = _177;
  _335 = *((&(*_334)));
  _336 = _189;
  _337 = *((&_335[((int64_t)(((int64_t)(int32_t)_336)))]));
  _338 = _177;
  _339 = *((&(*_338)));
  _340 = _189;
  _341 = *((&_339[((int64_t)(((int64_t)(int32_t)(llvm_sub_u32(_340, 0)))))]));
  _342 = _177;
  _343 = *((&(*_342)));
  _344 = _189;
  *((&_343[((int64_t)(((int64_t)(int32_t)_344)))])) = (((uint8_t)(llvm_sub_u32((((int32_t)(int8_t)_337)), (((int32_t)(int8_t)_341))))));
  goto _394;

_394:
#line 98 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_collapse_messages.cpp"
  _345 = _189;
  _189 = (llvm_add_u32(_345, 1));
  goto _391;

  } while (1); /* end of syntactic loop '' */
_393:
#line 103 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_collapse_messages.cpp"
  _Z25omp_for_pragma_talkad7420Pc(((&_OC_str_OC_10.array[((int64_t)0)])));
#line 104 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_collapse_messages.cpp"
  _Z3foov();
#line 105 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/teams_distribute_parallel_for_collapse_messages.cpp"
  _346 = _176;
  return _346;
}

