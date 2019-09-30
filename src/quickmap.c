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
	
	unsigned char pixelBuffer[width*height*3 + scanLinePadding*height];
	unsigned int newFileSize = (0x36 + width*height*3 + scanLinePadding*height); //header + pixels + padding
	
	unsigned char dimensions[8] = {width&0x000000FF,(width&0x0000FF00)>>8,(width&0x00FF0000)>>16,(width&0xFF000000)>>24,
								height&0x000000FF,(height&0x0000FF00)>>8,(height&0x00FF0000)>>16,(height&0xFF000000)>>24};
	unsigned char fileSize[4] = {newFileSize&0x000000FF,(newFileSize&0x0000FF00)>>8,(newFileSize&0x00FF0000)>>16,(newFileSize&0xFF000000)>>24}; // convert to little endian
	
	printf("file size: %i\n", newFileSize);
	printf("width: %i, height: %i\n", width, height);
	int line=0;
	int currX=0;
	int currY=0;
	int bytePos = 0;
	//for every byte
	for(int i = 0; i < (width*height*3 + scanLinePadding*height); i+=3){
//		printf("X: %i, Y: %i, i: %i\n",currX,currY,i);
		int pos = 3*((currX)+((width)*currY));
//		printf("[%i,%i,%i], [%i,%i,%i]\n",pos+2,pos+1,pos+0,imagePixels[pos+2],imagePixels[pos+1],imagePixels[pos+0]);
		pixelBuffer[i] = imagePixels[pos+2];
		pixelBuffer[i+1] = imagePixels[pos+1];
		pixelBuffer[i+2] = imagePixels[pos+0];
		
		currX++;
		
		if ((currX > width-1) && (scanLinePadding)){
//			printf("padding time\n");
			for(int p = 0; p < scanLinePadding; p++){
				pixelBuffer[i+3+p] = 0x00;
//				i++;
			}
			i+=scanLinePadding;
			currX=0;
			currY++;
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

//BMP_FILE * readBMP(char const* path){
//	unsigned char width_little_endian[4];
//	unsigned char height_little_endian[4];
//
//	FILE *fptr;
//	fptr = fopen(path, "rb");
//	if (fptr == NULL){
//		printf("fail file open\n");
//	}
//	//width and height at 18 bytes
//	fseek ( fptr, 18, SEEK_SET );
//	fread ( width_little_endian, sizeof(unsigned char), 4, fptr );
//	fread ( height_little_endian, sizeof(unsigned char), 4, fptr );
//
//	unsigned char bitmap_pixel_buffer[width * height * bits_per_pixel/8];
//
//	fseek ( fptr, 18, SEEK_SET );
//	fread ( bitmap_pixel_buffer, sizeof(unsigned char), width * height * bits_per_pixel/8, fptr );
//
//	fclose(fptr);
//
//
//
//	for (int i = 0; i < width * height * bits_per_pixel/8; ++i)
//	{
//		pixelBuffer[3 * (y * width + x)] = imagePixels[3 * (y * width + x)+2];
//		pixelBuffer[3 * (y * width + x)+1] = imagePixels[3 * (y * width + x)+1];
//		pixelBuffer[3 * (y * width + x)+2] = imagePixels[3 * (y * width + x)];
//	}
//
//	BMP_FILE * bmp = malloc(sizeof(BMP_FILE));
//	bmp->w = (width_little_endian[3] << 24) | (width_little_endian[2] << 16) | (width_little_endian[1] << 8) | (width_little_endian[0]);
//	bmp->h = (height_little_endian[3] << 24) | (height_little_endian[2] << 16) | (height_little_endian[1] << 8) | (height_little_endian[0]);
//
//	return bmp;
//}
