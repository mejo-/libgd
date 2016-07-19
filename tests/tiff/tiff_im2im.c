#ifdef _WIN32
#include <stdio.h>
int main()
{
	printf("skip");
	return 0;
}
#else
#include "gd.h"
#include "gdtest.h"

int main()
{
	gdImagePtr src, dst;
	int r, g, b;
	void *p;
	int size = 0;
	int status = 0;
	CuTestImageResult result = {0, 0};

	src = gdImageCreate(100, 100);
	if (src == NULL) {
		gdTestErrorMsg("could not create src\n");
		return 1;
	}
	r = gdImageColorAllocate(src, 0xFF, 0, 0);
	g = gdImageColorAllocate(src, 0, 0xFF, 0);
	b = gdImageColorAllocate(src, 0, 0, 0xFF);
	gdImageFilledRectangle(src, 0, 0, 99, 99, r);
	gdImageRectangle(src, 20, 20, 79, 79, g);
	gdImageEllipse(src, 70, 25, 30, 20, b);

#define OUTPUT_TIFF(name) do {							\
		FILE *fp = gdTestTempFp();						\
		gdImageTiff(name, fp);							\
		fclose(fp);										\
	} while (0)

	OUTPUT_TIFF(src);
	p = gdImageTiffPtr(src, &size);
	if (p == NULL) {
		status = 1;
		gdTestErrorMsg("p is null\n");
		goto door0;
	}
	if (size <= 0) {
		status = 1;
		gdTestErrorMsg("size is non-positive\n");
		goto door1;
	}

	dst = gdImageCreateFromTiffPtr(size, p);
	if (dst == NULL) {
		status = 1;
		gdTestErrorMsg("could not create dst\n");
		goto door1;
	}
	OUTPUT_TIFF(dst);

	gdTestImageDiff(src, dst, NULL, &result);
	if (result.pixels_changed > 0) {
		status = 1;
		printf("pixels changed: %d\n", result.pixels_changed);
	}

	gdImageDestroy(dst);
door1:
	gdFree(p);
door0:
	gdImageDestroy(src);
	return status;
}
#endif