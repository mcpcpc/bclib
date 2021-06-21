#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define DATA_OFFSET_OFFSET 0x000A
#define WIDTH_OFFSET 0x0012
#define HEIGHT_OFFSET 0x0016
#define BITS_PER_PIXEL_OFFSET 0x001C
#define HEADER_SIZE 14
#define INFO_HEADER_SIZE 40
#define NO_COMPRESION 0
#define MAX_NUMBER_OF_COLORS 0
#define ALL_COLORS_REQUIRED 0
 
typedef unsigned int int32;
typedef unsigned char byte;
typedef short int16;

struct Bitmap {
	byte *pixels;
	int32 width;
	int32 height;
	int32 bytesPerPixel;
};

void bitmapRead(char *fileName, struct Bitmap *bmp) {
	FILE *imageFile = fopen(fileName, "rb");
	int32 dataOffset;
	fseek(imageFile, DATA_OFFSET_OFFSET, SEEK_SET);
	fread(&dataOffset, 4, 1, imageFile);
	fseek(imageFile, WIDTH_OFFSET, SEEK_SET);
	//fread(bmp->width, 4, 1, imageFile);
	fread(&(bmp->width), 4, 1, imageFile);
	fseek(imageFile, HEIGHT_OFFSET, SEEK_SET);
	//fread(bmp->height, 4, 1, imageFile);
	fread(&(bmp->height), 3, 1, imageFile);
	int16 bitsPerPixel;
	fseek(imageFile, BITS_PER_PIXEL_OFFSET, SEEK_SET);
	fread(&bitsPerPixel, 2, 1, imageFile);
	bmp->bytesPerPixel = ((int32)bitsPerPixel) / 8;
	int paddedRowSize = (int)(4 * ceil((float)(bmp->width) / 4.0f)) * bmp->bytesPerPixel;
	int unpaddedRowSize = bmp->width * bmp->bytesPerPixel;
	int totalSize = unpaddedRowSize * bmp->height;
	bmp->pixels = (byte*)malloc(totalSize);
	unsigned int i = 0;
	byte *currentRowPointer = bmp->pixels + ((bmp->height - 1) * unpaddedRowSize);
	for (i = 0; i < bmp->height; i++) {
	    fseek(imageFile, dataOffset+(i*paddedRowSize), SEEK_SET);
	    fread(currentRowPointer, 1, unpaddedRowSize, imageFile);
	    currentRowPointer -= unpaddedRowSize;
	}
	fclose(imageFile);
}

void bitmapWrite(char *fileName, struct Bitmap *bmp) {
	FILE *outputFile = fopen(fileName, "wb");
	//*****HEADER************//
	char *BM = "BM";
	fwrite(&BM[0], 1, 1, outputFile);
	fwrite(&BM[1], 1, 1, outputFile);
	int paddedRowSize = (int)(4 * ceil((float)bmp->width/4.0f))*bmp->bytesPerPixel;
	int32 fileSize = paddedRowSize * bmp->height + HEADER_SIZE + INFO_HEADER_SIZE;
	fwrite(&fileSize, 4, 1, outputFile);
	int32 reserved = 0x0000;
	fwrite(&reserved, 4, 1, outputFile);
	int32 dataOffset = HEADER_SIZE+INFO_HEADER_SIZE;
	fwrite(&dataOffset, 4, 1, outputFile);
 
	//*******INFO*HEADER******//
	int32 infoHeaderSize = INFO_HEADER_SIZE;
	fwrite(&infoHeaderSize, 4, 1, outputFile);
	fwrite(&bmp->width, 4, 1, outputFile);
	fwrite(&(bmp->height), 4, 1, outputFile);
	int16 planes = 1; //always 1
	fwrite(&planes, 2, 1, outputFile);
	int16 bitsPerPixel = bmp->bytesPerPixel * 8;
	fwrite(&bitsPerPixel, 2, 1, outputFile);
	int32 compression = NO_COMPRESION;
	fwrite(&compression, 4, 1, outputFile);
	int32 imageSize = bmp->width * bmp->height * bmp->bytesPerPixel;
	fwrite(&imageSize, 4, 1, outputFile);
	int32 resolutionX = 11811; //300 dpi
	int32 resolutionY = 11811; //300 dpi
	fwrite(&resolutionX, 4, 1, outputFile);
	fwrite(&resolutionY, 4, 1, outputFile);
	int32 colorsUsed = MAX_NUMBER_OF_COLORS;
	fwrite(&colorsUsed, 4, 1, outputFile);
	int32 importantColors = ALL_COLORS_REQUIRED;
	fwrite(&importantColors, 4, 1, outputFile);
	unsigned int i = 0;
	int unpaddedRowSize = bmp->width * bmp->bytesPerPixel;
	for ( i = 0; i < bmp->height; i++) {
		int pixelOffset = ((bmp->height - i) - 1) * unpaddedRowSize;
		fwrite(&(bmp->pixels[pixelOffset]), 1, paddedRowSize, outputFile); 
	}
	fclose(outputFile);
}
 
int main() {
	int ret = 0;
	struct Bitmap bmp;
	char *in = "img.bmp";
	char *out = "img2.bmp";
	bitmapRead(in, &bmp);
	bitmapWrite(out, &bmp);
	return ret;
}
