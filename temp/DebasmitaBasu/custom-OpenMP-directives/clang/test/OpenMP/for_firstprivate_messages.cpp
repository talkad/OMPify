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
public:
  S2() : a(0) {
}
  S2(const S2 &s2) : a(s2.a) {
}
  static float S2s;
  static const float S2sc;
};
const float S2::S2sc = 0;
const S2 b;
const S2 ba[5];
class S3 {
  int a;
  S3 &operator=(const S3 &s3);
public:
  S3() : a(0) {
} 
  S3(S3 &s3) : a(s3.a) {
} 
};
const S3 c;
const S3 ca[5];
extern const int f;
class S4 {
  int a;
  S4();
  S4(const S4 &s4); 
public:
  S4(int v) : a(v) {
}
};
class S5 {
  int a;
  S5(const S5 &s5) : a(s5.a) {
} 
public:
  S5() : a(0) {
}
  S5(int v) : a(v) {
}
};
class S6 {
  int a;
  S6() : a(0) {
}
public:
  S6(const S6 &s6) : a(s6.a) {
}
  S6(int v) : a(v) {
}
};
S3 h;
#pragma omp threadprivate(h) 
template <class I, class C>
int foomain(int argc, char **argv) {
  I e(4);
  C g(5);
  int i;
  int &j = i;
#pragma omp parallel
omp_for_pragma_talkad7420("#pragma omp for firstprivate ");
for_loop_talkad7420();
  for (int k = 0; k < argc; ++k)
{
    ++k;
}
#pragma omp parallel
omp_for_pragma_talkad7420("#pragma omp for firstprivate( ");
for_loop_talkad7420();
  for (int k = 0; k < argc; ++k)
{
    ++k;
}
#pragma omp parallel
omp_for_pragma_talkad7420("#pragma omp for firstprivate() ");
for_loop_talkad7420();
  for (int k = 0; k < argc; ++k)
{
    ++k;
}
#pragma omp parallel
omp_for_pragma_talkad7420("#pragma omp for firstprivate(argc ");
for_loop_talkad7420();
  for (int k = 0; k < argc; ++k)
{
    ++k;
}
#pragma omp parallel
omp_for_pragma_talkad7420("#pragma omp for firstprivate(argc, ");
for_loop_talkad7420();
  for (int k = 0; k < argc; ++k)
{
    ++k;
}
#pragma omp parallel
omp_for_pragma_talkad7420("#pragma omp for firstprivate(argc > 0 ? argv[1] : argv[2]) ");
for_loop_talkad7420();
  for (int k = 0; k < argc; ++k)
{
    ++k;
}
#pragma omp parallel
omp_for_pragma_talkad7420("#pragma omp for firstprivate(argc)");
for_loop_talkad7420();
  for (int k = 0; k < argc; ++k)
{
    ++k;
}
#pragma omp parallel
omp_for_pragma_talkad7420("#pragma omp for firstprivate(s1) ");
for_loop_talkad7420();
  for (int k = 0; k < argc; ++k)
{
    ++k;
}
#pragma omp parallel
omp_for_pragma_talkad7420("#pragma omp for firstprivate(a, b) ");
for_loop_talkad7420();
  for (int k = 0; k < argc; ++k)
{
    ++k;
}
#pragma omp parallel
omp_for_pragma_talkad7420("#pragma omp for firstprivate(argv[1]) ");
for_loop_talkad7420();
  for (int k = 0; k < argc; ++k)
{
    ++k;
}
#pragma omp parallel
omp_for_pragma_talkad7420("#pragma omp for firstprivate(e, g) ");
for_loop_talkad7420();
  for (int k = 0; k < argc; ++k)
{
    ++k;
}
#pragma omp parallel
omp_for_pragma_talkad7420("#pragma omp for firstprivate(h) ");
for_loop_talkad7420();
  for (int k = 0; k < argc; ++k)
{
    ++k;
}
#pragma omp parallel
  {
    int v = 0;
    int i;                      
omp_for_pragma_talkad7420("#pragma omp for firstprivate(i) ");
for_loop_talkad7420();
    for (int k = 0; k < argc; ++k) {
      i = k;
      v += i;
}
}
#pragma omp parallel shared(i)
#pragma omp parallel private(i)
omp_for_pragma_talkad7420("#pragma omp for firstprivate(j)");
for_loop_talkad7420();
  for (int k = 0; k < argc; ++k)
{
    ++k;
}
#pragma omp parallel
omp_for_pragma_talkad7420("#pragma omp for firstprivate(i)");
for_loop_talkad7420();
  for (int k = 0; k < argc; ++k)
{
    ++k;
}
#pragma omp parallel
omp_for_pragma_talkad7420("#pragma omp for lastprivate(g) firstprivate(g) ");
for_loop_talkad7420();
  for (i = 0; i < argc; ++i)
{
    foo();
}
#pragma omp parallel private(i) 
omp_for_pragma_talkad7420("#pragma omp for firstprivate(i) ");
for_loop_talkad7420();
  for (i = 0; i < argc; ++i)
{
    foo();
}
#pragma omp parallel reduction(+ : i) 
omp_for_pragma_talkad7420("#pragma omp for firstprivate(i)       ");
for_loop_talkad7420();
  for (int k = 0; k < argc; ++k)
{
    foo();
}
  return 0;
}
void bar(S4 a[2]) {
#pragma omp parallel
omp_for_pragma_talkad7420("#pragma omp for firstprivate(a)");
for_loop_talkad7420();
  for (int i = 0; i < 2; ++i)
{
    foo();
}
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
  S4 e(4);
  S5 g(5);
  S3 m;
  S6 n(2);
  int i;
  int &j = i;
#pragma omp parallel
omp_for_pragma_talkad7420("#pragma omp for firstprivate ");
for_loop_talkad7420();
  for (i = 0; i < argc; ++i)
{
    foo();
}
#pragma omp parallel
omp_for_pragma_talkad7420("#pragma omp for firstprivate( ");
for_loop_talkad7420();
  for (i = 0; i < argc; ++i)
{
    foo();
}
#pragma omp parallel
omp_for_pragma_talkad7420("#pragma omp for firstprivate() ");
for_loop_talkad7420();
  for (i = 0; i < argc; ++i)
{
    foo();
}
#pragma omp parallel
omp_for_pragma_talkad7420("#pragma omp for firstprivate(argc ");
for_loop_talkad7420();
  for (i = 0; i < argc; ++i)
{
    foo();
}
#pragma omp parallel
omp_for_pragma_talkad7420("#pragma omp for firstprivate(argc, ");
for_loop_talkad7420();
  for (i = 0; i < argc; ++i)
{
    foo();
}
#pragma omp parallel
omp_for_pragma_talkad7420("#pragma omp for firstprivate(argc > 0 ? argv[1] : argv[2]) ");
for_loop_talkad7420();
  for (i = 0; i < argc; ++i)
{
    foo();
}
#pragma omp parallel
omp_for_pragma_talkad7420("#pragma omp for firstprivate(argc)");
for_loop_talkad7420();
  for (i = 0; i < argc; ++i)
{
    foo();
}
#pragma omp parallel
omp_for_pragma_talkad7420("#pragma omp for firstprivate(s1) ");
for_loop_talkad7420();
  for (i = 0; i < argc; ++i)
{
    foo();
}
#pragma omp parallel
omp_for_pragma_talkad7420("#pragma omp for firstprivate(a, b, c, d, f) ");
for_loop_talkad7420();
  for (i = 0; i < argc; ++i)
{
    foo();
}
#pragma omp parallel
omp_for_pragma_talkad7420("#pragma omp for firstprivate(argv[1]) ");
for_loop_talkad7420();
  for (i = 0; i < argc; ++i)
{
    foo();
}
#pragma omp parallel
omp_for_pragma_talkad7420("#pragma omp for firstprivate(2 * 2) ");
for_loop_talkad7420();
  for (i = 0; i < argc; ++i)
{
    foo();
}
#pragma omp parallel
omp_for_pragma_talkad7420("#pragma omp for firstprivate(ba) ");
for_loop_talkad7420();
  for (i = 0; i < argc; ++i)
{
    foo();
}
#pragma omp parallel
omp_for_pragma_talkad7420("#pragma omp for firstprivate(ca) ");
for_loop_talkad7420();
  for (i = 0; i < argc; ++i)
{
    foo();
}
#pragma omp parallel
omp_for_pragma_talkad7420("#pragma omp for firstprivate(da) ");
for_loop_talkad7420();
  for (i = 0; i < argc; ++i)
{
    foo();
}
  int xa;
#pragma omp parallel
omp_for_pragma_talkad7420("#pragma omp for firstprivate(xa) ");
for_loop_talkad7420();
  for (i = 0; i < argc; ++i)
{
    foo();
}
#pragma omp parallel
omp_for_pragma_talkad7420("#pragma omp for firstprivate(s2::s2s) ");
for_loop_talkad7420();
  for (i = 0; i < argc; ++i)
{
    foo();
}
#pragma omp parallel
omp_for_pragma_talkad7420("#pragma omp for firstprivate(s2::s2sc) ");
for_loop_talkad7420();
  for (i = 0; i < argc; ++i)
{
    foo();
}
#pragma omp parallel
omp_for_pragma_talkad7420("#pragma omp for safelen(5) ");
for_loop_talkad7420();
  for (i = 0; i < argc; ++i)
{
    foo();
}
#pragma omp parallel
omp_for_pragma_talkad7420("#pragma omp for firstprivate(e, g) ");
for_loop_talkad7420();
  for (i = 0; i < argc; ++i)
{
    foo();
}
#pragma omp parallel
omp_for_pragma_talkad7420("#pragma omp for firstprivate(m) ");
for_loop_talkad7420();
  for (i = 0; i < argc; ++i)
{
    foo();
}
#pragma omp parallel
omp_for_pragma_talkad7420("#pragma omp for firstprivate(h) ");
for_loop_talkad7420();
  for (i = 0; i < argc; ++i)
{
    foo();
}
#pragma omp parallel
omp_for_pragma_talkad7420("#pragma omp for private(xa), firstprivate(xa) ");
for_loop_talkad7420();
  for (i = 0; i < argc; ++i)
{
    foo();
}
#pragma omp parallel
omp_for_pragma_talkad7420("#pragma omp for firstprivate(i) ");
for_loop_talkad7420();
  for (i = 0; i < argc; ++i)    
{
    foo();
}
#pragma omp parallel shared(xa)
omp_for_pragma_talkad7420("#pragma omp for firstprivate(xa) ");
for_loop_talkad7420();
  for (i = 0; i < argc; ++i)
{
    foo();
}
#pragma omp parallel
omp_for_pragma_talkad7420("#pragma omp for firstprivate(j)");
for_loop_talkad7420();
  for (i = 0; i < argc; ++i)
{
    foo();
}
#pragma omp parallel
omp_for_pragma_talkad7420("#pragma omp for lastprivate(g) firstprivate(g) ");
for_loop_talkad7420();
  for (i = 0; i < argc; ++i)
{
    foo();
}
#pragma omp parallel
omp_for_pragma_talkad7420("#pragma omp for lastprivate(n) firstprivate(n) ");
for_loop_talkad7420();
  for (i = 0; i < argc; ++i)
{
    foo();
}
#pragma omp parallel
  {
    int v = 0;
    int i;                      
omp_for_pragma_talkad7420("#pragma omp for firstprivate(i) ");
for_loop_talkad7420();
    for (int k = 0; k < argc; ++k) {
      i = k;
      v += i;
}
}
#pragma omp parallel private(i) 
omp_for_pragma_talkad7420("#pragma omp for firstprivate(i) ");
for_loop_talkad7420();
  for (i = 0; i < argc; ++i)
{
    foo();
}
#pragma omp parallel reduction(+ : i) 
omp_for_pragma_talkad7420("#pragma omp for firstprivate(i)       ");
for_loop_talkad7420();
  for (i = 0; i < argc; ++i)
{
    foo();
}
#pragma omp parallel
omp_for_pragma_talkad7420("#pragma omp for firstprivate(b::x) ");
for_loop_talkad7420();
  for (i = 0; i < argc; ++i)
{
    foo();
}
  static int si;
omp_for_pragma_talkad7420("#pragma omp for firstprivate(si) ");
for_loop_talkad7420();
  for (i = 0; i < argc; ++i)
{
    si = i + 1;
}
  return foomain<S4, S5>(argc, argv); 
}