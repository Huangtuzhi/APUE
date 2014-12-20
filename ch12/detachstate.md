
在APUE P342讲了线程分离，下面展示了怎么让线程成为分离线程：

```
#include <pthread.h>
int makethread(void *(*fn)(void *), void *arg)
{
	int err;
	pthread_t tid;
	pthread_attr_t attr;

    //返回0表示初始化成功
	err = pthread_attr_init(&attr);
	if (err != 0)
	return err;

    //设置为线程分离状态 	
	err = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	if (err = 0)
	err = pthread_create(&tid, &attr, fn ,arg);
	return err;
}
```

----------------------------

##解释##
线程分离的作用是什么呢。如下解释：

```
int p1;
void usr()
{
    pthread_t  pid1;
    pthread_attr_t attr;
    /*这里做你的事情*/
    if(p1==0)
    {  
        pthread_attr_init(&attr);
        pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);       
        //因为线程不便于等待，设置为分离线程吧
        pthread_create(&pid1, &attr, task1, NULL);
    }
}

void* task1(void *arg1)
{
    p1=1;      //让子线程不会被多次调用
    int i=0;
    printf("thread1 begin./n");
    for(i=0;i<100;i++)
    {
    sleep(2);                 
    printf("At thread1: i is %d/n",i);      
    usr();                    //继续调用
    }
    pthread_exit();
}


int main()
{
    p1 = 0;
    usr();               //调用这个认为是你的触发函数
    getchar();
    return 1;
}

```

 我们看到，在这里task1这个线程函数居然会多次调用其父线程里的函数，
显然usr函数里，我们无法等待task1结束，反而task1会多次调用 usr，
一旦我们在usr里pthread_join，则在子线程退出前，有多个usr函数会等待，
很浪费资源。所以，此处，将task1设置为分离线程 是一种很好的做法。

--------------------------

##Reference##
[1].http://blog.csdn.net/china_hearts/article/details/4776930
