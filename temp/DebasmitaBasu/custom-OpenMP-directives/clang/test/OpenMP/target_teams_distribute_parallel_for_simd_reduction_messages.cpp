#include "_fake_typedefs.h"
#include "_fake_defines.h"
#include "patch.h"
void foo() {
}
bool foobool(int argc) {
  return argc;
}
struct S1; 
extern S1 a;
class S2 {
  mutable int a;
  S2 &operator+(const S2 &arg) { return (*this); 
} 
public:
  S2() : a(0) {
}
  S2(S2 &s2) : a(s2.a) {
}
  static float S2s; 
  static const float S2sc;
};
const float S2::S2sc = 0; 
S2 b;                     
const S2 ba[5];           
class S3 {
  int a;
public:
  int b;
  S3() : a(0) {
}
  S3(const S3 &s3) : a(s3.a) {
}
  S3 operator+(const S3 &arg1) { return arg1; 
}
};
int operator+(const S3 &arg1, const S3 &arg2) { return 5; 
}
S3 c;               
const S3 ca[5];     
extern const int f; 
class S4 {
  int a;
  S4(); 
  S4(const S4 &s4);
  S4 &operator+(const S4 &arg) { return (*this); 
}
public:
  S4(int v) : a(v) {
}
};
S4 &operator&=(S4 &arg1, S4 &arg2) { return arg1; 
}
class S5 {
  int a;
  S5() : a(0) {
} 
  S5(const S5 &s5) : a(s5.a) {
}
  S5 &operator+(const S5 &arg);
public:
  S5(int v) : a(v) {
}
};
class S6 { 
#if __cplusplus >= 201103L 
#endif
  int a;
public:
  S6() : a(6) {
}
  operator int() { return 6; 
}
} o;
S3 h, k;
#pragma omp threadprivate(h) 
template <class T>       
T tmain(T argc) {
  const T d = T();       
  const T da[5] = {T()
}; 
  T qa[5] = {T()
};
  T i;
  T &j = i;                    
  S3 &p = k;                   
  const T &r = da[(int)i];     
  T &q = qa[(int)i];           
  T fl;
omp_for_pragma_talkad7420("#pragma omp target teams distribute parallel for simd reduction ");
for_loop_talkad7420();
  for (int j=0; j<100; j++) foo();
{
}
omp_for_pragma_talkad7420("#pragma omp target teams distribute parallel for simd reduction + ");
for_loop_talkad7420();
  for (int j=0; j<100; j++) foo();
{
}
omp_for_pragma_talkad7420("#pragma omp target teams distribute parallel for simd reduction( ");
for_loop_talkad7420();
  for (int j=0; j<100; j++) foo();
{
}
omp_for_pragma_talkad7420("#pragma omp target teams distribute parallel for simd reduction(- ");
for_loop_talkad7420();
  for (int j=0; j<100; j++) foo();
{
}
omp_for_pragma_talkad7420("#pragma omp target teams distribute parallel for simd reduction() ");
for_loop_talkad7420();
  for (int j=0; j<100; j++) foo();
{
}
omp_for_pragma_talkad7420("#pragma omp target teams distribute parallel for simd reduction(*) ");
for_loop_talkad7420();
  for (int j=0; j<100; j++) foo();
{
}
omp_for_pragma_talkad7420("#pragma omp target teams distribute parallel for simd reduction(\) ");
for_loop_talkad7420();
  for (int j=0; j<100; j++) foo();
{
}
omp_for_pragma_talkad7420("#pragma omp target teams distribute parallel for simd reduction(& : argc ");
for_loop_talkad7420();
  for (int j=0; j<100; j++) foo();
{
}
omp_for_pragma_talkad7420("#pragma omp target teams distribute parallel for simd reduction(| : argc, ");
for_loop_talkad7420();
  for (int j=0; j<100; j++) foo();
{
}
omp_for_pragma_talkad7420("#pragma omp target teams distribute parallel for simd reduction(|| : argc ? i : argc) ");
for_loop_talkad7420();
  for (int j=0; j<100; j++) foo();
{
}
omp_for_pragma_talkad7420("#pragma omp target teams distribute parallel for simd reduction(foo : argc) ");
for_loop_talkad7420();
  for (int j=0; j<100; j++) foo();
{
}
omp_for_pragma_talkad7420("#pragma omp target teams distribute parallel for simd reduction(&& : argc)");
for_loop_talkad7420();
  for (int j=0; j<100; j++) foo();
{
}
omp_for_pragma_talkad7420("#pragma omp target teams distribute parallel for simd reduction(^ : t) ");
for_loop_talkad7420();
  for (int j=0; j<100; j++) foo();
{
}
omp_for_pragma_talkad7420("#pragma omp target teams distribute parallel for simd reduction(+ : a, b, c, d, f) ");
for_loop_talkad7420();
  for (int j=0; j<100; j++) foo();
{
}
omp_for_pragma_talkad7420("#pragma omp target teams distribute parallel for simd reduction(min : a, b, c, d, f) ");
for_loop_talkad7420();
  for (int j=0; j<100; j++) foo();
{
}
omp_for_pragma_talkad7420("#pragma omp target teams distribute parallel for simd reduction(max : h.b) ");
for_loop_talkad7420();
  for (int j=0; j<100; j++) foo();
{
}
omp_for_pragma_talkad7420("#pragma omp target teams distribute parallel for simd reduction(+ : ba) ");
for_loop_talkad7420();
  for (int j=0; j<100; j++) foo();
{
}
omp_for_pragma_talkad7420("#pragma omp target teams distribute parallel for simd reduction(* : ca) ");
for_loop_talkad7420();
  for (int j=0; j<100; j++) foo();
{
}
omp_for_pragma_talkad7420("#pragma omp target teams distribute parallel for simd reduction(- : da) ");
for_loop_talkad7420();
  for (int j=0; j<100; j++) foo();
{
}
omp_for_pragma_talkad7420("#pragma omp target teams distribute parallel for simd reduction(^ : fl) ");
for_loop_talkad7420();
  for (int j=0; j<100; j++) foo();
{
}
omp_for_pragma_talkad7420("#pragma omp target teams distribute parallel for simd reduction(&& : s2::s2s) ");
for_loop_talkad7420();
  for (int j=0; j<100; j++) foo();
{
}
omp_for_pragma_talkad7420("#pragma omp target teams distribute parallel for simd reduction(&& : s2::s2sc) ");
for_loop_talkad7420();
  for (int j=0; j<100; j++) foo();
{
}
omp_for_pragma_talkad7420("#pragma omp target teams distribute parallel for simd reduction(+ : h, k) ");
for_loop_talkad7420();
  for (int j=0; j<100; j++) foo();
{
}
omp_for_pragma_talkad7420("#pragma omp target teams distribute parallel for simd reduction(+ : o) ");
for_loop_talkad7420();
  for (int j=0; j<100; j++) foo();
{
}
omp_for_pragma_talkad7420("#pragma omp target teams distribute parallel for simd private(i), reduction(+ : j), reduction(+ : q) ");
for_loop_talkad7420();
  for (int j=0; j<100; j++) foo();
{
}
#pragma omp parallel private(k)
omp_for_pragma_talkad7420("#pragma omp target teams distribute parallel for simd reduction(+ : p), reduction(+ : p) ");
for_loop_talkad7420();
  for (int j=0; j<100; j++) foo();
{
}
omp_for_pragma_talkad7420("#pragma omp target teams distribute parallel for simd reduction(+ : p), reduction(+ : p) ");
for_loop_talkad7420();
  for (int j=0; j<100; j++) foo();
{
}
omp_for_pragma_talkad7420("#pragma omp target teams distribute parallel for simd reduction(+ : r) ");
for_loop_talkad7420();
  for (int j=0; j<100; j++) foo();
{
}
#pragma omp parallel shared(i)
#pragma omp parallel reduction(min : i)
omp_for_pragma_talkad7420("#pragma omp target teams distribute parallel for simd reduction(max : j) ");
for_loop_talkad7420();
  for (int j=0; j<100; j++) foo();
{
}
omp_for_pragma_talkad7420("#pragma omp target teams distribute parallel for simd reduction(+ : fl)");
for_loop_talkad7420();
    for (int j=0; j<100; j++) foo();
{
}
  return T();
}
namespace A {
double x;
#pragma omp threadprivate(x) 
}
namespace B {
using A::x;
}
int main(int argc, char **argv) {
  const int d = 5;       
  const int da[5] = {0
}; 
  int qa[5] = {0
};
  S4 e(4);
  S5 g(5);
  int i;
  int &j = i;                  
  S3 &p = k;                   
  const int &r = da[i];        
  int &q = qa[i];              
  float fl;
omp_for_pragma_talkad7420("#pragma omp target teams distribute parallel for simd reduction ");
for_loop_talkad7420();
  for (int j=0; j<100; j++) foo();
{
}
omp_for_pragma_talkad7420("#pragma omp target teams distribute parallel for simd reduction + ");
for_loop_talkad7420();
  for (int j=0; j<100; j++) foo();
{
}
omp_for_pragma_talkad7420("#pragma omp target teams distribute parallel for simd reduction( ");
for_loop_talkad7420();
  for (int j=0; j<100; j++) foo();
{
}
omp_for_pragma_talkad7420("#pragma omp target teams distribute parallel for simd reduction(- ");
for_loop_talkad7420();
  for (int j=0; j<100; j++) foo();
{
}
omp_for_pragma_talkad7420("#pragma omp target teams distribute parallel for simd reduction() ");
for_loop_talkad7420();
  for (int j=0; j<100; j++) foo();
{
}
omp_for_pragma_talkad7420("#pragma omp target teams distribute parallel for simd reduction(*) ");
for_loop_talkad7420();
  for (int j=0; j<100; j++) foo();
{
}
omp_for_pragma_talkad7420("#pragma omp target teams distribute parallel for simd reduction(\) ");
for_loop_talkad7420();
  for (int j=0; j<100; j++) foo();
{
}
omp_for_pragma_talkad7420("#pragma omp target teams distribute parallel for simd reduction(foo : argc ");
for_loop_talkad7420();
  for (int j=0; j<100; j++) foo();
{
}
omp_for_pragma_talkad7420("#pragma omp target teams distribute parallel for simd reduction(| : argc, ");
for_loop_talkad7420();
  for (int j=0; j<100; j++) foo();
{
}
omp_for_pragma_talkad7420("#pragma omp target teams distribute parallel for simd reduction(|| : argc > 0 ? argv[1] : argv[2]) ");
for_loop_talkad7420();
  for (int j=0; j<100; j++) foo();
{
}
omp_for_pragma_talkad7420("#pragma omp target teams distribute parallel for simd reduction(~ : argc) ");
for_loop_talkad7420();
  for (int j=0; j<100; j++) foo();
{
}
omp_for_pragma_talkad7420("#pragma omp target teams distribute parallel for simd reduction(&& : argc)");
for_loop_talkad7420();
  for (int j=0; j<100; j++) foo();
{
}
omp_for_pragma_talkad7420("#pragma omp target teams distribute parallel for simd reduction(^ : s1) ");
for_loop_talkad7420();
  for (int j=0; j<100; j++) foo();
{
}
omp_for_pragma_talkad7420("#pragma omp target teams distribute parallel for simd reduction(+ : a, b, c, d, f) ");
for_loop_talkad7420();
  for (int j=0; j<100; j++) foo();
{
}
omp_for_pragma_talkad7420("#pragma omp target teams distribute parallel for simd reduction(min : a, b, c, d, f) ");
for_loop_talkad7420();
  for (int j=0; j<100; j++) foo();
{
}
omp_for_pragma_talkad7420("#pragma omp target teams distribute parallel for simd reduction(max : h.b) ");
for_loop_talkad7420();
  for (int j=0; j<100; j++) foo();
{
}
omp_for_pragma_talkad7420("#pragma omp target teams distribute parallel for simd reduction(+ : ba) ");
for_loop_talkad7420();
  for (int j=0; j<100; j++) foo();
{
}
omp_for_pragma_talkad7420("#pragma omp target teams distribute parallel for simd reduction(* : ca) ");
for_loop_talkad7420();
  for (int j=0; j<100; j++) foo();
{
}
omp_for_pragma_talkad7420("#pragma omp target teams distribute parallel for simd reduction(- : da) ");
for_loop_talkad7420();
  for (int j=0; j<100; j++) foo();
{
}
omp_for_pragma_talkad7420("#pragma omp target teams distribute parallel for simd reduction(^ : fl) ");
for_loop_talkad7420();
  for (int j=0; j<100; j++) foo();
{
}
omp_for_pragma_talkad7420("#pragma omp target teams distribute parallel for simd reduction(&& : s2::s2s) ");
for_loop_talkad7420();
  for (int j=0; j<100; j++) foo();
{
}
omp_for_pragma_talkad7420("#pragma omp target teams distribute parallel for simd reduction(&& : s2::s2sc) ");
for_loop_talkad7420();
  for (int j=0; j<100; j++) foo();
{
}
omp_for_pragma_talkad7420("#pragma omp target teams distribute parallel for simd reduction(& : e, g) ");
for_loop_talkad7420();
  for (int j=0; j<100; j++) foo();
{
}
omp_for_pragma_talkad7420("#pragma omp target teams distribute parallel for simd reduction(+ : h, k, b::x) ");
for_loop_talkad7420();
  for (int j=0; j<100; j++) foo();
{
}
omp_for_pragma_talkad7420("#pragma omp target teams distribute parallel for simd reduction(+ : o) ");
for_loop_talkad7420();
  for (int j=0; j<100; j++) foo();
{
}
omp_for_pragma_talkad7420("#pragma omp target teams distribute parallel for simd private(i), reduction(+ : j), reduction(+ : q) ");
for_loop_talkad7420();
  for (int j=0; j<100; j++) foo();
{
}
#pragma omp parallel private(k)
omp_for_pragma_talkad7420("#pragma omp target teams distribute parallel for simd reduction(+ : p), reduction(+ : p) ");
for_loop_talkad7420();
  for (int j=0; j<100; j++) foo();
{
}
omp_for_pragma_talkad7420("#pragma omp target teams distribute parallel for simd reduction(+ : p), reduction(+ : p) ");
for_loop_talkad7420();
  for (int j=0; j<100; j++) foo();
{
}
omp_for_pragma_talkad7420("#pragma omp target teams distribute parallel for simd reduction(+ : r) ");
for_loop_talkad7420();
  for (int j=0; j<100; j++) foo();
{
}
#pragma omp parallel shared(i)
#pragma omp parallel reduction(min : i)
omp_for_pragma_talkad7420("#pragma omp target teams distribute parallel for simd reduction(max : j) ");
for_loop_talkad7420();
  for (int j=0; j<100; j++) foo();
{
}
omp_for_pragma_talkad7420("#pragma omp target teams distribute parallel for simd reduction(+ : fl)");
for_loop_talkad7420();
    for (int j=0; j<100; j++) foo();
{
}
  static int m;
omp_for_pragma_talkad7420("#pragma omp target teams distribute parallel for simd reduction(+ : m) ");
for_loop_talkad7420();
  for (int j=0; j<100; j++) foo();
{
}
  return tmain(argc) + tmain(fl); 
}