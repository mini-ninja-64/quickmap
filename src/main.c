#include "quickmap.h"
#include <stdio.h>
#include <stdlib.h>

#define WIDTH 1002
#define HEIGHT 1002

int main(int argc, char const *argv[])
{
	BMP_FILE * myBmp = newBMP(WIDTH,HEIGHT);

	for(int y = 0; y < HEIGHT; y++){
		for(int x = 0; x < WIDTH; x++){
			setPixel(x, y, 0xFF, y%0xFF, 0xFF, myBmp);
		}
	}
	// writeBMP(argv[1],myBmp->w,myBmp->h,myBmp->pixels);
	destroyBMP(myBmp);

	myBmp = readBMP("Test-Images/MARBLES.bmp");
	writeBMP(argv[1],myBmp->w,myBmp->h,myBmp->pixels);

	return 0;
}
