#include <QtGui/QApplication>
//#include<QtWidgets/QApplication>
#include <QTextCodec>
#include"Drv.h"
#include"lldrv.h"
#include"start_Monitor.h"
#include "jiance_widget.h"
#include"taskAcquire.h"
#include"taskControl.h"
#include"taskM79.h"
//#include"MonitorConfig.h"
#include"sem_com.h"
#include<QMetaType>
#include<QDebug>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include<time.h>
#include<signal.h>
#include <fcntl.h>
#include<unistd.h>
#include <errno.h>
#include<error.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<sys/shm.h>
#include<sys/sem.h>
#include </usr/include/linux/mtio.h>
#include<sys/resource.h>
#include "inputdialog.h"

#define CLOCKID CLOCK_REALTIME
#define MSGKEY 1024
/*用于设备控制的参数，可由用户自定义*/
//设置栈号和阈值
float dev_val::OIL1_LMT = 0.072;
float dev_val::OIL2_LMT = 0.06;
float dev_val::SPD1_LMT = 11000;
float dev_val::SPD2_LMT = 13000;
float dev_val::PROFIBUS_NUM = 5;

static int    nTimerIntCount = 0;
int             semSyncAcq,semSyncM79;
int             msqid;
int             shmid;
int             fd;
UINT8        switchs = 0;/*继电器寄存器*/

extern SAMPLEDATA     globalSampleData;					/*采集数据数组******/
/*定时器10ms到时之后会执行该函数*/
 void timer_thread(union sigval v)
 {
     sem_v(semSyncAcq);     //发送信号量，通知数据采集进程执行任务
     if(nTimerIntCount == 99)
     {
         sem_v(semSyncM79); //每100次，释放1次，通知Profibus进行一次数据传输
         nTimerIntCount = 0;
     }
     nTimerIntCount ++;
 }

 /*通过对话框获得的参数，通过此函数赋给相应变量*/
 void devvaltrans(user_def_val UserDefVal)//用户定义阈值和栈号传给设备值类
 {
     dev_val::OIL1_LMT = UserDefVal.IN_OIL1_LMT;
     dev_val::OIL2_LMT = UserDefVal.IN_OIL2_LMT;
     dev_val::SPD1_LMT =  UserDefVal.IN_SPD1_LMT;
     dev_val::SPD2_LMT =  UserDefVal.IN_SPD2_LMT;
     dev_val::PROFIBUS_NUM =  UserDefVal.IN_PROFIBUS_SLAVE;
 }
 int main(int argc, char *argv[])
 {
        QApplication a(argc, argv,QApplication::GuiServer); // 这样启动不需要加-qws***********
        QTextCodec::setCodecForTr(QTextCodec::codecForName("GB2312"));//设置字符编码，用于界面显示中文字符

        pid_t                 PID_taskAcquire,PID_taskControl,PID_taskM79;//依次分别时数据采集进程ID，设备控制进程ID，Profibus数据传输进程ID
        Jiance_Widget         JIANCEWIDGET;     //监测界面
        inputDialog           GetUserVal;        //获取自定义参数值的对话框
        unsigned short 	value;		/*监测系统读取值存储变量*****************/
        fd = open(DEV_EBI_1, O_RDWR);//打开EBI读写驱动文件
         if (fd < 0)
         {
               qDebug("open EBI devices faied!");
         }

        mwrite(M35_1_CONFIG,0x60); /* 设置AD，单极模式，增益1,中断禁止*/
        mwrite(M35_2_CONFIG,0x60);/* 设置AD，单极模式，增益1,中断禁止*/
        value = mread(M35_1_DATA); /* 读取数据 */
        value = mread(M35_2_DATA); /* 读取数据 */

        qDebug("first read AD the value is: %d",value);
        /*申请并初始化信号量semSyncAcq*/
        semSyncAcq = semget(ftok(".",'E'),1,0666|IPC_CREAT);
		//semget创建一个新信号量或取得一个已有信号量；int semget(key_t key, int num_sems, int sem_flags);
		//ftok函数得到系统建立通讯需要的ID
        init_sem(semSyncAcq,0);
        /*申请并初始化信号量semSyncM79*/
        semSyncM79 = semget(ftok(".",'C'),1,0666|IPC_CREAT);
        init_sem(semSyncM79,0);
        /*申请并初始化共享内存，其ID为shmid*/
        shmid = shmget(ftok(".",'F'),sizeof(globalSampleData),0666|IPC_CREAT);
        if(shmid < 0 )
        {
               qDebug()<<"shmget failed";
        }
        /*申请并初始化消息队列，其ID为msqid*/
        msqid=msgget(MSGKEY,IPC_EXCL);  /*检查消息队列是否存在*/
        if(msqid < 0) //如果消息队列不存在
        {
                 msqid = msgget(MSGKEY,IPC_CREAT|0666);/*创建消息队列*/
                 if(msqid <0)
                 {
                     qDebug("failed to create msq | errno=%d [%s]\n",errno,strerror(errno));
                    //exit(-1);
                 }
        }
        else//如果消息队列存在
        {
                 msgctl(msqid,IPC_RMID,NULL);
                 msqid = msgget(MSGKEY,IPC_CREAT|0666);/*创建消息队列*/
                 if(msqid <0)
                 {
                     qDebug("failed to create msq | errno=%d [%s]\n",errno,strerror(errno));
                     //exit(-1);
                 }
        }
        if(GetUserVal.exec() == QDialog::Accepted)//输入对话框定义的类，显示获取自定义参数的对话框，返回QDialog::Accepted说明对话框上的确认键按下
            {
                 devvaltrans(GetUserVal.UserDefVal);//把从对话框获得的自定义参数传到设备值结构体，UserDefVal传给dev_val
                 PID_taskAcquire = fork();//创建了数据采集进程，若成功调用一次则返回两个值，子进程返回0，父进程返回子进程ID
                 if(PID_taskAcquire == 0)//数据采集进程创建成功
                 {
                     /*在新进程中执行数据采集函数，进入槽函数*/
                     task_Acquire *TASK_ACQUIRE = new task_Acquire;
                     TASK_ACQUIRE->taskAcquire_slot(); //数据采集并处理
                 }
                 else//父进程
                 {
                     PID_taskControl = fork();//创建了设备控制进程
                     if(PID_taskControl == 0)
                     {
                           /*在设备控制进程中执行设备控制函数*/
                         task_control *TASK_CONTROL = new task_control;
                         TASK_CONTROL->taskControl_slot();
                     }
                     else//父进程
                     {
                         PID_taskM79 = fork();//创建了Profibus数据传输进程
                         if(PID_taskM79 == 0)
                         {
                               /*在设备控制进程中执行数据传输函数*/
                             taskM79 *TASK_M79 = new taskM79;
                             TASK_M79->taskM79_slot();
                         }
                         else//父进程
                         {
                                 timer_t            timerid;      			  //定时器ID
                                 struct sigevent    evp;           			  //配置定时器用的参数
                                 struct itimerspec timer_period;   			  //定时周期

                                 memset(&evp, 0, sizeof(struct sigevent));          //清零初始化
                                 evp.sigev_value.sival_int = 111;					//标识定时器				
                                 evp.sigev_notify = SIGEV_THREAD;                   //sigev_notify设置定时器到期后的行为，线程通知的方式，派驻新线程
																					//SIGEV_THREAD：定时器到期后，内核会以sigev_notification_attributes为线程属性创建一个线程，
																					//线程的入口地址为sigev_notify_function，传入sigev_value作为一个参数
                                 evp.sigev_notify_function = timer_thread;          //定时器到期，从该地址启动一个线程，线程函数地址，定时器到时会执行timer_thread函数，给数据采集发送信号，每100次给M79发一次
                                 if (timer_create(CLOCKID, &evp, &timerid) == -1)   //创建定时器 ，CLOCKID系统宏定义，日历时间
                                 {
                                     perror("fail to timer_create");
                                     //exit(-1);
                                 }

                                 timer_period.it_interval.tv_sec = 0;                   //定时器的定时周期 单位s
                                 timer_period.it_interval.tv_nsec = 10000000;   		//定时器的定时周期 单位ns    10ms
                                 timer_period.it_value.tv_sec = 0;                      //定时器初始值 单位s
                                 timer_period.it_value.tv_nsec = 10000000;       		//定时器初始值 单位ns   10ms

                                 if (timer_settime(timerid, 0, &timer_period, NULL) == -1)  //配置定时时间  0表示timer设置的是时间间隔 NULL不取上一次设置的new_value
                                 {
                                     perror("fail to timer_settime");
                                     //exit(-1);
                                 }
                                 JIANCEWIDGET.show();//显示监测界面
                                 return a.exec();
                        }
                       }
                     }
                 }
        }

