#include "_fake_typedefs.h"
#include "_fake_defines.h"
#include "patch.h"
void foo() {
}
#if __cplusplus >= 201103L
#endif
bool foobool(int argc) {
  return argc;
}
struct S1; 
template <class T, typename S, int N, int ST> 
T tmain(T argc, S **argv) { 
#pragma omp target
omp_for_pragma_talkad7420("#pragma omp teams distribute parallel for collapse ");
for_loop_talkad7420();
  for (int i = ST; i < N; i++)
{
    argv[0][i] = argv[0][i] - argv[0][i-ST];
}
#pragma omp target
omp_for_pragma_talkad7420("#pragma omp teams distribute parallel for collapse ( ");
for_loop_talkad7420();
  for (int i = ST; i < N; i++)
{
    argv[0][i] = argv[0][i] - argv[0][i-ST];
}
#pragma omp target
omp_for_pragma_talkad7420("#pragma omp teams distribute parallel for collapse () ");
for_loop_talkad7420();
  for (int i = ST; i < N; i++)
{
    argv[0][i] = argv[0][i] - argv[0][i-ST];
}
#pragma omp target
omp_for_pragma_talkad7420("#pragma omp teams distribute parallel for collapse (argc ");
for_loop_talkad7420();
  for (int i = ST; i < N; i++)
{
    argv[0][i] = argv[0][i] - argv[0][i-ST];
}
#pragma omp target
omp_for_pragma_talkad7420("#pragma omp teams distribute parallel for collapse (st ");
for_loop_talkad7420();
  for (int i = ST; i < N; i++)
{
    argv[0][i] = argv[0][i] - argv[0][i-ST];
}
#pragma omp target
omp_for_pragma_talkad7420("#pragma omp teams distribute parallel for collapse (1)) ");
for_loop_talkad7420();
  for (int i = ST; i < N; i++)
{
    argv[0][i] = argv[0][i] - argv[0][i-ST];
}
#pragma omp target
omp_for_pragma_talkad7420("#pragma omp teams distribute parallel for collapse ((st > 0) ? 1 + st : 2) ");
for_loop_talkad7420();
  for (int i = ST; i < N; i++)
{
    argv[0][i] = argv[0][i] - argv[0][i-ST]; 
}
#if __cplusplus >= 201103L
#endif
#pragma omp target
omp_for_pragma_talkad7420("#pragma omp teams distribute parallel for collapse (foobool(argc)), collapse (true), collapse (-5)");
for_loop_talkad7420();
  for (int i = ST; i < N; i++)
{
    argv[0][i] = argv[0][i] - argv[0][i-ST];
}
#pragma omp distribute collapse (S) 
for_loop_talkad7420();
  for (int i = ST; i < N; i++)
{
    argv[0][i] = argv[0][i] - argv[0][i-ST];
}
#if __cplusplus >= 201103L
#else
#endif
#pragma omp target
omp_for_pragma_talkad7420("#pragma omp teams distribute parallel for collapse (argv[1]=2) ");
for_loop_talkad7420();
  for (int i = ST; i < N; i++)
{
    argv[0][i] = argv[0][i] - argv[0][i-ST];
}
#pragma omp target
omp_for_pragma_talkad7420("#pragma omp teams distribute parallel for collapse (1)");
for_loop_talkad7420();
  for (int i = ST; i < N; i++)
{
    argv[0][i] = argv[0][i] - argv[0][i-ST];
}
#pragma omp target
omp_for_pragma_talkad7420("#pragma omp teams distribute parallel for collapse (n) ");
for_loop_talkad7420();
  for (T i = ST; i < N; i++)
{
    argv[0][i] = argv[0][i] - argv[0][i-ST];
}
#pragma omp target
omp_for_pragma_talkad7420("#pragma omp teams distribute parallel for collapse (2) ");
  foo(); 
  return argc;
}
int main(int argc, char **argv) {
#pragma omp target
omp_for_pragma_talkad7420("#pragma omp teams distribute parallel for collapse ");
for_loop_talkad7420();
  for (int i = 4; i < 12; i++)
{
    argv[0][i] = argv[0][i] - argv[0][i-4];
}
#pragma omp target
omp_for_pragma_talkad7420("#pragma omp teams distribute parallel for collapse ( ");
for_loop_talkad7420();
  for (int i = 4; i < 12; i++)
{
    argv[0][i] = argv[0][i] - argv[0][i-4];
}
#pragma omp target
omp_for_pragma_talkad7420("#pragma omp teams distribute parallel for collapse () ");
for_loop_talkad7420();
  for (int i = 4; i < 12; i++)
{
    argv[0][i] = argv[0][i] - argv[0][i-4];
}
#pragma omp target
omp_for_pragma_talkad7420("#pragma omp teams distribute parallel for collapse (4 ");
for_loop_talkad7420();
  for (int i = 4; i < 12; i++)
{
    argv[0][i] = argv[0][i] - argv[0][i-4]; 
}
#pragma omp target
omp_for_pragma_talkad7420("#pragma omp teams distribute parallel for collapse (2+2)) ");
for_loop_talkad7420();
  for (int i = 4; i < 12; i++)
{
    argv[0][i] = argv[0][i] - argv[0][i-4]; 
}
#if __cplusplus >= 201103L
#endif
#pragma omp target
omp_for_pragma_talkad7420("#pragma omp teams distribute parallel for collapse (foobool(1) > 0 ? 1 : 2) ");
for_loop_talkad7420();
  for (int i = 4; i < 12; i++)
{
    argv[0][i] = argv[0][i] - argv[0][i-4];
}
#if __cplusplus >= 201103L
#endif
#pragma omp target
omp_for_pragma_talkad7420("#pragma omp teams distribute parallel for collapse (foobool(argc)), collapse (true), collapse (-5) ");
for_loop_talkad7420();
  for (int i = 4; i < 12; i++)
{
    argv[0][i] = argv[0][i] - argv[0][i-4];
}
#pragma omp target
omp_for_pragma_talkad7420("#pragma omp teams distribute parallel for collapse (s1) ");
for_loop_talkad7420();
  for (int i = 4; i < 12; i++)
{
    argv[0][i] = argv[0][i] - argv[0][i-4];
}
#if __cplusplus >= 201103L
#else
#endif
#pragma omp target
omp_for_pragma_talkad7420("#pragma omp teams distribute parallel for collapse (argv[1]=2) ");
for_loop_talkad7420();
  for (int i = 4; i < 12; i++)
{
    argv[0][i] = argv[0][i] - argv[0][i-4];
}
#pragma omp target
omp_for_pragma_talkad7420("#pragma omp teams distribute parallel for collapse(collapse(tmain<int, char, -1, -2>(argc, argv) ");
  foo();
#pragma omp target
omp_for_pragma_talkad7420("#pragma omp teams distribute parallel for collapse (2) ");
  foo(); 
  return tmain<int, char, 1, 0>(argc, argv);
}