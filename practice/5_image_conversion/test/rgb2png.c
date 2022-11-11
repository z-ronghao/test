#include <stdint.h>
#include <stdlib.h>
#include <libpng/png.h>

void testRainbow() {
    int width = 2560, height = 1440, bit_depth = 8;
    uint8_t rgb24Data[width*height*3];
    png_structp png_ptr;
    png_infop info_ptr;

    // FILE *file = fopen("C:\\Users\\Administrator\\Desktop\\lodepng-rgb-rainbow.png", "wb+");
    FILE *png_file = fopen("./1.png", "wb");
    if (!png_file) {
        return ;
    }

    png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if(png_ptr == NULL) {
        printf("ERROR:png_create_write_struct/n");
        fclose(png_file);
        return ;
    }
    info_ptr = png_create_info_struct(png_ptr);
    if(info_ptr == NULL) {
        printf("ERROR:png_create_info_struct/n");
        png_destroy_write_struct(&png_ptr, NULL);
        return ;
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


    png_colorp palette = (png_colorp)png_malloc(png_ptr, PNG_MAX_PALETTE_LENGTH * sizeof(png_color));
    if (!palette) {
        fclose(png_file);
        png_destroy_write_struct(&png_ptr, &info_ptr);
        return ;
    }
    png_set_PLTE(png_ptr, info_ptr, palette, PNG_MAX_PALETTE_LENGTH);
    png_write_info(png_ptr, info_ptr);
    png_set_packing(png_ptr);

    //getRainbowRGB24Data(rgb24Data, width, height);
    FILE *rgbfile = fopen("./image/image.rgb","rb");
    fread(rgb24Data, 1, width* height* 3, rgbfile);

    //这里就是图像数据了
    png_bytepp rows = (png_bytepp)png_malloc(png_ptr, width * sizeof(png_bytep));
    for (int i = 0; i < height; ++i)
    {
        rows[i] = (png_bytep)(rgb24Data + (i) * width * 3);
    }

    png_write_image(png_ptr, rows);

    // delete[] rows;
    png_write_end(png_ptr, info_ptr);

    png_free(png_ptr, palette);
    palette=NULL;
    png_destroy_write_struct(&png_ptr, &info_ptr);
    fclose(png_file);
    fclose(rgbfile);
}



int main()
{
    testRainbow();
}