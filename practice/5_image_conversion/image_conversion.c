#include "image_conversion.h"

uint8_t clip_value(uint8_t x, uint8_t min_val, uint8_t  max_val)
{
	if(x>max_val){
		return max_val;
	}else if(x<min_val){
		return min_val;
	}else{
		return x;
	}
}

int bmp_2_rgb24(char* bmpName, uint8_t **rgbdata, int *file_width, int *file_height)
{
    BitmapFileHeader bmpfileheader;
    BitmapInfoHeader bmpfileinfo;
    int bmpline, i, j;

    FILE* bmpfile = fopen(bmpName, "rb");
    if(!bmpfile) {
        printf("Could not open file \n");
        return -1;
    }
    fread(&bmpfileheader, sizeof(BitmapFileHeader), 1, bmpfile);
    fread(&bmpfileinfo, sizeof(BitmapInfoHeader), 1, bmpfile);

    *file_width = bmpfileinfo.biWidth;
    *file_height = bmpfileinfo.biHeight;

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

    *rgbdata = (uint8_t *)malloc(bmpfileinfo.biWidth* bmpfileinfo.biHeight* 3);
    if(*rgbdata == NULL)
    {
        printf("malloc rgbdata error\n");
        return 0;
    }
    uint8_t* bmpbuffer = (uint8_t*)malloc(bmpline * bmpfileinfo.biHeight);
    fseek(bmpfile,bmpfileheader.bfOffBits,SEEK_SET);
    fread(bmpbuffer, bmpline * bmpfileinfo.biHeight, 1, bmpfile);
    
    // FILE* rgbfile = fopen("./700x700.rgb", "w+");
    // if(!rgbfile) {
    //     printf("Could not write file \n");
    //     return -1;
    // } 
    uint8_t *outdata = *rgbdata;
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
            outdata[current] = r;
            outdata[current+1] = g;
            outdata[current+2] = b;

        //    fwrite(&r, 1, 1, rgbfile);
        //    fwrite(&g, 1, 1, rgbfile);
        //    fwrite(&b, 1, 1, rgbfile);
        }
    }
    printf("read rgbdata ok\n");
    // fclose(rgbfile);
    fflush(bmpfile);
    fclose(bmpfile);
    free(bmpbuffer);
    return 0;
}
int png_2_rgb24(char* pngName, uint8_t **rgbdata, int *file_width, int *file_height)
{
    int width, height, bit_depth, color_type;
    png_uint_32 flag;
    FILE *pngfile;
    pngfile = fopen(pngName, "rb");
    if (!pngfile)
    {
        printf("open pngfile error\n");
        return -1;
    }

    png_structp png_ptr=NULL; //libpng的结构体
    png_infop   info_ptr=NULL; //libpng的信息
    int iRetVal;
    png_ptr=png_create_read_struct(PNG_LIBPNG_VER_STRING,NULL,NULL,NULL);
    if (!png_ptr)
    {
        printf("create_read_struct error\n");
        return -1;
    }
    info_ptr=png_create_info_struct(png_ptr);
    if (!info_ptr)
    {
        png_destroy_read_struct(&png_ptr,NULL,NULL);
        printf("create_info_struct error\n");
        return -1;
    }
    iRetVal= setjmp (png_jmpbuf(png_ptr)); //安装错误处理跳转点
//当libpng内部出现错误的时候，libpng会调用longjmp直接跳转到这里运行。
    if (iRetVal) //setjmp的返回值就是libpng跳转后提供的错误代码（貌似总是1，但是还是请大家看libpng的官方文档）
    {
        fprintf (stderr, "错误码：%d\n" ,iRetVal);
        png_destroy_read_struct(&png_ptr,&info_ptr,NULL);
        return -1;
    }

    png_init_io(png_ptr,pngfile);
    png_read_info(png_ptr, info_ptr);

    png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type, NULL, NULL, NULL);
    *file_width = width;
    *file_height = height;

    if (bit_depth==16)
        png_set_strip_16(png_ptr);
    if (color_type==PNG_COLOR_TYPE_PALETTE)
        png_set_expand(png_ptr); 
    if (bit_depth<8)
        png_set_expand(png_ptr);
    if (png_get_valid(png_ptr,info_ptr,PNG_INFO_tRNS))
        png_set_expand(png_ptr);
    if (color_type == PNG_COLOR_TYPE_GRAY || color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
        png_set_gray_to_rgb(png_ptr); //灰度必须转换成RGB

    *rgbdata = (uint8_t *)malloc(width* height* 3);
    uint8_t *outdata = *rgbdata ;
    png_bytepp rows = (png_bytepp)png_malloc(png_ptr, height * sizeof(png_bytep));
    for (int i = 0; i < height; ++i) 
    {
        rows[i] = (png_bytep)(outdata+ (i) * width * 3);
    }

    png_read_image(png_ptr, rows);
    png_read_end(png_ptr,info_ptr);
    png_destroy_read_struct(&png_ptr,&info_ptr,NULL);
    printf("read rgbdata ok\n");
    fflush(pngfile);
    fclose(pngfile);
    free(rows);
}
int jpeg_2_yuv420(char* jpegName, uint8_t **yuvdata, int *file_width, int *file_height, int *yuv_size)
{
    FILE *jpegFile = fopen(jpegName, "rb");
    struct stat statbuf;
    stat(jpegName, &statbuf);
    int jpeg_size=statbuf.st_size;
    printf("jpegfileLength: %d\n", jpeg_size);

    uint8_t *jpegData = malloc(jpeg_size);
    fread(jpegData, 1, jpeg_size, jpegFile);


    // tjpeg2yuv(unsigned char* jpeg_buffer, int jpeg_size, unsigned char** yuv_buffer, int* yuv_size, int* yuv_type)
    tjhandle handle = NULL;
    int width, height, subsample, colorspace, yuv_type;
    int flags = 0;
    int padding = 1; // 1或4均可，但不能是0
    int ret = 0;

    handle = tjInitDecompress();
    tjDecompressHeader3(handle, jpegData, jpeg_size, &width, &height, &subsample, &colorspace);
    *file_width = width;
    *file_height = height;

    printf("w: %d h: %d subsample: %d color: %d\n", width, height, subsample, colorspace);

    flags |= 0;

    yuv_type = subsample;
    *yuv_size = tjBufSizeYUV2(width, padding, height, subsample);
    *yuvdata =(uint8_t *)malloc(*yuv_size);

    ret = tjDecompressToYUV2(handle, jpegData, jpeg_size, *yuvdata, width, padding, height, flags);
    if (ret < 0)
    {
        printf("compress to jpeg failed: %s\n", tjGetErrorStr());
    }
    tjDestroy(handle);
    free(jpegData);
    fflush(jpegFile);
    fclose(jpegFile);
    printf("read yuvdata ok\n");
    return ret;
}

int rgb24_2_yuv420(uint8_t *rgbdata, uint8_t *yuvdata, int width, int height)
{
    size_t image_size = width * height;
    size_t upos = image_size;
    size_t vpos = upos + upos / 4;
    size_t i = 0;

    for( size_t line = 0; line < height; ++line ) {
        if( !(line % 2) ) {
            for( size_t x = 0; x < width; x += 2 ) 
            {
                uint8_t r = rgbdata[3 * i];
                uint8_t g = rgbdata[3 * i + 1];
                uint8_t b = rgbdata[3 * i + 2];

                yuvdata[i++] = ((66*r + 129*g + 25*b) >> 8) + 16;

                yuvdata[upos++] = ((-38*r + -74*g + 112*b) >> 8) + 128;
                yuvdata[vpos++] = ((112*r + -94*g + -18*b) >> 8) + 128;

                r = rgbdata[3 * i];
                g = rgbdata[3 * i + 1];
                b = rgbdata[3 * i + 2];

                yuvdata[i++] = ((66*r + 129*g + 25*b) >> 8) + 16;
            }
        } else {
            for( size_t x = 0; x < width; x += 1 ) 
            {
                uint8_t r = rgbdata[3 * i];
                uint8_t g = rgbdata[3 * i + 1];
                uint8_t b = rgbdata[3 * i + 2];

                yuvdata[i++] = ((66*r + 129*g + 25*b) >> 8) + 16;
            }
        }
    }
    printf("read yuv data ok\n");
}
int yuv420_2_rgb24(uint8_t *yuvdata, uint8_t *rgbdata, int width, int height)
{

int index_y, index_u, index_v;
	uint8_t y, u, v;
	uint8_t r, g, b;

	for (size_t i = 0; i < height; i++)
	{
		for (size_t j = 0; j < width; j++)
		{
			index_y = i * width + j;
			index_u = width * height + i / 4 * width + j / 2;
			index_v = width * height * 5 / 4 + i / 4 * width + j / 2;

			y = yuvdata[index_y];
			u = yuvdata[index_u];
			v = yuvdata[index_v];			

			b = y + 1.772*(u - 128);						//B = Y +1.779*(U-128)
			g = y - 0.34413*(u - 128) - 0.71414*(v - 128);	//G = Y-0.3455*(U-128)-0.7169*(V-128)
			r = y + 1.402*(v - 128);						//R = Y+1.4075*(V-128)

			*(rgbdata++) = clip_value(r, 0, 255);
			*(rgbdata++) = clip_value(g, 0, 255);
			*(rgbdata++) = clip_value(b, 0, 255);
			
		}
	}
    printf("read rgb data ok\n");
}

int rgb24_2_bmp(uint8_t *rgbdata, char *bmpName, int width, int height)
{
    FILE *bmpfile = fopen(bmpName, "wb");
    if(!bmpfile) {
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

    fwrite(&fileHeader, sizeof(fileHeader), 1, bmpfile);
    fwrite(&infoHeader, sizeof(infoHeader), 1, bmpfile);
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
        fseek(bmpfile, 54+bmplinebytes*(bmpheight-1),SEEK_SET);
        fwrite(lineBytesArray, sizeof(lineBytesArray), 1, bmpfile);
        bmpheight--;
    }
    fflush(bmpfile);
    fclose(bmpfile);
    printf("out bmp file ok\n");

}
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


    png_write_end(png_ptr, info_ptr);
    png_destroy_write_struct(&png_ptr, &info_ptr);
    fflush(png_file);
    fclose(png_file);
    printf("out png file ok\n");
}
int yuv420_2_jpeg(uint8_t *yuvdata, char *jpegName, int width, int height)
{
    tjhandle handle = NULL;
    int flags = 0;
    int padding = 4; //1 or 4 
    int need_size = 0;
    int ret = 0;
    int subsample = TJSAMP_420;
    int yuv_size = width* height* 3/ 2;
    int quality = 80;

    FILE* jpegfile = fopen(jpegName, "wb");
    uint8_t *jpegdata;
    uint64_t jpegsize;
    handle = tjInitCompress();

    flags |= 0;

    need_size = tjBufSizeYUV2(width, padding, height, subsample);
    if (need_size != yuv_size)
    {
        printf("we detect yuv size: %d, but you give: %d, check again.\n", need_size, yuv_size);
        return 0;
    }

    ret = tjCompressFromYUV(handle, yuvdata, width, padding, height, subsample, &jpegdata, &jpegsize, quality, flags);
    if (ret < 0)
    {
        printf("compress to jpeg failed: %s\n", tjGetErrorStr());
    }

    tjDestroy(handle);

    fwrite(jpegdata, 1, jpegsize, jpegfile);
    free(jpegdata);
    fflush(jpegfile);
    fclose(jpegfile);
    printf("out jpeg file ok\n");

    return ret;
}
