#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

#include "filter.h"
#include "normfilter.h"
	
void filter(float* filter_dx,float* filter_dy,int height, int width,float scale)
{
	int i,mid_height,mid_width;
	int j;
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
	
	/*for(i=0;i<height*width;i++)
	{
		temp_XY=X[i]*X[i]+Y[i]*Y[i];
		filter_dx[i]=sq_root*(X[i]*(exp(-(temp_XY))));
		filter_dy[i]=sq_root*(Y[i]*(exp(-(temp_XY))));
	}*/

	/*for(i=0;i<height;i++)
	{
		if(X[i]>-3.3)
		{
			printf("i th value is %d",i);
			break;
		}
	}*/

	for(i=0;i<height*width;i++)
	{
		temp_XY=exp(-(X[i]*X[i]+Y[i]*Y[i]));
		filter_dx[i]=sq_root*(X[i]*(temp_XY));
		filter_dy[i]=sq_root*(Y[i]*(temp_XY));
	}
		
	norm_filter(filter_dx,height,width,scale);
	norm_filter(filter_dy,height,width,scale);

	free(omegax);
	free(omegay);
	free(X);
	free(Y);
	free(temp);

	return;
}
