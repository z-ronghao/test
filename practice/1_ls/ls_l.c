#include<stdio.h>
#include<string.h>
#include<sys/stat.h>
#include<sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>

void ls_l(char *name);
void travel_dir(char *path);

void travel_dir(char* path){
	DIR *d = NULL; 
    char newpath[1024] = {0};
	struct dirent *dp = NULL; 
	
    if((d=opendir(path)) == NULL) 
    { 
        printf("opendir[%s] error: %m\n", path);
        return;
    }
    while((dp = readdir(d)) != NULL) 
    {       
        //printf("///%s\n",dp->d_name);
        if(strncmp(dp->d_name, ".", 1) != 0)
        {
        //memset(newpath,0,sizeof(newpath));
        snprintf(newpath,sizeof(newpath),"%s/%s",path,dp->d_name);
        ls_l(newpath);
        }
    }
    closedir(d);
}



void ls_l(char *name)
{
    struct stat my_stat;
   // printf("%s\n", name);
    int ret = stat(name,&my_stat);
    if (ret  == -1)
    {
        perror("stat");
        return ;
    }

    if (S_ISDIR(my_stat.st_mode))
        printf("d");
    else if (S_ISBLK(my_stat.st_mode))
        printf("b");
    else if (S_ISCHR(my_stat.st_mode))
        printf("c");
    else if (S_ISLNK(my_stat.st_mode))
        printf("l");
    else if (S_ISSOCK(my_stat.st_mode))
        printf("s");
    else if (S_ISREG(my_stat.st_mode))
        printf("-");
    else if (S_ISFIFO(my_stat.st_mode))
        printf("p");

    printf("%c",(my_stat.st_mode) & (1<<8)?'r':'-');
    printf("%c",(my_stat.st_mode) & (1<<7)?'w':'-');
    printf("%c",(my_stat.st_mode) & (1<<6)?'x':'-');
    printf("%c",(my_stat.st_mode) & (1<<5)?'r':'-');
    printf("%c",(my_stat.st_mode) & (1<<4)?'w':'-');
    printf("%c",(my_stat.st_mode) & (1<<3)?'x':'-');
    printf("%c",(my_stat.st_mode) & (1<<2)?'r':'-');
    printf("%c",(my_stat.st_mode) & (1<<1)?'w':'-');
    printf("%c",(my_stat.st_mode) & (1<<0)?'x':'-');

    printf(" %ld",my_stat.st_nlink);
    struct passwd *uid = getpwuid(my_stat.st_uid);
    printf(" %s",uid->pw_name);
    struct group *gid = getgrgid(my_stat.st_gid);
    printf(" %s",gid->gr_name);
    printf(" %-8ld",my_stat.st_size);
    printf(" %.12s ",4+ctime(&my_stat.st_mtim.tv_sec));
    printf("%-2s\n",name);

}

int main(int argc, char** argv)
{
    if(argc == 1)
    {
		travel_dir(".");
	}	
    else if(argc == 2)
    {
		travel_dir(argv[1]);
	}
}
