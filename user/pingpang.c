#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char **argv){
    int p[2];
    int q[2];//p为父进程向子进程输出的管道，q为子进程向父进程输出的管道
    char buf[] = {'a'};
    pipe(p);
    pipe(q);
    if(fork() == 0){
        char a;
        close(p[1]);
        close(q[0]);
        if(read(p[0], &a, 1) <= 0){
            exit(1);
        }
		printf("%d: received ping\n", getpid());
		write(q[1], &a, 1);
		exit(0);
    }
    char b;
    close(p[0]);
    close(q[1]);  
    write(p[1], buf, 1);
    if(read(q[0], &b, 1) <= 0){
		printf("error occur!\n");
        exit(1);     
    }
	printf("%d: received pong\n", getpid());
	wait(0);
	exit(0);   
}