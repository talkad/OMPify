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
struct l_array_37_uint8_t;
struct l_array_43_uint8_t;
struct l_array_5_uint32_t;
struct l_array_5_float;

/* Function definitions */
typedef uint32_t l_fptr_15(uint32_t);

typedef void l_fptr_5(struct l_struct_class_OC_S2*);

typedef void l_fptr_4(uint32_t*);

typedef void l_fptr_1(void);

typedef void l_fptr_11(uint8_t*, uint8_t, uint64_t, bool);

typedef uint8_t* l_fptr_17(uint8_t*, uint32_t, uint64_t);

typedef void l_fptr_13(struct l_struct_class_OC_S5*, uint32_t);

typedef void l_fptr_14(uint8_t*);

typedef bool l_fptr_2(uint32_t);

typedef void l_fptr_3(void);

typedef void l_fptr_12(struct l_struct_class_OC_S4*, uint32_t);

typedef struct l_struct_class_OC_S4* l_fptr_8(struct l_struct_class_OC_S4*, struct l_struct_class_OC_S4*);

typedef uint32_t l_fptr_10(uint32_t, uint8_t**);

typedef void l_fptr_9(struct l_struct_class_OC_S6*);

typedef uint32_t l_fptr_6(struct l_struct_class_OC_S3*, struct l_struct_class_OC_S3*);

typedef void l_fptr_7(struct l_struct_class_OC_S3*);

typedef float l_fptr_16(float);


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
struct l_array_37_uint8_t {
  uint8_t array[37];
};
struct l_array_43_uint8_t {
  uint8_t array[43];
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
void _Z6foobarRi(uint32_t*) __ATTRIBUTELIST__((noinline, nothrow));
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
static void _GLOBAL__sub_I_target_teams_reduction_messages_OC_cpp(void) __ATTRIBUTELIST__((noinline));
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
static const struct l_array_37_uint8_t _OC_str = { "#pragma omp parallel for private(fl)" };
static const struct l_array_43_uint8_t _OC_str_OC_7 = { "#pragma omp parallel for reduction(- : fl)" };
static uint32_t _ZZ4mainE1m;


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
#line 6 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_reduction_messages.cpp"
  ;
#line 7 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_reduction_messages.cpp"
  _3 = _2;
  return (((_3 != 0u)&1));
}


void _Z6foobarRi(uint32_t* _4) {
  uint32_t* _5;    /* Address-exposed local */

  _5 = _4;
#line 9 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_reduction_messages.cpp"
  ;
#line 11 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_reduction_messages.cpp"
  _Z3foov();
}


static void __cxx_global_var_init(void) {
#line 28 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_reduction_messages.cpp"
  _ZN2S2C2Ev((&b));
}


void _ZN2S2C2Ev(struct l_struct_class_OC_S2* _6) {
  struct l_struct_class_OC_S2* _7;    /* Address-exposed local */
  struct l_struct_class_OC_S2* _8;

  _7 = _6;
#line 0 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_reduction_messages.cpp"
  ;
  _8 = _7;
#line 20 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_reduction_messages.cpp"
  *((&_8->field0)) = 0;
}


static void __cxx_global_var_init_OC_1(void) {
  struct l_struct_class_OC_S2* _9;
  struct l_struct_class_OC_S2* _9__PHI_TEMPORARY;
  struct l_struct_class_OC_S2* _10;

  _9__PHI_TEMPORARY = ((&_ZL2ba.array[((int32_t)0)]));   /* for PHI node */
  goto _11;

  do {     /* Syntactic loop '' to make GCC happy */
_11:
#line 29 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_reduction_messages.cpp"
  _9 = _9__PHI_TEMPORARY;
  _ZN2S2C2Ev(_9);
  _10 = (&_9[((int64_t)1)]);
  if ((((_10 == ((&((&_ZL2ba.array[((int32_t)0)]))[((int64_t)5)])))&1))) {
    goto _12;
  } else {
    _9__PHI_TEMPORARY = _10;   /* for PHI node */
    goto _11;
  }

  } while (1); /* end of syntactic loop '' */
_12:
  return;
}


uint32_t _ZplRK2S3S1_(struct l_struct_class_OC_S3* _13, struct l_struct_class_OC_S3* _14) {
  struct l_struct_class_OC_S3* _15;    /* Address-exposed local */
  struct l_struct_class_OC_S3* _16;    /* Address-exposed local */

  _15 = _13;
#line 41 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_reduction_messages.cpp"
  ;
  _16 = _14;
  ;
  return 5;
}


static void __cxx_global_var_init_OC_2(void) {
#line 43 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_reduction_messages.cpp"
  _ZN2S3C2Ev((&c));
}


void _ZN2S3C2Ev(struct l_struct_class_OC_S3* _17) {
  struct l_struct_class_OC_S3* _18;    /* Address-exposed local */
  struct l_struct_class_OC_S3* _19;

  _18 = _17;
#line 0 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_reduction_messages.cpp"
  ;
  _19 = _18;
#line 34 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_reduction_messages.cpp"
  *((&_19->field0)) = 0;
}


static void __cxx_global_var_init_OC_3(void) {
  struct l_struct_class_OC_S3* _20;
  struct l_struct_class_OC_S3* _20__PHI_TEMPORARY;
  struct l_struct_class_OC_S3* _21;

  _20__PHI_TEMPORARY = ((&_ZL2ca.array[((int32_t)0)]));   /* for PHI node */
  goto _22;

  do {     /* Syntactic loop '' to make GCC happy */
_22:
#line 44 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_reduction_messages.cpp"
  _20 = _20__PHI_TEMPORARY;
  _ZN2S3C2Ev(_20);
  _21 = (&_20[((int64_t)1)]);
  if ((((_21 == ((&((&_ZL2ca.array[((int32_t)0)]))[((int64_t)5)])))&1))) {
    goto _23;
  } else {
    _20__PHI_TEMPORARY = _21;   /* for PHI node */
    goto _22;
  }

  } while (1); /* end of syntactic loop '' */
_23:
  return;
}


struct l_struct_class_OC_S4* _ZaNR2S4S0_(struct l_struct_class_OC_S4* _24, struct l_struct_class_OC_S4* _25) {
  struct l_struct_class_OC_S4* _26;    /* Address-exposed local */
  struct l_struct_class_OC_S4* _27;    /* Address-exposed local */
  struct l_struct_class_OC_S4* _28;

  _26 = _24;
#line 56 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_reduction_messages.cpp"
  ;
  _27 = _25;
  ;
  _28 = _26;
  return _28;
}


static void __cxx_global_var_init_OC_4(void) {
#line 78 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_reduction_messages.cpp"
  _ZN2S6C2Ev((&o));
}


void _ZN2S6C2Ev(struct l_struct_class_OC_S6* _29) {
  struct l_struct_class_OC_S6* _30;    /* Address-exposed local */
  struct l_struct_class_OC_S6* _31;

  _30 = _29;
#line 0 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_reduction_messages.cpp"
  ;
  _31 = _30;
#line 74 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_reduction_messages.cpp"
  *((&_31->field0)) = 6;
}


static void __cxx_global_var_init_OC_5(void) {
#line 79 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_reduction_messages.cpp"
  _ZN2S3C2Ev((&h));
}


static void __cxx_global_var_init_OC_6(void) {
  _ZN2S3C2Ev((&k));
}


int main(int argc, char ** argv) {
  uint32_t _32 = (uint32_t)argc;
  uint8_t** _33 = (uint8_t**)argv;
  uint32_t _34;    /* Address-exposed local */
  uint32_t _35;    /* Address-exposed local */
  uint8_t** _36;    /* Address-exposed local */
  uint32_t _37;    /* Address-exposed local */
  __MSALIGN__(16) struct l_array_5_uint32_t _38 __attribute__((aligned(16)));    /* Address-exposed local */
  __MSALIGN__(16) struct l_array_5_uint32_t _39 __attribute__((aligned(16)));    /* Address-exposed local */
  struct l_struct_class_OC_S4 _40;    /* Address-exposed local */
  struct l_struct_class_OC_S5 _41;    /* Address-exposed local */
  uint32_t _42;    /* Address-exposed local */
  uint32_t* _43;    /* Address-exposed local */
  struct l_struct_class_OC_S3* _44;    /* Address-exposed local */
  uint32_t* _45;    /* Address-exposed local */
  uint32_t* _46;    /* Address-exposed local */
  float _47;    /* Address-exposed local */
  uint32_t _48;    /* Address-exposed local */
  uint32_t _49;    /* Address-exposed local */
  uint8_t* _50;
  uint8_t* _51;
  uint32_t _52;
  uint32_t _53;
  uint32_t _54;
  uint32_t _55;
  uint32_t _56;
  uint32_t _57;
  uint32_t _58;
  uint32_t _59;
  float _60;
  float _61;

  _34 = 0;
  _35 = _32;
#line 180 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_reduction_messages.cpp"
  ;
  _36 = _33;
  ;
#line 181 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_reduction_messages.cpp"
  ;
  _37 = 5;
#line 182 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_reduction_messages.cpp"
  ;
  _50 = memset((((uint8_t*)(&_38))), 0, 20);
#line 184 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_reduction_messages.cpp"
  ;
  _51 = memset((((uint8_t*)(&_39))), 0, 20);
#line 186 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_reduction_messages.cpp"
  ;
  _ZN2S4C2Ei((&_40), 4);
#line 187 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_reduction_messages.cpp"
  ;
  _ZN2S5C2Ei((&_41), 5);
#line 188 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_reduction_messages.cpp"
  ;
#line 189 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_reduction_messages.cpp"
  ;
  _43 = (&_42);
#line 190 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_reduction_messages.cpp"
  ;
  _44 = (&k);
#line 191 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_reduction_messages.cpp"
  ;
  _52 = _42;
  _45 = ((&_38.array[((int64_t)(((int64_t)(int32_t)_52)))]));
#line 192 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_reduction_messages.cpp"
  ;
  _53 = _42;
  _46 = ((&_39.array[((int64_t)(((int64_t)(int32_t)_53)))]));
#line 193 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_reduction_messages.cpp"
  ;
#line 195 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_reduction_messages.cpp"
  _Z3foov();
#line 197 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_reduction_messages.cpp"
  _Z3foov();
#line 199 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_reduction_messages.cpp"
  _Z3foov();
#line 201 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_reduction_messages.cpp"
  _Z3foov();
#line 203 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_reduction_messages.cpp"
  _Z3foov();
#line 205 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_reduction_messages.cpp"
  _Z3foov();
#line 207 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_reduction_messages.cpp"
  _Z3foov();
#line 209 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_reduction_messages.cpp"
  _Z3foov();
#line 211 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_reduction_messages.cpp"
  _Z3foov();
#line 213 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_reduction_messages.cpp"
  _Z3foov();
#line 215 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_reduction_messages.cpp"
  _Z3foov();
#line 217 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_reduction_messages.cpp"
  _Z3foov();
#line 219 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_reduction_messages.cpp"
  _Z3foov();
#line 221 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_reduction_messages.cpp"
  _Z3foov();
#line 223 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_reduction_messages.cpp"
  _Z3foov();
#line 225 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_reduction_messages.cpp"
  _Z3foov();
#line 227 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_reduction_messages.cpp"
  _Z3foov();
#line 229 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_reduction_messages.cpp"
  _Z3foov();
#line 231 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_reduction_messages.cpp"
  _Z3foov();
#line 233 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_reduction_messages.cpp"
  _Z3foov();
#line 235 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_reduction_messages.cpp"
  _Z3foov();
#line 237 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_reduction_messages.cpp"
  _Z3foov();
#line 239 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_reduction_messages.cpp"
  _Z3foov();
#line 241 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_reduction_messages.cpp"
  _Z3foov();
#line 243 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_reduction_messages.cpp"
  _Z3foov();
#line 245 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_reduction_messages.cpp"
  _Z3foov();
#line 248 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_reduction_messages.cpp"
  _Z3foov();
#line 250 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_reduction_messages.cpp"
  _Z3foov();
#line 252 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_reduction_messages.cpp"
  _Z3foov();
#line 256 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_reduction_messages.cpp"
  _Z3foov();
#line 258 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_reduction_messages.cpp"
  _Z25omp_for_pragma_talkad7420Pc(((&_OC_str.array[((int64_t)0)])));
#line 259 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_reduction_messages.cpp"
  _Z19for_loop_talkad7420v();
#line 260 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_reduction_messages.cpp"
  ;
  _48 = 0;
  goto _62;

  do {     /* Syntactic loop '' to make GCC happy */
_62:
  _54 = _48;
  if ((((((int32_t)_54) < ((int32_t)10u))&1))) {
    goto _63;
  } else {
    goto _64;
  }

_63:
  goto _65;

_65:
  _55 = _48;
  _48 = (llvm_add_u32(_55, 1));
  goto _62;

  } while (1); /* end of syntactic loop '' */
_64:
#line 264 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_reduction_messages.cpp"
  _Z3foov();
#line 266 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_reduction_messages.cpp"
  _Z25omp_for_pragma_talkad7420Pc(((&_OC_str_OC_7.array[((int64_t)0)])));
#line 267 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_reduction_messages.cpp"
  _Z19for_loop_talkad7420v();
#line 268 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_reduction_messages.cpp"
  ;
  _49 = 0;
  goto _66;

  do {     /* Syntactic loop '' to make GCC happy */
_66:
  _56 = _49;
  if ((((((int32_t)_56) < ((int32_t)10u))&1))) {
    goto _67;
  } else {
    goto _68;
  }

_67:
  goto _69;

_69:
  _57 = _49;
  _49 = (llvm_add_u32(_57, 1));
  goto _66;

  } while (1); /* end of syntactic loop '' */
_68:
#line 272 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_reduction_messages.cpp"
  _Z3foov();
#line 275 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_reduction_messages.cpp"
  _Z3foov();
#line 276 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_reduction_messages.cpp"
  _58 = _35;
  _59 = _Z5tmainIiET_S0_(_58);
  _60 = _47;
  _61 = _Z5tmainIfET_S0_(_60);
  return (((int32_t)(llvm_fadd_f32((((float)(int32_t)_59)), _61))));
}


void _ZN2S4C2Ei(struct l_struct_class_OC_S4* _70, uint32_t _71) {
  struct l_struct_class_OC_S4* _72;    /* Address-exposed local */
  uint32_t _73;    /* Address-exposed local */
  struct l_struct_class_OC_S4* _74;
  uint32_t _75;

  _72 = _70;
#line 0 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_reduction_messages.cpp"
  ;
  _73 = _71;
#line 53 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_reduction_messages.cpp"
  ;
  _74 = _72;
  _75 = _73;
  *((&_74->field0)) = _75;
}


void _ZN2S5C2Ei(struct l_struct_class_OC_S5* _76, uint32_t _77) {
  struct l_struct_class_OC_S5* _78;    /* Address-exposed local */
  uint32_t _79;    /* Address-exposed local */
  struct l_struct_class_OC_S5* _80;
  uint32_t _81;

  _78 = _76;
#line 0 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_reduction_messages.cpp"
  ;
  _79 = _77;
#line 66 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_reduction_messages.cpp"
  ;
  _80 = _78;
  _81 = _79;
  *((&_80->field0)) = _81;
}


uint32_t _Z5tmainIiET_S0_(uint32_t _82) {
  uint32_t _83;    /* Address-exposed local */
  uint32_t _84;    /* Address-exposed local */
  __MSALIGN__(16) struct l_array_5_uint32_t _85 __attribute__((aligned(16)));    /* Address-exposed local */
  __MSALIGN__(16) struct l_array_5_uint32_t _86 __attribute__((aligned(16)));    /* Address-exposed local */
  uint32_t _87;    /* Address-exposed local */
  uint32_t* _88;    /* Address-exposed local */
  struct l_struct_class_OC_S3* _89;    /* Address-exposed local */
  uint32_t* _90;    /* Address-exposed local */
  uint32_t* _91;    /* Address-exposed local */
  uint32_t _92;    /* Address-exposed local */
  uint32_t _93;    /* Address-exposed local */
  uint32_t _94;    /* Address-exposed local */
  uint8_t* _95;
  uint8_t* _96;
  uint32_t _97;
  uint32_t _98;
  uint32_t _99;
  uint32_t _100;
  uint32_t _101;
  uint32_t _102;

  _83 = _82;
#line 82 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_reduction_messages.cpp"
  ;
#line 83 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_reduction_messages.cpp"
  ;
  _84 = 0;
#line 84 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_reduction_messages.cpp"
  ;
  _95 = memset((((uint8_t*)(&_85))), 0, 20);
#line 86 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_reduction_messages.cpp"
  ;
  _96 = memset((((uint8_t*)(&_86))), 0, 20);
#line 88 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_reduction_messages.cpp"
  ;
#line 89 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_reduction_messages.cpp"
  ;
  _88 = (&_87);
#line 90 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_reduction_messages.cpp"
  ;
  _89 = (&k);
#line 91 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_reduction_messages.cpp"
  ;
  _97 = _87;
  _90 = ((&_85.array[((int64_t)(((int64_t)(int32_t)_97)))]));
#line 92 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_reduction_messages.cpp"
  ;
  _98 = _87;
  _91 = ((&_86.array[((int64_t)(((int64_t)(int32_t)_98)))]));
#line 93 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_reduction_messages.cpp"
  ;
#line 95 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_reduction_messages.cpp"
  _Z3foov();
#line 97 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_reduction_messages.cpp"
  _Z3foov();
#line 99 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_reduction_messages.cpp"
  _Z3foov();
#line 101 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_reduction_messages.cpp"
  _Z3foov();
#line 103 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_reduction_messages.cpp"
  _Z3foov();
#line 105 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_reduction_messages.cpp"
  _Z3foov();
#line 107 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_reduction_messages.cpp"
  _Z3foov();
#line 109 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_reduction_messages.cpp"
  _Z3foov();
#line 111 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_reduction_messages.cpp"
  _Z3foov();
#line 113 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_reduction_messages.cpp"
  _Z3foov();
#line 115 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_reduction_messages.cpp"
  _Z3foov();
#line 117 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_reduction_messages.cpp"
  _Z3foov();
#line 119 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_reduction_messages.cpp"
  _Z3foov();
#line 121 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_reduction_messages.cpp"
  _Z3foov();
#line 123 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_reduction_messages.cpp"
  _Z3foov();
#line 125 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_reduction_messages.cpp"
  _Z3foov();
#line 127 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_reduction_messages.cpp"
  _Z3foov();
#line 129 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_reduction_messages.cpp"
  _Z3foov();
#line 131 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_reduction_messages.cpp"
  _Z3foov();
#line 133 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_reduction_messages.cpp"
  _Z3foov();
#line 135 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_reduction_messages.cpp"
  _Z3foov();
#line 137 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_reduction_messages.cpp"
  _Z3foov();
#line 139 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_reduction_messages.cpp"
  _Z3foov();
#line 141 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_reduction_messages.cpp"
  _Z3foov();
#line 143 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_reduction_messages.cpp"
  _Z3foov();
#line 146 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_reduction_messages.cpp"
  _Z3foov();
#line 148 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_reduction_messages.cpp"
  _Z3foov();
#line 150 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_reduction_messages.cpp"
  _Z3foov();
#line 154 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_reduction_messages.cpp"
  _Z3foov();
#line 156 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_reduction_messages.cpp"
  _Z25omp_for_pragma_talkad7420Pc(((&_OC_str.array[((int64_t)0)])));
#line 157 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_reduction_messages.cpp"
  _Z19for_loop_talkad7420v();
#line 158 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_reduction_messages.cpp"
  ;
  _93 = 0;
  goto _103;

  do {     /* Syntactic loop '' to make GCC happy */
_103:
  _99 = _93;
  if ((((((int32_t)_99) < ((int32_t)10u))&1))) {
    goto _104;
  } else {
    goto _105;
  }

_104:
  goto _106;

_106:
  _100 = _93;
  _93 = (llvm_add_u32(_100, 1));
  goto _103;

  } while (1); /* end of syntactic loop '' */
_105:
#line 162 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_reduction_messages.cpp"
  _Z3foov();
#line 164 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_reduction_messages.cpp"
  _Z25omp_for_pragma_talkad7420Pc(((&_OC_str_OC_7.array[((int64_t)0)])));
#line 165 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_reduction_messages.cpp"
  _Z19for_loop_talkad7420v();
#line 166 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_reduction_messages.cpp"
  ;
  _94 = 0;
  goto _107;

  do {     /* Syntactic loop '' to make GCC happy */
_107:
  _101 = _94;
  if ((((((int32_t)_101) < ((int32_t)10u))&1))) {
    goto _108;
  } else {
    goto _109;
  }

_108:
  goto _110;

_110:
  _102 = _94;
  _94 = (llvm_add_u32(_102, 1));
  goto _107;

  } while (1); /* end of syntactic loop '' */
_109:
#line 170 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_reduction_messages.cpp"
  _Z3foov();
  return 0;
}


float _Z5tmainIfET_S0_(float _111) {
  float _112;    /* Address-exposed local */
  float _113;    /* Address-exposed local */
  __MSALIGN__(16) struct l_array_5_float _114 __attribute__((aligned(16)));    /* Address-exposed local */
  __MSALIGN__(16) struct l_array_5_float _115 __attribute__((aligned(16)));    /* Address-exposed local */
  float _116;    /* Address-exposed local */
  float* _117;    /* Address-exposed local */
  struct l_struct_class_OC_S3* _118;    /* Address-exposed local */
  float* _119;    /* Address-exposed local */
  float* _120;    /* Address-exposed local */
  float _121;    /* Address-exposed local */
  uint32_t _122;    /* Address-exposed local */
  uint32_t _123;    /* Address-exposed local */
  uint8_t* _124;
  uint8_t* _125;
  float _126;
  float _127;
  uint32_t _128;
  uint32_t _129;
  uint32_t _130;
  uint32_t _131;

  _112 = _111;
#line 82 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_reduction_messages.cpp"
  ;
#line 83 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_reduction_messages.cpp"
  ;
  _113 = 0;
#line 84 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_reduction_messages.cpp"
  ;
  _124 = memset((((uint8_t*)(&_114))), 0, 20);
#line 86 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_reduction_messages.cpp"
  ;
  _125 = memset((((uint8_t*)(&_115))), 0, 20);
#line 88 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_reduction_messages.cpp"
  ;
#line 89 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_reduction_messages.cpp"
  ;
  _117 = (&_116);
#line 90 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_reduction_messages.cpp"
  ;
  _118 = (&k);
#line 91 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_reduction_messages.cpp"
  ;
  _126 = _116;
  _119 = ((&_114.array[((int64_t)(((int64_t)(int32_t)(((int32_t)_126)))))]));
#line 92 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_reduction_messages.cpp"
  ;
  _127 = _116;
  _120 = ((&_115.array[((int64_t)(((int64_t)(int32_t)(((int32_t)_127)))))]));
#line 93 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_reduction_messages.cpp"
  ;
#line 95 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_reduction_messages.cpp"
  _Z3foov();
#line 97 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_reduction_messages.cpp"
  _Z3foov();
#line 99 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_reduction_messages.cpp"
  _Z3foov();
#line 101 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_reduction_messages.cpp"
  _Z3foov();
#line 103 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_reduction_messages.cpp"
  _Z3foov();
#line 105 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_reduction_messages.cpp"
  _Z3foov();
#line 107 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_reduction_messages.cpp"
  _Z3foov();
#line 109 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_reduction_messages.cpp"
  _Z3foov();
#line 111 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_reduction_messages.cpp"
  _Z3foov();
#line 113 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_reduction_messages.cpp"
  _Z3foov();
#line 115 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_reduction_messages.cpp"
  _Z3foov();
#line 117 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_reduction_messages.cpp"
  _Z3foov();
#line 119 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_reduction_messages.cpp"
  _Z3foov();
#line 121 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_reduction_messages.cpp"
  _Z3foov();
#line 123 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_reduction_messages.cpp"
  _Z3foov();
#line 125 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_reduction_messages.cpp"
  _Z3foov();
#line 127 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_reduction_messages.cpp"
  _Z3foov();
#line 129 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_reduction_messages.cpp"
  _Z3foov();
#line 131 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_reduction_messages.cpp"
  _Z3foov();
#line 133 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_reduction_messages.cpp"
  _Z3foov();
#line 135 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_reduction_messages.cpp"
  _Z3foov();
#line 137 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_reduction_messages.cpp"
  _Z3foov();
#line 139 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_reduction_messages.cpp"
  _Z3foov();
#line 141 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_reduction_messages.cpp"
  _Z3foov();
#line 143 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_reduction_messages.cpp"
  _Z3foov();
#line 146 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_reduction_messages.cpp"
  _Z3foov();
#line 148 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_reduction_messages.cpp"
  _Z3foov();
#line 150 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_reduction_messages.cpp"
  _Z3foov();
#line 154 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_reduction_messages.cpp"
  _Z3foov();
#line 156 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_reduction_messages.cpp"
  _Z25omp_for_pragma_talkad7420Pc(((&_OC_str.array[((int64_t)0)])));
#line 157 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_reduction_messages.cpp"
  _Z19for_loop_talkad7420v();
#line 158 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_reduction_messages.cpp"
  ;
  _122 = 0;
  goto _132;

  do {     /* Syntactic loop '' to make GCC happy */
_132:
  _128 = _122;
  if ((((((int32_t)_128) < ((int32_t)10u))&1))) {
    goto _133;
  } else {
    goto _134;
  }

_133:
  goto _135;

_135:
  _129 = _122;
  _122 = (llvm_add_u32(_129, 1));
  goto _132;

  } while (1); /* end of syntactic loop '' */
_134:
#line 162 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_reduction_messages.cpp"
  _Z3foov();
#line 164 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_reduction_messages.cpp"
  _Z25omp_for_pragma_talkad7420Pc(((&_OC_str_OC_7.array[((int64_t)0)])));
#line 165 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_reduction_messages.cpp"
  _Z19for_loop_talkad7420v();
#line 166 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_reduction_messages.cpp"
  ;
  _123 = 0;
  goto _136;

  do {     /* Syntactic loop '' to make GCC happy */
_136:
  _130 = _123;
  if ((((((int32_t)_130) < ((int32_t)10u))&1))) {
    goto _137;
  } else {
    goto _138;
  }

_137:
  goto _139;

_139:
  _131 = _123;
  _123 = (llvm_add_u32(_131, 1));
  goto _136;

  } while (1); /* end of syntactic loop '' */
_138:
#line 170 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_reduction_messages.cpp"
  _Z3foov();
  return 0;
}


static void _GLOBAL__sub_I_target_teams_reduction_messages_OC_cpp(void) {
#line 0 "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP/target_teams_reduction_messages.cpp"
  __cxx_global_var_init();
  __cxx_global_var_init_OC_1();
  __cxx_global_var_init_OC_2();
  __cxx_global_var_init_OC_3();
  __cxx_global_var_init_OC_4();
  __cxx_global_var_init_OC_5();
  __cxx_global_var_init_OC_6();
}

