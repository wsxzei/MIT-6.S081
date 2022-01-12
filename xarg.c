#include "kernel/types.h"
#include "kernel/param.h"
#include "user/user.h"


int main(int argc, char **argv)
{
    char *new_argv[MAXARG];
    int argnum = 0, i = 0;//argnum为输入文本的行数，决定了xargs的运行次数
    char buf[512], *p = buf;
    while(read(0, p, 1) > 0){
        if(*p = '\n'){
            *p = '\0';
            argnum++;
        }
        p++;
    }
    *p = '\0';
    memset(new_argv, 0, sizeof(char *)*MAXARG);
    for(i = 0; i < argc - 1; i++) 
        new_argv[i] = argv[i + 1];
    new_argv[argc] = 0;
    p = buf;
    for(i = 0; i < argnum; i++){
        new_argv[argc - 1] = p;
        p += strlen(p) + 1;
        if(fork() == 0){
            exec(new_argv[0], new_argv);
            exit(1);
        }
    }
}