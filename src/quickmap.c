#include "quickmap.h"
#include <stdlib.h>

//TODO: clean up all the things

int writeBMP(char const* filename, unsigned int width, unsigned int height, unsigned char * imagePixels) {
	int scanLinePadding = 0; // number of bytes of scanline padding neccesary
	

//		scanLinePadding = width % 4;
	if((width*3) % 4 != 0 ) {
		scanLinePadding = 4 - ((width*3)%4);
		printf("image needs end of scan line padding %i\n",scanLinePadding);
	}
	
	unsigned int paddedPixelBufferSize = (width*height*3 + scanLinePadding*height);
	unsigned int pixelBufferSize = width*height*3;
	unsigned int newFileSize = (0x36 + paddedPixelBufferSize); //header + pixels + padding
	unsigned char pixelBuffer[paddedPixelBufferSize];	
	unsigned char dimensions[8] = {width&0x000000FF,(width&0x0000FF00)>>8,(width&0x00FF0000)>>16,(width&0xFF000000)>>24,
								height&0x000000FF,(height&0x0000FF00)>>8,(height&0x00FF0000)>>16,(height&0xFF000000)>>24};
	unsigned char fileSize[4] = {newFileSize&0x000000FF,(newFileSize&0x0000FF00)>>8,(newFileSize&0x00FF0000)>>16,(newFileSize&0xFF000000)>>24}; // convert to little endian
	
	printf("padding: %i\n", scanLinePadding);
	printf("file size: %i\n", newFileSize);
	printf("width: %i, height: %i\n", width, height);
	int line=0;
	int currX=0;
	int currY=0;
	int bytePos = 0;
	//for every byte
	for(int i = 0; i < paddedPixelBufferSize; i+=3){
//		printf("X: %i, Y: %i, i: %i\n",currX,currY,i);
		int pos = 3*((width-1-currX)+((width)*currY));

		// printf("%i ",currX);

		// printf("[%i,%i,%i], [%i,%i,%i]\n",pos+2,pos+1,pos+0,imagePixels[pos+2],imagePixels[pos+1],imagePixels[pos+0]);
		pixelBuffer[i] = imagePixels[pixelBufferSize-1-pos-0];
		pixelBuffer[i+1] = imagePixels[pixelBufferSize-1-pos-1];
		pixelBuffer[i+2] = imagePixels[pixelBufferSize-1-pos-2];
		
		currX++;
		if (currX > width-1){
			currX = 0;
			currY++;
			if (scanLinePadding){
				// printf("padding time\n");
				for(int p = 0; p < scanLinePadding; p++){
					pixelBuffer[i+3+p] = 0x00;
//					i++;
				}
				i+=scanLinePadding;
			}
		}
	}

	FILE *fptr;
	fptr = fopen(filename, "wb");
	fseek ( fptr , 0 , SEEK_SET );
	
	//	write pixel data
	fwrite(BMP_HEADER, sizeof(unsigned char), 0x36, fptr);
	fwrite(pixelBuffer, sizeof(unsigned char), width*height*3 + scanLinePadding*height, fptr);
	
	// write width and height
	fseek ( fptr , 18 , SEEK_SET );
	fwrite(dimensions, sizeof(unsigned char), 8, fptr);
	
	//	write new file size
	fseek ( fptr , 2 , SEEK_SET );
	fwrite(fileSize, sizeof(unsigned char), 4, fptr);
	
	fclose(fptr);
	return 0;
}

BMP_FILE * readBMP(char const* path){
	unsigned char width_little_endian[4];
	unsigned char height_little_endian[4];
	unsigned char image_offset_little_endian[4];

	FILE *fptr;
	fptr = fopen(path, "rb");
	if (fptr == NULL){
		printf("fail file open\n");
	}

	//
	fseek ( fptr, 10, SEEK_SET );
	fread ( image_offset_little_endian, sizeof(unsigned char), 4, fptr);
	// width and height at 18 bytes
	fseek ( fptr, 18, SEEK_SET );
	fread ( width_little_endian, sizeof(unsigned char), 4, fptr );
	fread ( height_little_endian, sizeof(unsigned char), 4, fptr );

	BMP_FILE * bmp = malloc(sizeof(BMP_FILE));
	bmp->w = (width_little_endian[3] << 24) | (width_little_endian[2] << 16) | (width_little_endian[1] << 8) | (width_little_endian[0]);
	bmp->h = (height_little_endian[3] << 24) | (height_little_endian[2] << 16) | (height_little_endian[1] << 8) | (height_little_endian[0]);
	int image_offset = (image_offset_little_endian[3] << 24) | (image_offset_little_endian[2] << 16) | (image_offset_little_endian[1] << 8) | (image_offset_little_endian[0]);

	int scanLinePadding = 4 - ((bmp->w*3)%4);
	if (scanLinePadding == 4){
		scanLinePadding = 0;
	}
	// printf("padding: %i", scanLinePadding);
	unsigned int paddedPixelBufferSize = (bmp->w*bmp->h*3 + scanLinePadding*bmp->h);
	unsigned int pixelBufferSize = bmp->w*bmp->h*3;
	unsigned char bitmapPixelBuffer[paddedPixelBufferSize];
	unsigned char pixelBuffer[pixelBufferSize];
	bmp->pixels = malloc(pixelBufferSize);

	fseek ( fptr, image_offset, SEEK_SET );
	fread(bitmapPixelBuffer, sizeof(unsigned char), paddedPixelBufferSize, fptr);
	printf("r: %i, g: %i, b: %i", bitmapPixelBuffer[2], bitmapPixelBuffer[1], bitmapPixelBuffer[0]);

	for(int y = 0; y < bmp->h; y++){
		for(int x = 0; x < bmp->w; x++){
			int posFlipped = 3*((bmp->w-1-x)+(bmp->w*y));
			int pos = 3*((x)+(bmp->w*y));
			int scanLineOffset = y * scanLinePadding;
			bmp->pixels[pixelBufferSize-1-(posFlipped+2)] = bitmapPixelBuffer[pos+2+scanLineOffset];
			bmp->pixels[pixelBufferSize-1-(posFlipped+1)] = bitmapPixelBuffer[pos+1+scanLineOffset];
			bmp->pixels[pixelBufferSize-1-(posFlipped+0)] = bitmapPixelBuffer[pos+0+scanLineOffset];
		}
	}
	
	return bmp;
}

int getPixel(int x, int y, BMP_FILE * bmp){
	return 0;
}

int pixelPos(int x, int y, int w, int h){
	if(x >= w) return -1;
	if(y >= h) return -1;
	return 3*((x)+(w*y));
}

void setR(unsigned char r, unsigned int pos, BMP_FILE * bmp) { bmp->pixels[pos+0] = r; }
void setG(unsigned char g, unsigned int pos, BMP_FILE * bmp) { bmp->pixels[pos+1] = g; }
void setB(unsigned char b, unsigned int pos, BMP_FILE * bmp) { bmp->pixels[pos+2] = b; }

int setPixel(int x, int y, unsigned char r, unsigned char g, unsigned char b, BMP_FILE * bmp) {
	int pos = pixelPos(x, y, bmp->w, bmp->h);
	bmp->pixels[pos+0] = r;
	bmp->pixels[pos+1] = g;
	bmp->pixels[pos+2] = b;

	return 0;
}

void destroyBMP(BMP_FILE * bmp) {
	free(bmp->pixels);
	free(bmp);
}

BMP_FILE * newBMP(unsigned int w, unsigned int h) {
	BMP_FILE * bmp = malloc(sizeof(BMP_FILE));
	bmp->w = w;
	bmp->h = h;
	bmp->pixels = malloc(3*w*h);
	return bmp;
}