#include"MonitorFunction.h"
#include"QString"
#include<QDebug>
#include"stdio.h"
#include<time.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <fcntl.h>
#include <unistd.h>
#include"Drv.h"
#include"lldrv.h"
#include<unistd.h>
//继电器控制函数   AD读取  比较大小
extern UINT8  switchs;//8位
static int 			nTimerIntCount = 0;/*定时计数器*/

/*填充继电器开关数据至变量*/
void relaySet(int chan,unsigned short val)
{
        if(val)
        {
                switchs = switchs | (1<<chan);//1左移chan位
        }
        else
        {
                switchs = switchs & ~(1<<chan);
        }

     // mwrite(M43_BASE_ADDR,switchs);
}
/*向M43写入数据，控制继电器开或者关*/
void SWaction()
{
    mwrite(M43_BASE_ADDR,switchs);
}

/*
*读取两个AD转换值,dataBuf返回数组，大小32个数据
*/
void m35Data(unsigned short* dataBuf)
{
        int	i=0,j=0;/*i,j循环变量*/
        unsigned short value;
        mwrite(M35_1_CONFIG,0x00);
        mwrite(M35_2_CONFIG,0x00);
        value = mread(M35_1_DATA);
        value = mread(M35_2_DATA);
        for(i=0;i<16;i++)
        {
			usleep(200);
			dataBuf[i] = (mread(M35_1_DATAINC)&0xffff) >> 2;//M35输出14位数据，除以4
			dataBuf[16+i] = (mread(M35_2_DATAINC)&0xffff) >> 2;
        }
}


/*
*arg1和arg2指向的两个数大小比较规则，第一个比第二个大返回1,反之返回0
*/
int compare(const void *arg1 , const void *arg2)
{
        long int ret = *(UINT16*)(arg1)-*(UINT16*)(arg2);
        if(ret>0)
                return 1;
        else if(ret<0)
                return -1;
        else
                return 0;
}

 