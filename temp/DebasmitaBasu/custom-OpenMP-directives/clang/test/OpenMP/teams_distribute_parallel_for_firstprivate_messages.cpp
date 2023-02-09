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
  S4(int v):a(v) { 
}
};
class S5 {
  int a;
  S5():a(0) {
}
  S5(const S5 &s5):a(s5.a) { 
}
public:
  S5(int v):a(v) { 
}
};
class S6 {
  int a;
public:
  S6() : a(0) { 
}
};
S3 h;
#pragma omp threadprivate(h) 
int main(int argc, char **argv) {
  const int d = 5;
  const int da[5] = { 0 
};
  S4 e(4);
  S5 g(5);
  S6 p;
  int i;
  int &j = i;
#pragma omp target
omp_for_pragma_talkad7420("#pragma omp teams distribute parallel for firstprivate ");
for_loop_talkad7420();
  for (i = 0; i < argc; ++i) foo();
{
}
#pragma omp target
omp_for_pragma_talkad7420("#pragma omp teams distribute parallel for firstprivate ( ");
for_loop_talkad7420();
  for (i = 0; i < argc; ++i) foo();
{
}
#pragma omp target
omp_for_pragma_talkad7420("#pragma omp teams distribute parallel for firstprivate () ");
for_loop_talkad7420();
  for (i = 0; i < argc; ++i) foo();
{
}
#pragma omp target
omp_for_pragma_talkad7420("#pragma omp teams distribute parallel for firstprivate (argc ");
for_loop_talkad7420();
  for (i = 0; i < argc; ++i) foo();
{
}
#pragma omp target
omp_for_pragma_talkad7420("#pragma omp teams distribute parallel for firstprivate (argc, ");
for_loop_talkad7420();
  for (i = 0; i < argc; ++i) foo();
{
}
#pragma omp target
omp_for_pragma_talkad7420("#pragma omp teams distribute parallel for firstprivate (argc > 0 ? argv[1] : argv[2]) ");
for_loop_talkad7420();
  for (i = 0; i < argc; ++i) foo();
{
}
#pragma omp target
omp_for_pragma_talkad7420("#pragma omp teams distribute parallel for firstprivate (argc)");
for_loop_talkad7420();
  for (i = 0; i < argc; ++i) foo();
{
}
#pragma omp target
omp_for_pragma_talkad7420("#pragma omp teams distribute parallel for firstprivate (s1) ");
for_loop_talkad7420();
  for (i = 0; i < argc; ++i) foo();
{
}
#pragma omp target
omp_for_pragma_talkad7420("#pragma omp teams distribute parallel for firstprivate (a, b, c, d, f) ");
for_loop_talkad7420();
  for (i = 0; i < argc; ++i) foo();
{
}
#pragma omp target
omp_for_pragma_talkad7420("#pragma omp teams distribute parallel for firstprivate (argv[1]) ");
for_loop_talkad7420();
  for (i = 0; i < argc; ++i) foo();
{
}
#pragma omp target
omp_for_pragma_talkad7420("#pragma omp teams distribute parallel for firstprivate(ba)");
for_loop_talkad7420();
  for (i = 0; i < argc; ++i) foo();
{
}
#pragma omp target
omp_for_pragma_talkad7420("#pragma omp teams distribute parallel for firstprivate(ca) ");
for_loop_talkad7420();
  for (i = 0; i < argc; ++i) foo();
{
}
#pragma omp target
omp_for_pragma_talkad7420("#pragma omp teams distribute parallel for firstprivate(da)");
for_loop_talkad7420();
  for (i = 0; i < argc; ++i) foo();
{
}
#pragma omp target
omp_for_pragma_talkad7420("#pragma omp teams distribute parallel for firstprivate(s2::s2s)");
for_loop_talkad7420();
  for (i = 0; i < argc; ++i) foo();
{
}
#pragma omp target
omp_for_pragma_talkad7420("#pragma omp teams distribute parallel for firstprivate(s2::s2sc)");
for_loop_talkad7420();
  for (i = 0; i < argc; ++i) foo();
{
}
#pragma omp target
omp_for_pragma_talkad7420("#pragma omp teams distribute parallel for firstprivate(h) ");
for_loop_talkad7420();
  for (i = 0; i < argc; ++i) foo();
{
}
#pragma omp target
omp_for_pragma_talkad7420("#pragma omp teams distribute parallel for private(i), firstprivate(i) ");
for_loop_talkad7420();
  for (i = 0; i < argc; ++i) foo();
{
}
#pragma omp target
omp_for_pragma_talkad7420("#pragma omp teams distribute parallel for firstprivate(i)");
for_loop_talkad7420();
  for (j = 0; j < argc; ++j) foo();
{
}
#pragma omp target
omp_for_pragma_talkad7420("#pragma omp teams distribute parallel for firstprivate(i) ");
for_loop_talkad7420();
  for (i = 0; i < argc; ++i) foo(); 
{
}
#pragma omp target
omp_for_pragma_talkad7420("#pragma omp teams distribute parallel for firstprivate(j)");
for_loop_talkad7420();
  for (i = 0; i < argc; ++i) foo();
{
}
#pragma omp target
omp_for_pragma_talkad7420("#pragma omp teams distribute parallel for lastprivate(argc), firstprivate(argc) ");
for_loop_talkad7420();
  for (i = 0; i < argc; ++i) foo();
{
}
  return 0;
}