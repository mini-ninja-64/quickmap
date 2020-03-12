#include <quickmap.h>
#include <stdio.h>
#include <stdlib.h>

#define WIDTH 4000
#define HEIGHT 4000

int main(int argc, char const *argv[]) {
	BMP * myBmp = newBMP (4000,4000);

	if (!myBmp) {
		printf("Error creating bitmap\n");
		return -1;
	}

	for (unsigned int y = 0; y < myBmp->h; y++) {
		for (unsigned int x = 0; x < myBmp->w; x++) {
			setPixel(x, y, x%0xFF, y%0xFF, (x+y)%0xFF, myBmp);
		}
	}

	if (argc >= 2) {
		writeBMP(argv[1],myBmp);
	} else {
		printf("No file path given\n");
	}

	destroyBMP(myBmp);

	return 0;
}
