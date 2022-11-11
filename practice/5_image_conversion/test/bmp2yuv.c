#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

//#pragma pack(2)
typedef struct {
    //unsigned short bfType;
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

uint8_t bmp2rgb(char *name)
{   
    unsigned short bfType;
    BitmapFileHeader bmpfileheader;
    BitmapInfoHeader bmpfileinfo;
    int bmpline, i, j;

    FILE* bmpfile = fopen(name, "r");
    if(!bmpfile) {
        printf("Could not open file \n");
        return -1;
    }
    fread(&bfType, sizeof(unsigned short), 1, bmpfile);
    fread(&bmpfileheader, sizeof(BitmapFileHeader), 1, bmpfile);
    fread(&bmpfileinfo, sizeof(BitmapInfoHeader), 1, bmpfile);

    if ((bmpfileinfo.biWidth% 4) == 0)
        bmpline = bmpfileinfo.biWidth*3;
    else
        bmpline = (bmpfileinfo.biWidth*bmpfileinfo.biBitCount+ 31) / 32 * 4;

    if(bmpfileinfo.biHeight < 0)
    {
        printf("bmpheigth = %d\n",bmpfileinfo.biHeight);
        bmpfileinfo.biHeight = -bmpfileinfo.biHeight;
    }
    printf("bmpheigth = %d\n",bmpfileinfo.biHeight);
    printf("bmpwidth = %d\n",bmpfileinfo.biWidth);
    printf("bmpline = %d\n",bmpline);

    uint8_t* bmpbuffer = (uint8_t*)malloc(bmpline * bmpfileinfo.biHeight);
    fseek(bmpfile,bmpfileheader.bfOffBits,SEEK_SET);
    fread(bmpbuffer, bmpline * bmpfileinfo.biHeight, 1, bmpfile);
    
    FILE* rgbfile = fopen("./700x700.rgb", "w+");
    if(!rgbfile) {
        printf("Could not write file \n");
        return -1;
    } 

    uint8_t b,g,r;
    for(i=0;i<bmpfileinfo.biHeight;i++)
    { //对于每一行
		for(j=0;j<bmpline;j++)
        { //对于每一列
            if(*(bmpbuffer+bmpline*(bmpfileinfo.biHeight-1-i)+j) != 0)
             b=*(bmpbuffer+bmpline*(bmpfileinfo.biHeight-1-i)+j); //从最后一行往上读

			j++;
            if(*(bmpbuffer+bmpline*(bmpfileinfo.biHeight-1-i)+j) != 0)
			 g=*(bmpbuffer+bmpline*(bmpfileinfo.biHeight-1-i)+j);

			j++;
            if(*(bmpbuffer+bmpline*(bmpfileinfo.biHeight-1-i)+j) != 0)
			 r=*(bmpbuffer+bmpline*(bmpfileinfo.biHeight-1-i)+j);

           fwrite(&r, 1, 1, rgbfile);
           fwrite(&g, 1, 1, rgbfile);
           fwrite(&b, 1, 1, rgbfile);
        }
    }
    fclose(rgbfile);
    fclose(bmpfile);
    free(bmpbuffer);
    return 0;
}

uint8_t bmp2rgb1(char *name, uint8_t *rgbdata)
{   
    unsigned short bfType;
    BitmapFileHeader bmpfileheader;
    BitmapInfoHeader bmpfileinfo;
    int bmpline, i, j;

    FILE* bmpfile = fopen(name, "r");
    if(!bmpfile) {
        printf("Could not open file \n");
        return -1;
    }
    fread(&bfType, sizeof(unsigned short), 1, bmpfile);
    fread(&bmpfileheader, sizeof(BitmapFileHeader), 1, bmpfile);
    fread(&bmpfileinfo, sizeof(BitmapInfoHeader), 1, bmpfile);

    if ((bmpfileinfo.biWidth% 4) == 0)
        bmpline = bmpfileinfo.biWidth*3;
    else
        bmpline = (bmpfileinfo.biWidth*bmpfileinfo.biBitCount+ 31) / 32 * 4;

    if(bmpfileinfo.biHeight < 0)
    {
        printf("bmpheigth = %d\n",bmpfileinfo.biHeight);
        bmpfileinfo.biHeight = -bmpfileinfo.biHeight;
    }
    printf("bmpheigth = %d\n",bmpfileinfo.biHeight);
    printf("bmpwidth = %d\n",bmpfileinfo.biWidth);
    printf("bmpline = %d\n",bmpline);

    uint8_t* bmpbuffer = (uint8_t*)malloc(bmpline * bmpfileinfo.biHeight);
    fseek(bmpfile,bmpfileheader.bfOffBits,SEEK_SET);
    fread(bmpbuffer, bmpline * bmpfileinfo.biHeight, 1, bmpfile);
    
    FILE* rgbfile = fopen("./700x700.rgb", "w+");
    if(!rgbfile) {
        printf("Could not write file \n");
        return -1;
    } 

    uint8_t b,g,r;
    for(i=0;i<bmpfileinfo.biHeight;i++)
    { //对于每一行
		for(j=0;j<3*bmpfileinfo.biWidth;j++)
        { //对于每一列
        
            b=*(bmpbuffer+bmpline*(bmpfileinfo.biHeight-1-i)+j); //从最后一行往上读
            j++;
            g=*(bmpbuffer+bmpline*(bmpfileinfo.biHeight-1-i)+j);
            j++;
            r=*(bmpbuffer+bmpline*(bmpfileinfo.biHeight-1-i)+j);

            long current = 3*bmpfileinfo.biWidth*(i)+(j-2);
            rgbdata[current] = r;
            rgbdata[current+1] = g;
            rgbdata[current+2] = b;

        //    fwrite(&r, 1, 1, rgbfile);
        //    fwrite(&g, 1, 1, rgbfile);
        //    fwrite(&b, 1, 1, rgbfile);
        }
    }
    fclose(rgbfile);
    fclose(bmpfile);
    free(bmpbuffer);
    return 0;
}

void rgb24ToYuv420p(uint8_t *destination, uint8_t *rgb, int width, int height) {
    size_t image_size = width * height;
    size_t upos = image_size;
    size_t vpos = upos + upos / 4;
    size_t i = 0;

    for( size_t line = 0; line < height; ++line ) {
        if( !(line % 2) ) {
            for( size_t x = 0; x < width; x += 2 ) {
                uint8_t r = rgb[3 * i];
                uint8_t g = rgb[3 * i + 1];
                uint8_t b = rgb[3 * i + 2];

                destination[i++] = ((66*r + 129*g + 25*b) >> 8) + 16;

                destination[upos++] = ((-38*r + -74*g + 112*b) >> 8) + 128;
                destination[vpos++] = ((112*r + -94*g + -18*b) >> 8) + 128;

                r = rgb[3 * i];
                g = rgb[3 * i + 1];
                b = rgb[3 * i + 2];

                destination[i++] = ((66*r + 129*g + 25*b) >> 8) + 16;
            }
        } else {
            for( size_t x = 0; x < width; x += 1 ) {
                uint8_t r = rgb[3 * i];
                uint8_t g = rgb[3 * i + 1];
                uint8_t b = rgb[3 * i + 2];

                destination[i++] = ((66*r + 129*g + 25*b) >> 8) + 16;
            }
        }
    }
}

// void rgb24ToYuv444p(const uint8_t *rgb24Data, uint8_t *yuv444pData, int width, int height) {

//     int8_t yuv_y[width*height];
//     int8_t yuv_u[width*height];
//     int8_t yuv_v[width*height];

//     for (int i = 0; i < width; ++i) {
//         for (int j = 0; j < height; ++j) {
//             uint8_t Y, U, V;
//             uint8_t R, G, B;

//             int currentRGBIndex = 3*(i*height+j);
//             R = rgb24Data[currentRGBIndex];
//             G = rgb24Data[currentRGBIndex+1];
//             B = rgb24Data[currentRGBIndex+2];

//            // rgbToYuv(R, G, B, &Y, &U, &V);

//             int currentYUVIndex = i*height+j;
//             yuv_y[currentYUVIndex] = Y;
//             yuv_u[currentYUVIndex] = U;
//             yuv_v[currentYUVIndex] = V;
//         }
//     }
    
//     memcpy(yuv444pData, yuv_y, sizeof(yuv_y));
//     memcpy(yuv444pData + sizeof(yuv_y), yuv_u, sizeof(yuv_u));
//     memcpy(yuv444pData + sizeof(yuv_y) + sizeof(yuv_u), yuv_v, sizeof(yuv_v));
    
// }


int main()
{
    int width ,height;
    width = height =700;
    uint8_t rgb24Data[width*height*3];
    uint8_t yuv420pData[width*height*3/2];
    char *filename = "./2.bmp";
    bmp2rgb1(filename, rgb24Data);
    rgb24ToYuv420p(yuv420pData, rgb24Data, width, height);
    FILE *rgbFile = fopen("700x700.rgb", "wb");
    fwrite(rgb24Data, 1, width*height*3, rgbFile);
    FILE *yuvFile = fopen("700x700.yuv", "wb");
    fwrite(yuv420pData, 1, width*height*3/2, yuvFile);
    fclose(yuvFile);
    fclose(rgbFile);
    
}

