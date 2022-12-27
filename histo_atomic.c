/*
 * This file is a part of the Forensic Document Examination Toolset
 * created by Yan Solihin as part of his Master thesis at the Nanyang
 * Technological University, Singapore.
 *  
 * Copyright (C) 1995 1996 1997 by Yan Solihin
 *
 * This source file is distributed "as is" in the hope that it will be
 * useful.  The tool set comes with no warranty, and no author or
 * distributor accepts any responsibility for the consequences of its
 * use. 
 * 
 * Everyone is granted permission to copy, modify and redistribute
 * this tool set under the following conditions:
 *
 *    Any publication of results or modification to the unique techniques
 *    implemented in this toolset must make a reference to the author's
 *    work either by refering to the original publication of this technique 
 *    by the author, or by refering it as "private communication" with 
 *    the author, whichever is applicable.
 *
 *    This source code is distributed for non-commercial use only. 
 *    Please contact the maintainer for restrictions applying to 
 *    commercial use.
 *
 *    Permission is granted to anyone to make or distribute copies
 *    of this source code, either as received or modified, in any
 *    medium, provided that all copyright notices, permission and
 *    nonwarranty notices are preserved, and that the distributor
 *    grants the recipient permission for further redistribution as
 *    permitted by this document.
 *
 *    Permission is granted to distribute this file in compiled
 *    or executable form under the same conditions that apply for
 *    source code, provided that either:
 *
 *    A. it is accompanied by the corresponding machine-readable
 *       source code,
 *    B. it is accompanied by a written offer, with no time limit,
 *       to give anyone a machine-readable copy of the corresponding
 *       source code in return for reimbursement of the cost of
 *       distribution.  This written offer must permit verbatim
 *       duplication by anyone, or
 *    C. it is distributed by someone who received only the
 *       executable form, and is accompanied by a copy of the
 *       written offer of source code that they received concurrently.
 *
 * In other words, you are welcome to use, share and improve this
 * source file.  You are forbidden to forbid anyone else to use, share
 * and improve what you give them.
 *
 * INTERNET: solihin@uiuc.edu
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#include <omp.h>

typedef enum {eBlackAndWhite,
              eGrayScale8,
              eGrayScale16,
              eColor8,
              eColor16,
              eColor24}
        tImageType;

typedef struct {
  int imageIdx;                 /* id of this image */
  unsigned char ** content;     /* content of a bitmapped image */
  unsigned char ** tag;         /* general purpose tag */
  int row;                      /* image's height */
  int col;                      /* image's width */
  tImageType type;              /* type of image */
  int status;           /* processing status of an image */
} tImage;
typedef tImage* pImage;



unsigned char** read_img(char *filename, int *row, int *col,
                         int *imgtype)
{   
  char     mw[5];
  char    str[12];      
  int    com;
  unsigned char**  image;
  int     i, j, maxint;

   FILE    *fptr;
    
  printf("Opening file %s ....\n", filename); 
  fptr = fopen(filename, "r");
  assert(fptr != NULL);  
  
  if (fscanf(fptr, "%s", mw) == 0) {      /* read in header. */
    printf("Error reading header\n");
  }

  if (strcmp(mw,"P1") == 0)
    *imgtype = 1;
  else if (strcmp(mw,"P2") == 0)
    *imgtype = 2;
  else if (strcmp(mw,"P3") == 0)
    *imgtype = 3;
  else if (strcmp(mw,"P4") == 0)
    *imgtype = 4;
  else if (strcmp(mw,"P5") == 0)
    *imgtype = 5;
  else *imgtype = -1;

  assert(*imgtype == 5);

  com=fgetc(fptr);      /* discard newline character. */
  com=fgetc(fptr);

  i = 0;

  while (com == 35) {
    while((com = fgetc(fptr))!=10);       /* ignore comments */
    com = fgetc(fptr);  /* read first char of the new line */
  }
  str[i++] = com;    /* no comment, recover first char */

  while ((com = fgetc(fptr)) != 10)
    str[i++] = com;
  str[i] = '\0';

  sscanf(str,"%d %d",col,row);
  printf("debug : rows = %d cols = %d\n",*row,*col);

  printf("succeed in reading col and row\n");
  if (fscanf(fptr, "%d ", &maxint) == 0) {
    printf("error reading maxint\n");
  }

  /* allocate memory for each row pointers. */
  image = (unsigned char**)malloc((*row)*sizeof(unsigned char*));
  assert(image != NULL);

  /* allocate memory for each row. */
  for(i = 0; i < *row; i++) {
    image[i] = (unsigned char*)malloc((*col)*sizeof(unsigned char));
    assert(image[i] != NULL);
  }
  
  for(i = 0 ; i < *row; i++)      /* read in image & put in array */
    for(j = 0; j < *col; j++) {
      image[i][j] = fgetc(fptr);
    }
  
  fclose(fptr);      

  printf("Finished reading image file !\n");
    
  return image;
}

void write_img(char *filename, unsigned char **image, int row,
               int col, char *comment)
{
  char*   mw="P5";

  int   i, j, maxint=255;

  FILE  *fptr;

  printf("Writing file %s ....\n", filename);
  fptr = fopen(filename, "w");

  fprintf(fptr, "%s\n", mw);    /* write in header. */
  fprintf(fptr, "%s\n", comment); /* write comment */
  fprintf(fptr, "%d %d\n", col, row);
  fprintf(fptr, "%d\n", maxint);

  for(i = 0 ; i < row; i++)    /* write in image data. */
    for(j = 0; j < col; j++)
      fputc(image[i][j], fptr);

  printf("Finished writing image file !\n");

  fclose(fptr);
}

tImage *allocate_storage(int row, int col)
{
  tImage *output;
  int i;

  output = (tImage *) malloc (sizeof(tImage));
  assert(output != NULL);

  output->content = (unsigned char **)
                     malloc (row * sizeof(unsigned char *));
  assert(output->content != NULL);
  
  for (i=0; i<row; i++) {
    output->content[i] = (unsigned char *)
                          malloc(col * sizeof(unsigned char));
    assert(output->content[i] != NULL); 
  }
  
  output->row = row;
  output->col = col;
  output->tag = NULL;
  output->type = 0; /* default type */
  return output;
}



/* read_img read an image of format P5 */
pImage Image_Read(char *filename)
{
  unsigned char **read_img(char*,int*,int*,int*);
  tImage *image;
  int row, col;
  int imgtype;

  image = (tImage *) malloc(sizeof(tImage));
  assert(image != NULL);

  image->imageIdx = 0;
  image->content = read_img(filename,&row,&col,&imgtype);
  image->tag = NULL;
  image->row = row;
  image->col = col;

  /* to be perfected later Jul 26, Yan */
  image->type = eGrayScale8;
  image->status = 0;

  return image;
}

void Image_Destroy(pImage* image)
{
  free((*image)->content);
  (*image)->content = NULL;
  free((*image)->tag);
  (*image)->tag = NULL;
  free(*image);
  *image = NULL;
}

void Image_Write(tImage *image, char *filename, char *comment)
{
  void write_img(char*, unsigned char **, int, int, char *);

  write_img(filename,image->content,image->row,image->col, comment);
}

tImage *mean_filter(tImage *image, int iter)
/*
   this function performs linear mean filtering with window size
   3x3. winrow and wincol should be odd integer.
   The output is returned.
*/
{
  int rowmin, rowmax, colmin, colmax;
  int i, j, k, l , m;
  int sum, avg;
  tImage *output;
  
  /* allocate storage for output image */
  output = allocate_storage(image->row,image->col);
  printf("succeed allocating storage row = %d col = %d \n",
         output->row,output->col);

  /* calculate window's size */
  rowmin = (3 - 1 )/ 2;  /* base index for array = 0 */
  rowmax = image->row - rowmin;
  colmin = (3 - 1)/ 2;
  colmax = image->col -colmin;

  k = 9; /* window size */
  for (m=0; m<iter; m++) {
    for (i=rowmin; i<rowmax; i++) {
      for (j=colmin; j<colmax; j++) {
	sum = image->content[i-1][j-1] + image->content[i-1][j] + image->content[i-1][j+1] +
	  image->content[i][j-1] + image->content[i][j] + image->content[i][j+1] +
	  image->content[i+1][j-1] + image->content[i+1][j] + image->content[i+1][j+1];
	output->content[i][j] = (int) (sum/k);
      }
    }
  }

  /* copy the rest (frame) of the image */
  for (i=0; i<rowmin; i++)
    for (j=0; j<image->col; j++)
      output->content[i][j] = image->content[i][j];
  for (i=rowmin; i<rowmax; i++) {
    for (j=0; j<colmin; j++)
      output->content[i][j] = image->content[i][j];
    for (j=colmax; j<image->col; j++)
      output->content[i][j] = image->content[i][j];
  }
  for (i=rowmax; i<image->row; i++)
    for (j=0; j<image->col; j++)
      output->content[i][j] = image->content[i][j];

  return output;
}

tImage *nn_filter(tImage *image, int iter)
/*
   this function performs nearest neighbor mean filtering with window size
   3x3.    The output is returned.
*/
{
  int rowmin, rowmax, colmin, colmax;
  int i, j, k, l , m;
  double sum;
  tImage *output;

  /* allocate storage for output image */
  output = allocate_storage(image->row,image->col);
  printf("succeed allocating storage row = %d col = %d \n",
         output->row,output->col);

  /* calculate window's size */
  rowmin = 1; 
  rowmax = image->row - 1; 
  colmin = 1; 
  colmax = image->col - 1; 

  for (m=0; m<iter; m++) {
    for (i=rowmin; i<rowmax; i++) {
      for (j=colmin; j<colmax; j++) {
         sum = image->content[i][j] + image->content[i][j-1] + image->content[i][j+1] +
	   image->content[i-1][j] + image->content[i+1][j];
         output->content[i][j] = (int) (sum/5.0);
      }
    }
  }

  /* copy the rest (frame) of the image */
  for (i=0; i<rowmin; i++)
    for (j=0; j<image->col; j++)
      output->content[i][j] = image->content[i][j];
  for (i=rowmin; i<rowmax; i++) {
    for (j=0; j<colmin; j++)
      output->content[i][j] = image->content[i][j];
    for (j=colmax; j<image->col; j++)
      output->content[i][j] = image->content[i][j];
  }
  for (i=rowmax; i<image->row; i++)
    for (j=0; j<image->col; j++)
      output->content[i][j] = image->content[i][j];

  return output;
}


tImage *dupl_image(tImage *image, int row, int col)
/*
   this function increases the image size by row times and col times
*/
{
  int i, j, r, c; 
  tImage *output;

  /* allocate storage for output image */
  output = allocate_storage(row*image->row, col*image->col);
  printf("succeed allocating storage row = %d col = %d \n", row, col);

  /* start duplicating image by row and col times */
  for (r=0; r<row; r++) {
    for (c=0; c<col; c++) {
      for (i=0; i<image->row; i++) {
        for (j=0; j<image->col; j++) {
          output->content[r*image->row+i][c*image->col+j] = image->content[i][j];
        }
      }
    }
  }
  return output;
}

long* histogram(char* fn_input) {
  pImage image;
  int i, j, m;
  long* histo;
  double t_start, t_end;

  /* initalization & reading image file */
  histo = malloc(256*sizeof(long));
    /* initalization & reading image file */
  histo = malloc(256*sizeof(long));
  for (i=0; i<256; i++) {
    histo[i] = 0;
  }
  image = Image_Read(fn_input);

  t_start = omp_get_wtime();

  //omp_set_num_threads(2);
  /* obtain histogram from image, repeated 100 times */
  for (m=0; m<100; m++) {
#pragma omp parallel for private(i,j) collapse(2)
   for (i=0; i<image->row; i++) { 
     //#pragma omp parallel for private(j)
     for (j=0; j<image->col; j++) {
	#pragma omp atomic update
        histo[image->content[i][j]]++;
      }
    }
  }

  t_end = omp_get_wtime();

 /* ------- Termination */
  Image_Destroy(&image);
  printf("--- Histogram Content ---\n");
  for (i=0; i<256; i++)
    printf("histo[%d] = %ld\n", i, histo[i]);

  printf("\nRuntime = %10.2f seconds\n", t_end-t_start);

  return histo;
}


int main(int argc, char** argv)
{
  long* histo; 

  if (argc != 2) {
    printf("Usage: main <input-file-name>\n");
    assert(argc == 2);
  }

  histo = histogram(argv[1]);
  return 0;
}
