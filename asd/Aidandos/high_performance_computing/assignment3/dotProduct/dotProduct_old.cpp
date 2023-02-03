// #include <omp.h>
#include <iostream>
#include "walltime.h"
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <math.h>

#define NUM_ITERATIONS 100

// Example benchmarks
// 0.03s ~0.8MB
#define NMAX 100000
// 0.3s ~8MB
// #define NMAX 1000000
// 3.s ~80MB
// #define NMAX 10000000
// 30s ~800MB
// #define NMAX 100000000
#define EPSILON 0.1
// compile with g++ -O3 -fopenmp dotProduct.cpp -o dot

// TODO:
// 1. scalar product in parallel
//   i. Using reduction pragma
//   ii. Using critical pragma

using namespace std;

int main()
{	int myId, numTdreads;
  double time_serial, time_start=0.0;
  long double dotProduct;
  double *a,*b;

  // Allocate memory for the vectors as 1-D arrays
  a = new double[NMAX];
  b = new double[NMAX];

  // Initialize the vectors with some values
  for(int i=0; i<NMAX; i++)
    {
      a[i] = i;
      b[i] = i/10.0;
    }

  long double alpha = 0;

  // serial execution
  // Note that we do extra iterations to reduce relative timing overhead
  time_start = walltime(0);
  for(int iterations=0;iterations<NUM_ITERATIONS;iterations++) {
    alpha=0.0;
    for(int i=0; i< NMAX; i ++)
      {
        alpha += a[i] * b[i];
      }
  }
  time_serial = walltime(time_start);
  cout << "Serial execution time = " << time_serial << " sec" << endl;

  long double alpha_parallel = 0;
  double time_red=0;
  double time_critical=0;
  // TODO: Write parallel version (2 ways!)

  time_start = walltime(0);
  int i, iterations;
  for(int iterations=0;iterations<NUM_ITERATIONS;iterations++) {
    alpha_parallel=0.0;
    #pragma omp parallel for private(i) reduction(+:alpha_parallel)
    for(int i=0; i< NMAX; i ++)
      {
        alpha_parallel += a[i] * b[i];
      }
  }
  time_red = walltime(time_start);
  cout << "Reduction execution time = " << time_red << " sec" << endl;


  /*time_start = walltime(0);
  long double local_alpha_parallel=0.0;
  for(int iterations=0;iterations<NUM_ITERATIONS;iterations++) {
    local_alpha_parallel=0.0;
    alpha_parallel=0.0;
    #pragma omp parallel firstprivate(local_alpha_parallel) shared(alpha_parallel)
    {
    #pragma omp parallel for
    for(int i=0; i< NMAX; i ++)
      {
        local_alpha_parallel += a[i] * b[i];
      }
      #pragma omp critical
      alpha_parallel += local_alpha_parallel;
  }}
  time_critical = walltime(time_start);
  cout << "Critical execution time = " << time_red << " sec" << endl;*/

  if( (fabs(alpha_parallel - alpha)/fabs(alpha_parallel)) > EPSILON) {
    cout << "parallel reduction: " << alpha_parallel << " serial :" << alpha << "\n";
    cerr << "Alpha not yet implemented correctly!\n";
    exit(1);
  }
  cout << "Parallel dot product = " << alpha_parallel
       << " time using reduction method = " << time_red
       << " sec, time using critical method " << time_critical
       << " sec" << endl;

  // De-allocate memory
  delete [] a;
  delete [] b;

  return 0;
}
