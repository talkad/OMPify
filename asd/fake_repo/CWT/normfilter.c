#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

#include "filter.h"
#include "normfilter.h"


void norm_filter(float* filter,int row,int col,float scale)
{
	int count=0,i,j,power=2;
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
