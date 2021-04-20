#include <unistd.h>
#include <stdio.h>

#define HELLO_PATH "/root/huashan/grpc/get_netshield_filter/demo9/greeter_client"
void do_test(void)
{
        int iRet = 0;
        iRet = execl(HELLO_PATH, "greeter_client", NULL);
        if(-1 == iRet)
        {
                printf("error happend!!\n");
        }
        return;
}
