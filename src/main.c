#include "quickmap.h"

#define WIDTH 50
#define HEIGHT 100

int main(int argc, char const *argv[])
{
	unsigned char pix[3*WIDTH*HEIGHT];
	for(int i = 0; i < WIDTH*HEIGHT*3; i+=3){
		pix[i] = 0; // r 0 3 6
		pix[i+1] = 0; // g 1 4 7
		pix[i+2] = 255; // b 2 5 8
	}
	writeBMP("test.bmp",WIDTH,HEIGHT,pix);
//	printf("%i, %i", file->w, file->h);
	return 0;
}
