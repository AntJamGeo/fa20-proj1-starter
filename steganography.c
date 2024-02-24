/************************************************************************
**
** NAME:        steganography.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Dan Garcia  -  University of California at Berkeley
**              Copyright (C) Dan Garcia, 2020. All rights reserved.
**				Justin Yokota - Starter Code
**				YOUR NAME HERE
**
** DATE:        2020-08-23
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "imageloader.h"

//Determines what color the cell at the given row/col should be. This should not affect Image, and should allocate space for a new Color.
Color *evaluateOnePixel(Image *image, int row, int col)
{
	uint32_t pixel_index = row*image->cols + col;
	Color *pixel = *(image->image + pixel_index);
	uint8_t b = pixel->B;
	uint8_t value = 255 * (b & 1);
	Color *color = (Color *) malloc(sizeof(Color));
	color->R = color->G = color->B = value;
	return color;
}

//Given an image, creates a new image extracting the LSB of the B channel.
Image *steganography(Image *image)
{
	Image *new_image = (Image *) malloc(sizeof(Image));
	uint32_t pixels = image->rows * image->cols;
	new_image->image = (Color **) malloc(sizeof(Color *) * pixels);
	new_image->rows = image->rows;
	new_image->cols = image->cols;
	Color **p = new_image->image;
	for (uint32_t i=0; i < image->rows; i++) {
		for (uint32_t j=0; j < image->cols; j++) {
			*p = evaluateOnePixel(image, i, j);
			p++;
		}
	}
	return new_image;
}

/*
Loads a file of ppm P3 format from a file, and prints to stdout (e.g. with printf) a new image, 
where each pixel is black if the LSB of the B channel is 0, 
and white if the LSB of the B channel is 1.

argc stores the number of arguments.
argv stores a list of arguments. Here is the expected input:
argv[0] will store the name of the program (this happens automatically).
argv[1] should contain a filename, containing a file of ppm P3 format (not necessarily with .ppm file extension).
If the input is not correct, a malloc fails, or any other error occurs, you should exit with code -1.
Otherwise, you should return from main with code 0.
Make sure to free all memory before returning!
*/
int main(int argc, char **argv)
{
	Image *image = readData(argv[1]);
	if (!image) {
		return -1;
	}
	Image *new_image = steganography(image);
	writeData(new_image);
	freeImage(new_image);
	freeImage(image);
	return 0;
}
