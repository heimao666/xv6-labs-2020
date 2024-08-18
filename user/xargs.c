// xargs.c
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/param.h"

#define MSGSIZE 32

int main(int argc, char *argv[]){
    
    char buf[MSGSIZE];
    read(0,buf,MSGSIZE);

    char *myargv[MAXARG];
    int myargc=0;

    for (int i = 1; i < argc; i++)
    {
        myargv[myargc]=argv[i];
        myargc++;
    }
    char *p =buf;
    for(int i=0;i<MSGSIZE;++i){
        if(buf[i]=='\n'){
            int pid=fork();
            if (pid>0){
                p=&buf[i+1];
                wait(0);
            }else{
                buf[i]=0;
                myargv[myargc]=p;
                myargc++;
                myargv[myargc]=0;
                myargc++;

                exec(myargv[0],myargv);
                exit(0);
            }
        }
    }
    wait(0);
    exit(0);

}