#ifndef _IMAGE_CONVERSION_H
#define _IMAGE_CONVERSION_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <setjmp.h>
#include <sys/stat.h>
#include <libpng/png.h>
#include <libturbo-jpeg/turbojpeg.h>

#pragma pack(2)
typedef struct {
    unsigned short bfType;
    unsigned int   bfSize;           /* Size of file */
    unsigned short bfReserved1;      /* Reserved */
    unsigned short bfReserved2;      /* ... */
    unsigned int   bfOffBits;        /* Offset to bitmap data */
} BitmapFileHeader;

typedef struct {
    unsigned int   biSize; /* Size of info header */
    int            biWidth; /* Width of image */
    int            biHeight; /* Height of image */
    unsigned short biPlanes; /* Number of color planes */
    unsigned short biBitCount; /* Number of bits per pixel */
    unsigned int   biCompression; /* Type of compression to use */
    unsigned int   biSizeImage; /* Size of image data */
    int            biXPelsPerMeter; /* X pixels per meter */
    int            biYPelsPerMeter; /* Y pixels per meter */
    unsigned int   biClrUsed; /* Number of colors used */
    unsigned int   biClrImportant; /* Number of important colors */
} BitmapInfoHeader;

uint8_t clip_value(uint8_t x, uint8_t min_val, uint8_t  max_val);

int bmp_2_rgb24(char* bmpName, uint8_t **rgbdata, int *file_width, int *file_height);
int png_2_rgb24(char* pngName, uint8_t **rgbdata, int *file_width, int *file_height);
int jpeg_2_yuv420(char* bmpName, uint8_t **yuvata,int *file_width, int *file_height, int* yuv_size);

int rgb24_2_yuv420(uint8_t *rgbdata, uint8_t *yuvdata, int width, int height);
int yuv420_2_rgb24(uint8_t *yuvdata, uint8_t *rgbdata, int width, int height);

int rgb24_2_bmp(uint8_t *rgbdata, char *bmpName, int width, int height);
int rgb24_2_png(uint8_t *rgbdata, char *pngName, int width, int height);
int yuv420_2_jpeg(uint8_t *yuvdata, char *jpegName, int width, int height);



#endif





