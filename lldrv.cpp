#include"lldrv.h"
#include"Drv.h"
#include<QDebug>

extern"C"
{
//#include "atmel_ebi_sama5.c"
//#include"at91_gpio.h"
#include"common.h"
#include <stdio.h>
#include<sys/ipc.h>
#include<sys/types.h>
#include<sys/sem.h>
#include<semaphore.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <QDebug>

extern int fd;
extern int semSyncAcq;
extern int semSyncM79;
static sem_t semio;

/*
封装的VME总线读函数
off:地址
返回值：读到的数据
*/
unsigned short mread(unsigned short off)
{
    int ret;
    struct sama5d3_ebi_arg arg;
    memset(&arg,0,sizeof(arg));
    arg.ebi_high_addr = 0xe0;
    arg.ebi_addr=off;
    arg.mode = 0x00;
    arg.modeA = 0x00;
    ret=read(fd,&arg,sizeof(arg));
    if(ret == 0x08)
        qDebug ("==================================read  vertime\n");
    return arg.ebi_data;
}

/*
封装的VME总线写函数
off:地址
val:要写入的数据
返回值：写数据是否成功的标志
*/
 bool mwrite(unsigned short off,unsigned short val)
{
    int ret=0;
    struct sama5d3_ebi_arg arg;
    memset(&arg,0,sizeof(arg));
    arg.ebi_high_addr = 0xe0;
    arg.ebi_addr=off;
    arg.ebi_data=val;
    arg.mode = 0x00;
    arg.modeA = 0x00;
    ret=write(fd, &arg, sizeof(arg));
    if(ret != 0)
    {
        if(ret == 0x08)
            qDebug ("=================================read  overtime\n");
        return false;
    }
    else
    {
        return true;
    }
}
}
