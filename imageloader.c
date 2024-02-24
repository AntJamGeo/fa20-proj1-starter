/************************************************************************
**
** NAME:        imageloader.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Dan Garcia  -  University of California at Berkeley
**              Copyright (C) Dan Garcia, 2020. All rights reserved.
**              Justin Yokota - Starter Code
**				YOUR NAME HERE
**
**
** DATE:        2020-08-15
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include "imageloader.h"

//Opens a .ppm P3 image file, and constructs an Image object. 
//You may find the function fscanf useful.
//Make sure that you close the file with fclose before returning.
Image *readData(char *filename) 
{
	FILE *f = fopen(filename, "r");
	if (!f) {
		printf("Unable to open file %s\n", filename);
		return NULL;
	}
	char buf[3];
	fscanf(f, "%s", buf);
	if (buf[0] != 'P' || buf[1] != '3') {
		printf("Expected P3 PPM file\n");
		return NULL;
	}
	uint32_t cols, rows, scale;
	fscanf(f, "%u %u %u", &cols, &rows, &scale);
	if (scale != 255) {
		printf("Scale should be 255\n");
		return NULL;
	}
	Image *image = (Image *) malloc(sizeof(Image));
	image->cols = cols;
	image->rows = rows;
	uint32_t pixels = rows * cols;
	image->image = (Color **) malloc(sizeof(Color *) * pixels);
	for (uint32_t i=0; i < pixels; i++) {
		*(image->image + i) = (Color *) malloc(sizeof(Color));
		Color *pixel = *(image->image + i);
		fscanf(f, "%hhu %hhu %hhu", &pixel->R, &pixel->G, &pixel->B);
	}
	fclose(f);
	return image;
}

//Given an image, prints to stdout (e.g. with printf) a .ppm P3 file with the image's data.
void writeData(Image *image)
{
	printf("P3\n%u %u\n255\n", image->cols, image->rows);
	Color **p = image->image;
	for (uint32_t i=0; i < image->rows; i++) {
		for (uint32_t j=0; j < image->cols-1; j++) {
			printf("%3hhu %3hhu %3hhu   ", (*p)->R, (*p)->G, (*p)->B);
			p++;
		}
		printf("%3hhu %3hhu %3hhu\n", (*p)->R, (*p)->G, (*p)->B);
		p++;
	}
}

//Frees an image
void freeImage(Image *image)
{
	uint32_t pixels = image->rows * image->cols;
	Color **p = image->image;
	for (uint32_t i=0; i < pixels; i++) {
		free(*(p++));
	}
	free(image->image);
	free(image);
}
