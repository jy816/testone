#ifndef LL_DRV_H
#define LL_DRV_H
//#include"Drv.h"
extern"C"
{
  void MonitorInit();
 bool mwrite(unsigned short off,unsigned short val);/*在总线上写一个数据*/
 unsigned short mread(unsigned short off);/*在总线上读取数据*/
}

#endif // LL_DRV_H
