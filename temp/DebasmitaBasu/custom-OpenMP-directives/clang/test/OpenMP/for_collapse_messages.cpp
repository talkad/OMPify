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
omp_for_pragma_talkad7420("#pragma omp for collapse ");
for_loop_talkad7420();
  for (int i = ST; i < N; i++) argv[0][i] = argv[0][i] - argv[0][i-ST];
{
}
omp_for_pragma_talkad7420("#pragma omp for collapse ( ");
for_loop_talkad7420();
  for (int i = ST; i < N; i++) argv[0][i] = argv[0][i] - argv[0][i-ST];
{
}
omp_for_pragma_talkad7420("#pragma omp for collapse () ");
for_loop_talkad7420();
  for (int i = ST; i < N; i++) argv[0][i] = argv[0][i] - argv[0][i-ST];
{
}
omp_for_pragma_talkad7420("#pragma omp for collapse (argc ");
for_loop_talkad7420();
  for (int i = ST; i < N; i++) argv[0][i] = argv[0][i] - argv[0][i-ST];
{
}
omp_for_pragma_talkad7420("#pragma omp for collapse (st ");
for_loop_talkad7420();
  for (int i = ST; i < N; i++) argv[0][i] = argv[0][i] - argv[0][i-ST];
{
}
omp_for_pragma_talkad7420("#pragma omp for collapse (1)) ");
for_loop_talkad7420();
  for (int i = ST; i < N; i++) argv[0][i] = argv[0][i] - argv[0][i-ST];
{
}
omp_for_pragma_talkad7420("#pragma omp for collapse ((st > 0) ? 1 + st : 2) ");
for_loop_talkad7420();
  for (int i = ST; i < N; i++) argv[0][i] = argv[0][i] - argv[0][i-ST]; 
{
}
#if __cplusplus >= 201103L
#endif
omp_for_pragma_talkad7420("#pragma omp for collapse (foobool(argc)), collapse (true), collapse (-5)");
for_loop_talkad7420();
  for (int i = ST; i < N; i++) argv[0][i] = argv[0][i] - argv[0][i-ST];
{
}
omp_for_pragma_talkad7420("#pragma omp for collapse (s) ");
for_loop_talkad7420();
  for (int i = ST; i < N; i++) argv[0][i] = argv[0][i] - argv[0][i-ST];
{
}
#if __cplusplus <= 199711L
#else
#endif
omp_for_pragma_talkad7420("#pragma omp for collapse (argv[1]=2) ");
for_loop_talkad7420();
  for (int i = ST; i < N; i++) argv[0][i] = argv[0][i] - argv[0][i-ST];
{
}
omp_for_pragma_talkad7420("#pragma omp for collapse (1)");
for_loop_talkad7420();
  for (int i = ST; i < N; i++) argv[0][i] = argv[0][i] - argv[0][i-ST];
{
}
omp_for_pragma_talkad7420("#pragma omp for collapse (n) ");
for_loop_talkad7420();
  for (T i = ST; i < N; i++) argv[0][i] = argv[0][i] - argv[0][i-ST];
{
}
omp_for_pragma_talkad7420("#pragma omp for collapse (2) ");
  foo(); 
  return argc;
}
int main(int argc, char **argv) {
omp_for_pragma_talkad7420("#pragma omp for collapse ");
for_loop_talkad7420();
  for (int i = 4; i < 12; i++) argv[0][i] = argv[0][i] - argv[0][i-4];
{
}
omp_for_pragma_talkad7420("#pragma omp for collapse ( ");
for_loop_talkad7420();
  for (int i = 4; i < 12; i++) argv[0][i] = argv[0][i] - argv[0][i-4];
{
}
omp_for_pragma_talkad7420("#pragma omp for collapse () ");
for_loop_talkad7420();
  for (int i = 4; i < 12; i++) argv[0][i] = argv[0][i] - argv[0][i-4];
{
}
omp_for_pragma_talkad7420("#pragma omp for collapse (4 ");
for_loop_talkad7420();
  for (int i = 4; i < 12; i++) argv[0][i] = argv[0][i] - argv[0][i-4]; 
{
}
omp_for_pragma_talkad7420("#pragma omp for collapse (2+2)) ");
for_loop_talkad7420();
  for (int i = 4; i < 12; i++) argv[0][i] = argv[0][i] - argv[0][i-4]; 
{
}
#if __cplusplus >= 201103L
#endif
omp_for_pragma_talkad7420("#pragma omp for collapse (foobool(1) > 0 ? 1 : 2)");
for_loop_talkad7420();
  for (int i = 4; i < 12; i++) argv[0][i] = argv[0][i] - argv[0][i-4];
{
}
#if __cplusplus >= 201103L
#endif
omp_for_pragma_talkad7420("#pragma omp for collapse (foobool(argc)), collapse (true), collapse (-5) ");
for_loop_talkad7420();
  for (int i = 4; i < 12; i++) argv[0][i] = argv[0][i] - argv[0][i-4];
{
}
omp_for_pragma_talkad7420("#pragma omp for collapse (s1) ");
for_loop_talkad7420();
  for (int i = 4; i < 12; i++) argv[0][i] = argv[0][i] - argv[0][i-4];
{
}
#if __cplusplus <= 199711L
#else
#endif
omp_for_pragma_talkad7420("#pragma omp for collapse (argv[1]=2) ");
for_loop_talkad7420();
  for (int i = 4; i < 12; i++) argv[0][i] = argv[0][i] - argv[0][i-4];
{
}
omp_for_pragma_talkad7420("#pragma omp for collapse(collapse(tmain<int, char, -1, -2>(argc, argv) ");
  foo();
omp_for_pragma_talkad7420("#pragma omp for collapse (2) ");
  foo(); 
  return tmain<int, char, 1, 0>(argc, argv);
}