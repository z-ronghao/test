#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
//#include <sys/types.h>		//定义了一些常用数据类型，比如size_t
//#include <fcntl.h>			//定义了open、creat等函数，以及表示文件权限的宏定义
//#include <unistd.h>			//定义了read、write、close、lseek等函数

int sizeof_file(char* filename)
{
    struct stat statbuf;
    stat(filename,&statbuf);
    int size = statbuf.st_size;
    return size;
}

int main(int argc, char *argv[])
{
	FILE *infile, *outfile;
	int infile_size;
	char *buf;
	
	if(argc < 3)
	{
		printf("a.out[OPTION] 1_sourse 2_dest 3_half_flags\n");
		return -1;
	}

	infile = fopen(argv[1], "r");
	if(infile == NULL)
	{
		printf("open infile failed!\n");
		return -1;
	}
	
	outfile = fopen(argv[2], "w");
	if(outfile == NULL)
	{
		printf("open outfile failed!\n");
		return -1;
	}

	infile_size = sizeof_file(argv[1]);
	printf("file size is %d\n", infile_size);
	buf = (char *)malloc(infile_size);

	if(argc == 4)
	{
		fseek(infile, infile_size / 2, SEEK_SET);
		infile_size /= 2;
		if(infile_size % 2 != 0)
		{
			infile_size += 1;
		}
		printf("half file size is %d\n", infile_size);
	}

	while(infile_size > 0)
	{
		fread(buf, sizeof(char), 1, infile);
		fwrite(buf, sizeof(char), 1, outfile);
		infile_size--;
	}

	exit(0);
	
}
