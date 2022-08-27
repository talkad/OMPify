#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include "cv.h"
#include "highgui.h"
#include "fftw3.h"

#define REAL 0
#define IMAG 1

void filter(float*,float*,int,int);
void norm_filter(float*,int,int);

int main(void)
{
	IplImage* img=cvLoadImage("/home/manas/Pictures/sample.jpg",CV_LOAD_IMAGE_COLOR);
	IplImage* gray_img=cvCreateImage(cvGetSize(img),IPL_DEPTH_8U,1);
	
	int i,j,row,col,count=0;
	unsigned int img_dim,one_d;
	float *filter_dx,*filter_dy;

	img_dim=gray_img->width*gray_img->height;

	fftwf_complex *in,*transform,*inverse_dx,*filter_gauss,*mult_filt_data,*inverse_dy;
	fftwf_plan plan_forward,plan_backward;

	cvCvtColor(img,gray_img,CV_RGB2GRAY);

	filter_dx=(float*)malloc(sizeof(float)*img_dim);
	filter_dy=(float*)malloc(sizeof(float)*img_dim);
	
	in=(fftwf_complex*)fftwf_malloc(sizeof(fftwf_complex)*img_dim);
	transform=(fftwf_complex*)fftwf_malloc(sizeof(fftwf_complex)*img_dim);
	filter_gauss=(fftwf_complex*)fftwf_malloc(sizeof(fftwf_complex)*img_dim);
	mult_filt_data=(fftwf_complex*)fftwf_malloc(sizeof(fftwf_complex)*img_dim);
	inverse_dx=(fftwf_complex*)fftwf_malloc(sizeof(fftwf_complex)*img_dim);
	inverse_dy=(fftwf_complex*)fftwf_malloc(sizeof(fftwf_complex)*img_dim);

	for(row=0;row<gray_img->height;row++)
	{
		const uchar* ptr=(const uchar*)(gray_img->imageData+row*gray_img->widthStep);
		for(col=0;col<gray_img->width;col++)
		{
			in[count][REAL]=*ptr++;
			in[count][IMAG]=0.0;
			count++;
		}
	}

	plan_forward=fftwf_plan_dft_2d(gray_img->height,gray_img->width,in,transform,FFTW_FORWARD,FFTW_ESTIMATE);

	fftwf_execute(plan_forward);
fftwf_free(in);
	filter(filter_dx,filter_dy,gray_img->height,gray_img->width);	

	//********** filter dx multiplication with data start *********//
	for(i=0;i<img_dim;i++)
	{
		filter_gauss[i][REAL]=0.0;
		filter_gauss[i][IMAG]=*filter_dx++;
	}

	for(i=0;i<img_dim;i++)
	{
		mult_filt_data[i][REAL]= -filter_gauss[i][IMAG]*transform[i][IMAG];
		mult_filt_data[i][IMAG]= filter_gauss[i][IMAG]*transform[i][REAL];
	}

	plan_backward=fftwf_plan_dft_2d(gray_img->height,gray_img->width,mult_filt_data,inverse_dx,FFTW_BACKWARD,FFTW_ESTIMATE);

	fftwf_execute(plan_backward);fftwf_destroy_plan(plan_forward);

	for(i=0;i<img_dim;i++)
	{
		inverse_dx[i][REAL]=inverse_dx[i][REAL]/img_dim;
	}
	//********** filter dx multiplication with data over *********//

	//********** filter dy multiplication with data start *********//

	for(i=0;i<img_dim;i++)
	{
		filter_gauss[i][REAL]=0.0;
		filter_gauss[i][IMAG]=*filter_dy++;
	}

	for(i=0;i<img_dim;i++)
	{
		mult_filt_data[i][REAL]=-filter_gauss[i][IMAG]*transform[i][IMAG];
		mult_filt_data[i][IMAG]=filter_gauss[i][IMAG]*transform[i][REAL];
	}
fftwf_free(filter_gauss);
fftwf_free(transform);	
	plan_backward=fftwf_plan_dft_2d(gray_img->height,gray_img->width,mult_filt_data,inverse_dy,FFTW_BACKWARD,FFTW_ESTIMATE);

	fftwf_execute(plan_backward);fftwf_destroy_plan(plan_backward);
	
	for(i=0;i<img_dim;i++)
	{
		inverse_dy[i][REAL]=inverse_dy[i][REAL]/img_dim;
	}
	//********** filter dx multiplication with data over *********//

	//reusing filter_dx to hold magnitude and filter_dy to hold the angle, or i can use aliasing which is a good idea//
	for(i=0;i<img_dim;i++)
	{
		filter_dx[i]=sqrt(pow(abs(inverse_dx[i][REAL]),2)+pow(abs(inverse_dy[i][REAL]),2));
	}
fftwf_free(inverse_dx);
fftwf_free(inverse_dy);
fftwf_free(mult_filt_data);

printf(" the complex number is %fi\n",filter_dx[78*256+6]);
//printf("----------------");

	//free(filter_dx);
	//free(filter_dy);
	
	//cvReleaseImage(&img);
	cvReleaseImage(&gray_img);
	
	return 0;
	
}	
	
void filter(float* filter_dx,float* filter_dy,int height, int width)
{
	int i,mid_height,mid_width;
	int scale=2,j;
	float *omegax,*omegay,*X,*Y,*temp;
	float sq_root,temp_XY;

	mid_height=(-1)*height/2;
	mid_width=(-1)*height/2;

	omegax=(float*)malloc(sizeof(float)*height);
	omegay=(float*)malloc(sizeof(float)*width);
	temp=(float*)malloc(sizeof(float)*width);	

	for(i=0;i<height;i++)
	{
		omegax[i]=mid_height++;
		omegax[i]=omegax[i]/height*2*M_PI;
		omegax[i]=scale*omegax[i];
	}

	for(i=0;i<width;i++)
	{
		omegay[i]=mid_width++;
		omegay[i]=omegay[i]/height*2*M_PI;
		omegay[i]=scale*omegay[i];
	}

	X=(float*)malloc(sizeof(float)*height*width);
	Y=(float*)malloc(sizeof(float)*height*width);

	for(i=0;i<height;i++)
	{
		memcpy((X+i*width),omegax,sizeof(float)*width);
	}

	for(j=0;j<width;j++)
	{
		for(i=0;i<height;i++)
		memcpy((temp+i),(omegay+j),sizeof(float));
		memcpy((Y+j*width),temp,sizeof(float)*width);
	}

	sq_root=sqrt(2*M_PI);
	
	for(i=0;i<height*width;i++)
	{
		temp_XY=X[i]*X[i]+Y[i]*Y[i];
		filter_dx[i]=sq_root*(X[i]*(exp(-(temp_XY))));
		filter_dy[i]=sq_root*(Y[i]*(exp(-(temp_XY))));
	}
		
	norm_filter(filter_dx,height,width);
	norm_filter(filter_dy,height,width);

	free(omegax);
	free(omegay);
	free(X);
	free(Y);
	free(temp);

	return;
}

void norm_filter(float* filter,int row,int col)
{
	int count=0,i,j,power=2,scale=2;
	float temp=0, mean_value=0,norm,norm_scale;
	float *filter_transpose;
	
	filter_transpose=(float*)malloc(sizeof(float)*row*col);
	//shift_filter=(float*)malloc(sizeof(float)*row*col);
	
	for(i=0;i<col;i++)
	{
		for(j=0;j<row;j++)
		{
			filter_transpose[count]=*(filter+j*col+i);
			count++;
		}
	}
	
	for(i=0;i<row/2;i++)
	{
		memcpy((filter+(row/2+i)*col+col/2),(filter_transpose+col*i),sizeof(float)*col/2);
		memcpy((filter+(row/2+i)*col),(filter_transpose+col*i+col/2),sizeof(float)*col/2);
		memcpy((filter+col*i+col/2),(filter_transpose+(row/2+i)*col),sizeof(float)*col/2);
		memcpy((filter+col*i),(filter_transpose+(row/2+i)*col+col/2),sizeof(float)*col/2);
	}
	
	for(i=0;i<col;i++)
		filter[row/2*col+i]=0;

	for(i=0;i<row;i++)
		filter[col/2+i*col]=0;

	for(i=0;i<col*row;i++)
	{
		temp+=pow(filter[i],2);
	}

	mean_value=temp/(row*col);

	norm=sqrt(mean_value);
	
	norm_scale=-1*norm*scale;

	for(i=0;i<row*col;i++)
	{
		filter[i]=filter[i]/norm_scale;
	}
	
	count=0;
	/*for(i=0;i<4;i++)
	{
		for(j=0;j<4;j++)
		{
			printf(" %f ",filter[count++]);
		}
		printf("\n");
	}*/
	//printf(" %f ",filter[213*256+242]);
	
	free(filter_transpose);
	return;
}
