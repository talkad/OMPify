#include "_fake_typedefs.h"
#include "_fake_defines.h"
#include "patch.h"
#ifndef HEADER
#define HEADER
long long get_val() { return 0; 
}
double *g_ptr;
void simple(float *a, float *b, float *c, float *d) {
omp_for_pragma_talkad7420("#pragma omp parallel for simd");
for_loop_talkad7420();
  for (int i = 3; i < 32; i += 5) {
    a[i] = b[i] * c[i] * d[i];
}
  long long k = get_val();
omp_for_pragma_talkad7420("#pragma omp parallel for simd linear(k : 3) schedule(dynamic)");
for_loop_talkad7420();
  for (int i = 10; i > 1; i--) {
    a[k]++;
    k = k + 3;
}
  int lin = 12;
omp_for_pragma_talkad7420("#pragma omp parallel for simd linear(lin : get_val()), linear(g_ptr)");
for_loop_talkad7420();
  for (unsigned long long it = 2000; it >= 600; it-=400) {
    *g_ptr++ = 0.0;
    a[it + lin]++;
}
omp_for_pragma_talkad7420("#pragma omp parallel for simd");
for_loop_talkad7420();
  for (short it = 6; it <= 20; it-=-4) {
}
omp_for_pragma_talkad7420("#pragma omp parallel for simd");
for_loop_talkad7420();
  for (unsigned char it = 'z'; it >= 'a'; it+=-1) {
}
omp_for_pragma_talkad7420("#pragma omp parallel for simd");
for_loop_talkad7420();
  for (unsigned i=100; i<10; i+=10) {
}
  int A;
  {
  A = -1;
omp_for_pragma_talkad7420("#pragma omp parallel for simd lastprivate(a)");
for_loop_talkad7420();
  for (long long i = -10; i < 10; i += 3) {
    A = i;
}
}
  int R;
  {
  R = -1;
omp_for_pragma_talkad7420("#pragma omp parallel for simd reduction(*:r)");
for_loop_talkad7420();
  for (long long i = -10; i < 10; i += 3) {
    R *= i;
}
}
}
template <class T, unsigned K> T tfoo(T a) { return a + K; 
}
template <typename T, unsigned N>
int templ1(T a, T *z) {
omp_for_pragma_talkad7420("#pragma omp parallel for simd collapse(n)");
for_loop_talkad7420();
  for (int i = 0; i < N * 2; i++) {
for_loop_talkad7420();
    for (long long j = 0; j < (N + N + N + N); j += 2) {
      z[i + j] = a + tfoo<T, N>(i + j);
}
}
  return 0;
}
void inst_templ1() {
  float a;
  float z[100];
  templ1<float,2> (a, z);
}
typedef int MyIdx;
class IterDouble {
  double *Ptr;
public:
  IterDouble operator++ () const {
    IterDouble n;
    n.Ptr = Ptr + 1;
    return n;
}
  bool operator < (const IterDouble &that) const {
    return Ptr < that.Ptr;
}
  double & operator *() const {
    return *Ptr;
}
  MyIdx operator - (const IterDouble &that) const {
    return (MyIdx) (Ptr - that.Ptr);
}
  IterDouble operator + (int Delta) {
    IterDouble re;
    re.Ptr = Ptr + Delta;
    return re;
}
};
void iter_simple(IterDouble ia, IterDouble ib, IterDouble ic) {
omp_for_pragma_talkad7420("#pragma omp parallel for simd");
for_loop_talkad7420();
  for (IterDouble i = ia; i < ib; ++i) {
   *i = *ic * 0.5;
   ++ic;
}
}
void collapsed(float *a, float *b, float *c, float *d) {
  int i; 
  unsigned j; 
  short l; 
omp_for_pragma_talkad7420("#pragma omp parallel for simd collapse(4)");
for_loop_talkad7420();
  for (i = 1; i < 3; i++) 
{
for_loop_talkad7420();
    for (j = 2u; j < 5u; j++) 
{
for_loop_talkad7420();
      for (int k = 3; k <= 6; k++) 
{
for_loop_talkad7420();
        for (l = 4; l < 9; ++l) 
        {
    float res = b[j] * c[k];
    a[i] = res * d[l];
}
}
}
}
}
extern char foo();
extern double globalfloat;
void widened(float *a, float *b, float *c, float *d) {
  int i; 
  short j; 
  globalfloat = 1.0;
  int localint = 1;
omp_for_pragma_talkad7420("#pragma omp parallel for simd collapse(2) private(globalfloat, localint)");
for_loop_talkad7420();
  for (i = 1; i < 3; i++) 
{
for_loop_talkad7420();
    for (j = 0; j < foo(); j++) 
  {
    globalfloat = (float)j/i;
    float res = b[j] * c[j];
    a[i] = res * d[i];
    localint = (int)j;
}
}
  localint = (int)j;
}
int bar() {return 0;
};
void parallel_simd(float *a) {
omp_for_pragma_talkad7420("#pragma omp parallel for simd");
for_loop_talkad7420();
  for (unsigned i = 131071; i <= 2147483647; i += 127)
{
    a[i] += bar();
}
}
#endif 