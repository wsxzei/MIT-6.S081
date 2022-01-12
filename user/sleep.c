#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char **argv){
    if(argc != 2){
        fprintf(2, "error: sleep <time>\n");
        exit(1);
    }
    int tickNum = atoi(argv[1]);
    if(tickNum == 0){
        fprintf(2, "invalid arguments!\n");
        exit(1);
    }
    sleep(tickNum);
    exit(0);
}