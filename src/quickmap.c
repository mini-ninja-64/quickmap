#include "quickmap.h"
#include <stdlib.h>

unsigned int calculateScanlinePadding (int width) {
	return width%4;
}

int writePixels (char const* filename, unsigned int width, unsigned int height, unsigned char * imagePixels) {
	printf ("%i, %i\n", width, height);
	unsigned int scanLinePadding = calculateScanlinePadding (width);
	const unsigned int pixelBufferSize = width*height*3;
	const unsigned int paddedPixelBufferSize = (pixelBufferSize + scanLinePadding*height);
	const unsigned int newFileSize = (0x36 + paddedPixelBufferSize); //header + pixels + padding
	unsigned char * pixelBuffer = malloc (paddedPixelBufferSize);
	if (!pixelBuffer) {
		printf ("Failed to allocate space for writing buffer\n");
		return -1;
	}

	unsigned char dimensions[8] = {
		(width&0x000000FF),
		(width&0x0000FF00) >> 8,
		(width&0x00FF0000) >> 16,
		(width&0xFF000000) >> 24,
		(height&0x000000FF),
		(height&0x0000FF00) >> 8,
		(height&0x00FF0000) >> 16,
		(height&0xFF000000) >> 24
	};

	unsigned char fileSize[4] = {
		(newFileSize&0x000000FF),
		(newFileSize&0x0000FF00) >> 8,
		(newFileSize&0x00FF0000) >> 16,
		(newFileSize&0xFF000000) >> 24
	};

	int line=0;
	int currX=0;
	int currY=0;
	int bytePos = 0;

	for(int i = 0; i < paddedPixelBufferSize; i+=3) {
		int pos = 3*((width-1-currX)+((width)*currY));

		pixelBuffer[i] = imagePixels[pixelBufferSize-1-pos-0];
		pixelBuffer[i+1] = imagePixels[pixelBufferSize-1-pos-1];
		pixelBuffer[i+2] = imagePixels[pixelBufferSize-1-pos-2];

		currX++;
		if (currX > width-1){
			currX = 0;
			currY++;
			if (scanLinePadding){
				for(int p = 0; p < scanLinePadding; p++){
					pixelBuffer[i+3+p] = 0x00;
				}
				i+=scanLinePadding;
			}
		}
	}

	FILE *fptr;
	fptr = fopen(filename, "wb");
	fseek ( fptr , 0 , SEEK_SET );

	//	write pixel data
	fwrite (BMP_HEADER, sizeof(unsigned char), 0x36, fptr);
	fwrite (pixelBuffer, sizeof(unsigned char), width*height*3 + scanLinePadding*height, fptr);
	free (pixelBuffer);

	// write width and height
	fseek ( fptr , 18 , SEEK_SET );
	fwrite (dimensions, sizeof(unsigned char), 8, fptr);

	//	write new file size
	fseek ( fptr , 2 , SEEK_SET );
	fwrite (fileSize, sizeof(unsigned char), 4, fptr);

	fclose (fptr);
	return 0;
}

unsigned char * readPixels (char const* filename, unsigned int * width, unsigned int * height) {
	unsigned char dimensionsLittleEndian[8];
	unsigned char imageOffsetLittleEndian[4];

	FILE *fptr;
	fptr = fopen (filename, "rb");
	if (!fptr){
		printf ("fail file open\n");
		return NULL;
	}

	// image offset at 10 bytes
	fseek ( fptr, 10, SEEK_SET );
	fread ( imageOffsetLittleEndian, sizeof(unsigned char), 4, fptr);
	unsigned int imageOffset = (imageOffsetLittleEndian[3] << 24) | (imageOffsetLittleEndian[2] << 16) | (imageOffsetLittleEndian[1] << 8) | (imageOffsetLittleEndian[0]);

	// width and height at 18 bytes
	fseek ( fptr, 18, SEEK_SET );
	fread ( dimensionsLittleEndian, sizeof(unsigned char), 8, fptr );

	unsigned int widthFromBitmap =
		(dimensionsLittleEndian[3] << 24)	|
		(dimensionsLittleEndian[2] << 16)	|
		(dimensionsLittleEndian[1] << 8)	|
		(dimensionsLittleEndian[0]);
	unsigned int heightFromBitmap =
		(dimensionsLittleEndian[7] << 24)	|
		(dimensionsLittleEndian[6] << 16)	|
		(dimensionsLittleEndian[5] << 8)	|
		(dimensionsLittleEndian[4]);

	unsigned int scanLinePadding = calculateScanlinePadding(widthFromBitmap);
	unsigned int pixelBufferSize = widthFromBitmap * heightFromBitmap * 3;
	unsigned int paddedPixelBufferSize = pixelBufferSize + (heightFromBitmap * scanLinePadding);

	unsigned char * bitmapPixelBuffer = malloc (paddedPixelBufferSize);
	if (!bitmapPixelBuffer) {
		printf("Failed to allocate space for bitmap pixel buffer\n");
		fclose(fptr);
		return NULL;
	}

	unsigned char * pixelBuffer = malloc (pixelBufferSize);
	if (!pixelBuffer) {
		printf("Failed to allocate space for pixel buffer\n");
		fclose(fptr);
		free(bitmapPixelBuffer);
		return NULL;
	}

	fseek (fptr, imageOffset, SEEK_SET);
	fread (bitmapPixelBuffer, sizeof(unsigned char), paddedPixelBufferSize, fptr);
	fclose (fptr);

	for(int y = 0; y < heightFromBitmap; y++){
		for(int x = 0; x < widthFromBitmap; x++){
			int posFlipped = 3*((widthFromBitmap-1-x)+(widthFromBitmap*y));
			int pos = 3*((x)+(widthFromBitmap*y));
			int scanLineOffset = y * scanLinePadding;
			pixelBuffer[pixelBufferSize-1-(posFlipped+2)] = bitmapPixelBuffer[pos+2+scanLineOffset];
			pixelBuffer[pixelBufferSize-1-(posFlipped+1)] = bitmapPixelBuffer[pos+1+scanLineOffset];
			pixelBuffer[pixelBufferSize-1-(posFlipped+0)] = bitmapPixelBuffer[pos+0+scanLineOffset];
		}
	}

	free(bitmapPixelBuffer);

	if ( width )  *width = widthFromBitmap;
	if ( height ) *height = heightFromBitmap;

	return pixelBuffer;
}

int writeBMP(char const * filename, BMP * bmp) {
	return writePixels(filename, bmp->w, bmp->h, bmp->pixels);
}

BMP * readBMP (char const * filename) {
	BMP * bmp = malloc(sizeof(BMP));
	if (!bmp) {
		printf("Failed to allocate new BMP\n");
		return NULL;
	}

	bmp->pixels = readPixels(filename, &bmp->w, &bmp->h);
	return bmp;
}

BMP * newBMP (unsigned int w, unsigned int h) {
	BMP * bmp = malloc(sizeof(BMP));
	if (!bmp) {
		printf("Failed to allocate new BMP\n");
		return NULL;
	}

	bmp->pixels = malloc(3*w*h);
	if (!bmp->pixels) {
		printf("Failed to allocate new BMP Pixels\n");
		free(bmp);
		return NULL;
	}

	bmp->w = w;
	bmp->h = h;

	return bmp;
}

void destroyBMP (BMP * bmp) {
	free(bmp->pixels);
	free(bmp);
}

int pixelPos (int x, int y, int width, int height) {
	if ( x >= width ) return -1;
	if ( y >= height ) return -1;
	return 3*(x+( width * y ));
}

int setPixel (int x, int y, unsigned char redValue, unsigned char greenValue, unsigned char blueValue, BMP * bmp) {
	int pos = pixelPos(x, y, bmp->w, bmp->h);
	// printf("pos: %i\n", pos);
	bmp->pixels[pos+0] = redValue;
	bmp->pixels[pos+1] = greenValue;
	bmp->pixels[pos+2] = blueValue;

	return 0;
}

int setPixelChannel (int x, int y, unsigned char channel, unsigned char value, BMP * bmp) {
	int pos = pixelPos(x, y, bmp->w, bmp->h);
	if(pos >= 0) bmp->pixels[pos+channel] = value;
	return pos;
}
