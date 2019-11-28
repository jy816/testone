/*
*703Drv.h文件
*包括703Drv.a驱动函数的定义和其它相关宏定义
*/
#ifndef DRV_H
#define DRV_H
#include"M79.h"

#define M79_BASE_ADDR	   (0x3000)/*M79基地址*/
#define M35_1_BASE_ADDR    (0x4000)/*第一个AD模块基地址*/
#define M35_2_BASE_ADDR    (0x5000)/*第二个AD模块基地址*/

#define M35_1_CONFIG       (M35_1_BASE_ADDR)/*第一个AD配置宏*/
#define M35_2_CONFIG       (M35_2_BASE_ADDR)/*第二个AD配置宏*/

#define M35_1_DATA         (M35_1_BASE_ADDR+0xA)/*第一个AD读数据宏定义*/
#define M35_2_DATA         (M35_2_BASE_ADDR+0xA)/*第二个AD读取数据宏定义*/

#define M35_1_DATAINC      (M35_1_BASE_ADDR+0xE)/*读数据，通道自动加1的宏定义*/
#define M35_2_DATAINC      (M35_2_BASE_ADDR+0xE)/*读数据，通道自动加1的宏定义*/

#define M31_BASE_ADDR      (0x7000)/*光藕基地址*/
#define M43_BASE_ADDR      (0x6000)/*继电器基地址*/

#define NUM_SAMPLE_LOOP       (10)/*数据取样次数*/
#define NUM_ADC_CHANNEL       (20)/*AD数据转换通道数*/
#define NUM_DIC_CHANNEL	(16)/*DI数据转换通道数*/
#define NUM_SWC_CHANNEL	(8)/*SW数据转换通道数*/
#define MSG_QUEUE_LENGTH      (1000)/*消息队列长度*/

/*M35通道定义*/
#define PRESS_VAL   11/*压气机出口压力通道*/
#define DPRESS_VAL   12/*压气机进口压差通道*/
#define OIL_VAL   15/*滑油总管压力通道*/
#define PSPD_VAL   16/*测速叶轮泵出口压力通道*/
#define SPD_VAL   18/*气轮机转速通道*/

/*M43通道定义*/
#define EMC_STOP   0	/*停机信号通道*/
#define OIL1_ALRM_CH   1/*油压力低1值通道*/
#define OIL2_ALRM_CH   2/*油压力低2值通道*/
#define SPD1_ALRM_CH   3/*超速1值通道*/
#define SPD2_ALRM_CH   4/*超速2值通道*/
#define CHUANZHEN_CH  5	/*喘振通道*/
#define SELFTEST_CH      6     /*检测指示灯通道*/
#define PANCHE_DIS_CH  7/*盘车禁止通道*/

/*M31通道定义*/
#define SELF_TST_CH   5/*自检通道*/
#define PROT_ON_CH   6/*保护投入通道*/
#define PANCHE   8/*盘机通道*/



#define RELAY_STATUS_NO   (0x0001)/*继电器闭合*/
#define RELAY_STATUS_NC   (0x0000) /*继电器不闭合*/

#define SEND_PRO_DATA_LENGTH  21 /*向profibus发送数据长度*/

#define DEV_EBI_1 "/dev/sama5d3x-ebi-1"
#define DEV_EBI_2 "/dev/sama5d3x-ebi-2"

class dev_val//设备控制参数，设备值类
{
public:
  static   float	OIL1_LMT;			/*滑油压力低I值报警值**************/
  static   float	OIL2_LMT;			/*滑油压力低II值报警值*************/
  static   float	SPD1_LMT;		/*超速I值报警值********************/
  static   float	SPD2_LMT;		/*超速II值报警值*******************/
  static   float	PROFIBUS_NUM;       /*栈号*****************************/
};

typedef unsigned char UINT8;
typedef unsigned short  UINT16;
typedef unsigned int UINT32;
typedef int STATUS;
typedef bool BOOL;
//设置VME总线
struct sama5d3_ebi_arg{
            unsigned short ebi_addr;	//地址
            unsigned short ebi_high_addr;//高8位地址
            unsigned short ebi_data;	//数据
            unsigned char  mode;		//VME总线读写方式  
            unsigned char  modeA;		//读写模式标志 0  16位寻址方式  1   24位寻址方式
};
//信号量共用体
union semun
{
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};
typedef struct _STAUS_DATA_
{
        float 	adVal[20];/*AD数据*/
        UINT8 	diVal[16];/*光藕数据*/
        UINT8 	swVal[8];/*继电器数据*/
}SAMPLEDATA;//存储采集AD DI DO数据
//共享内存结构体
struct shm_buff
{
    int pid;
    char buffer[256];
};
//消息队列结构体
struct msgstru
{
   long msgtype;
   char msgtext[256];
};
extern int m79Init(MM79* pMM79,UINT16 base_address);/*m79初始化*/
extern int m79InitSlave(MM79* pMM79,T_DPS_INIT_SLAVE_REQ *req);/*m79从状态初始化*/
extern int m79ExitSlave(MM79* pMM79);/*m79退出*/
extern unsigned short  m79GetStatus(MM79* pMM79);/*m79获取状态*/
extern int m79SetSlaveInput(MM79* pMM79,unsigned char *data_ptr, int len);/*m79发送数据*/
#endif // 703DRV_H
