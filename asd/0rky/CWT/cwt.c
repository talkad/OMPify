/*************************************************************************************/
/********************2D continous wavelet transform**************************************/
/* This is the main program for the 2D_CWT calculation, it uses fft technique to compute
the cwt coefficients. 

Author: Manas Jyoti Das, July:05:2016 */

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>
#include<sys/time.h>
#include<omp.h>
#include "cv.h"
#include "highgui.h"
#include "fftw3.h"

#include "filter.h"
#include "normfilter.h"

#define REAL 0
#define IMAG 1

float *filter_dx,*filter_dy;
#pragma omp threadprivate(filter_dx,filter_dy)

int main(void)
{
	IplImage* img=cvLoadImage("/home/manas/Pictures/lung.jpg",CV_LOAD_IMAGE_COLOR);
	IplImage* gray_img=cvCreateImage(cvGetSize(img),IPL_DEPTH_8U,1);
	
	int i,j,row,col,count=0;
	int temp,init_thread;
	struct timeval t0,t1,t_loop;
	long double elapsed;
	double time_spent;
	float scale[16],temp1;
	unsigned int img_dim,one_d;
	float *in,*inverse_dx,*inverse_dy,*filter_dx_rearrange,*filter_dy_rearrange;
	clock_t start = clock(), diff;
	const char *filename_image="/home/manas/Documents/wisdom_image";
	const char *filename_mult="/home/manas/Documents/wisdom_mult";
	
	img_dim=gray_img->width*gray_img->height;

	//init_thread=fftwf_init_threads();

	fftwf_complex *transform,*mult_filt_data;
	fftwf_plan plan_forward,plan_backward;

	cvCvtColor(img,gray_img,CV_RGB2GRAY);
	gettimeofday(&t0, 0);	

	in=(float*)fftwf_malloc(sizeof(float)*img_dim);
	transform=(fftwf_complex*)fftwf_malloc(sizeof(fftwf_complex)*(gray_img->height*(gray_img->width/2+1)));
		
	/*********Generate filter scale**************/

	for(temp1=1;temp1<4.1;temp1+=0.2)
	{
		scale[count]=pow(2,temp1); printf("scale is %f",scale[count]);
		count++;
	}
	fftwf_import_wisdom_from_filename(filename_image);
	plan_forward=fftwf_plan_dft_r2c_2d(gray_img->height,gray_img->width,in,transform,FFTW_MEASURE);
	/*if(fftwf_export_wisdom_to_filename(filename_image))
		printf("\nexported successfully wisdom\n");
	else
		printf("\nun-successfully wisdom export\n");*/
	count=0;
	for(row=0;row<gray_img->height;row++)
	{
		const uchar* ptr=(const uchar*)(gray_img->imageData+row*gray_img->widthStep);
		for(col=0;col<gray_img->width;col++)
		{
			in[count]=*ptr++;
			count++;
		}
	}


	fftwf_execute(plan_forward);
	//printf(" [%f %fi] ",transform[255*126][REAL],transform[255*126][IMAG]);	
	
	//plan_backward=fftwf_plan_dft_c2r_2d(gray_img->height,gray_img->width,mult_filt_data,inverse_dy,FFTW_MEASURE);
	omp_set_num_threads(4);
	# pragma omp parallel shared(transform,gray_img,scale,img_dim) private(i,filter_dx_rearrange,filter_dy_rearrange,inverse_dx,inverse_dy,mult_filt_data,temp,plan_backward,t_loop) 
	{
	
	filter_dx=(float*)fftwf_malloc(sizeof(float)*img_dim);
	filter_dy=(float*)fftwf_malloc(sizeof(float)*img_dim);	
	inverse_dx=(float*)fftwf_malloc(sizeof(float)*img_dim);
	inverse_dy=(float*)fftwf_malloc(sizeof(float)*img_dim);
	filter_dx_rearrange=(float*)fftwf_malloc(sizeof(float)*(gray_img->height*(gray_img->width/2+1)));
	filter_dy_rearrange=(float*)fftwf_malloc(sizeof(float)*(gray_img->height*(gray_img->width/2+1)));	
	
	mult_filt_data=(fftwf_complex*)fftwf_malloc(sizeof(fftwf_complex)*(gray_img->height*(gray_img->width/2+1)));

	#pragma omp critical
	{	fftwf_import_wisdom_from_filename(filename_mult);	
		plan_backward=fftwf_plan_dft_c2r_2d(gray_img->height,gray_img->width,mult_filt_data,inverse_dy,FFTW_MEASURE); 
		/*if(fftwf_export_wisdom_to_filename(filename_image))
			printf("\nexported successfully wisdom\n");
		else
			printf("\nun-successfully wisdom export\n");*/
	}
	#pragma omp for 
	for(temp=0;temp<16;temp++)
	{

	gettimeofday(&t_loop,0);

	filter(filter_dx,filter_dy,gray_img->height,gray_img->width,scale[temp]);	

	/**********rearrange the filters for r2c hermitian symmetric data************/

	memcpy(filter_dx_rearrange,filter_dx,sizeof(int)*(gray_img->width/2+1));
	memcpy(filter_dy_rearrange,filter_dy,sizeof(int)*(gray_img->width/2+1));

	for(i=1;i<=(gray_img->height-1);i++)
	{
		memcpy(filter_dx_rearrange+((i-1)*(gray_img->width/2+1)+(gray_img->width/2+1)),filter_dx+i*gray_img->width,sizeof(int)*(gray_img->width/2+1));
		memcpy(filter_dy_rearrange+((i-1)*(gray_img->width/2+1)+(gray_img->width/2+1)),filter_dy+i*gray_img->width,sizeof(int)*(gray_img->width/2+1));
	}


	//********** filter dx multiplication with data start *********//
	
	//plan_backward=fftwf_plan_dft_c2r_2d(gray_img->height,gray_img->width,mult_filt_data,inverse_dx,FFTW_ESTIMATE);

	for(i=0;i<(gray_img->height*(gray_img->width/2+1));i++)
	{
		mult_filt_data[i][REAL]= -filter_dx_rearrange[i]*transform[i][IMAG];
		mult_filt_data[i][IMAG]= filter_dx_rearrange[i]*transform[i][REAL];
	}

	fftwf_execute(plan_backward);
	
	for(i=0;i<img_dim;i++)
	{
		inverse_dx[i]=inverse_dy[i]/img_dim;
	}
	//********** filter dx multiplication with data over *********//

	//********** filter dy multiplication with data start *********//

	//plan_backward=fftwf_plan_dft_c2r_2d(gray_img->height,gray_img->width,mult_filt_data,inverse_dy,FFTW_ESTIMATE);

	for(i=0;i<(gray_img->height*(gray_img->width/2+1));i++)
	{
		mult_filt_data[i][REAL]=-filter_dy_rearrange[i]*transform[i][IMAG];
		mult_filt_data[i][IMAG]=filter_dy_rearrange[i]*transform[i][REAL];
	}

	fftwf_execute(plan_backward);
		
	for(i=0;i<img_dim;i++)
	{
		inverse_dy[i]=inverse_dy[i]/img_dim;
	}
	//********** filter dx multiplication with data over *********//

	//reusing filter_dx to hold magnitude and filter_dy to hold the angle, or i can use aliasing which is a good idea//
	for(i=0;i<img_dim;i++)
	{
		filter_dx[i]=sqrt(pow(inverse_dx[i],2)+pow(inverse_dy[i],2));
	}

	gettimeofday(&t1, 0);
	elapsed = (t1.tv_sec-t_loop.tv_sec)*1000000 + t1.tv_usec-t_loop.tv_usec;
	printf("\n time wall for scale %f %Lf thread ID: %d\n",scale[temp],elapsed/1000000,omp_get_thread_num());

	printf(" the magnitude is %f for scale %d\n",filter_dx[0],temp);

	}

	fftwf_free(filter_dy_rearrange);
	fftwf_free(filter_dx_rearrange);
	fftwf_free(inverse_dx);
	fftwf_free(inverse_dy);
	fftwf_free(mult_filt_data);	
	fftwf_free(filter_dx);
	fftwf_free(filter_dy);
	fftwf_destroy_plan(plan_backward);	
	
	 /* end of for loop of scales */
	}

	gettimeofday(&t1, 0);
	elapsed = (t1.tv_sec-t0.tv_sec)*1000000 + t1.tv_usec-t0.tv_usec;
	printf("\n time wall %Lf\n",elapsed/1000000);
	time_spent = (double)(clock() - start) / CLOCKS_PER_SEC;
	printf("\n Time spent %f\n",time_spent);

	fftwf_destroy_plan(plan_forward);
	fftwf_free(in);	
	fftwf_free(transform);

	cvReleaseImage(&img);
	cvReleaseImage(&gray_img);
	
	return 0;
	
}	

