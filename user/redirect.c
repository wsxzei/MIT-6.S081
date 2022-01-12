#include "kernel/types.h"
#include "user/user.h"
#include "kernel/fcntl.h"

int main()
{
    int pid;
    pid = fork();
    if(pid == 0){
        close(1);//关闭标准输出描述符
        open("output.txt", O_WRONLY|O_CREATE);//总是返回最小的没打开的描述符，因此为1
        char *argv[] = {"echo", "THIS", "IS", "ECHO", 0};
        exec("echo", argv);//默认向描述符为1的文件输出内容
        printf("exec failed!\n");
        exit(1);
    }else{
        wait((int *)0);
    }
    exit(0);
}
/*在xv6操作系统中出现了output.txt文件，且文件内容如下
$ cat output.txt
THIS IS ECHO
*/