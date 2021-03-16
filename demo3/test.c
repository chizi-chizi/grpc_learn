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
    unsigned str_len = sizeof("chihuashan");
    char* pname = (char*)malloc(str_len);

    strncpy(pname, "chihuashan", str_len);
    p.name = pname;
    p.age = 30;
    p.weight = 70;

    len =  test__person__get_packed_size(&p);
    buf = malloc(len);
    test__person__pack(&p, buf);

    return buf;
}

