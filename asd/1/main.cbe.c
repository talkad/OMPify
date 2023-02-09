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
struct l_struct_class_OC_std_KD__KD_ios_base_KD__KD_Init;
struct l_struct_struct_OC_std_KD__KD_ios_base_KD__KD__Callback_list;
struct l_struct_struct_OC_std_KD__KD_ios_base_KD__KD__Words;
struct l_array_8_struct_AC_l_struct_struct_OC_std_KD__KD_ios_base_KD__KD__Words;
struct l_array_4_uint8_t;
struct l_struct_class_OC_std_KD__KD_locale_KD__KD_facet;
struct l_struct_class_OC_std_KD__KD_locale_KD__KD__Impl;
struct l_struct_class_OC_std_KD__KD_locale;
struct l_struct_class_OC_std_KD__KD_ios_base;
struct l_struct_class_OC_std_KD__KD_basic_streambuf;
struct l_struct_class_OC_std_KD__KD_locale_KD__KD_facet_OC_base;
struct l_array_13_void_KC_;
struct l_array_13_uint8_t_KC_;
struct l_struct_struct_OC___locale_struct;
struct l_array_7_uint8_t;
struct l_array_256_uint8_t;
struct l_array_6_uint8_t;
struct l_struct_class_OC_std_KD__KD_ctype;
struct l_struct_class_OC_std_KD__KD_num_put;
struct l_struct_class_OC_std_KD__KD_num_get;
struct l_struct_class_OC_std_KD__KD_basic_ios;
struct l_struct_class_OC_std_KD__KD_basic_ostream;
struct l_array_25_uint8_t;

/* Function definitions */
typedef void l_fptr_5(struct l_struct_class_OC_std_KD__KD_ios_base_KD__KD_Init*);

typedef void l_fptr_8(void);

typedef void l_fptr_1(uint8_t*);

typedef void l_fptr_3(uint32_t, struct l_struct_class_OC_std_KD__KD_ios_base*, uint32_t);

typedef struct l_struct_class_OC_std_KD__KD_basic_ostream* l_fptr_9(struct l_struct_class_OC_std_KD__KD_basic_ostream*, uint8_t);

typedef struct l_struct_class_OC_std_KD__KD_basic_ostream* l_fptr_10(struct l_struct_class_OC_std_KD__KD_basic_ostream*, uint32_t);

typedef uint32_t l_fptr_2(int, ...);

typedef void l_fptr_4(void);

typedef uint32_t l_fptr_6(l_fptr_1*, uint8_t*, uint8_t*);

typedef uint32_t l_fptr_7(void);


/* Types Definitions */
struct l_struct_class_OC_std_KD__KD_ios_base_KD__KD_Init {
  uint8_t field0;
};
struct l_struct_struct_OC_std_KD__KD_ios_base_KD__KD__Callback_list {
  struct l_struct_struct_OC_std_KD__KD_ios_base_KD__KD__Callback_list* field0;
  l_fptr_3* field1;
  uint32_t field2;
  uint32_t field3;
};
struct l_struct_struct_OC_std_KD__KD_ios_base_KD__KD__Words {
  uint8_t* field0;
  uint64_t field1;
};
struct l_array_8_struct_AC_l_struct_struct_OC_std_KD__KD_ios_base_KD__KD__Words {
  struct l_struct_struct_OC_std_KD__KD_ios_base_KD__KD__Words array[8];
};
struct l_array_4_uint8_t {
  uint8_t array[4];
};
#ifdef _MSC_VER
#pragma pack(push, 1)
#endif
struct l_struct_class_OC_std_KD__KD_locale_KD__KD_facet {
  l_fptr_2** field0;
  uint32_t field1;
  struct l_array_4_uint8_t field2;
} __attribute__ ((packed));
#ifdef _MSC_VER
#pragma pack(pop)
#endif
struct l_struct_class_OC_std_KD__KD_locale_KD__KD__Impl {
  uint32_t field0;
  struct l_struct_class_OC_std_KD__KD_locale_KD__KD_facet** field1;
  uint64_t field2;
  struct l_struct_class_OC_std_KD__KD_locale_KD__KD_facet** field3;
  uint8_t** field4;
};
struct l_struct_class_OC_std_KD__KD_locale {
  struct l_struct_class_OC_std_KD__KD_locale_KD__KD__Impl* field0;
};
struct l_struct_class_OC_std_KD__KD_ios_base {
  l_fptr_2** field0;
  uint64_t field1;
  uint64_t field2;
  uint32_t field3;
  uint32_t field4;
  uint32_t field5;
  struct l_struct_struct_OC_std_KD__KD_ios_base_KD__KD__Callback_list* field6;
  struct l_struct_struct_OC_std_KD__KD_ios_base_KD__KD__Words field7;
  struct l_array_8_struct_AC_l_struct_struct_OC_std_KD__KD_ios_base_KD__KD__Words field8;
  uint32_t field9;
  struct l_struct_struct_OC_std_KD__KD_ios_base_KD__KD__Words* field10;
  struct l_struct_class_OC_std_KD__KD_locale field11;
};
struct l_struct_class_OC_std_KD__KD_basic_streambuf {
  l_fptr_2** field0;
  uint8_t* field1;
  uint8_t* field2;
  uint8_t* field3;
  uint8_t* field4;
  uint8_t* field5;
  uint8_t* field6;
  struct l_struct_class_OC_std_KD__KD_locale field7;
};
#ifdef _MSC_VER
#pragma pack(push, 1)
#endif
struct l_struct_class_OC_std_KD__KD_locale_KD__KD_facet_OC_base {
  l_fptr_2** field0;
  uint32_t field1;
} __attribute__ ((packed));
#ifdef _MSC_VER
#pragma pack(pop)
#endif
struct l_array_13_void_KC_ {
  void* array[13];
};
struct l_array_13_uint8_t_KC_ {
  uint8_t* array[13];
};
struct l_struct_struct_OC___locale_struct {
  struct l_array_13_void_KC_ field0;
  uint16_t* field1;
  uint32_t* field2;
  uint32_t* field3;
  struct l_array_13_uint8_t_KC_ field4;
};
struct l_array_7_uint8_t {
  uint8_t array[7];
};
struct l_array_256_uint8_t {
  uint8_t array[256];
};
struct l_array_6_uint8_t {
  uint8_t array[6];
};
#ifdef _MSC_VER
#pragma pack(push, 1)
#endif
struct l_struct_class_OC_std_KD__KD_ctype {
  struct l_struct_class_OC_std_KD__KD_locale_KD__KD_facet_OC_base field0;
  struct l_array_4_uint8_t field1;
  struct l_struct_struct_OC___locale_struct* field2;
  uint8_t field3;
  struct l_array_7_uint8_t field4;
  uint32_t* field5;
  uint32_t* field6;
  uint16_t* field7;
  uint8_t field8;
  struct l_array_256_uint8_t field9;
  struct l_array_256_uint8_t field10;
  uint8_t field11;
  struct l_array_6_uint8_t field12;
} __attribute__ ((packed));
#ifdef _MSC_VER
#pragma pack(pop)
#endif
struct l_struct_class_OC_std_KD__KD_num_put {
  struct l_struct_class_OC_std_KD__KD_locale_KD__KD_facet_OC_base field0;
  struct l_array_4_uint8_t field1;
};
struct l_struct_class_OC_std_KD__KD_num_get {
  struct l_struct_class_OC_std_KD__KD_locale_KD__KD_facet_OC_base field0;
  struct l_array_4_uint8_t field1;
};
struct l_struct_class_OC_std_KD__KD_basic_ios {
  struct l_struct_class_OC_std_KD__KD_ios_base field0;
  struct l_struct_class_OC_std_KD__KD_basic_ostream* field1;
  uint8_t field2;
  uint8_t field3;
  struct l_struct_class_OC_std_KD__KD_basic_streambuf* field4;
  struct l_struct_class_OC_std_KD__KD_ctype* field5;
  struct l_struct_class_OC_std_KD__KD_num_put* field6;
  struct l_struct_class_OC_std_KD__KD_num_get* field7;
};
struct l_struct_class_OC_std_KD__KD_basic_ostream {
  l_fptr_2** field0;
  struct l_struct_class_OC_std_KD__KD_basic_ios field1;
};
struct l_array_25_uint8_t {
  uint8_t array[25];
};

/* External Global Variable Declarations */
extern uint8_t __dso_handle;
extern struct l_struct_class_OC_std_KD__KD_basic_ostream _ZSt4cout;

/* Function Declarations */
static void __cxx_global_var_init(void) __ATTRIBUTELIST__((noinline));
void _ZNSt8ios_base4InitC1Ev(struct l_struct_class_OC_std_KD__KD_ios_base_KD__KD_Init*);
void _ZNSt8ios_base4InitD1Ev(struct l_struct_class_OC_std_KD__KD_ios_base_KD__KD_Init*) __ATTRIBUTELIST__((nothrow));
uint32_t __cxa_atexit(l_fptr_1*, uint8_t*, uint8_t*) __ATTRIBUTELIST__((nothrow));
int main(void) __ATTRIBUTELIST__((noinline));
void _Z25omp_for_pragma_talkad7420Pc(uint8_t*);
void _Z19for_loop_talkad7420v(void);
struct l_struct_class_OC_std_KD__KD_basic_ostream* _ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_c(struct l_struct_class_OC_std_KD__KD_basic_ostream*, int8_t);
struct l_struct_class_OC_std_KD__KD_basic_ostream* _ZNSolsEi(struct l_struct_class_OC_std_KD__KD_basic_ostream*, uint32_t);
static void _GLOBAL__sub_I_main_OC_cpp(void) __ATTRIBUTELIST__((noinline));


/* Global Variable Definitions and Initialization */
static struct l_struct_class_OC_std_KD__KD_ios_base_KD__KD_Init _ZStL8__ioinit;
static const struct l_array_25_uint8_t _OC_str = { "#pragma omp parallel for" };


/* LLVM Intrinsic Builtin Function Bodies */
static __forceinline uint32_t llvm_add_u32(uint32_t a, uint32_t b) {
  uint32_t r = a + b;
  return r;
}


/* Function Bodies */

static void __cxx_global_var_init(void) {
  uint32_t _1;

#line 74 "//usr/bin/../lib/gcc/x86_64-linux-gnu/9/../../../../include/c++/9/iostream"
  _ZNSt8ios_base4InitC1Ev((&_ZStL8__ioinit));
  _1 = __cxa_atexit(((l_fptr_1*)_ZNSt8ios_base4InitD1Ev), ((&_ZStL8__ioinit.field0)), (&__dso_handle));
}


int main(void) {
  uint32_t _2;    /* Address-exposed local */
  uint32_t _3;    /* Address-exposed local */
  uint32_t _4;
  uint32_t _5;
  struct l_struct_class_OC_std_KD__KD_basic_ostream* _6;
  struct l_struct_class_OC_std_KD__KD_basic_ostream* _7;
  uint32_t _8;

  _2 = 0;
#line 6 "/home/talkad/OpenMPdb/temp/abdelrahim-hentabli/image_convolution/src/main.cpp"
  _Z25omp_for_pragma_talkad7420Pc(((&_OC_str.array[((int64_t)0)])));
#line 7 "/home/talkad/OpenMPdb/temp/abdelrahim-hentabli/image_convolution/src/main.cpp"
  _Z19for_loop_talkad7420v();
#line 8 "/home/talkad/OpenMPdb/temp/abdelrahim-hentabli/image_convolution/src/main.cpp"
  ;
  _3 = 0;
  goto _9;

  do {     /* Syntactic loop '' to make GCC happy */
_9:
  _4 = _3;
  if ((((((int32_t)_4) < ((int32_t)10u))&1))) {
    goto _10;
  } else {
    goto _11;
  }

_10:
#line 9 "/home/talkad/OpenMPdb/temp/abdelrahim-hentabli/image_convolution/src/main.cpp"
  _5 = _3;
  _6 = _ZNSolsEi((&_ZSt4cout), _5);
  _7 = _ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_c(_6, 10);
  goto _12;

_12:
#line 8 "/home/talkad/OpenMPdb/temp/abdelrahim-hentabli/image_convolution/src/main.cpp"
  _8 = _3;
  _3 = (llvm_add_u32(_8, 1));
  goto _9;

  } while (1); /* end of syntactic loop '' */
_11:
  return 0;
}


static void _GLOBAL__sub_I_main_OC_cpp(void) {
#line 0 "/home/talkad/OpenMPdb/temp/abdelrahim-hentabli/image_convolution/src/main.cpp"
  __cxx_global_var_init();
}

