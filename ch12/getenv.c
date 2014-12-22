#include <limits.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define MAXSTRINGSZ 4096

static envbuf[MAXSTRINGSZ];
extern char **environ;

char * getenv(const char *name)
{
    int i,len;
    len = strlen(name);
    for(i = 0;environ[i] != NULL;i++){
        if((strncmp(name, environ[i], len) == 0) &&
                (environ[i][len] == '=')){
            strncpy(envbuf, &environ[i][len+1], MAXSTRINGSZ-1);
            return(envbuf);
        }
    }
    return(NULL);
}

int main(int argc, char **argv)
{
    int i=0;
    char *path = getenv("PWD");
    for(i = 0;environ[i] != NULL;i++)
        printf("environ %d: %s\n",i,environ[i]);

    printf("PWD:%s \n", path);
    return 0;

}
