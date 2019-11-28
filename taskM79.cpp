#include "taskM79.h"
#include<QDebug>
#include"sem_com.h"
#include"stdio.h"
#include"taskAcquire.h"
#include"Drv.h"
#include"MonitorFunction.h"
#include<sys/ipc.h>
#include<sys/types.h>
#include<sys/sem.h>
#include<sys/shm.h>
#include<semaphore.h>

float 						globalSendPlc[SEND_PRO_DATA_LENGTH];	/*profibus发送数据缓冲区    21****/
extern float 				globalADValue[NUM_ADC_CHANNEL];		/*存取两个M35的值****20**/
extern UINT8 				globalDIValue[NUM_DIC_CHANNEL];			/*存取M31的值**16*****/
extern UINT8 				globalSWValue[NUM_SWC_CHANNEL];		/*存取M43的值****8****/
extern int                                  semSyncM79;
extern int                                  shmid;
taskM79::taskM79(QObject *parent) :
    QObject(parent)
{
    qDebug()<<"taskM79 start";
}
//使用M79模块向profibus总线传输数据
void taskM79::taskM79_slot()
{
    int		i=0;			/*循环变量*/
    char 	*pswap = (char*)globalSendPlc;
    char 	tmpc = 0;
    int 	temp = 0;
    void    *shared_memory = NULL;
    MM79	m79;             //结构体（地址、中断、控制寄存器、状态寄存器）
    T_DPS_INIT_SLAVE_REQ req;

    memset(&tmp_data_m79,0,sizeof(SAMPLEDATA) );//把临时存储监测数据的结构体清空
    m79Init(&m79,M79_BASE_ADDR);                //初始化，给个M79基地址
	//设置结构体参数
    req.slave_add  = dev_val::PROFIBUS_NUM;	/*M79的PROFIBUS栈号*/		/* basic initialize slave request */
    req.min_tsdr  = 0x0B;
    req.max_input_data_len = 0x54;/* 默认的输入长度180字节相当于45个长整型数default values for dps_init_slave */
    req.max_output_data_len = 0x00;/* 默认的输出长度180字节相当于45个长整型数default values for dps_init_slave */
    req.ident_number = 0x1234;	/*与M79的PROFIBUS GSD文件一致*/
    req.user_watchdog_timeout = 20000;     /*M79缺省超时设置DPS_USER_WATCHDOG_DISABLED*/
    req.cfg_data_len = 21;       /*M79通信长度设置，128/16=8 表示8个16字节IN和8个16字节OUT,共16个*/
    req.ext_diag_data_len = 0;
    for(i=0;i<21;i++)
            req.cfg_data[i] = 0x13; /*GSD文件中对应 Module = "16 Bytes In " 0x1f*/

    m79InitSlave(&m79,&req);  /*进行M79初始化，对下面的寄存器进行操作，还未发送数据，所以从站满标志位为1*/

    while(1)
    {
             sem_p(semSyncM79);
             shared_memory = shmat(shmid,(void*)0,0);//进入共享内存
             if(shared_memory == (void*)-1)
             {
                 qDebug()<<"shmat";
             }
             memcpy(&tmp_data_m79.adVal[0],shared_memory,sizeof(SAMPLEDATA));
            if((m79GetStatus(&m79)&DP_STATUS)==DPS_DATAEX)/*检查可以交换数据的状态*/
             /*DPS_DATAEX	0x0002；DP_STATUS 0x0003 即检查状态寄存器低四位是否为xx10*/
            {
        /*******************将数据发送到 profibus*********************/
        /*************************填充数据***************************/
                   for(i=0;i<SEND_PRO_DATA_LENGTH-4;i++)           /* SEND_PRO_DATA_LENGTH  21 /*向profibus发送数据长度*/
                    globalSendPlc[i]= tmp_data_m79.adVal[i];//17个AD数据放入发送缓冲区，包括四个压力数据
                   
   				    temp = (tmp_data_m79.diVal[0]<<24 & 0xFF000000) | (tmp_data_m79.diVal[1]<<16 & 0xFF0000) | (tmp_data_m79.diVal[2]<<8 & 0xFF00) | (tmp_data_m79.diVal[3]<<0 & 0xFF);
                    memcpy(&globalSendPlc[SEND_PRO_DATA_LENGTH-4],&temp,sizeof(int));//第18个位置        光耦数据前四个，有一个就为1
                    memcpy(&globalSendPlc[SEND_PRO_DATA_LENGTH-3],&tmp_data_m79.adVal[SPD_VAL],sizeof(float));//第19个位置    SPD_VAL   18  汽轮机转速通道
                    temp= (tmp_data_m79.swVal[0]<<24 & 0xFF000000) | (tmp_data_m79.swVal[1]<<16 & 0xFF0000) | (tmp_data_m79.swVal[2]<<8 & 0xFF00) | (tmp_data_m79.swVal[3]<<0 & 0xFF);
                    memcpy(&globalSendPlc[SEND_PRO_DATA_LENGTH-2],&temp,sizeof(int));//第20个位置  继电器前四个数据，有一个一就为1
                    temp = (tmp_data_m79.swVal[4]<<24 & 0xFF000000) | (tmp_data_m79.swVal[5]<<16 & 0xFF0000) | (tmp_data_m79.diVal[6]<<8 & 0xFF00) | (tmp_data_m79.swVal[7]<<0 & 0xFF);
                    memcpy(&globalSendPlc[SEND_PRO_DATA_LENGTH-1],&temp,sizeof(int));//第21个位置   继电器前四个数据，有一个一就为1
                   
				   for(i = 0;i<21;i++)  /*   这里进行字节重排  如32位的数  ABCD 变成 DCBA*/
                    {
						tmpc = *(pswap+4*i);//pswap为数据缓冲区数组指针，指向数组第0个元素
						*(pswap+4*i) = *(pswap+4*i+3);
						*(pswap+4*i+3) = tmpc;//03互换
						tmpc = *(pswap+4*i+1);
						*(pswap+4*i+1) = *(pswap+4*i+2);
						*(pswap+4*i+2) = tmpc;//12互换
                    }
                    m79SetSlaveInput(&m79,(UINT8*)globalSendPlc,84);/*向Profibus写数据使用PROFI_DpsSetInData函数，缓冲数组数据写入结构体*/
            }
            else
                 shmdt(shared_memory);//断开共享内存
    }

}

taskM79::~taskM79()
{
    del_sem(semSyncM79);
    qDebug()<<"delete semSyncM79";
}
