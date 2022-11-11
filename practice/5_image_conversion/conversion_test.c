#include "image_conversion.h"
#include "image_conversion.c"

int main(int argc, char *argv[])
{
    char *infilename;
    char *outfilename;
    // filename = "./image/2560x1440.rgb";
    // FILE *rgbfile = fopen(filename, "rb");
    // uint8_t *rgbdata = (uint8_t *)malloc(3* 2560 *1440);
    // fread(rgbdata, 1, 3* 2560 *1440, rgbfile);
    // rgb_2_bmp(rgbdata, "2560x1440.bmp", 2560, 1440);
    // free(rgbdata);
    // fclose(rgbfile);

    // filename = "./image/2560x1440.rgb";
    // FILE *rgbfile = fopen(filename, "wb");
    // uint8_t *rgbdata = (uint8_t *)malloc(3* 2560 *1440);
    // bmp_2_rgb24("./image/2560x1440.bmp", rgbdata);
    // fwrite(rgbdata, 1, 3* 2560* 1440, rgbfile);
    // fclose(rgbfile);
    // free(rgbdata);

    infilename = "./image/2560x1440.rgb";
    outfilename = "./test.png";
    FILE *rgbfile = fopen(infilename, "rb");
    uint8_t *rgbdata = (uint8_t *)malloc(3* 2560 *1440);
    fread(rgbdata, 1, 3* 2560 *1440, rgbfile);
    rgb24_2_png(rgbdata, outfilename, 2560, 1440);
    
    free(rgbdata);
    fclose(rgbfile);

}