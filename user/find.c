#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

char *fmtname(char *path)
{
  static char buf[DIRSIZ+1];
  char *p;

  // Find first character after last slash.
  for(p=path+strlen(path); p >= path && *p != '/'; p--);
  p++;
  memmove(buf, p, strlen(p) + 1);
  return buf;
}

void find(char *path, char *file){
    char buf[512], *p;
    int fd;
    struct dirent de;//获取目录中文件的名称
    struct stat st;
    char *filename;//case之后不能进行变量声明
    if((fd = open(path, 0)) < 0){
        fprintf(2, "ls: cannot open %s\n", path);
        exit(1);
    }
    if(fstat(fd, &st) < 0){
        fprintf(2, "ls: cannot stat %s\n", path);
        close(fd);
        exit(1);
    }
    
    switch(st.type){
        case T_FILE:
            filename = fmtname(path);
            if(!strcmp(file, filename)){
                printf("%s\n", path);
            }
            close(fd);
            break;
        case T_DIR:
            strcpy(buf, path);
            p = buf + strlen(buf);
            *p++ = '/';
    //对目录文件读，可以获得struct derent，其中包含文件的inode和文件名，大小为16字节
            while(read(fd, &de, sizeof(de)) == sizeof(de)){
                if(!de.inum || !strcmp(de.name, ".") || !strcmp(de.name, ".."))
                    continue;    
                strcpy(p, de.name);
                find(buf, file);
            }
            close(fd);
            break;
    }
}

int main(int argc, char **argv)
{
    if(argc == 3 ){
        find(argv[1], argv[2]);
        exit(0);
    }
    else if(argc == 2){
        find(".", argv[1]);
        exit(0);
    }
    printf("error: find <path> <filename>\n");
    exit(1);
}