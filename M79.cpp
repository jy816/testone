#include"M79.h"
#include"lldrv.h"
#include "HitHW.h"
#include<QDebug>
extern "C"
{
#include<sys/ipc.h>
#include<sys/types.h>
#include<semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include <termios.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/ioctl.h>
//M79模块需要的函数实现
int m79Init(MM79* pMM79,UINT16 base_address)//m79初始化
{
        qDebug()<<"interrupt enable";
        pMM79->base_address=base_address; /*结构体中基地址赋值*/
        return 1;
}

int  m79InitSlave(MM79* pMM79,T_DPS_INIT_SLAVE_REQ *req)//进行M79初始化，m79从状态初始化
{
        unsigned short ret_1;
        int len = (req->cfg_data_len+1)/2 + 5;//（配置数据长度+1）/2+5
        int i=0;
        pMM79->ctrl_reg=0x0000;//控制寄存器
        UINT16 *preq = (UINT16*)req;                 /*强制类型转换，向VME总线写16位的数据*/

         for(i = 0;i<len;i++)
		 {
             mwrite(pMM79->base_address + 0x200 *M79_SLOT_NUM + INIT_BASE + 2*i,*preq++);
		 }
		//for循环作以下事
		//向偏移地址0000H写0xxxxH(取决于参数)；表示从站地址为05H，从站最小反应时间0BH
		//向偏移地址0002H写1234H；表示从站标识号为1234H
		//向偏移地址0004H写4E20H；表示看门狗监视时间为200秒  4E20H=20000 默认是否设置M79看门狗禁能？？？？
		//向偏移地址0006H写0054H；表示从站不接收输出数据，输入数据长度为84字节？？？？
		//向偏移地址0008H写0015H；表示从站配置数据位21字节，  0015H=21  这里16位数据的高八位为0；ext_diag_data_len无意义？？？？
		//向偏移地址000AH到001CH写1313、向偏移地址001E写0013H，表示从站向主站发送21个浮点数   8位的13H拼凑成16位1313H
        pMM79->ctrl_reg  |= (GO|IRQ_HOST_EN);/* 先对控制寄存器赋值，#define GO 0x01，#define IRQ_HOST_EN	0x04*/
        mwrite(pMM79->base_address  + 0x200 * M79_SLOT_NUM + CTRL_REG,PWD|pMM79->ctrl_reg);
		/*# define CTRL_REG 0xF0  控制寄存器，D0=1启动/D0=0停止从站；
            D2=1，中断使能；D3=1从站满，写5305，开中断，启动从站
            本地控制器会清0，控制器在更新前，会查询这位是否清零*/
        ret_1 = mread(pMM79->base_address  + 0x200 * M79_SLOT_NUM + CTRL_REG);
        return 1;
}

int  m79ExitSlave(MM79* pMM79)//m79退出
{
        pMM79->ctrl_reg &= ~GO;
        /* update CTRL_REG	*/
        return 0;
}


unsigned short m79GetStatus(MM79* pMM79)/*检查可以交换数据的状态*/
{
        UINT16 status=0;
        status = mread(pMM79->base_address + 0x200 * M79_SLOT_NUM + STATUS_REG);/*读M79内部状态寄存器，#define STATUS_REG 0xF4？？？？*/
        //qDebug()<<"status="<<status;
        return status;              /*返回状态寄存器值*/
}


int  m79SetSlaveInput(MM79* pMM79,unsigned char *data_ptr, int len)//向Profibus写数据使用PROFI_DpsSetInData函数
{
        int i=0;
        UINT16 *ptr = (UINT16*)data_ptr;             /*8位的指针变为16位*/
        len = (int)((len+1)/2);                               /* len=84*/
        if(len > DIN_MAX)                                     /*#DIN_MAX=100  */
                return 0;
        for(i = 0;i<len;i++)
                mwrite(pMM79->base_address + 0x200 *M79_SLOT_NUM + DIN_BASE + 2*i, *ptr++);  /*DIN_BASE=30H M79输入数据起始地址*/
        /* TELL CPU NEW DATA is COMMING...*/
        mwrite(pMM79->base_address + 0x200 * M79_SLOT_NUM + HC_IRQ, NEW_DIN_DATA);       /*HC_IRQ=F2H，NEW_DIN_DATA=0001H 中断向量寄存器
                                                                                                                                                   每次写的时候都写一遍,通知从站本地控制器有输入数据更新*/
        mwrite(pMM79->base_address + 0x200 * M79_SLOT_NUM + CTRL_REG, PWD|pMM79->ctrl_reg|IRQ_CPU);
        return 1;
}
}
