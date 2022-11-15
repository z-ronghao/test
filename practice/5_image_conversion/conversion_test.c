#include "image_conversion.h"

int main(int argc, char *argv[])
{
    char infilename[64];
    char outfilename[64];
    int file_width, file_height, yuv_size;
    const char *pFile;
    uint8_t infilelen;
    uint8_t *rgbdata;
    uint8_t *yuvdata;

    if (argc > 1)
	{
	    strcpy(infilename, argv[1]);
        strcpy(outfilename, infilename);
	    infilelen = strlen(infilename);
    }
    else
    {   
        printf("****   Please set input file(BMP/PNG/JPEG) ****\n\n");
        return 0;
    }

    pFile = strrchr(infilename, '.'); 
    if (pFile != NULL)
    { 
        if (strcmp(pFile, ".bmp") == 0)
        { 
            printf("\n*************Input File is bmp File************\n\n");
            strcpy(outfilename + (infilelen - 3), "rgb");
            bmp_2_rgb24(infilename, &rgbdata, &file_width, &file_height);

            FILE *rgbfile = fopen(outfilename, "wb");
            if(!rgbfile) 
            {
                printf("Could not open rgbfile \n");
                return -1;
            }
            fwrite(rgbdata, 1, file_width* file_height* 3, rgbfile);
            fflush(rgbfile);
            fclose(rgbfile);
            printf("out rgb file ok\n");

		    strcpy(outfilename + (infilelen - 3), "png");
            rgb24_2_png(rgbdata, outfilename, file_width, file_height);

            strcpy(outfilename + (infilelen - 3), "yuv");
            yuvdata = malloc(file_height* file_width* 3 /2);
            rgb24_2_yuv420(rgbdata, yuvdata, file_width, file_height);

            FILE *yuvfile = fopen(outfilename, "wb");
            if(!yuvfile) 
            {
                printf("Could not open yuvfile \n");
                return -1;
            }
            fwrite(yuvdata, 1, file_width* file_height* 3/2, yuvfile);
            fflush(yuvfile);
            fclose(yuvfile);
            printf("out yuv file ok\n");

            strcpy(outfilename + (infilelen - 3), "jpeg");
            yuv420_2_jpeg(yuvdata, outfilename, file_width, file_height);

 
        }
        else if (strcmp(pFile, ".png") == 0)
        { 
            printf("\n*************Input File is png File*************\n\n");
            strcpy(outfilename + (infilelen - 3), "rgb");
            png_2_rgb24(infilename, &rgbdata, &file_width, &file_height);
            
            FILE *rgbfile = fopen(outfilename, "wb");
            if(!rgbfile) 
            {
                printf("Could not open rgbfile \n");
                return -1;
            }
            fwrite(rgbdata, 1, file_width* file_height* 3, rgbfile);
            fflush(rgbfile);
            fclose(rgbfile);
            printf("out rgb file ok\n");

		    strcpy(outfilename + (infilelen - 3), "bmp");
            rgb24_2_bmp(rgbdata, outfilename, file_width, file_height);

            strcpy(outfilename + (infilelen - 3), "yuv");
            yuvdata = malloc(file_height* file_width* 3 /2);
            rgb24_2_yuv420(rgbdata, yuvdata, file_width, file_height);

            FILE *yuvfile = fopen(outfilename, "wb");
            if(!yuvfile) 
            {
                printf("Could not open yuvfile \n");
                return -1;
            }
            fwrite(yuvdata, 1, file_width* file_height* 3/2, yuvfile);
            fflush(yuvfile);
            fclose(yuvfile);
            printf("out yuv file ok\n");

            strcpy(outfilename + (infilelen - 3), "jpeg");
            yuv420_2_jpeg(yuvdata, outfilename, file_width, file_height);

        }
        else if(strcmp(pFile, ".jpg") == 0)
        {
            printf("\n*************Input File is jpg File*************\n\n");
            strcpy(outfilename + (infilelen - 3), "yuv");
            jpeg_2_yuv420(infilename, &yuvdata, &file_width, &file_height, &yuv_size);

            FILE *yuvfile = fopen(outfilename, "wb");
            if(!yuvfile) 
            {
                printf("Could not open yuvfile \n");
                return -1;
            }
            fwrite(yuvdata, 1, file_width* file_height* 3/2, yuvfile);
            fflush(yuvfile);
            fclose(yuvfile);
            printf("out yuv file ok\n");
            
            strcpy(outfilename + (infilelen - 3), "rgb");
            rgbdata = (uint8_t *)malloc(file_width* file_height* 3);
            yuv420_2_rgb24(yuvdata, rgbdata, file_width, file_height);

            FILE *rgbfile = fopen(outfilename, "wb");
            if(!rgbfile) 
            {
                printf("Could not open rgbfile \n");
                return -1;
            }
            fwrite(rgbdata, 1, file_width* file_height* 3, rgbfile);
            fflush(rgbfile);
            fclose(rgbfile);
            printf("out rgb file ok\n");

            strcpy(outfilename + (infilelen - 3), "bmp");
            rgb24_2_bmp(rgbdata, outfilename, file_width, file_height);

            strcpy(outfilename + (infilelen - 3), "png");
            rgb24_2_png(rgbdata, outfilename, file_width, file_height);
        }
        else if(strcmp(pFile, ".jpeg") == 0)
        {
            printf("\n*************Input File is jpeg File*************\n\n");
            strcpy(outfilename + (infilelen - 4), "yuv");
            jpeg_2_yuv420(infilename, &yuvdata, &file_width, &file_height, &yuv_size);

            FILE *yuvfile = fopen(outfilename, "wb");
            if(!yuvfile) 
            {
                printf("Could not open yuvfile \n");
                return -1;
            }
            fwrite(yuvdata, 1, file_width* file_height* 3/2, yuvfile);
            fflush(yuvfile);
            fclose(yuvfile);
            printf("out yuv file ok\n");

            strcpy(outfilename + (infilelen - 4), "rgb");
            rgbdata = (uint8_t *)malloc(file_width* file_height* 3);
            yuv420_2_rgb24(yuvdata, rgbdata, file_width, file_height);
            
            FILE *rgbfile = fopen(outfilename, "wb");
            if(!rgbfile) 
            {
                printf("Could not open rgbfile \n");
                return -1;
            }
            fwrite(rgbdata, 1, file_width* file_height* 3, rgbfile);
            fflush(rgbfile);
            fclose(rgbfile);
            printf("out rgb file ok\n");

            strcpy(outfilename + (infilelen - 4), "bmp");
            rgb24_2_bmp(rgbdata, outfilename, file_width, file_height);


            strcpy(outfilename + (infilelen - 4), "png");
            rgb24_2_png(rgbdata, outfilename, file_width, file_height);
        }
        else
        {
            printf("****Input Error ****\n\n");
            return 0;
        }
    }

    free(rgbdata);
    free(yuvdata);
    return 0;
    
}