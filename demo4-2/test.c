//https://github.com/protobuf-c/protobuf-c/wiki/Examples
//https://blog.51cto.com/kid2412/1851728
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "test.pb-c.h"

void* getPbMsg(void)
{
    Test__Person p = TEST__PERSON__INIT;
    void *buf;
    unsigned len;
    unsigned str_len = sizeof("chizi");
    char* pname = (char*)malloc(str_len);
    int i=0;

    strncpy(pname, "chizi", str_len);
    p.name = pname;
    p.age = 100;
    p.weight = 100;

    len =  test__person__get_packed_size(&p);
    buf = malloc(len);
    printf("len:%d\n", len);
    test__person__pack(&p, buf);
    for(; i<len; i++)
    {
        printf("%x ", ((char*)buf)[i]);
    }
    printf("\n");

    return buf;
}
int main()
{
    getPbMsg();
    return 0;
}

