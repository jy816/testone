#ifndef M79_H
#define M79_H
//#include"Drv.h"
#include"HitHW.h"
/**定义M79模块数据传输使用的结构体和函数***/
extern "C"
{
#define M79_SLOT_NUM	0
#define DP_MAX_CFG_DATA_LEN	23
#define DOUT_TIME_OUT	100

/* Buffer Base Definiation	*/
/* ALL MAX SIZE DEFINE as BYTE(8 Bit)	*/
#define INIT_BASE	0x00

#define DIAG_BASE             0x22
#define DIAG_MAX              14
#define DIN_BASE              0x30
#define DIN_MAX		100
#define DOUT_BASE           0xA0
#define DOUT_MAX            48


#define CTRL_REG	0xF0
#define HC_IRQ		0xF2
#define STATUS_REG	0xF4
#define SSA_REG		0xF6

#define DP_STATUS	0x0003
#define DPS_DATAEX	0x0002

#define PWD	0x5300
#define GO	0x01
#define IRQ_HOST_EN	0x04
#define IRQ_CPU		0x08
#define IRQ_HOST	0x10

/* CPU2HOST INTERRUPT VECTOR	*/
#define NEW_DOUT_IVEC	0x01
#define DIN_UPDATE_ACK	0x02
#define DIAG_UPDATE_ACK	0x04

/* 主机通知控制器的"中断向量"	*/
#define NEW_DIN_DATA	0x0001
#define NEW_DIAG_DATA	0x0002
#define NEW_DOUT_ACK	0x0004

#define IOCMD_GPIO_OUT_HIGH				_IO('Q', 0x61)	//IO控制命令，控制GPIO引脚输出高电平，测试用
#define IOCMD_GPIO_OUT_LOW				_IO('Q', 0x62)	//IO控制命令，控制GPIO引脚输出低电平，测试用
#define IOCMD_INTERRUPT_ENABLE			_IO('Q', 0x63)	//IO控制命令，中断使能
#define IOCMD_INTERRUPT_DISABLE			_IO('Q', 0x64)	//IO控制命令，中断禁能
#define IOCMD_INTERRUPT_ACCOMPLISH		_IO('Q', 0x65)	//IO控制命令，中断应答完成

#define DEV_GPIO_IRQ "/dev/sama5d3x_vme_irq"

typedef unsigned char UINT8;
typedef unsigned short  UINT16;
typedef unsigned int UINT32;

typedef struct _T_DPS_INIT_SLAVE_REQ
{
        UINT8   slave_add; /**< 0..125, DPS_DEFAULT_SLAVE_ADD, DPS_NON_VOLATILE_SLAVE_ADD *///m79的PROFIBUS栈号
        UINT8   min_tsdr;   /**< min. station delay responder, default 0, 11..255 */	//从站最小反应时间
        UINT16  ident_number;             /**< 0 == Softing IdentNumber (Default) *///从站标识号 与M79的PROFIBUS GSD文件一致
        UINT16  user_watchdog_timeout;   /**< WD timeout [1 ms], 0 == WD disabled *///看门狗监视时间
        UINT8   max_input_data_len;                 /**< 0..DP_MAX_INPUT_DATA_LEN */ //默认输入数据长度
        UINT8   max_output_data_len;               /**< 0..DP_MAX_OUTPUT_DATA_LEN */ //默认输出数据长度
        UINT8   cfg_data_len;     /**< length of cfg_data, 1..DP_MAX_CFG_DATA_LEN */ //M79配置数据长度设置
        UINT8   ext_diag_data_len;
        UINT8   cfg_data [DP_MAX_CFG_DATA_LEN];             /**< initial CFG data 初始化配置数据*/

} T_DPS_INIT_SLAVE_REQ;

typedef struct _MM79
{
        UINT16	base_address;/*基地址*/
        UINT8	irqNbr; /*中断号*/
        UINT16	ctrl_reg;/*控制寄存器*/
        UINT16	status;/*状态寄存器*/
} MM79;

unsigned short m79GetStatus(MM79* pMM79);
int m79Init(MM79* pMM79,UINT16 base_address);
int m79InitSlave(MM79* pMM79,T_DPS_INIT_SLAVE_REQ *req);
int  m79ExitSlave(MM79* pMM79);
int  m79SetSlaveInput(MM79* pMM79,unsigned char *data_ptr, int len);
}
#endif // M79_H
