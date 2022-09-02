/*
 OpenMP sort
 Albert Szmigielski
	alberts@alumni.sfu.ca
 to compile: g++ -g  par_omp_sort.cpp -o par_omp_sort -fopenmp 
 usage: ./par_omp_sort <number of threads> <size of list>
 **<number of threads> must be power of two
 

*/
#include <algorithm>
#include <math.h>
#include <iostream>
#include <omp.h>

using namespace std;


int Num_Threads=4; 	//default is 4 can be changed via argument
int size =1000000;	//default is 1M can be chaged via argument

/**************************    PAR MERGE    ******************************/
void par_merge(float * Array, int num_lists, int mul, int interval){
		//merge until 1 list left
		//merge list 1&2, 3&4, ... (n-1 & n)
	
	mul = mul*2;
	int par_merge_threads = num_lists/2;
	if (num_lists > 1){
		#pragma omp parallel num_threads(par_merge_threads)
		{
			int start = interval * omp_get_thread_num() *mul;
			int end = interval * (omp_get_thread_num()+1) *mul ;
			int middle = floor(start+ (end - start)/2);
			if (omp_get_thread_num() == par_merge_threads-1) 
				{ end=size; }
			inplace_merge(&Array[start], &Array[middle], &Array[end]);			
		}
		num_lists = ceil(num_lists/2);
		par_merge (Array, num_lists, mul, interval);			
	}		
}	

/**************************    PAR SORT    ******************************/
void par_sort (float * Array) {
	// break into num_threads chunks, sort each chunk
	int interval = floor(size/Num_Threads);
	#pragma omp parallel num_threads(Num_Threads)
	{	
		int start = interval * omp_get_thread_num();
		int end = interval * (omp_get_thread_num()+1) ;
		if (omp_get_thread_num() == Num_Threads-1) 
			{end=size;}
		sort (&Array[start], &Array[end]);		
	}
	par_merge(Array, Num_Threads, 1, interval);
}


/****************************   MAIN   **********************************/
int main (int argc, char** argv) {
double start_s, start_p, end_s, end_p;
float  * Array, * Array1;

if (argc>1)  Num_Threads= atoi(argv[1]);
if (argc>2)  size = atoi(argv[2]);


int range = 100, error=0;
Array = new float [size];
Array1 = new float [size];

#pragma omp parallel for
	for (int i=0; i<size; i++){	
		#pragma omp critical
		Array[i]  = (rand()/(RAND_MAX +1.0));
		Array1[i]  = Array[i];
	}


//time sequential sort
start_s =  omp_get_wtime( );
	sort(&Array1[0], &Array1[size]);
end_s =  omp_get_wtime( );

//time parallel sort
start_p =  omp_get_wtime( );
	par_sort(Array);
end_p =  omp_get_wtime( );

//check correctness
my_awesome_function("#pragma omp parallel for ");
#pragma omp parallel for 
	for (int i=0; i<size;i++){	
				for(i = 1; i < 3; i++){			

		if (Array[i] != Array1[i]){
			cout << " Error " << "position "<< i ;
			error = 1;
		}
				}
	}


if (error) cout << "ERROR \n ";
else cout << "OK \n";
cout << "Sorted " << size << " numbers with seq. sort in "<< end_s - start_s << " seconds\n" ;
cout << "Sorted " << size << " numbers with par. sort in "<< end_p - start_p << " seconds\n" ;
//cout << Num_Threads << ", " << size << ", " << (end_s - start_s) << ", " << (end_p - start_p) <<"\n";
return 0;
}
