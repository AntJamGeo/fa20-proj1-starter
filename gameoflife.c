/************************************************************************
**
** NAME:        gameoflife.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Justin Yokota - Starter Code
**				YOUR NAME HERE
**
**
** DATE:        2020-08-23
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "imageloader.h"

Color *rctopx(Image *image, int row, int col) {
	uint32_t pixel_index = row*image->cols + col;
	Color *pixel = *(image->image + pixel_index);
	return pixel;
}

//Determines what color the cell at the given row/col should be. This function allocates space for a new Color.
//Note that you will need to read the eight neighbors of the cell in question. The grid "wraps", so we treat the top row as adjacent to the bottom row
//and the left column as adjacent to the right column.
Color *evaluateOneCell(Image *image, int row, int col, uint32_t rule)
{
	Color *original_pixel = rctopx(image, row, col);
	int8_t row_min = row ? -1 : 0;
	int8_t row_max = (row+1 == image->rows) ? 0 : 1;
	int8_t col_min = col ? -1 : 0;
	int8_t col_max = (col+1 == image->cols) ? 0 : 1;
	Color *color = (Color *) malloc(sizeof(Color));
	color->R = color->G = color->B = 0;
	for (uint8_t bit=1; bit; bit <<= 1) {
		uint8_t rcount = 0;
		uint8_t gcount = 0;
		uint8_t bcount = 0;
		for (int8_t i=row_min; i <= row_max; i++) {
			for (int8_t j=col_min; j <= col_max; j++) {
				if (!i && !j) {
					continue;
				}
				Color *pixel = rctopx(image, row+i, col+j);
				if (pixel->R & bit) {
					rcount++;
				}
				if (pixel->G & bit) {
					gcount++;
				}
				if (pixel->B & bit) {
					bcount++;
				}
			}
		}
		uint32_t rmask = (original_pixel->R & bit ? 1 << 9 : 1) << rcount;
		color->R |= (rmask & rule) ? bit : 0;
		uint32_t gmask = (original_pixel->G & bit ? 1 << 9 : 1) << gcount;
		color->G |= (gmask & rule) ? bit : 0;
		uint32_t bmask = (original_pixel->B & bit ? 1 << 9 : 1) << bcount;
		color->B |= (bmask & rule) ? bit : 0;
	}
	return color;
}

//The main body of Life; given an image and a rule, computes one iteration of the Game of Life.
//You should be able to copy most of this from steganography.c
Image *life(Image *image, uint32_t rule)
{
	Image *new_image = (Image *) malloc(sizeof(Image));
	uint32_t pixels = image->rows * image->cols;
	new_image->image = (Color **) malloc(sizeof(Color *) * pixels);
	new_image->rows = image->rows;
	new_image->cols = image->cols;
	Color **p = new_image->image;
	for (uint32_t i=0; i < image->rows; i++) {
		for (uint32_t j=0; j < image->cols; j++) {
			*p = evaluateOneCell(image, i, j, rule);
			p++;
		}
	}
	return new_image;
}

/*
Loads a .ppm from a file, computes the next iteration of the game of life, then prints to stdout the new image.

argc stores the number of arguments.
argv stores a list of arguments. Here is the expected input:
argv[0] will store the name of the program (this happens automatically).
argv[1] should contain a filename, containing a .ppm.
argv[2] should contain a hexadecimal number (such as 0x1808). Note that this will be a string.
You may find the function strtol useful for this conversion.
If the input is not correct, a malloc fails, or any other error occurs, you should exit with code -1.
Otherwise, you should return from main with code 0.
Make sure to free all memory before returning!

You may find it useful to copy the code from steganography.c, to start.
*/
int main(int argc, char **argv)
{
	if (argc != 3) {
		fprintf(stderr, "\033[31mERROR:\033[0m Wrong number of arguments. Expected a filename and a rule.\n");
		return -1;
	}
	char *filename = argv[1];
	uint32_t rule = strtoul(argv[2], NULL, 16);
	Image *image = readData(filename);
	Image *new_image = life(image, rule);
	writeData(new_image);
	freeImage(new_image);
	freeImage(image);
}
