#include <unistd.h>
#include <stdarg.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>

// 定义最大子进程数量
#define MAX_PROCESS_NUM  10000

pid_t createProcess(void(*ptr) ());
int addInProcessList(pid_t pid);
void waitAllProcess();
int isOutsideProcess(pid_t pid);
int isOutsideAllProcess();
int processError();
void killProcess();
void killChildren();

void childProcess();
void parentForkInfo(pid_t pid);
int tprintf(const char* fmt, ...);
#define HELLO_PATH "./greeter_client"
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

// 初始化进程计数器i
int processCount = 0;
// 初始化子进程数组
pid_t processList[MAX_PROCESS_NUM];

int main(int argc, char** argv){
    int i = 0, j = 0;
    int index = 0;
    int process_num = 0;
    pid_t pid[MAX_PROCESS_NUM];
    time_t begin_time, end_time;

    if(2 != argc)
    {
        printf("usage: %s <process num:1~%d>\n", argv[0], MAX_PROCESS_NUM);
        return -1;
    }
    process_num = atoi(argv[1]);
    if(process_num < 1 || process_num > MAX_PROCESS_NUM)
    {
        printf("usage: %s <process num:1~%d>\n", argv[0], MAX_PROCESS_NUM);
        return -1;
    }
    

    begin_time = time(NULL);
    printf("Parent Process, PID is %d.\n", getpid());
    for(index=0; index < process_num ; index++)
    {
    	pid[index]= createProcess(childProcess);
    }

    if (isOutsideAllProcess()){
    	for(index=0; index < process_num; index++)
    	{
        	parentForkInfo(pid[index]);
    	}
        waitAllProcess();
        tprintf("Children processes have exited.\n");
        tprintf("Parent had exited.\n");
    }
    else if(!processError()){
        tprintf("Everything was gone without error.\n");
    }

    end_time = time(NULL);
    printf("process count:%d, total_time:%lds\n", process_num, end_time-begin_time);
    return 0;
}

// 将子进程添加到子进程数组中
int addInProcessList(pid_t pid){
    if (processCount < MAX_PROCESS_NUM){
        processList[processCount] = pid;
        processCount = processCount + 1;
        return 1;
    }
    return -1;
}

// wait 所有子进程
void waitAllProcess(){
    int i;
    for(i=0;i<processCount; ++i){
        waitpid(processList[i], NULL, 0);
    }
}

// 检测子进程是否出错。
int processError(){
    int i;
    for(i=0;i<processCount; ++i){
        if(processList[i] == -1){
            return 0;
        }
    }
    return 1;
}

// 创建子进程
pid_t createProcess(void(*ptr) ()){
    pid_t pid;
    // isOutsideAllProcess() 满足的时候，表明当前处于父进程。
    if (isOutsideAllProcess()){
        // 创建子进程
        pid = fork();
        // 将所创建的子进程存入 pid_t 数组中
        if (addInProcessList(pid) != -1){
            if(pid == 0){
                // 处于所创建的子进程执行状态下，并执行子进程程序。
                (*ptr)();
            }
        }else{
            printf("Full processes!");
            killChildren();
            exit(1);
        }
    }
    return pid;
}

// kill 进程
void killProcess(pid_t pid){
    if(pid == 0){
        printf("Process %d exit!", getpid());
        exit(1);
    }
}

// kill 所有子进程
void killChildren(){
    int i;
    for(i=0;i<processCount; ++i){
        killProcess(processList[i]);
    }
}

// 判断当前状态是否处于某子进程外
int isOutsideProcess(pid_t pid){
    return (int) (pid != -1 && pid != 0);
}

// 判断当前状态是否处于所有子进程外，即当前为父进程状态。
int isOutsideAllProcess(){
    int i;
    for (i=0;i<processCount;++i){
        if (isOutsideProcess(processList[i]) == 0){
            return 0;
        }
    }
    return 1;
}

// 子进程执行
void childProcess(){
    int i;
    tprintf("Child Process id:%d\n", getpid());
    do_test();
    sleep(1);
}

// 输出父进程信息
void parentForkInfo(pid_t pid){
    tprintf("Parent forked one child process -- %d.\n", pid);
    tprintf("Parent is waiting for children to exit.\n");
}

int tprintf(const char* fmt, ...){
    va_list args;
    struct tm * tstruct;
    time_t tsec = time(NULL);
    tstruct = localtime (&tsec);
    printf("%02d:%02d:%02d: %5d|", tstruct->tm_hour, tstruct->tm_min, tstruct->tm_sec, getpid());
    va_start(args, fmt);
    return vprintf(fmt, args);
}
