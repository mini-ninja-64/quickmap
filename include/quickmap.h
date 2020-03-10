#ifndef QUICKMAP_H
#define QUICKMAP_H

#include <stdio.h>

/*
how to bitmap - simple, cba to write fancy reader lolll ( that why we missing the pallete ), windows bitmap, damn this is a simple bitmap fuk me lol

|---------------|
|    header		|--type of bitmap(2 byte), size of file(4), reserved1 (2), reserved2 (2), offset to image data(4) total:{14}
|---------------|
|				|
|  info header	|--size of header (4), w (4), h (4), num of colour planes(2), bits per pixel(2), compression(4), ppm h (4), ppm v (4), num of colours (4), num of important colours (4) total:{46}
|				|
|---------------|
|				|
|   IMAGE DATA	|--24bit bgr
|				|
|---------------|

(yn, x0)-->(y0,xn)
*/


//92x64 24 bit bitmap header
static const char BMP_HEADER[] = {	0x42,0x4D, // bmp tiype (2)
									0x00,0x00,0x00,0x00, // file size (4)
									0x00,0x00, // reserved 1 (2)
									0x00,0x00, // reserved 2 (2)
									0x36,0x00,0x00,0x00, // offset to image data (4)
	
									0x28,0x00,0x00,0x00, // header size (4)
									0x00,0x00,0x00,0x00, // img width (4)
									0x40,0x00,0x00,0x00, // img height (4)
									0x01,0x00, // colour planes (2)
									0x18,0x00, // bits per pixel (2)
									0x00,0x00,0x00,0x00, // compression (4)
									0x80,0x28,0x00,0x00, // ppm h (4)
									0x00,0x00,0x00,0x00, // ppm v (4)
									0x00,0x00,0x00,0x00, //num of important colours (4)
									0x00,0x00,0x00,0x00,
									0x00,0x00,0x00,0x00
};

typedef struct {
	unsigned int w;
	unsigned int h;
	unsigned char * pixels;
}BMP_FILE;

int writeBMP(char const* filename, unsigned int width, unsigned int height, unsigned char * imagePixels);

BMP_FILE * readBMP(char const* path);

int setPixel(int x, int y, unsigned char r, unsigned char g, unsigned char b, BMP_FILE * bmp);

void destroyBMP(BMP_FILE * bmp);
BMP_FILE * newBMP(unsigned int w, unsigned int h);
#endif /* QUICKMAP_H */
