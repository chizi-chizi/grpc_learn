#include<stdio.h>
#include<stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

char* GetFileContent(char* file_path)
{
    int fd = -1;
    ssize_t iRet = -1; 
    char buf[64] = {0};
    char *pcfile_content = NULL;
    int count = 1;
    int length = 0;

    pcfile_content = (char*)(void*)malloc(1024*1024);
    if(NULL == pcfile_content)
    {
        printf("malloc failed!\n");
        return NULL;
    }

    fd = open(file_path, O_RDONLY);
    if(-1 == fd)
    {
        printf("open %s failed\n", file_path);
        return NULL;
    }
    for(;;count++)
    {
        iRet = read(fd, buf, sizeof(buf));
        if(iRet == -1)
        {
            printf("read error !\n");
            return NULL;
        }
        else if(iRet != sizeof(buf))
        {
            printf("read %d times   %ld bytes! finish!!!\n", count, iRet);
            memcpy(pcfile_content+length, buf, iRet);
            length += iRet;
            break;
        }
        else
        {
            printf("read %d times    %ld byte!\n", count, iRet);
            memcpy(pcfile_content+length, buf, iRet);
            length += iRet;
        }
    }
    pcfile_content[length]='\0';


    printf("file_path:%s\n", file_path);
    return pcfile_content;
}


/*
int main()
{
    char *pcFilterContent = NULL;

    pcFilterContent = GetFileContent("./test.txt");
    printf("filter content:\n");
    printf("-----------------------\n");
    printf("%s\n", pcFilterContent);

    return 0;
}
*/
