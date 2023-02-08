#include <stdlib.h>
#include <stdio.h>

#include <unistd.h>
#include <time.h>
#include <sys/time.h>

#include "pngwriter.h"
#include "consts.h"


unsigned long get_time()
{
    struct timeval tp;
    gettimeofday (&tp, NULL);
    return tp.tv_sec * 1000000 + tp.tv_usec;
}

int main(int argc, char** argv)
{
	png_data* pPng = png_create(IMAGE_WIDTH, IMAGE_HEIGHT);

	double x, y, x2, y2, cx, cy;
	cy = MIN_Y;

	double fDeltaX = (MAX_X - MIN_X) / (double) IMAGE_WIDTH;
	double fDeltaY = (MAX_Y - MIN_Y) / (double) IMAGE_HEIGHT;

	long nTotalIterationsCount = 0;
  long localIterationsCount = 0;
  long i = 0;
  long n = 0;
	unsigned long nTimeStart = get_time();

	long j,z,c;

  n = 0;
	// do the calculation
  #pragma omp parallel\
  firstprivate(localIterationsCount,c,cy,i,n,fDeltaX,fDeltaY,j, x, y, x2, y2,cx)\
   shared (pPng,nTotalIterationsCount)
  {
  #pragma omp for schedule(static)
	for (j = 0; j < IMAGE_HEIGHT; j++)
	{

    cy = MIN_Y + j*fDeltaY;
    cx = MIN_X;
		for (i = 0; i < IMAGE_WIDTH; i++)
		{
      x = 0;
      y = 0;
			//x = cx;
			//y = cy;
			x2 = x * x;
			y2 = y * y;
      cx = MIN_X + i*fDeltaX;
      for(n=0; n < MAX_ITERS  && ((x2 + y2)) < 4; n++){
        y = 2*x*y + cy;
        x = x2 - y2 + cx;
        x2 = x*x;
        y2 = y*y;
        localIterationsCount++;
      }

			// plot the number of iterations at point (i, j)
      /*if(n == MAX_ITERS){
          #pragma omp critical
          png_plot(pPng, i, j, 255, 255, 255);
      }
      else{
          #pragma omp critical
          png_plot(pPng, i, j, 0, 0, 0);
      }*/
			int c = ((long) n * 255) / MAX_ITERS;
      #pragma omp critical
			png_plot (pPng, i, j, c, c, c);

			//cx += fDeltaX;
		}
		//cy += fDeltaY;
	}
    #pragma omp critical
    nTotalIterationsCount += localIterationsCount;
  }
  //#pragma omp critical{
  //  nTotalIterationsCount += localIterationsCount;
  //}}


	unsigned long nTimeEnd = get_time();

	// print benchmark data
	printf("Total time:                 %g ms\n", (nTimeEnd - nTimeStart) / 1000.0);
	printf("Image size:                 %ld x %ld = %ld Pixels\n",
	(long) IMAGE_WIDTH, (long) IMAGE_HEIGHT, (long) (IMAGE_WIDTH * IMAGE_HEIGHT));
	printf("Total number of iterations: %ld\n", nTotalIterationsCount);
	printf("Avg. time per pixel:        %g µs\n", (nTimeEnd - nTimeStart) / (double) (IMAGE_WIDTH * IMAGE_HEIGHT));
	printf("Avg. time per iteration:    %g µs\n", (nTimeEnd - nTimeStart) / (double) nTotalIterationsCount);
	printf("Iterations/second:          %g\n", nTotalIterationsCount / (double) (nTimeEnd - nTimeStart) * 1e6);
	// assume there are 8 floating point operations per iteration
	printf("MFlop/s:                    %g\n", nTotalIterationsCount * 8.0 / (double) (nTimeEnd - nTimeStart));

	png_write(pPng, "mandel.png");
	return 0;
}
