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
struct l_array_119_uint8_t;
struct l_array_215_uint8_t;
struct l_array_226_uint8_t;

/* Function definitions */
typedef uint32_t l_fptr_1(uint32_t, uint8_t**);

typedef void l_fptr_3(uint8_t*);

typedef uint8_t l_fptr_6(uint8_t);

typedef void l_fptr_4(void);

typedef uint32_t l_fptr_5(uint32_t);

typedef void l_fptr_2(void);


/* Types Definitions */
struct l_array_119_uint8_t {
  uint8_t array[119];
};
struct l_array_215_uint8_t {
  uint8_t array[215];
};
struct l_array_226_uint8_t {
  uint8_t array[226];
};

/* External Global Variable Declarations */

/* Function Declarations */
int main(int, char **) __ATTRIBUTELIST__((noinline));
void _Z25omp_for_pragma_talkad7420Pc(uint8_t*);
void _Z19for_loop_talkad7420v(void);
uint32_t _Z5tmainIiLi5EET_S0_(uint32_t) __ATTRIBUTELIST__((noinline)) __ATTRIBUTE_WEAK__;
int8_t _Z5tmainIcLi1EET_S0_(int8_t) __ATTRIBUTELIST__((noinline)) __ATTRIBUTE_WEAK__;


/* Global Variable Definitions and Initialization */
static uint32_t _ZZ4mainE1a;
static float _ZZ4mainE1g;
static const struct l_array_119_uint8_t _OC_str = { "#pragma omp distribute parallel for schedule(guided, argc) default(none) copyin(g) dist_schedule(static, a) private(a)" };
static const struct l_array_215_uint8_t _OC_str_OC_1 = { "#pragma omp distribute parallel for private(argc, b), firstprivate(argv, c), lastprivate(d, f) collapse(2) schedule(auto) if (argc) num_threads(a) default(shared) shared(e) reduction(+ : h) dist_schedule(static, b)" };
uint32_t _ZZ5tmainIiLi5EET_S0_E1a __attribute__((common));
uint32_t _ZZ5tmainIiLi5EET_S0_E1g __attribute__((common));
static const struct l_array_119_uint8_t _OC_str_OC_2 = { "#pragma omp distribute parallel for dist_schedule(static, a) schedule(dynamic) default(none) copyin(g) firstprivate(a)" };
static const struct l_array_226_uint8_t _OC_str_OC_3 = { "#pragma omp distribute parallel for private(argc, b), firstprivate(c, d), lastprivate(d, f) collapse(n) schedule(static, n) if (parallel :argc) num_threads(n) default(shared) shared(e) reduction(+ : h) dist_schedule(static,n)" };
uint8_t _ZZ5tmainIcLi1EET_S0_E1a __attribute__((common));
uint8_t _ZZ5tmainIcLi1EET_S0_E1g __attribute__((common));


/* LLVM Intrinsic Builtin Function Bodies */
static __forceinline uint8_t llvm_add_u8(uint8_t a, uint8_t b) {
  uint8_t r = a + b;
  return r;
}
static __forceinline uint32_t llvm_add_u32(uint32_t a, uint32_t b) {
  uint32_t r = a + b;
  return r;
}


/* Function Bodies */

int main(int argc, char ** argv) {
  uint32_t _1 = (uint32_t)argc;
  uint8_t** _2 = (uint8_t**)argv;
  uint32_t _3;    /* Address-exposed local */
  uint32_t _4;    /* Address-exposed local */
  uint8_t** _5;    /* Address-exposed local */
  uint32_t _6;    /* Address-exposed local */
  uint32_t _7;    /* Address-exposed local */
  uint32_t _8;    /* Address-exposed local */
  uint32_t _9;    /* Address-exposed local */
  uint32_t _10;    /* Address-exposed local */
  uint32_t _11;    /* Address-exposed local */
  uint32_t _12;    /* Address-exposed local */
  uint32_t _13;    /* Address-exposed local */
  uint32_t _14;    /* Address-exposed local */
  uint32_t _15;
  uint32_t _16;
  uint32_t _17;
  uint32_t _18;
  uint32_t _19;
  uint32_t _20;
  uint32_t _21;
  uint32_t _22;
  uint32_t _23;
  uint32_t _24;
  uint8_t** _25;
  uint8_t* _26;
  uint8_t _27;
  uint8_t _28;

  _3 = 0;
  _4 = _1;
#line 129 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/distribute_parallel_for_ast_print.cpp"
  ;
  _5 = _2;
  ;
#line 130 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/distribute_parallel_for_ast_print.cpp"
  ;
  _15 = _4;
  _6 = _15;
  ;
  ;
  ;
  ;
  ;
#line 136 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/distribute_parallel_for_ast_print.cpp"
  _Z25omp_for_pragma_talkad7420Pc(((&_OC_str.array[((int64_t)0)])));
#line 137 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/distribute_parallel_for_ast_print.cpp"
  _Z19for_loop_talkad7420v();
#line 138 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/distribute_parallel_for_ast_print.cpp"
  ;
  _12 = 0;
  goto _29;

  do {     /* Syntactic loop '' to make GCC happy */
_29:
  _16 = _12;
  if ((((((int32_t)_16) < ((int32_t)2u))&1))) {
    goto _30;
  } else {
    goto _31;
  }

_30:
#line 140 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/distribute_parallel_for_ast_print.cpp"
  _ZZ4mainE1a = 2;
  goto _32;

_32:
#line 138 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/distribute_parallel_for_ast_print.cpp"
  _17 = _12;
  _12 = (llvm_add_u32(_17, 1));
  goto _29;

  } while (1); /* end of syntactic loop '' */
_31:
#line 144 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/distribute_parallel_for_ast_print.cpp"
  _Z25omp_for_pragma_talkad7420Pc(((&_OC_str_OC_1.array[((int64_t)0)])));
#line 145 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/distribute_parallel_for_ast_print.cpp"
  _Z19for_loop_talkad7420v();
#line 146 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/distribute_parallel_for_ast_print.cpp"
  ;
  _13 = 0;
  goto _33;

  do {     /* Syntactic loop '' to make GCC happy */
_33:
  _18 = _13;
  if ((((((int32_t)_18) < ((int32_t)10u))&1))) {
    goto _34;
  } else {
    goto _35;
  }

_34:
#line 148 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/distribute_parallel_for_ast_print.cpp"
  _Z19for_loop_talkad7420v();
#line 149 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/distribute_parallel_for_ast_print.cpp"
  ;
  _14 = 0;
  goto _36;

  do {     /* Syntactic loop '' to make GCC happy */
_36:
  _19 = _14;
  if ((((((int32_t)_19) < ((int32_t)10u))&1))) {
    goto _37;
  } else {
    goto _38;
  }

_37:
#line 151 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/distribute_parallel_for_ast_print.cpp"
  _20 = _ZZ4mainE1a;
  _ZZ4mainE1a = (llvm_add_u32(_20, 1));
  goto _39;

_39:
#line 149 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/distribute_parallel_for_ast_print.cpp"
  _21 = _14;
  _14 = (llvm_add_u32(_21, 1));
  goto _36;

  } while (1); /* end of syntactic loop '' */
_38:
  goto _40;

_40:
#line 146 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/distribute_parallel_for_ast_print.cpp"
  _22 = _13;
  _13 = (llvm_add_u32(_22, 1));
  goto _33;

  } while (1); /* end of syntactic loop '' */
_35:
#line 154 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/distribute_parallel_for_ast_print.cpp"
  _23 = _4;
  _24 = _Z5tmainIiLi5EET_S0_(_23);
  _25 = _5;
  _26 = *((&(*_25)));
  _27 = *((&(*_26)));
  _28 = _Z5tmainIcLi1EET_S0_(_27);
  return (llvm_add_u32(_24, (((int32_t)(int8_t)_28))));
}


uint32_t _Z5tmainIiLi5EET_S0_(uint32_t _41) {
  uint32_t _42;    /* Address-exposed local */
  uint32_t _43;    /* Address-exposed local */
  uint32_t _44;    /* Address-exposed local */
  uint32_t _45;    /* Address-exposed local */
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
  uint32_t _82;
  uint32_t _83;

  _42 = _41;
#line 70 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/distribute_parallel_for_ast_print.cpp"
  ;
#line 71 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/distribute_parallel_for_ast_print.cpp"
  ;
  _60 = _42;
  _43 = _60;
  ;
  ;
  ;
  ;
  ;
#line 77 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/distribute_parallel_for_ast_print.cpp"
  _Z25omp_for_pragma_talkad7420Pc(((&_OC_str_OC_2.array[((int64_t)0)])));
#line 78 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/distribute_parallel_for_ast_print.cpp"
  _Z19for_loop_talkad7420v();
#line 79 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/distribute_parallel_for_ast_print.cpp"
  ;
  _49 = 0;
  goto _84;

  do {     /* Syntactic loop '' to make GCC happy */
_84:
  _61 = _49;
  if ((((((int32_t)_61) < ((int32_t)2u))&1))) {
    goto _85;
  } else {
    goto _86;
  }

_85:
#line 81 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/distribute_parallel_for_ast_print.cpp"
  _ZZ5tmainIiLi5EET_S0_E1a = 2;
  goto _87;

_87:
#line 79 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/distribute_parallel_for_ast_print.cpp"
  _62 = _49;
  _49 = (llvm_add_u32(_62, 1));
  goto _84;

  } while (1); /* end of syntactic loop '' */
_86:
#line 85 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/distribute_parallel_for_ast_print.cpp"
  _Z25omp_for_pragma_talkad7420Pc(((&_OC_str_OC_3.array[((int64_t)0)])));
#line 86 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/distribute_parallel_for_ast_print.cpp"
  _Z19for_loop_talkad7420v();
#line 87 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/distribute_parallel_for_ast_print.cpp"
  ;
  _50 = 0;
  goto _88;

  do {     /* Syntactic loop '' to make GCC happy */
_88:
  _63 = _50;
  if ((((((int32_t)_63) < ((int32_t)2u))&1))) {
    goto _89;
  } else {
    goto _90;
  }

_89:
#line 89 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/distribute_parallel_for_ast_print.cpp"
  _Z19for_loop_talkad7420v();
#line 90 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/distribute_parallel_for_ast_print.cpp"
  ;
  _51 = 0;
  goto _91;

  do {     /* Syntactic loop '' to make GCC happy */
_91:
  _64 = _51;
  if ((((((int32_t)_64) < ((int32_t)2u))&1))) {
    goto _92;
  } else {
    goto _93;
  }

_92:
#line 92 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/distribute_parallel_for_ast_print.cpp"
  _Z19for_loop_talkad7420v();
#line 93 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/distribute_parallel_for_ast_print.cpp"
  ;
  _52 = 0;
  goto _94;

  do {     /* Syntactic loop '' to make GCC happy */
_94:
  _65 = _52;
  if ((((((int32_t)_65) < ((int32_t)2u))&1))) {
    goto _95;
  } else {
    goto _96;
  }

_95:
#line 95 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/distribute_parallel_for_ast_print.cpp"
  _Z19for_loop_talkad7420v();
#line 96 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/distribute_parallel_for_ast_print.cpp"
  ;
  _53 = 0;
  goto _97;

  do {     /* Syntactic loop '' to make GCC happy */
_97:
  _66 = _53;
  if ((((((int32_t)_66) < ((int32_t)2u))&1))) {
    goto _98;
  } else {
    goto _99;
  }

_98:
#line 98 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/distribute_parallel_for_ast_print.cpp"
  _Z19for_loop_talkad7420v();
#line 99 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/distribute_parallel_for_ast_print.cpp"
  ;
  _54 = 0;
  goto _100;

  do {     /* Syntactic loop '' to make GCC happy */
_100:
  _67 = _54;
  if ((((((int32_t)_67) < ((int32_t)2u))&1))) {
    goto _101;
  } else {
    goto _102;
  }

_101:
#line 101 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/distribute_parallel_for_ast_print.cpp"
  _Z19for_loop_talkad7420v();
#line 102 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/distribute_parallel_for_ast_print.cpp"
  ;
  _55 = 0;
  goto _103;

  do {     /* Syntactic loop '' to make GCC happy */
_103:
  _68 = _55;
  if ((((((int32_t)_68) < ((int32_t)2u))&1))) {
    goto _104;
  } else {
    goto _105;
  }

_104:
#line 104 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/distribute_parallel_for_ast_print.cpp"
  _Z19for_loop_talkad7420v();
#line 105 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/distribute_parallel_for_ast_print.cpp"
  ;
  _56 = 0;
  goto _106;

  do {     /* Syntactic loop '' to make GCC happy */
_106:
  _69 = _56;
  if ((((((int32_t)_69) < ((int32_t)2u))&1))) {
    goto _107;
  } else {
    goto _108;
  }

_107:
#line 107 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/distribute_parallel_for_ast_print.cpp"
  _Z19for_loop_talkad7420v();
#line 108 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/distribute_parallel_for_ast_print.cpp"
  ;
  _57 = 0;
  goto _109;

  do {     /* Syntactic loop '' to make GCC happy */
_109:
  _70 = _57;
  if ((((((int32_t)_70) < ((int32_t)2u))&1))) {
    goto _110;
  } else {
    goto _111;
  }

_110:
#line 110 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/distribute_parallel_for_ast_print.cpp"
  _Z19for_loop_talkad7420v();
#line 111 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/distribute_parallel_for_ast_print.cpp"
  ;
  _58 = 0;
  goto _112;

  do {     /* Syntactic loop '' to make GCC happy */
_112:
  _71 = _58;
  if ((((((int32_t)_71) < ((int32_t)2u))&1))) {
    goto _113;
  } else {
    goto _114;
  }

_113:
#line 113 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/distribute_parallel_for_ast_print.cpp"
  _Z19for_loop_talkad7420v();
#line 114 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/distribute_parallel_for_ast_print.cpp"
  ;
  _59 = 0;
  goto _115;

  do {     /* Syntactic loop '' to make GCC happy */
_115:
  _72 = _59;
  if ((((((int32_t)_72) < ((int32_t)2u))&1))) {
    goto _116;
  } else {
    goto _117;
  }

_116:
#line 116 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/distribute_parallel_for_ast_print.cpp"
  _73 = _ZZ5tmainIiLi5EET_S0_E1a;
  _ZZ5tmainIiLi5EET_S0_E1a = (llvm_add_u32(_73, 1));
  goto _118;

_118:
#line 114 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/distribute_parallel_for_ast_print.cpp"
  _74 = _59;
  _59 = (llvm_add_u32(_74, 1));
  goto _115;

  } while (1); /* end of syntactic loop '' */
_117:
  goto _119;

_119:
#line 111 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/distribute_parallel_for_ast_print.cpp"
  _75 = _58;
  _58 = (llvm_add_u32(_75, 1));
  goto _112;

  } while (1); /* end of syntactic loop '' */
_114:
  goto _120;

_120:
#line 108 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/distribute_parallel_for_ast_print.cpp"
  _76 = _57;
  _57 = (llvm_add_u32(_76, 1));
  goto _109;

  } while (1); /* end of syntactic loop '' */
_111:
  goto _121;

_121:
#line 105 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/distribute_parallel_for_ast_print.cpp"
  _77 = _56;
  _56 = (llvm_add_u32(_77, 1));
  goto _106;

  } while (1); /* end of syntactic loop '' */
_108:
  goto _122;

_122:
#line 102 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/distribute_parallel_for_ast_print.cpp"
  _78 = _55;
  _55 = (llvm_add_u32(_78, 1));
  goto _103;

  } while (1); /* end of syntactic loop '' */
_105:
  goto _123;

_123:
#line 99 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/distribute_parallel_for_ast_print.cpp"
  _79 = _54;
  _54 = (llvm_add_u32(_79, 1));
  goto _100;

  } while (1); /* end of syntactic loop '' */
_102:
  goto _124;

_124:
#line 96 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/distribute_parallel_for_ast_print.cpp"
  _80 = _53;
  _53 = (llvm_add_u32(_80, 1));
  goto _97;

  } while (1); /* end of syntactic loop '' */
_99:
  goto _125;

_125:
#line 93 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/distribute_parallel_for_ast_print.cpp"
  _81 = _52;
  _52 = (llvm_add_u32(_81, 1));
  goto _94;

  } while (1); /* end of syntactic loop '' */
_96:
  goto _126;

_126:
#line 90 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/distribute_parallel_for_ast_print.cpp"
  _82 = _51;
  _51 = (llvm_add_u32(_82, 1));
  goto _91;

  } while (1); /* end of syntactic loop '' */
_93:
  goto _127;

_127:
#line 87 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/distribute_parallel_for_ast_print.cpp"
  _83 = _50;
  _50 = (llvm_add_u32(_83, 1));
  goto _88;

  } while (1); /* end of syntactic loop '' */
_90:
  return 0;
}


int8_t _Z5tmainIcLi1EET_S0_(int8_t _128) {
  uint8_t _129;    /* Address-exposed local */
  uint8_t _130;    /* Address-exposed local */
  uint8_t _131;    /* Address-exposed local */
  uint8_t _132;    /* Address-exposed local */
  uint8_t _133;    /* Address-exposed local */
  uint8_t _134;    /* Address-exposed local */
  uint8_t _135;    /* Address-exposed local */
  uint32_t _136;    /* Address-exposed local */
  uint32_t _137;    /* Address-exposed local */
  uint32_t _138;    /* Address-exposed local */
  uint32_t _139;    /* Address-exposed local */
  uint32_t _140;    /* Address-exposed local */
  uint32_t _141;    /* Address-exposed local */
  uint32_t _142;    /* Address-exposed local */
  uint32_t _143;    /* Address-exposed local */
  uint32_t _144;    /* Address-exposed local */
  uint32_t _145;    /* Address-exposed local */
  uint32_t _146;    /* Address-exposed local */
  uint8_t _147;
  uint32_t _148;
  uint32_t _149;
  uint32_t _150;
  uint32_t _151;
  uint32_t _152;
  uint32_t _153;
  uint32_t _154;
  uint32_t _155;
  uint32_t _156;
  uint32_t _157;
  uint32_t _158;
  uint32_t _159;
  uint8_t _160;
  uint32_t _161;
  uint32_t _162;
  uint32_t _163;
  uint32_t _164;
  uint32_t _165;
  uint32_t _166;
  uint32_t _167;
  uint32_t _168;
  uint32_t _169;
  uint32_t _170;

  _129 = _128;
#line 70 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/distribute_parallel_for_ast_print.cpp"
  ;
#line 71 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/distribute_parallel_for_ast_print.cpp"
  ;
  _147 = _129;
  _130 = _147;
  ;
  ;
  ;
  ;
  ;
#line 77 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/distribute_parallel_for_ast_print.cpp"
  _Z25omp_for_pragma_talkad7420Pc(((&_OC_str_OC_2.array[((int64_t)0)])));
#line 78 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/distribute_parallel_for_ast_print.cpp"
  _Z19for_loop_talkad7420v();
#line 79 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/distribute_parallel_for_ast_print.cpp"
  ;
  _136 = 0;
  goto _171;

  do {     /* Syntactic loop '' to make GCC happy */
_171:
  _148 = _136;
  if ((((((int32_t)_148) < ((int32_t)2u))&1))) {
    goto _172;
  } else {
    goto _173;
  }

_172:
#line 81 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/distribute_parallel_for_ast_print.cpp"
  _ZZ5tmainIcLi1EET_S0_E1a = 2;
  goto _174;

_174:
#line 79 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/distribute_parallel_for_ast_print.cpp"
  _149 = _136;
  _136 = (llvm_add_u32(_149, 1));
  goto _171;

  } while (1); /* end of syntactic loop '' */
_173:
#line 85 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/distribute_parallel_for_ast_print.cpp"
  _Z25omp_for_pragma_talkad7420Pc(((&_OC_str_OC_3.array[((int64_t)0)])));
#line 86 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/distribute_parallel_for_ast_print.cpp"
  _Z19for_loop_talkad7420v();
#line 87 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/distribute_parallel_for_ast_print.cpp"
  ;
  _137 = 0;
  goto _175;

  do {     /* Syntactic loop '' to make GCC happy */
_175:
  _150 = _137;
  if ((((((int32_t)_150) < ((int32_t)2u))&1))) {
    goto _176;
  } else {
    goto _177;
  }

_176:
#line 89 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/distribute_parallel_for_ast_print.cpp"
  _Z19for_loop_talkad7420v();
#line 90 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/distribute_parallel_for_ast_print.cpp"
  ;
  _138 = 0;
  goto _178;

  do {     /* Syntactic loop '' to make GCC happy */
_178:
  _151 = _138;
  if ((((((int32_t)_151) < ((int32_t)2u))&1))) {
    goto _179;
  } else {
    goto _180;
  }

_179:
#line 92 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/distribute_parallel_for_ast_print.cpp"
  _Z19for_loop_talkad7420v();
#line 93 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/distribute_parallel_for_ast_print.cpp"
  ;
  _139 = 0;
  goto _181;

  do {     /* Syntactic loop '' to make GCC happy */
_181:
  _152 = _139;
  if ((((((int32_t)_152) < ((int32_t)2u))&1))) {
    goto _182;
  } else {
    goto _183;
  }

_182:
#line 95 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/distribute_parallel_for_ast_print.cpp"
  _Z19for_loop_talkad7420v();
#line 96 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/distribute_parallel_for_ast_print.cpp"
  ;
  _140 = 0;
  goto _184;

  do {     /* Syntactic loop '' to make GCC happy */
_184:
  _153 = _140;
  if ((((((int32_t)_153) < ((int32_t)2u))&1))) {
    goto _185;
  } else {
    goto _186;
  }

_185:
#line 98 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/distribute_parallel_for_ast_print.cpp"
  _Z19for_loop_talkad7420v();
#line 99 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/distribute_parallel_for_ast_print.cpp"
  ;
  _141 = 0;
  goto _187;

  do {     /* Syntactic loop '' to make GCC happy */
_187:
  _154 = _141;
  if ((((((int32_t)_154) < ((int32_t)2u))&1))) {
    goto _188;
  } else {
    goto _189;
  }

_188:
#line 101 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/distribute_parallel_for_ast_print.cpp"
  _Z19for_loop_talkad7420v();
#line 102 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/distribute_parallel_for_ast_print.cpp"
  ;
  _142 = 0;
  goto _190;

  do {     /* Syntactic loop '' to make GCC happy */
_190:
  _155 = _142;
  if ((((((int32_t)_155) < ((int32_t)2u))&1))) {
    goto _191;
  } else {
    goto _192;
  }

_191:
#line 104 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/distribute_parallel_for_ast_print.cpp"
  _Z19for_loop_talkad7420v();
#line 105 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/distribute_parallel_for_ast_print.cpp"
  ;
  _143 = 0;
  goto _193;

  do {     /* Syntactic loop '' to make GCC happy */
_193:
  _156 = _143;
  if ((((((int32_t)_156) < ((int32_t)2u))&1))) {
    goto _194;
  } else {
    goto _195;
  }

_194:
#line 107 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/distribute_parallel_for_ast_print.cpp"
  _Z19for_loop_talkad7420v();
#line 108 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/distribute_parallel_for_ast_print.cpp"
  ;
  _144 = 0;
  goto _196;

  do {     /* Syntactic loop '' to make GCC happy */
_196:
  _157 = _144;
  if ((((((int32_t)_157) < ((int32_t)2u))&1))) {
    goto _197;
  } else {
    goto _198;
  }

_197:
#line 110 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/distribute_parallel_for_ast_print.cpp"
  _Z19for_loop_talkad7420v();
#line 111 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/distribute_parallel_for_ast_print.cpp"
  ;
  _145 = 0;
  goto _199;

  do {     /* Syntactic loop '' to make GCC happy */
_199:
  _158 = _145;
  if ((((((int32_t)_158) < ((int32_t)2u))&1))) {
    goto _200;
  } else {
    goto _201;
  }

_200:
#line 113 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/distribute_parallel_for_ast_print.cpp"
  _Z19for_loop_talkad7420v();
#line 114 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/distribute_parallel_for_ast_print.cpp"
  ;
  _146 = 0;
  goto _202;

  do {     /* Syntactic loop '' to make GCC happy */
_202:
  _159 = _146;
  if ((((((int32_t)_159) < ((int32_t)2u))&1))) {
    goto _203;
  } else {
    goto _204;
  }

_203:
#line 116 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/distribute_parallel_for_ast_print.cpp"
  _160 = _ZZ5tmainIcLi1EET_S0_E1a;
  _ZZ5tmainIcLi1EET_S0_E1a = (llvm_add_u8(_160, 1));
  goto _205;

_205:
#line 114 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/distribute_parallel_for_ast_print.cpp"
  _161 = _146;
  _146 = (llvm_add_u32(_161, 1));
  goto _202;

  } while (1); /* end of syntactic loop '' */
_204:
  goto _206;

_206:
#line 111 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/distribute_parallel_for_ast_print.cpp"
  _162 = _145;
  _145 = (llvm_add_u32(_162, 1));
  goto _199;

  } while (1); /* end of syntactic loop '' */
_201:
  goto _207;

_207:
#line 108 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/distribute_parallel_for_ast_print.cpp"
  _163 = _144;
  _144 = (llvm_add_u32(_163, 1));
  goto _196;

  } while (1); /* end of syntactic loop '' */
_198:
  goto _208;

_208:
#line 105 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/distribute_parallel_for_ast_print.cpp"
  _164 = _143;
  _143 = (llvm_add_u32(_164, 1));
  goto _193;

  } while (1); /* end of syntactic loop '' */
_195:
  goto _209;

_209:
#line 102 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/distribute_parallel_for_ast_print.cpp"
  _165 = _142;
  _142 = (llvm_add_u32(_165, 1));
  goto _190;

  } while (1); /* end of syntactic loop '' */
_192:
  goto _210;

_210:
#line 99 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/distribute_parallel_for_ast_print.cpp"
  _166 = _141;
  _141 = (llvm_add_u32(_166, 1));
  goto _187;

  } while (1); /* end of syntactic loop '' */
_189:
  goto _211;

_211:
#line 96 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/distribute_parallel_for_ast_print.cpp"
  _167 = _140;
  _140 = (llvm_add_u32(_167, 1));
  goto _184;

  } while (1); /* end of syntactic loop '' */
_186:
  goto _212;

_212:
#line 93 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/distribute_parallel_for_ast_print.cpp"
  _168 = _139;
  _139 = (llvm_add_u32(_168, 1));
  goto _181;

  } while (1); /* end of syntactic loop '' */
_183:
  goto _213;

_213:
#line 90 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/distribute_parallel_for_ast_print.cpp"
  _169 = _138;
  _138 = (llvm_add_u32(_169, 1));
  goto _178;

  } while (1); /* end of syntactic loop '' */
_180:
  goto _214;

_214:
#line 87 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/distribute_parallel_for_ast_print.cpp"
  _170 = _137;
  _137 = (llvm_add_u32(_170, 1));
  goto _175;

  } while (1); /* end of syntactic loop '' */
_177:
  return 0;
}

