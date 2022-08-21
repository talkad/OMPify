#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <math.h>
#include <omp.h>

int gaussElim(double *A, double *b, double *det, int N)
{
	int i,j,k,flag;
	double temp,ratio;
	double tol = 1e-30;
	int npivot = 0;
	*det = 1.0;
    omp_set_num_threads(2);

	/* Forward ELimination */
    for (k=0; k<N; k++) { 
        for (i=k+1; i<N; i++){
            if(fabs(A[i*N+k]) > fabs(A[k*N+k])) {
                npivot++;
                /* Interchange of Rows */
                for (j=0; j<N; j++) {
                    temp = A[i*N+j];
                    A[i*N+j] = A[k*N+j];
                    A[k*N+j] = temp;
                }
                temp = b[i];
                b[i] = b[k];
                b[k] = temp;
            }
        }
        /* Singularity Test */
        if (fabs(A[k*N+k]) < tol) {
            flag = 1;
            return flag;
        }

        *det = *det * A[k*N+k];
    #pragma omp parallel
    {
        #pragma omp for 
            for (i=k+1; i<N; i++) {
                ratio = A[i*N+k]/A[k*N+k];
                b[i] = b[i] - b[k] * ratio;
                for (j=k; j<N; j++) {
                    A[i*N+j] = A[i*N+j] - A[k*N+j] * ratio;
                }
            }
        }
    }
	/* Adjust determinant */
	if (npivot % 2 == 1)
		*det = *det * (-1.0);
	/* BackSubstitution */
	b[N] = b[N] / A[N*N+N];
   //#pragma omp parallel
	for (i=N-1; i>1; i--) {
		for (j=N; j>i+1; j--) {
			b[i] = b[i] - A[i*N+j] * b[j];
		}
		b[i] = b[i]/A[(i-1)*N+i];
	} 
	flag = 0;
	return flag;
}

int main(int argc, char ** argv)
{
	int i,j,flag,option;
	int N = 10;
	struct timeval t1, t2, result;
	double det;
	unsigned int seed = 2039478;
	srand48(seed);

	while ((option = getopt(argc, argv, "n:")) != -1) {
		switch(option) {

			case 'n':
				N = atoi(optarg);
				break;

			default:
				printf("Enter number of linear equations to solve.\n");
				exit(-1);
		}
	}

	double *A = (double*)malloc(N*N*sizeof(double));;
	for (i=0; i<N; i++) {
		for (j=0; j<N; j++) {
			A[i*N+j] = lrand48() % 5;
		}
	}

	double *b = malloc(N*sizeof(double));
	for (i=0; i<N; i++) {
		b[i] = lrand48() % 5;
	} 	

	gettimeofday(&t1, NULL);
	flag = gaussElim(A,b,&det,N);
	gettimeofday(&t2, NULL);

	timersub(&t2, &t1, &result);
	printf("\n\tElapsed Time = %ld Seconds %ld Microseconds\n\n", result.tv_sec, result.tv_usec);

	if (N < 30) {	
		if (flag == 0) {
			printf("\n\tSolution of Linear Equations\n");
			for (i=0; i<N; i++) {
				printf("\n\t x(%d) = %.1f ", i+1, b[i]);
			}
			printf("\n\tDeterminant = %.1f \n\n", det);
		}
		else printf("\n\tSingular Matrix\n");
	}

	free(b);
	free(A);
	return 0;
}                                                     
