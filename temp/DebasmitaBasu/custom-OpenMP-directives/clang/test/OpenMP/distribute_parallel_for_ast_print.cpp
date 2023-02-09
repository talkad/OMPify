#include "_fake_typedefs.h"
#include "_fake_defines.h"
#include "patch.h"
#ifndef HEADER
#define HEADER
struct S {
  S(): a(0) {
}
  S(int v) : a(v) {
}
  int a;
  typedef int type;
};
template <typename T>
class S7 : public T {
protected:
  T a;
  S7() : a(0) {
}
public:
  S7(typename T::type v) : a(v) {
#pragma omp target
#pragma omp teams
omp_for_pragma_talkad7420("#pragma omp distribute parallel for private(a) private(this->a) private(t::a)");
for_loop_talkad7420();
    for (int k = 0; k < a.a; ++k)
{
      ++this->a.a;
}
}
  S7 &operator=(S7 &s) {
#pragma omp target
#pragma omp teams
omp_for_pragma_talkad7420("#pragma omp distribute parallel for private(a) private(this->a)");
for_loop_talkad7420();
    for (int k = 0; k < s.a.a; ++k)
{
      ++s.a.a;
}
    return *this;
}
};
class S8 : public S7<S> {
  S8() {
}
public:
  S8(int v) : S7<S>(v){
#pragma omp target
#pragma omp teams
omp_for_pragma_talkad7420("#pragma omp distribute parallel for private(a) private(this->a) private(s7<s>::a)");
for_loop_talkad7420();
    for (int k = 0; k < a.a; ++k)
{
      ++this->a.a;
}
}
  S8 &operator=(S8 &s) {
#pragma omp target
#pragma omp teams
omp_for_pragma_talkad7420("#pragma omp distribute parallel for private(a) private(this->a)");
for_loop_talkad7420();
    for (int k = 0; k < s.a.a; ++k)
{
      ++s.a.a;
}
    return *this;
}
};
template <class T, int N>
T tmain(T argc) {
  T b = argc, c, d, e, f, h;
  static T a;
  static T g;
#pragma omp threadprivate(g)
#pragma omp target
#pragma omp teams
omp_for_pragma_talkad7420("#pragma omp distribute parallel for dist_schedule(static, a) schedule(dynamic) default(none) copyin(g) firstprivate(a)");
for_loop_talkad7420();
  for (int i = 0; i < 2; ++i)
{
    a = 2;
}
#pragma omp target
#pragma omp teams
omp_for_pragma_talkad7420("#pragma omp distribute parallel for private(argc, b), firstprivate(c, d), lastprivate(d, f) collapse(n) schedule(static, n) if (parallel :argc) num_threads(n) default(shared) shared(e) reduction(+ : h) dist_schedule(static,n)");
for_loop_talkad7420();
  for (int i = 0; i < 2; ++i)
{
for_loop_talkad7420();
    for (int j = 0; j < 2; ++j)
{
for_loop_talkad7420();
      for (int j = 0; j < 2; ++j)
{
for_loop_talkad7420();
        for (int j = 0; j < 2; ++j)
{
for_loop_talkad7420();
          for (int j = 0; j < 2; ++j)
{
for_loop_talkad7420();
  for (int i = 0; i < 2; ++i)
{
for_loop_talkad7420();
    for (int j = 0; j < 2; ++j)
{
for_loop_talkad7420();
      for (int j = 0; j < 2; ++j)
{
for_loop_talkad7420();
        for (int j = 0; j < 2; ++j)
{
for_loop_talkad7420();
          for (int j = 0; j < 2; ++j)
{
	    a++;
}
}
}
}
}
}
}
}
}
}
  return T();
}
int main(int argc, char **argv) {
  int b = argc, c, d, e, f, h;
  static int a;
  static float g;
#pragma omp threadprivate(g)
#pragma omp target
#pragma omp teams
omp_for_pragma_talkad7420("#pragma omp distribute parallel for schedule(guided, argc) default(none) copyin(g) dist_schedule(static, a) private(a)");
for_loop_talkad7420();
  for (int i = 0; i < 2; ++i)
{
    a = 2;
}
#pragma omp target
#pragma omp teams
omp_for_pragma_talkad7420("#pragma omp distribute parallel for private(argc, b), firstprivate(argv, c), lastprivate(d, f) collapse(2) schedule(auto) if (argc) num_threads(a) default(shared) shared(e) reduction(+ : h) dist_schedule(static, b)");
for_loop_talkad7420();
  for (int i = 0; i < 10; ++i)
{
for_loop_talkad7420();
    for (int j = 0; j < 10; ++j)
{
      a++;
}
}
  return (tmain<int, 5>(argc) + tmain<char, 1>(argv[0][0]));
}
#endif