#include"sem_com.h"
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/sem.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
//信号量操作函数，现成的
union semun {
    int val;
    struct semid_ds *buf;
    unsigned short int *array;
    struct seminfo *__buf;
};
/*封装好的信号量操作函数，初始化某个信号量*/
int init_sem(int sem_id, int init_value)
{
    union semun sem_union;
    sem_union.val = init_value;
	//int semctl(int sem_id, int sem_num, int command, union semun arg)   sem_num指定信号量集合中的成员，在0和信号量数量-1之间
    if(semctl(sem_id,0,SETVAL,sem_union) == -1) //初始化一个信号量，该信号量ID是sem_id，初始值是init_value  SETVAL把信号量初始化为一个已知的值  第四个参数是一个结构体
    {
        perror("Initialize semaphore");
        return -1;
    }
    return 0;
}
/*封装好的信号量操作函数，删除某个信号量*/
int del_sem(int sem_id)
{
    union semun sem_union;
     if(semctl(sem_id,0,IPC_RMID,sem_union) == -1)//IPC_RMID用于删除一个已经无需继续使用的信号量标识符
     {
         perror("Delete semaphore");
         return -1;
     }
     return 0;
}
/*封装好的信号量操作函数，对某个信号量执行P操作*/
int sem_p(int sem_id)
{
    struct sembuf sem_b;
    sem_b.sem_num = 0;// 指定要操作的信号，0是第一个。除非使用一组信号量，否则它为0
    sem_b.sem_op = -1;// 信号量在一次操作中需要改变的数据，通常是两个数，一个是-1，即P（等待）操作， 一个是+1，即V（发送信号）操作
    sem_b.sem_flg = 0;//0 设置信号量的默认操作；通常为SEM_UNDO,如果该进程崩溃，则根据这个进程的UNDO记录自动恢复相应信号量的计数值；IPC_NOWAIT设置信号量操作不等待
    if (semop(sem_id,&sem_b,1) == -1)//1为进行操作信号量的个数
    {
        perror("P operation");
        return -1;
    }
    return 0;
}
/*封装好的信号量操作函数，对某个信号量执行V操作*/
int sem_v(int sem_id)
{
    struct sembuf sem_b;
    sem_b.sem_num = 0;
    sem_b.sem_op = 1;
    sem_b.sem_flg = 0;
    if (semop(sem_id,&sem_b,1) == -1)
    {
        perror("V operation");
        return -1;
    }
    return 0;
}
