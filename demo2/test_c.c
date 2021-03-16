//https://github.com/protobuf-c/protobuf-c/wiki/Examples
//https://blog.51cto.com/kid2412/1851728
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "test_c.pb-c.h"

int main()
{
    TestC__Person p = TEST_C__PERSON__INIT;
    void *buf;
    unsigned len;
    unsigned str_len = sizeof("chizi");
    char* pname = (char*)malloc(str_len);

    strncpy(pname, "chizi", str_len);
    p.name = pname;
    p.age = 100;
    p.weight = 100;

    len =  test_c__person__get_packed_size(&p);
    buf = malloc(len);
    test_c__person__pack(&p, buf);

    fprintf(stderr,"Writing %d serialized bytes\n",len); // See the length of message
    fwrite(buf,len,1,stdout); // Write to stdout to allow direct command line piping
    
    free(buf);
    free(pname);

    return 0;
}

