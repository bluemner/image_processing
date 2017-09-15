#include "../headers/PGM.h"
#include <cstdio>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <malloc.h>  
#include <memory.h>

namespace uwm{
	void PGM::read(const std::string file_path, std::vector<unsigned char> &vector_image, int &x_dimension, int &y_dimension){
		FILE *fp;
		if ((fp=fopen(file_path.c_str(), "rb"))==NULL){
			printf("read error...\n");
			exit(-1);
			}	
		int maxraw;
		int c;
		int i,j;
		int val;
		unsigned char *line;
		char buf[1024];
		unsigned char *image;
	
		while ((c=fgetc(fp)) == '#')
			fgets(buf, 1024, fp);
		 ungetc(c, fp);
		 if (fscanf(fp, "P%d\n", &c) != 1) {
		   printf ("read error ....");
		   exit(0);
		 }
		 if (c != 5 && c != 2) {
		   printf ("read error ....");
		   exit(0);
		 }
	
		 if (c==5) {
		   while ((c=fgetc(fp)) == '#')
			 fgets(buf, 1024, fp);
		   ungetc(c, fp);
		   if (fscanf(fp, "%d%d%d",&x_dimension, &y_dimension, &maxraw) != 3) {
			 printf("failed to read width/height/max\n");
			 exit(0);
		   }
		   printf("Width=%d, Height=%d \nMaximum=%d\n",x_dimension,y_dimension,maxraw);
	
		   image = (unsigned char*)malloc(sizeof(unsigned char)*x_dimension*y_dimension);
		   getc(fp);
	
		   line = (unsigned char *)malloc(sizeof(unsigned char)*x_dimension);
		   for (j=0; j<y_dimension; j++) {
			  fread(line, 1, x_dimension, fp);
			  for (i=0; i<x_dimension; i++) {
				image[j*x_dimension+i] = line[i];
			 }
		   }
		   free(line);
	
		 }
	
		 else if (c==2) {
		   while ((c=fgetc(fp)) == '#')
			 fgets(buf, 1024, fp);
		   ungetc(c, fp);
		   if (fscanf(fp, "%d%d%d", &x_dimension, &y_dimension, &maxraw) != 3) {
			 printf("failed to read width/height/max\n");
			 exit(0);
		   }
		   printf("Width=%d, Height=%d \nMaximum=%d,\n",x_dimension,y_dimension,maxraw);
	
		   image = (unsigned char*)malloc(sizeof(unsigned char)*x_dimension*y_dimension);
		   getc(fp);
	
		   for (j=0; j<y_dimension; j++)
			 for (i=0; i<x_dimension; i++) {
				fscanf(fp, "%d", &val);
				image[j*x_dimension+i] = val;
			 }
	
		 }
		 
		 fclose(fp);
		 free (fp);
		 free(image);
		 

	}

	void PGM::write(const std::string file_path,const std::vector<unsigned char> &vector_image,const int &x_dimension,const int &y_dimension){
		if(vector_image.empty())
			throw "no image found";

		FILE *fp;
		const unsigned char * image = vector_image.data(); 
		if ((fp=fopen(file_path.c_str(), "wb")) == NULL){
			printf("write pgm error....\n");
			exit(0);
		}

		int i,j;
		
	
		fprintf(fp, "P5\n%d %d\n%d\n", x_dimension, y_dimension, 255);
		for (j=0; j<y_dimension; j++)
			for (i=0; i<x_dimension; i++) {
				fputc(image[j*x_dimension+i], fp);
			}
	
		fclose(fp);
		free (fp);
	 
	}
}
