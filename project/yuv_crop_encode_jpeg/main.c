#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <libyuv/libyuv.h>
#include <libturbo-jpeg/turbojpeg.h>

int get_timer_now ()
{
    struct timeval now;
    gettimeofday(&now, NULL);
    return(now.tv_sec * 1000 + now.tv_usec / 1000);
}

void rgb2yuv(uint8_t *rgbData, uint8_t *yuvData, int width, int height) 
{
    RAWToI420
    (
        rgbData, width*3,
        yuvData, width,
        yuvData + width*height, (width+1)/2,
        yuvData + width*height + ((width+1)/2)*((height+1)/2), (width+1)/2,
        width, height
    );
}

void Crop(uint8_t *srcYuvData, uint8_t *dstYuvData, int width, int height, int CropLeft, int CropTop, int cropWidth, int cropHeight) 
{
    ConvertToI420
    (
        srcYuvData,
        width*height*3/2,
        dstYuvData,
        cropWidth,
        dstYuvData+cropWidth*cropHeight,
        (cropWidth+1)/2,
        dstYuvData+cropWidth*cropHeight+((cropWidth+1)/2)*((cropHeight+1)/2),
        (cropWidth+1)/2,
        CropLeft,
        CropTop,
        width,
        height,
        cropWidth,
        cropHeight,
        kRotate0,
        FOURCC_YU12
    );
        
}

int tyuv2jpeg(uint8_t* yuv_buffer, int yuv_size, int width, int height, int subsample, uint8_t **jpeg_buffer, uint64_t* jpeg_size, int quality)
{
    tjhandle handle = NULL;
    int flags = 0;
    int padding = 1; //1 or 4 
    int need_size = 0;
    int ret = 0;

    handle = tjInitCompress();

    flags |= 0;

    need_size = tjBufSizeYUV2(width, padding, height, subsample);
    if (need_size != yuv_size)
    {
        printf("we detect yuv size: %d, but you give: %d, check again.\n", need_size, yuv_size);
        return 0;
    }

    ret = tjCompressFromYUV(handle, yuv_buffer, width, padding, height, subsample, jpeg_buffer, jpeg_size, quality, flags);
    if (ret < 0)
    {
        printf("compress to jpeg failed: %s\n", tjGetErrorStr());
    }

    tjDestroy(handle);

    return ret;
}

uint8_t *readrgbData(int Width, int Height, char *rgbfilename)
{
    int rgbDataSize = Width * Height * 3;
    int yuvDataSize = Width * Height * 3 / 2;

    uint8_t *rgbData = malloc(rgbDataSize);
    uint8_t *outyuvData = malloc(yuvDataSize);

    FILE *rgbFile = fopen(rgbfilename, "rb");
    fread(rgbData, rgbDataSize, 1, rgbFile);
    rgb2yuv(rgbData, outyuvData, Width, Height);

    fflush(rgbFile);
    fclose(rgbFile);
    free(rgbData);

    return outyuvData;
};

uint8_t *readyuvData(int Width, int Height, char *yuvfilename)
{
    int yuvDataSize = Width * Height * 3 / 2;

    uint8_t *outyuvData = malloc(yuvDataSize);

    FILE *yuvFile = fopen(yuvfilename, "rb");
    fread(outyuvData, yuvDataSize, 1, yuvFile);

    fflush(yuvFile);
    fclose(yuvFile);

    return outyuvData;
};

int main(int argc, char *argv[])
{
        /**
         * Get input and out FileName, If the output file name is not set,
         * Add jpeg to the end of the input file name as the output file name.
         */
    char infile[64];
    char outfile[64];
    const char *pFile;
    uint8_t infilelen = strlen(infile);
    if(argc <= 1)
    {   
        printf("****   Please set input file(RBG24/YUV420) ****\n\n");
        return 0;
    }
    if (argc > 1)
	{
	    strcpy(infile, argv[1]);
		if (argc > 2)
			strcpy(outfile, argv[2]);
		else
		{
			strcpy(outfile, infile);
		    infilelen = strlen(infile);
		    strcpy(outfile + (infilelen - 3), "jpeg");
	    }
    }

        int FileType = 0;              // rgb file is 0 ,yuv file is 1
        pFile = strrchr(infile, '.'); 
    if (pFile != NULL)
    { 
        if (strcmp(pFile, ".rgb") == 0)
        { 
            printf("\n*************Input File is RGB File************\n\n");
            FileType = 0;
        }
        else if (strcmp(pFile, ".yuv") == 0)
        { 
            printf("\n*************Input File is YUV File*************\n\n");
            FileType = 1;
        }
        else
        {
            printf("****Input Error ,Please choose yuv or rgb file****\n\n");
            return 0;
        }
    }


        /**
         * The width and height of the received file
         * Set the crop image data
         * x and y are the starting point for cropping
         * the output image size is CropWidth * CropHeight
         */
    int Width = 700, Height = 700, CropLeft = 200, CropTop = 200, CropWidth = 200, CropHeight = 200;
    printf("***  请设置输入的文件的宽和高  ***\n");
    printf("the Width is :\n");
    scanf("%d", &Width);
    printf("the Height is :\n");
    scanf("%d", &Height);

    while(1)
    {   
        printf("\n***  请设置左侧和上方开始裁剪的位置  ***\n");
        printf("the CropLeft coordinate is :\n");
        scanf("%d", &CropLeft);
        printf("the CropTop coordinate is :\n");
        scanf("%d", &CropTop);
        if (CropLeft <= Width && CropTop <= Height)
        {
            break;
        }
        else
        {
            printf("**** Setting error, Please set (CropLeft <= Width) && (CropTop <= Height) ****\n\n");
        }
        
    }

    while(1)
    {
        printf("***  请设置图片裁剪后的宽和高  ***\n");
        printf("the CropWidth is :\n");
        scanf("%d", &CropWidth);
        printf("the CropHeight is :\n");
        scanf("%d", &CropHeight);
        printf("\n\n");
        if (CropWidth <= (Width - CropLeft) && CropHeight <= (Height - CropTop))
        {
            break;
        }
        else
        {
            printf("**** Setting error, Please set CropWidth <= (Width - CropLeft) && CropHeight <= (Height - CropTop) ****\n\n");
        }
    }
    
        /**
         * Reads files based on file type, Store the YUV file to memory
         * If it is an rgb file, it will be converted to a yuv file and then read into memory
         */
    int start = get_timer_now(); 
    uint8_t *yuvData;
    if(FileType == 0)
    {
        yuvData = readrgbData(Width, Height, infile);  
        int readrgbTime = get_timer_now();
        printf(" 读取rgb文件并转换为yuv文件的时间: %dms\n\n",readrgbTime-start);     
    }
    else
    {
        yuvData = readyuvData(Width, Height, infile);
        int readyuvTime = get_timer_now();
        printf(" 读取yuv文件的时间: %dms\n\n",readyuvTime-start);
    }
    if (NULL == yuvData)
    {
        printf(" read file failed\n\n");
        return 1;
    }
    
        /**
         * Reads the yuv data in memory and crops it
         */
    int cropstrat = get_timer_now();
    int yuvCropDataSize = CropWidth * CropHeight * 3 / 2;

    uint8_t *yuvCropData = malloc(yuvCropDataSize);

    Crop(yuvData, yuvCropData, Width, Height, CropLeft, CropTop, CropWidth, CropHeight);
    free(yuvData);

    int cropend = get_timer_now();
    printf(" 裁剪yuv数据的时间: %dms \n\n",cropend-cropstrat);

        /**
         * Reads the Cropyuv data in memory and encode to jpeg data
         */
    int yuv2jpegstart = get_timer_now();
    printf("↓↓↓↓↓↓↓↓↓↓ Encode YUV to JPEG ↓↓↓↓↓↓↓↓↓↓\n\n");
    int yuvSubsample = TJSAMP_420;
    uint8_t *yuv2jpegBuffer;
    uint64_t yuv2JpegSize;
    tyuv2jpeg(yuvCropData, yuvCropDataSize, CropWidth, CropHeight, yuvSubsample, &yuv2jpegBuffer, &yuv2JpegSize, 80);
    int yuv2jpegend = get_timer_now();
    printf(" 将裁剪后的yuv数据编码为jpeg数据的时间: %dms\n\n",yuv2jpegend-yuv2jpegstart);
    printf(" 编码后jpeg数据的大小: %ld 字节\n\n", yuv2JpegSize);

        /**
         * write jpeg file
         */
    int wjpegstart = get_timer_now();
    FILE *yuv2JpegOutFile = fopen(outfile, "wb");
    fwrite(yuv2jpegBuffer, yuv2JpegSize, 1, yuv2JpegOutFile);
    int wjpegend = get_timer_now();
    printf(" 写入jpeg图片的时间: %dms\n\n",wjpegend-wjpegstart);
    printf(" 总过程用时: %dms\n\n",wjpegend - start);

    fflush(yuv2JpegOutFile);
    fclose(yuv2JpegOutFile);
    free(yuvCropData);
    free(yuv2jpegBuffer);

    printf("↑↑↑↑↑↑↑↑↑↑ Encode YUV to JPEG ↑↑↑↑↑↑↑↑↑↑\n\n");
    
    return 0;
}