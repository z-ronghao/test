#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

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
    fclose(bmpfile);

    FILE* rgbfile = fopen("./700x700.rgb", "wb");
    if(!rgbfile) {
        printf("Could not write file \n");
        return -1;
    }

    //uint8_t* rgbbuffer = (uint8_t*)malloc(3 * bmpfileinfo.biWidth *bmpfileinfo.biHeight);
    //uint8_t* rgbbuffer = (uint8_t*)malloc(bmpline *bmpfileinfo.biHeight);

    for(i=0;i<bmpfileinfo.biHeight;i++)
    { //对于每一行
		for(j=0;j<bmpfileinfo.biWidth*3;j++)
        { //对于每一列
			// uint8_t r=*(bmpbuffer+bmpline*(bmpfileinfo.biHeight-1-i)+j); //从最后一行往上读
			// j++;
			// uint8_t g=*(bmpbuffer+bmpline*(bmpfileinfo.biHeight-1-i)+j);
			// j++;
			// uint8_t b=*(bmpbuffer+bmpline*(bmpfileinfo.biHeight-1-i)+j);
            uint8_t b=*(bmpbuffer+bmpline*(bmpfileinfo.biHeight-1-i)+j); //从最后一行往上读
			j++;
			uint8_t g=*(bmpbuffer+bmpline*(bmpfileinfo.biHeight-1-i)+j);
			j++;
			uint8_t r=*(bmpbuffer+bmpline*(bmpfileinfo.biHeight-1-i)+j);
           fwrite(&r, 1, 1, rgbfile);
           fwrite(&g, 1, 1, rgbfile);
           fwrite(&b, 1, 1, rgbfile);
        }
    }


    //fwrite(rgbbuffer, bmpline *bmpfileinfo.biHeight, 1, rgbfile);
    //fwrite(rgbbuffer, 3 * bmpfileinfo.biWidth *bmpfileinfo.biHeight, 1, rgbfile);
    fclose(rgbfile);
    free(bmpbuffer);
    //free(rgbbuffer);
    return 0;
}

int main()
{
    char *filename = "./4.bmp";
    bmp2rgb(filename);
}

