#include "taskAcquire.h"
#include"Drv.h"
#include"lldrv.h"
#include"MonitorFunction.h"
#include<QDebug>
#include"sem_com.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include<sys/sem.h>
#include <sys/msg.h>
#include <errno.h>
#include<error.h>
#include<sys/shm.h>

SAMPLEDATA 	    globalSampleData;					 		 /*采集数据数组******/
float 			globalADValue[NUM_ADC_CHANNEL] = { 0 };      /*存取两个M35的值***/
UINT8 			globalDIValue[NUM_DIC_CHANNEL] = { 0 };		 /*存取M31的值*******/
UINT8 			globalSWValue[NUM_SWC_CHANNEL] = { 0 };      /*存取M43的值*******/
static int      sampleLoop=0;
extern int      semSyncAcq;
extern int      msqid;
extern int      shmid;//去其它程序中找shmid
static int      LOOP=0;

UINT8 temdi_val[5] = { 0 };
UINT8 temdi = 0 ;


task_Acquire::task_Acquire(QObject *parent) :
    QObject(parent)
{
    qDebug()<<"taskAcquire start";
}

void task_Acquire::taskAcquire_slot()
{
    int 			i=0,j=0,k=0,ret_value;	/*循环变量i，j。返回值存储变量ret_value*/
    unsigned short 	rawData[32],value;
    static unsigned short myData[NUM_ADC_CHANNEL][NUM_SAMPLE_LOOP];// 原始数据 NUM_ADC_CHANNEL (20)AD数据转换通道数 NUM_SAMPLE_LOOP (10)数据取样次数
    UINT8 			switchValue;
    int 			dataSum = 0;
    const float 	sampleFullRange[]={100,100,100,100,100,100,100,100,100,100,100,400,35,1.6,0.6,
                                            0.4,1.6,100,15000,100};/*	kPa,kPa,*/
    float tmp_AD=0.0;//用于存储模拟量输入数据处理的中间值


   /*以下四项是用于模拟量数据处理的常量*/
    float NUM_FOURTY=40.0;
    float NUM_FOUR=4.0;
    float NUM_SIXTY=16.0;
    float NUM_THOUSAND=16384.0;


    void *shared_memory = NULL; //  用于保存共享内存的地址
    struct msgstru msgS;            //消息队列
    msgS.msgtype = 1;               //消息队列的参数
    /*
    *读取M43,M31,M35的值，并将读取值存入globalADValue，
    *globalDIValue，globalSWValue和消息队列缓冲globalSampleData中
    */
    while(1)
    {
        int di_eq_cnt = 0;
        sem_p(semSyncAcq);//等待信号量semSyncAcq资源释放，有的话消耗一个信号量并往下进行，没有资源的话就阻塞
        m35Data((unsigned short*)rawData);  //  读两个模拟量输入模块，读取两个AD转换值到rawData[32]数组，大小32个数据（两个AD2*16）
        for(j=0;j<NUM_ADC_CHANNEL;j++)//把采集到的数据放到处理数据用的数组myData中  NUM_ADC_CHANNEL=20,每次采20个通道
        {
            myData[j][sampleLoop] = rawData[j];//sampleLoop=0-9，把每个通道采集10次得到的数写入myData[20][10]（原始数据，rawdata32个数据，但是只需要前20个）
        }
                if(sampleLoop%2==0)//02468次
                {
                        temdi = (mread(M31_BASE_ADDR)&0xffff);
                        k = (sampleLoop/2);
                        temdi_val[k]=temdi;
                }
                //原始AD模拟量数据读取完毕
        if(sampleLoop == NUM_SAMPLE_LOOP-1)
        {
            for(i=0;i<NUM_ADC_CHANNEL;i++)//采集10次之后，每个通道10个数去掉最大最小2个，然后求均值，存储到global全局变量
                {
                                        qsort(myData[i],NUM_SAMPLE_LOOP,sizeof(unsigned short),compare);/*C语言排序函数 按照从小到大*/
                                        dataSum = 0;
                                        for(j=2;j<(NUM_SAMPLE_LOOP-2);j++)			/*	去掉最小和最大两个值	*/
                                        {
                                                        dataSum += myData[i][j];
                                        }
                                        globalADValue[i] = (float)dataSum/(NUM_SAMPLE_LOOP-4)/NUM_THOUSAND;//AD每个通道的模拟量
                                        tmp_AD =NUM_FOURTY* globalADValue[i]-20;
                                        /* CH0 压气机机匣温度*/
                                        globalADValue[i] = ((tmp_AD- NUM_FOUR)/NUM_SIXTY)*sampleFullRange[i];		/* 得到两个M35的值，4.0mA - 0	20.0mA - 满量程*/
                }

                                for(i=0;i<5;i++)//判断有没有不一样的数
                                {
                                        for(j=i;j<5;j++)
                                        {
                                                if(temdi_val[i]-temdi_val[j] != 0)
                                                {
                                                    di_eq_cnt = di_eq_cnt + 1;
                                                }
                                        }
                                }
                                if(di_eq_cnt==0)
                                {
                                    for(i=0;i<16;i++)   //  把数字量输入数据存进数组
                                        {
                                        globalDIValue[i] = ((temdi)>>i)&1;		/*光耦开关量输入，用户拨动按键输入*/
                                        }
                                }
                switchValue = mread(M43_BASE_ADDR)&0xff;//读数字量输出状态
                for(i=0;i<8;i++) //  把数字量输出数据存进数组
                {
                          globalSWValue[i] = (switchValue>>i)&0x01;   /*继电器模块状态监测值*/
                         // qDebug()<<"SW =s "<<globalSWValue[i];
                }
                //把采集到的数据放到消息队列的数组中
                memcpy(&msgS.msgtext[0],&globalADValue[0],sizeof(globalADValue));
                memcpy(&msgS.msgtext[80],&globalDIValue[0],sizeof(globalDIValue));
                memcpy(&msgS.msgtext[96],&globalSWValue[0],sizeof(globalSWValue));

                /*把采集到的数据通过消息队列发送到设备控制进程*/
                ret_value = msgsnd(msqid,&msgS,sizeof(globalSampleData),0);
                if(ret_value < 0)
                {
                    qDebug()<<"msgsend failed and exit";
                    //exit(0);
                }

                if(LOOP == 10)                    /*进程执行10次，把数据放到共享内存*/
                {
                    shared_memory = shmat(shmid,(void*)0,0);//shmat把共享内存区对象映射到调用进程的地址空间，成功返回附加好的共享内存地址
                    if(shared_memory == (void*)-1)
                    {
                        qDebug()<<"shmat";
                    }
                     memcpy(shared_memory,&msgS.msgtext[0],sizeof(globalSampleData));//把消息队列中的数据复制到共享内存地址
                     LOOP = 0 ;
                }
                LOOP ++;
                sampleLoop = 0;
        }
        else
        {
                sampleLoop++;
        }
    }
}
task_Acquire::~task_Acquire()
{
    del_sem(semSyncAcq);//删除信号量
    qDebug()<<"delete semSyncAcq";
}

