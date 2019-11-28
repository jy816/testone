#include "taskControl.h"
#include"Drv.h"
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
#include<iostream>
#include <QTime>
using namespace std;
volatile BOOL IF_SELFTEST;//多线程使用同一变量，且变量值会变，使用volatile声明，必须每次从内存中读取，防止出错
volatile BOOL IF_STAY_2;
volatile BOOL IF_STAY_10;
float 				globalSpeed = 0;/*转速值***************/
extern int          msqid;
task_control::task_control(QObject *parent) :
    QObject(parent)
{
    qDebug()<<"taskControl start!";
}
//判断各项数据指标是否正常，来输出警报和控制
void task_control::taskControl_slot()
{
    int                     ret_value;
    float                   TEN_FOUR_SEVEN = 14.47;
    float                   SEVEN_FOUR = 74;
    struct msgstru          msgR;
    SAMPLEDATA              rcvSampleData;
    BOOL                    tmp_delay10=false;//临时存储IF_STAY_10的值
	IF_SELFTEST=false;//标志是否为自检状态
	IF_STAY_2 = false;//标志延时2S是否继续
	IF_STAY_10 = false;//标志延时10S是否继续
    while(1)
    {
        /*从数据采集进程收的数据，收不到就阻塞等待*/
        ret_value = msgrcv(msqid,(void *)&msgR,sizeof(SAMPLEDATA),1,0);//从msqid的消息队列中读取消息并存于msgR中，把消息从队列中删除，返回实际读取到的消息数据长度
        if(ret_value < 0 )
        {
             qDebug()<<"msgrcv failed!";
             //exit(0);
        }

         memcpy(&rcvSampleData,&msgR.msgtext,sizeof(SAMPLEDATA));//msgR.msgtext数据复制到rcvSampleData（SAMPLEDATA）
             tmp_delay10 = false;
             if(rcvSampleData.diVal[PROT_ON_CH])/*保护投入有效则动作，宏定义M31  16位数字量输入 光耦*/
             {
                 if(rcvSampleData.diVal[SELF_TST_CH])/*自检通道有效*/
                 {
                     relaySet(SELFTEST_CH,RELAY_STATUS_NO); //自检信号灯亮
                     IF_SELFTEST = TRUE;    //标志为自检信号有效态
                     IF_STAY_2 = true;            //标志着自检状态继续保持
                 }
                 else//自检通道无效
                 {
                      IF_STAY_2 = false;//标志着自检状态不再保持
                 }

                 if(!IF_SELFTEST)//自检无效的情况下
                 {
                     if(rcvSampleData.diVal[PANCHE]&&(rcvSampleData.adVal[SPD_VAL]>500||rcvSampleData.adVal[OIL_VAL]<dev_val::OIL2_LMT))
                     {	//盘车投入且（转速>500或者滑油压力低II值报警值）
                         relaySet(PANCHE_DIS_CH,RELAY_STATUS_NO);//盘车禁止有效
                     }
                     else
                     {
                         relaySet(PANCHE_DIS_CH,RELAY_STATUS_NC);//盘车不禁止
                     }
                     /*滑油压力低于II，发出报警信号*/
                     if(rcvSampleData.adVal[OIL_VAL] <dev_val::OIL2_LMT)
                     {
                         relaySet(OIL1_ALRM_CH,RELAY_STATUS_NO);
                         relaySet(OIL2_ALRM_CH,RELAY_STATUS_NO); 
                         tmp_delay10 = true;
                         if(!delay_qthread.isRunning())//如果延时线程没有在执行，就启动  isRunning判断线程是否正在运行
                         delay_qthread.start(QThread::HighestPriority);//设置线程优先级参数HighestPriority
                     }
                     else if(rcvSampleData.adVal[OIL_VAL] < dev_val::OIL1_LMT)	/*滑油压力低，发出第一报警信号 压力1  0.072   压力2   0.06*/
                     {
                         relaySet(OIL1_ALRM_CH,RELAY_STATUS_NO);
                     }
                     if(rcvSampleData.adVal[OIL_VAL] > dev_val::OIL1_LMT)	/*压力恢复到阈值以上或以下20% 才能解除*/
                     {
                         relaySet(OIL1_ALRM_CH,RELAY_STATUS_NC);
                         relaySet(OIL2_ALRM_CH,RELAY_STATUS_NC);
                     }
                     else if(rcvSampleData.adVal[OIL_VAL] > dev_val::OIL2_LMT)	/*压力恢复到阈值以上或以下20% 才能解除*/
                     {
                         relaySet(OIL2_ALRM_CH,RELAY_STATUS_NC);
                     }
                     if(rcvSampleData.adVal[SPD_VAL] > dev_val::SPD2_LMT)/*超速2值，发出报警信号  速度1  11000  速度2 13000*/
                     {
                         relaySet(SPD1_ALRM_CH,RELAY_STATUS_NO);
                         relaySet(SPD2_ALRM_CH,RELAY_STATUS_NO);
                         tmp_delay10 = true;
                         if(!delay_qthread.isRunning())//如果延时线程没有在执行，就启动
                         delay_qthread.start(QThread::HighestPriority);
                     }
                     else if(rcvSampleData.adVal[SPD_VAL] > dev_val::SPD1_LMT)
                     {
                         relaySet(SPD1_ALRM_CH,RELAY_STATUS_NO);
                     }
                     if(rcvSampleData.adVal[SPD_VAL] < dev_val::SPD1_LMT)	/*速度恢复到速度1值以下解除*/
                     {
                         relaySet(SPD1_ALRM_CH,RELAY_STATUS_NC);
                         relaySet(SPD2_ALRM_CH,RELAY_STATUS_NC);
                     }
                     else if(rcvSampleData.adVal[SPD_VAL] < dev_val::SPD2_LMT)	/*速度恢复到速度值2以下解除*/
                     {
                         relaySet(SPD2_ALRM_CH,RELAY_STATUS_NC);
                     }
                     if(rcvSampleData.adVal[PRESS_VAL]>= (TEN_FOUR_SEVEN*rcvSampleData.adVal[DPRESS_VAL] +SEVEN_FOUR))//14.47*val+74  
                     {
                         relaySet(CHUANZHEN_CH,RELAY_STATUS_NO);
                          tmp_delay10 = true;
                          if(!delay_qthread.isRunning())//如果延时线程没有在执行，就启动
                         delay_qthread.start(QThread::HighestPriority);
                     }
                     else
                     {
                         relaySet(CHUANZHEN_CH,RELAY_STATUS_NC);
                     }
                 }
                 if(IF_SELFTEST)//自检状态，相关信号有效2S
                     {
                        if(!delay_qthread.isRunning())//如果延时线程没有在执行，就启动
                        delay_qthread.start(QThread::HighestPriority);
                     }
             }
             else	/*上位系统来的"保护投入命令无效（接点接通）时无条件解除报警和保护，有效时重新投入。*/
             {
                 relaySet(EMC_STOP,RELAY_STATUS_NC);
                 relaySet(OIL1_ALRM_CH,RELAY_STATUS_NC);
                 relaySet(OIL2_ALRM_CH,RELAY_STATUS_NC);
                 relaySet(SPD1_ALRM_CH,RELAY_STATUS_NC);
                 relaySet(SPD2_ALRM_CH,RELAY_STATUS_NC);
                 relaySet(CHUANZHEN_CH,RELAY_STATUS_NC);
                 relaySet(SELFTEST_CH,RELAY_STATUS_NC);
                 relaySet(PANCHE_DIS_CH,RELAY_STATUS_NC);
             }
             IF_STAY_10 = tmp_delay10;//标志IF_STAY_10有效就一直保持停机有效状态
             SWaction();//ZH20190628  统一动作 该函数控制M43 数字量输出
       
    }
}

task_control::~task_control()//删除消息队列
{
    if ((msgctl(msqid,IPC_RMID,NULL))<0)
    {
        qDebug()<<"delete msqid";
    }
}
