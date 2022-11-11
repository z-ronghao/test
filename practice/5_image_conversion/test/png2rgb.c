#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <libpng/png.h>
#include <setjmp.h>

int png2yuv(char *pngName, char *rgbName)
{
    int width, height, bit_depth, color_type;
    png_uint_32 flag;
    FILE *pngfile,*rgbfile;
    pngfile = fopen(pngName, "rb");
    if (!pngfile)
    {
        printf("open pngfile error\n");
        return -1;
    }
    rgbfile = fopen(rgbName, "w+");
    if (!rgbfile)
    {
        printf("open rgbfile error\n");
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

    uint8_t* rgbdata = (uint8_t *)malloc(width* height* 3);
    png_bytepp rows = (png_bytepp)png_malloc(png_ptr, height * sizeof(png_bytep));
    for (int i = 0; i < height; ++i) {
        rows[i] = (png_bytep)(rgbdata+ (i) * width * 3);
    }

    png_read_image(png_ptr, rows);
    png_read_end(png_ptr,info_ptr);
    png_destroy_read_struct(&png_ptr,&info_ptr,NULL);
    fclose(pngfile);
    fwrite(rgbdata, 1, width* height* 3, rgbfile);
    fclose(rgbfile);
    free(rows);
    free(rgbdata);

}

int main()
{
    char *pngName, *rgbName;
    pngName = "./rainbow.png";
    rgbName = "./rainbow.rgb";
    png2yuv(pngName, rgbName);
    return 0;
}