#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <pthread.h>

// 父子进程的fd不同，但是共享一个打开文件描述，因此文件位置相同

int main(int argc, char** argv)
{
    int fd = open("file", O_RDWR);
    if(fork() == 0){
        write(fd, "456", 3);
    }
    else{
        write(fd, "789", 3);
    }

    return 0;
}
