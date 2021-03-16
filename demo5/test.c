//https://github.com/protobuf-c/protobuf-c/wiki/Examples
//https://blog.51cto.com/kid2412/1851728
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "test.pb-c.h"

int getPbMsg(void *buf, int* len)
{
    Test__Person p = TEST__PERSON__INIT;
    unsigned str_len = sizeof("chizi");
    char* pname = (char*)malloc(str_len);
    int i=0;

    strncpy(pname, "chizi", str_len);
    p.name = pname;
    p.age = 100;
    p.weight = 100;

    *len =  test__person__get_packed_size(&p);
    test__person__pack(&p, buf);

    return 0;
}

