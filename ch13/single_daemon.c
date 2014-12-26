#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <syslog.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <sys/stat.h>
#include "../apue.h"

#define LOCKFILE "/var/run/daemon.pid"
#define LOCKMODE (S_IRUSR |S_IWUSR |S_IRGRP |S_IROTH)


int already_running(void)
{
    int fd;
    char buf[16];

    fd = open(LOCKFILE, O_RDWR|O_CREAT, LOCKMODE);
    if(fd < 0){
        syslog(LOG_ERR, "can't open %s: %s", LOCKFILE, strerror(errno));
        printf("can't open\n");
        exit(1);
    }
    if(lockf(fd, F_TLOCK, 0) < 0){
        syslog(LOG_ERR, "can't lock %s: %s", LOCKFILE, strerror(errno));
        printf("can't lock\n");
        exit(1);// a instance running
        }
    ftruncate(fd, 0);//将文件大小置为0
    sprintf(buf, "%ld", (long)getpid());
    write(fd, buf, strlen(buf)+1);
    printf("excute ok once\n");
    exit(0);
}

int main()
{
    pid_t pid;
    if((pid = fork()) < 0)
        printf("fork error\n");
    else if(pid == 0)
    {
        printf("child process, pid: %d\n",getpid());
        already_running();
    }
    
    already_running();
    printf("parent process, pid: %d\n",getpid());
    exit(0);
}
