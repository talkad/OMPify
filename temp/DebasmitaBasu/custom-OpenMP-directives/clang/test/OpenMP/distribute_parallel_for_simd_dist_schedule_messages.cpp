#include "_fake_typedefs.h"
#include "_fake_defines.h"
#include "patch.h"
void foo() {
}
bool foobool(int argc) {
  return argc;
}
struct S1; 
template <class T, int N>
T tmain(T argc) {
  T b = argc, c, d, e, f, g;
  char ** argv;
  static T a;
#pragma omp target
#pragma omp teams
omp_for_pragma_talkad7420("#pragma omp distribute parallel for simd dist_schedule ");
for_loop_talkad7420();
  for (int i = 0; i < 10; ++i) foo();
{
}
#pragma omp target
#pragma omp teams
omp_for_pragma_talkad7420("#pragma omp distribute parallel for simd dist_schedule ( ");
for_loop_talkad7420();
  for (int i = 0; i < 10; ++i) foo();
{
}
#pragma omp target
#pragma omp teams
omp_for_pragma_talkad7420("#pragma omp distribute parallel for simd dist_schedule () ");
for_loop_talkad7420();
  for (int i = 0; i < 10; ++i) foo();
{
}
#pragma omp target
#pragma omp teams
omp_for_pragma_talkad7420("#pragma omp distribute parallel for simd dist_schedule (static ");
for_loop_talkad7420();
  for (int i = 0; i < 10; ++i) foo();
{
}
#pragma omp target
#pragma omp teams
omp_for_pragma_talkad7420("#pragma omp distribute parallel for simd dist_schedule (static, ");
for_loop_talkad7420();
  for (int i = 0; i < 10; ++i) foo();
{
}
#pragma omp target
#pragma omp teams
omp_for_pragma_talkad7420("#pragma omp distribute parallel for simd dist_schedule (argc)) ");
for_loop_talkad7420();
  for (int i = 0; i < 10; ++i) foo();
{
}
#pragma omp target
#pragma omp teams
omp_for_pragma_talkad7420("#pragma omp distribute parallel for simd dist_schedule (static, argc > 0 ? argv[1] : argv[2]) ");
for_loop_talkad7420();
  for (int i = 0; i < 10; ++i) foo();
{
}
#pragma omp target
#pragma omp teams
omp_for_pragma_talkad7420("#pragma omp distribute parallel for simd dist_schedule (static), dist_schedule (static, 1) ");
for_loop_talkad7420();
  for (int i = 0; i < 10; ++i) foo();
{
}
#pragma omp target
#pragma omp teams
omp_for_pragma_talkad7420("#pragma omp distribute parallel for simd dist_schedule (static, s1) ");
for_loop_talkad7420();
  for (int i = 0; i < 10; ++i) foo();
{
}
#pragma omp target
#pragma omp teams
omp_for_pragma_talkad7420("#pragma omp distribute parallel for simd dist_schedule (static, argv[1]=2) ");
for_loop_talkad7420();
  for (int i = 0; i < 10; ++i) foo();
{
}
  return T();
}
int main(int argc, char **argv) {
#pragma omp target
#pragma omp teams
omp_for_pragma_talkad7420("#pragma omp distribute parallel for simd dist_schedule ");
for_loop_talkad7420();
  for (int i = 0; i < 10; ++i) foo();
{
}
#pragma omp target
#pragma omp teams
omp_for_pragma_talkad7420("#pragma omp distribute parallel for simd dist_schedule ( ");
for_loop_talkad7420();
  for (int i = 0; i < 10; ++i) foo();
{
}
#pragma omp target
#pragma omp teams
omp_for_pragma_talkad7420("#pragma omp distribute parallel for simd dist_schedule () ");
for_loop_talkad7420();
  for (int i = 0; i < 10; ++i) foo();
{
}
#pragma omp target
#pragma omp teams
omp_for_pragma_talkad7420("#pragma omp distribute parallel for simd dist_schedule (static ");
for_loop_talkad7420();
  for (int i = 0; i < 10; ++i) foo();
{
}
#pragma omp target
#pragma omp teams
omp_for_pragma_talkad7420("#pragma omp distribute parallel for simd dist_schedule (static, ");
for_loop_talkad7420();
  for (int i = 0; i < 10; ++i) foo();
{
}
#pragma omp target
#pragma omp teams
omp_for_pragma_talkad7420("#pragma omp distribute parallel for simd dist_schedule (argc)) ");
for_loop_talkad7420();
  for (int i = 0; i < 10; ++i) foo();
{
}
#pragma omp target
#pragma omp teams
omp_for_pragma_talkad7420("#pragma omp distribute parallel for simd dist_schedule (static, argc > 0 ? argv[1] : argv[2]) ");
for_loop_talkad7420();
  for (int i = 0; i < 10; ++i) foo();
{
}
#pragma omp target
#pragma omp teams
omp_for_pragma_talkad7420("#pragma omp distribute parallel for simd dist_schedule (static), dist_schedule (static, 1) ");
for_loop_talkad7420();
  for (int i = 0; i < 10; ++i) foo();
{
}
#pragma omp target
#pragma omp teams
omp_for_pragma_talkad7420("#pragma omp distribute parallel for simd dist_schedule (static, s1) ");
for_loop_talkad7420();
  for (int i = 0; i < 10; ++i) foo();
{
}
#pragma omp target
#pragma omp teams
omp_for_pragma_talkad7420("#pragma omp distribute parallel for simd dist_schedule (static, argv[1]=2) ");
for_loop_talkad7420();
  for (int i = 0; i < 10; ++i) foo();
{
}
  return (tmain<int, 5>(argc) + tmain<char, 1>(argv[0][0])); 
}