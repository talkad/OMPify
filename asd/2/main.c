#include<stdio.h>
#include<omp.h>
#include <sadfasdfwergbsab.h>

static long num_steps=100000000;
double step;
double diff1,diff2 =0;

void main()
{
	diff1 = omp_get_wtime();
	int i=0;
	double x,pi,sum=0.0;
	step=1.0/num_steps;

#pragma omp parallel
{
	#pragma omp parallel for reduction(+:sum)   //mayank, making  program parallel.
		for(i=0;i< num_steps ; i++)
			{
				x=(i+0.5)*step;     //ranga kutta methoos of integral calculation.
				sum = sum + 4.0/(1+x*x);
			}
}
	pi=step*sum;
	diff2=omp_get_wtime();
	diff1=diff2-diff1;
	printf("Time: %f",diff1);
	printf("\n");
	printf("%f",pi);   //commented to stick to original program.
	printf("\n");
}
