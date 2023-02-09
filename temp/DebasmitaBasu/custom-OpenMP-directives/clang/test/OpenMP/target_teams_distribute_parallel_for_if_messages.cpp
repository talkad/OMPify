#include "_fake_typedefs.h"
#include "_fake_defines.h"
#include "patch.h"
void foo() {
}
bool foobool(int argc) {
  return argc;
}
struct S1; 
template <class T, class S> 
int tmain(T argc, S **argv) {
  T i;
omp_for_pragma_talkad7420("#pragma omp target teams distribute parallel for if ");
for_loop_talkad7420();
  for (i = 0; i < argc; ++i) foo();
{
}
omp_for_pragma_talkad7420("#pragma omp target teams distribute parallel for if ( ");
for_loop_talkad7420();
  for (i = 0; i < argc; ++i) foo();
{
}
omp_for_pragma_talkad7420("#pragma omp target teams distribute parallel for if () ");
for_loop_talkad7420();
  for (i = 0; i < argc; ++i) foo();
{
}
omp_for_pragma_talkad7420("#pragma omp target teams distribute parallel for if (argc ");
for_loop_talkad7420();
  for (i = 0; i < argc; ++i) foo();
{
}
omp_for_pragma_talkad7420("#pragma omp target teams distribute parallel for if (argc)) ");
for_loop_talkad7420();
  for (i = 0; i < argc; ++i) foo();
{
}
omp_for_pragma_talkad7420("#pragma omp target teams distribute parallel for if (argc > 0 ? argv[1] : argv[2])");
for_loop_talkad7420();
  for (i = 0; i < argc; ++i) foo();
{
}
omp_for_pragma_talkad7420("#pragma omp target teams distribute parallel for if (foobool(argc)), if (true) ");
for_loop_talkad7420();
  for (i = 0; i < argc; ++i) foo();
{
}
omp_for_pragma_talkad7420("#pragma omp target teams distribute parallel for if (s) ");
for_loop_talkad7420();
  for (i = 0; i < argc; ++i) foo();
{
}
omp_for_pragma_talkad7420("#pragma omp target teams distribute parallel for if (argv[1]=2) ");
for_loop_talkad7420();
  for (i = 0; i < argc; ++i) foo();
{
}
omp_for_pragma_talkad7420("#pragma omp target teams distribute parallel for if (argc argc) ");
for_loop_talkad7420();
  for (i = 0; i < argc; ++i) foo();
{
}
omp_for_pragma_talkad7420("#pragma omp target teams distribute parallel for if(argc)");
for_loop_talkad7420();
  for (i = 0; i < argc; ++i) foo();
{
}
omp_for_pragma_talkad7420("#pragma omp target teams distribute parallel for if(parallel ");
for_loop_talkad7420();
  for (i = 0; i < argc; ++i) foo();
{
}
omp_for_pragma_talkad7420("#pragma omp target teams distribute parallel for if(parallel : ");
for_loop_talkad7420();
  for (i = 0; i < argc; ++i) foo();
{
}
omp_for_pragma_talkad7420("#pragma omp target teams distribute parallel for if(parallel : argc ");
for_loop_talkad7420();
  for (i = 0; i < argc; ++i) foo();
{
}
omp_for_pragma_talkad7420("#pragma omp target teams distribute parallel for if(parallel : argc)");
for_loop_talkad7420();
  for (i = 0; i < argc; ++i) foo();
{
}
omp_for_pragma_talkad7420("#pragma omp target teams distribute parallel for if(parallel : argc) if (for:argc) ");
for_loop_talkad7420();
  for (i = 0; i < argc; ++i) foo();
{
}
omp_for_pragma_talkad7420("#pragma omp target teams distribute parallel for if(parallel : argc) if (parallel:argc) ");
for_loop_talkad7420();
  for (i = 0; i < argc; ++i) foo();
{
}
omp_for_pragma_talkad7420("#pragma omp target teams distribute parallel for if(target : argc) if (target:argc) ");
for_loop_talkad7420();
  for (i = 0; i < argc; ++i) foo();
{
}
omp_for_pragma_talkad7420("#pragma omp target teams distribute parallel for if(parallel : argc) if (argc) ");
for_loop_talkad7420();
  for (i = 0; i < argc; ++i) foo();
{
}
omp_for_pragma_talkad7420("#pragma omp target teams distribute parallel for if(target: argc) if (argc) ");
for_loop_talkad7420();
  for (i = 0; i < argc; ++i) foo();
{
}
omp_for_pragma_talkad7420("#pragma omp target teams distribute parallel for if(distribute : argc) ");
for_loop_talkad7420();
  for (i = 0; i < argc; ++i) foo();
{
}
  return 0;
}
int main(int argc, char **argv) {
  int i;
omp_for_pragma_talkad7420("#pragma omp target teams distribute parallel for if ");
for_loop_talkad7420();
  for (i = 0; i < argc; ++i) foo();
{
}
omp_for_pragma_talkad7420("#pragma omp target teams distribute parallel for if ( ");
for_loop_talkad7420();
  for (i = 0; i < argc; ++i) foo();
{
}
omp_for_pragma_talkad7420("#pragma omp target teams distribute parallel for if () ");
for_loop_talkad7420();
  for (i = 0; i < argc; ++i) foo();
{
}
omp_for_pragma_talkad7420("#pragma omp target teams distribute parallel for if (argc ");
for_loop_talkad7420();
  for (i = 0; i < argc; ++i) foo();
{
}
omp_for_pragma_talkad7420("#pragma omp target teams distribute parallel for if (argc)) ");
for_loop_talkad7420();
  for (i = 0; i < argc; ++i) foo();
{
}
omp_for_pragma_talkad7420("#pragma omp target teams distribute parallel for if (argc > 0 ? argv[1] : argv[2])");
for_loop_talkad7420();
  for (i = 0; i < argc; ++i) foo();
{
}
omp_for_pragma_talkad7420("#pragma omp target teams distribute parallel for if (foobool(argc)), if (true) ");
for_loop_talkad7420();
  for (i = 0; i < argc; ++i) foo();
{
}
omp_for_pragma_talkad7420("#pragma omp target teams distribute parallel for if (s1) ");
for_loop_talkad7420();
  for (i = 0; i < argc; ++i) foo();
{
}
omp_for_pragma_talkad7420("#pragma omp target teams distribute parallel for if (argv[1]=2) ");
for_loop_talkad7420();
  for (i = 0; i < argc; ++i) foo();
{
}
omp_for_pragma_talkad7420("#pragma omp target teams distribute parallel for if (argc argc) ");
for_loop_talkad7420();
  for (i = 0; i < argc; ++i) foo();
{
}
omp_for_pragma_talkad7420("#pragma omp target teams distribute parallel for if (1 0) ");
for_loop_talkad7420();
  for (i = 0; i < argc; ++i) foo();
{
}
omp_for_pragma_talkad7420("#pragma omp target teams distribute parallel for if(if(tmain(argc, argv) ");
for_loop_talkad7420();
  for (i = 0; i < argc; ++i) foo();
{
}
omp_for_pragma_talkad7420("#pragma omp target teams distribute parallel for if(parallel ");
for_loop_talkad7420();
  for (i = 0; i < argc; ++i) foo();
{
}
omp_for_pragma_talkad7420("#pragma omp target teams distribute parallel for if(parallel : ");
for_loop_talkad7420();
  for (i = 0; i < argc; ++i) foo();
{
}
omp_for_pragma_talkad7420("#pragma omp target teams distribute parallel for if(parallel : argc ");
for_loop_talkad7420();
  for (i = 0; i < argc; ++i) foo();
{
}
omp_for_pragma_talkad7420("#pragma omp target teams distribute parallel for if(parallel : argc)");
for_loop_talkad7420();
  for (i = 0; i < argc; ++i) foo();
{
}
omp_for_pragma_talkad7420("#pragma omp target teams distribute parallel for if(parallel : argc) if (for:argc) ");
for_loop_talkad7420();
  for (i = 0; i < argc; ++i) foo();
{
}
omp_for_pragma_talkad7420("#pragma omp target teams distribute parallel for if(parallel : argc) if (parallel:argc) ");
for_loop_talkad7420();
  for (i = 0; i < argc; ++i) foo();
{
}
omp_for_pragma_talkad7420("#pragma omp target teams distribute parallel for if(target: argc) if (target:argc) ");
for_loop_talkad7420();
  for (i = 0; i < argc; ++i) foo();
{
}
omp_for_pragma_talkad7420("#pragma omp target teams distribute parallel for if(parallel : argc) if (argc) ");
for_loop_talkad7420();
  for (i = 0; i < argc; ++i) foo();
{
}
omp_for_pragma_talkad7420("#pragma omp target teams distribute parallel for if(target: argc) if (argc) ");
for_loop_talkad7420();
  for (i = 0; i < argc; ++i) foo();
{
}
omp_for_pragma_talkad7420("#pragma omp target teams distribute parallel for if(distribute : argc) ");
for_loop_talkad7420();
  for (i = 0; i < argc; ++i) foo();
{
}
  return tmain(argc, argv);
}