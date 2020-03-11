#include <quickmap.h>
#include <stdio.h>
#include <stdlib.h>


int main(int argc, char const *argv[]) {
	BMP * myBmp = readBMP(argv[1]);
	// BMP * myBmp = newBMP(1000,1000);

	printf("%i, %i\n", myBmp->w, myBmp->h);

	// for(int y = 0; y < myBmp->h; y++){
	// 	for(int x = 0; x < myBmp->w; x++){
	// 		// printf("x: %i, y: %i\n", x, y);
	// 		// setPixel(x, y, 0xFF, 0x00, 0x00, myBmp);
	// 		// setPixelChannel(x, y, B_Channel, 0x00, myBmp);
	// 	}
	// }

	// for(int y = 0; y < myBmp->h; y++){
	// 	for(int x = 0; x < myBmp->w; x++){
	// 		setPixel(x, y, x%0xFF, y%0xFF, (x+y)%0xFF, myBmp);
	// 	}
	// }

	writeBMP(argv[2],myBmp);
	destroyBMP(myBmp);

	return 0;
}


// BMP * myBmp = newBMP(WIDTH,HEIGHT);
//
// for(int y = 0; y < HEIGHT; y++){
// 	for(int x = 0; x < WIDTH; x++){
// 		setPixel(x, y, x%0xFF, y%0xFF, (x+y)%0xFF, myBmp);
// 	}
// }
// writeBMP(argv[1],myBmp);
// destroyBMP(myBmp);
