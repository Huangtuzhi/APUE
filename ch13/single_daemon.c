#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <syslog.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <sys/stat.h>

#define LOCKFILE "/var/run/daemon.pid"
#define LOCKMODE (S_IRUSR |S_IWUSR |S_IRGRP |S_IROTH)


int already_running(void)
{
    int fd;
    char buf[16];

    fd = open(LOCKFILE, O_RDWR|O_CREAT, LOCKMODE);
    if(fd < 0){
        syslog(LOG_ERR, "can't open %s: %s", LOCKFILE, strerror(errno));
        exit(1);
    }
    if(lockf(fd, F_TLOCK, 0) < 0){
        syslog(LOG_ERR, "can't lock %s: %s", LOCKFILE, strerror(errno));
        exit(1);// a instance running
        }
    ftruncate(fd, 0);//将文件大小置为0
    sprintf(buf, "%ld", (long)getpid());
    write(fd, buf, strlen(buf)+1);
    exit(0);
}

int main()
{
    pid_t pid;
    if((pid = fork()) < 0)
        printf("fork error\n");
    else if(pid == 0)
    {
        printf("child process\n");
        already_running();
    }
    else
    {
       already_running();
       printf("parent process\n");
    }
    exit(0);
}
