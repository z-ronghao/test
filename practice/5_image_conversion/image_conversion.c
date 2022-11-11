#include "image_conversion.h"

int read_bmpinfo(char *bmpName, int *width, int *height)
{
    BitmapFileHeader bmpfileheader;
    BitmapInfoHeader bmpfileinfo;
    int bmpline, i, j;

    FILE* bmpfile = fopen(bmpName, "r");
    if(!bmpfile) {
        printf("Could not open file \n");
        return -1;
    }
    fread(&bmpfileheader, sizeof(BitmapFileHeader), 1, bmpfile);
    fread(&bmpfileinfo, sizeof(BitmapInfoHeader), 1, bmpfile);
    width = &bmpfileinfo.biWidth;
    height = &bmpfileinfo.biHeight;
    fclose(bmpfile);
}
int read_pnginfo(char *pngName, int *width, int *height);
int read_jpeginfo(char *jpegName, int *width, int *height);

int bmp_2_rgb24(char* bmpName, uint8_t *rgbdata)
{
    BitmapFileHeader bmpfileheader;
    BitmapInfoHeader bmpfileinfo;
    int bmpline, i, j;

    FILE* bmpfile = fopen(bmpName, "r");
    if(!bmpfile) {
        printf("Could not open file \n");
        return -1;
    }
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
    
    // FILE* rgbfile = fopen("./700x700.rgb", "w+");
    // if(!rgbfile) {
    //     printf("Could not write file \n");
    //     return -1;
    // } 

    uint8_t r,g,b;
    for(i=0;i<bmpfileinfo.biHeight;i++)
    { //对于每一行
		for(j=0;j<3*bmpfileinfo.biWidth;j++)
        { //对于每一列
        
            b=*(bmpbuffer+bmpline*(bmpfileinfo.biHeight-1-i)+j); //从最后一行往上读
            j++;
            g=*(bmpbuffer+bmpline*(bmpfileinfo.biHeight-1-i)+j);
            j++;
            r=*(bmpbuffer+bmpline*(bmpfileinfo.biHeight-1-i)+j);

            int current = 3*bmpfileinfo.biWidth*(i)+(j-2);
            rgbdata[current] = r;
            rgbdata[current+1] = g;
            rgbdata[current+2] = b;

        //    fwrite(&r, 1, 1, rgbfile);
        //    fwrite(&g, 1, 1, rgbfile);
        //    fwrite(&b, 1, 1, rgbfile);
        }
    }
    // fclose(rgbfile);
    fclose(bmpfile);
    free(bmpbuffer);
    return 0;
}
int png_2_rgb24(char* pngName, uint8_t *rgbdata);
int jpeg_2_rgb24(char* jpegName, uint8_t *rgbdata);
int jpeg_2_yuv420(char* bmpName, uint8_t *yuvata);

int rgb24_2_yuv420(uint8_t *rgbdata, uint8_t *yuvdata);
int yuv420_2_rgb24(uint8_t *yuvdata, uint8_t *rgbdata);

int rgb24_2_bmp(uint8_t *rgbdata, char *bmpName, int width, int height)
{
    FILE *bitmapFile = fopen(bmpName, "wb");
    if(!bitmapFile) {
        printf("Could not write file \n");
        return 0;
    }      

    BitmapFileHeader fileHeader;
    fileHeader.bfType = 0x4d42;
    fileHeader.bfReserved1 = 0;
    fileHeader.bfReserved2 = 0;
    fileHeader.bfSize = sizeof(BitmapFileHeader) + sizeof(BitmapInfoHeader) + width*height*3;
    fileHeader.bfOffBits = 0x36;

    BitmapInfoHeader infoHeader;
    infoHeader.biSize = sizeof(BitmapInfoHeader);
    infoHeader.biWidth = width;
//    infoHeader.biHeight = height;
    infoHeader.biHeight = height;
    infoHeader.biPlanes = 1;
    infoHeader.biBitCount = 24;
    infoHeader.biSizeImage = 0;
    infoHeader.biCompression = 0;
    infoHeader.biXPelsPerMeter = 5000;//?
    infoHeader.biYPelsPerMeter = 5000;//?
    infoHeader.biClrUsed = 0;
    infoHeader.biClrImportant = 0;

    int bmplinebytes;
    if ((width% 4) == 0)
        bmplinebytes = width*3;
    else
        bmplinebytes = (width*24+ 31) / 32 * 4;

    fwrite(&fileHeader, sizeof(fileHeader), 1, bitmapFile);
    fwrite(&infoHeader, sizeof(infoHeader), 1, bitmapFile);
    int bmpheight = height;
    uint8_t r,g,b;
    for (int i = 0; i < height; i++) 
    {
        uint8_t lineBytesArray[bmplinebytes];
        
        for (int j = 0; j < 3* width; j++) 
        {
            r =*(rgbdata + (i* width* 3) + j);
            j++;
            g =*(rgbdata + (i* width* 3) + j);
            j++;
            b =*(rgbdata + (i* width* 3) + j);

            int currentIndex = j-2;
            lineBytesArray[currentIndex] = b;
            lineBytesArray[currentIndex+1] = g;
            lineBytesArray[currentIndex+2] = r;
        }
        fseek(bitmapFile, 54+bmplinebytes*(bmpheight-1),SEEK_SET);
        fwrite(lineBytesArray, sizeof(lineBytesArray), 1, bitmapFile);
        bmpheight--;
    }
    fclose(bitmapFile);

}

// int rgb24_2_png(uint8_t *rgbdata, char *pngName, int width, int height)
// {
//     png_structp png_ptr;
//     png_infop info_ptr;
//     int bit_depth = 8;

//     //uint8_t rgb24Data[width*height*3];

//     // FILE *file = fopen("C:\\Users\\Administrator\\Desktop\\lodepng-rgb-rainbow.png", "wb+");
//     FILE *png_file = fopen(pngName, "wb");
//     if (!png_file) {
//         return 0;
//     }

//     png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
//     if(png_ptr == NULL) {
//         printf("ERROR:png_create_write_struct/n");
//         fclose(png_file);
//         return 0;
//     }
//     info_ptr = png_create_info_struct(png_ptr);
//     if(info_ptr == NULL) {
//         printf("ERROR:png_create_info_struct/n");
//         png_destroy_write_struct(&png_ptr, NULL);
//         return 0;
//     }
//     png_init_io(png_ptr, png_file);
//     png_set_IHDR(
//             png_ptr,
//             info_ptr,
//             width,
//             height,
//             bit_depth,
//             PNG_COLOR_TYPE_RGB,
//             PNG_INTERLACE_NONE,
//             PNG_COMPRESSION_TYPE_BASE,
//             PNG_FILTER_TYPE_BASE);


//     png_colorp palette = (png_colorp)png_malloc(png_ptr, PNG_MAX_PALETTE_LENGTH * sizeof(png_color));
//     if (!palette) {
//         fclose(png_file);
//         png_destroy_write_struct(&png_ptr, &info_ptr);
//         return 0;
//     }
//     png_set_PLTE(png_ptr, info_ptr, palette, PNG_MAX_PALETTE_LENGTH);
//     png_write_info(png_ptr, info_ptr);
//     png_set_packing(png_ptr);

//     // //getRainbowRGB24Data(rgb24Data, width, height);
//     // FILE *rgbfile = fopen("./rainbow.rgb","rb");
//     // fread(rgbdata, 1, width* height* 3, rgbfile);

//     //这里就是图像数据了
//     png_bytepp rows = (png_bytepp)png_malloc(png_ptr, height * sizeof(png_bytep));
//     for (int i = 0; i < height; ++i)
//     {
//         rows[i] = (png_bytep)(rgbdata + (i) * width * 3);
//     }

//     png_write_image(png_ptr, rows);

//     // delete[] rows;
//     png_write_end(png_ptr, info_ptr);

//     png_free(png_ptr, palette);
//     palette=NULL;
//     png_destroy_write_struct(&png_ptr, &info_ptr);
//     fclose(png_file);
//     //fclose(rgbfile);
// }

int rgb24_2_png(uint8_t *rgbdata, char *pngName, int width, int height)
{
    png_structp png_ptr = NULL;
    png_infop info_ptr = NULL;
    int iRetVal;
    int bit_depth = 8;

    FILE *png_file = fopen(pngName, "wb");
    if (!png_file) 
    {
        printf("pngfile open error\n");
        return -1;
    }

    png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if(png_ptr == NULL) 
    {
        printf("ERROR:png_create_write_struct/n");
        fclose(png_file);
        return -1;
    }
    info_ptr = png_create_info_struct(png_ptr);
    if(info_ptr == NULL) 
    {
        printf("ERROR:png_create_info_struct/n");
        png_destroy_write_struct(&png_ptr, NULL);
        fclose(png_file);
        return -1;
    }
    iRetVal = setjmp(png_jmpbuf(png_ptr));
    if(iRetVal)
    {
        fprintf(stderr,"error code: %d\n",iRetVal);
        png_destroy_write_struct(&png_ptr, &info_ptr);
        return 0;
    }
    png_init_io(png_ptr, png_file);
    png_set_IHDR(
            png_ptr,
            info_ptr,
            width,
            height,
            bit_depth,
            PNG_COLOR_TYPE_RGB,
            PNG_INTERLACE_NONE,
            PNG_COMPRESSION_TYPE_BASE,
            PNG_FILTER_TYPE_BASE);

    // png_colorp palette = (png_colorp)png_malloc(png_ptr, PNG_MAX_PALETTE_LENGTH * sizeof(png_color));
    // if (!palette) 
    // {
    //     fclose(png_file);
    //     png_destroy_write_struct(&png_ptr, &info_ptr);
    //     return 0;
    // }
    // png_set_PLTE(png_ptr, info_ptr, palette, PNG_MAX_PALETTE_LENGTH);
    png_set_packing(png_ptr);
    png_write_info(png_ptr, info_ptr);


    //这里就是图像数据了
    png_uint_32 png_width = width, png_height = height;
    png_bytep rows[png_height];
    png_uint_32 i;
    for (i = 0; i < height; ++i)
    {
        rows[i] = rgbdata + (i)* png_width* 3;
    }

    png_write_image(png_ptr, rows);

    // delete[] rows;
    png_write_end(png_ptr, info_ptr);

    //png_free(png_ptr);
    png_destroy_write_struct(&png_ptr, &info_ptr);
    fclose(png_file);
    //fclose(rgbfile);
}
int rgb24_2_jpeg(uint8_t *rgbdata, char *jpegName, int width, int height);
int yuv420_2_jpeg(uint8_t *yuvdata, char *jpegName, int width, int height);