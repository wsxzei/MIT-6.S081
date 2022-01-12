#include "kernel/types.h"
#include "user/user.h"

void sieve(char prime, int fd){//从管道读端口fd接收数据，筛除prime的倍数
    char num[34], rcv;
    int count = 0;
    while(read(fd, &rcv, 1) > 0){
        if(rcv % prime)
            num[count++] = rcv;//count记录num中元素个数
    }
    close(fd);
    int new_prime = (int)num[0];
    printf("prime %d\n", new_prime);
    if(count != 1){//需要继续筛选
        int p[2], i;
        pipe(p);
        if(fork() == 0){
            close(p[1]);//关闭写端口
            sieve((char)new_prime, p[0]);
            exit(0);
        }
        close(p[0]);
        for(i = 0; i < count; i++){
            write(p[1], num + i, 1);
        }
        close(p[1]);//一定要关闭写端口，否则子进程被阻塞在read处
        wait(0);
    }else if(count == 1){//最后一个素数
        return;
    }
}

int main(int argc, char **argv){
    char i = 0;
    int p[2];
    pipe(p);
    printf("prime 2\n");
    if(fork() == 0){
        close(p[1]);//关闭写端口
        sieve(2, p[0]);
        exit(0);
    }
    close(p[0]);
    for(i = 2; i <= 35; i++){
        write(p[1], &i, 1);
    }
    close(p[1]);//注意要关闭写端口，否则子进程会一直等待
    wait(0);
    exit(0);
}