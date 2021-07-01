#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "bc.h"

int main(int argc, char *argv[]) {
	int ret = 0;
	if (argc != 3) {
		puts("usage: bc [src.bmp] [dest.bmp]");
		ret = 1;
	} else {
		char *in = argv[1];
		char *out = argv[2];
		struct Bitmap bmp;
		bitmapRead(in, &bmp);
		bitmapWrite(out, &bmp);
		bitmapFree(&bmp);
	}
	return ret;
}
