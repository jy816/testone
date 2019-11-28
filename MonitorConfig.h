#ifndef MONITORCONFIG_H
#define MONITORCONFIG_H

 int  getMonitorConfigValue(float * p_getOIL1_LMT,/*滑油压力低I值*/
                                                float * p_getOIL2_LMT,/*滑油压力低II值*/
                                                float * p_getSPD1_LMT,/*超速I值*/
                                                float * p_getSPD2_LMT/*超速II值*/
                                                /*float * p_getTURB_SPEED,*//*气轮机转速设定值*/
                                                /*float * p_getOIL_PRE_VALUE,*//*滑油总管压力最大值*/
                                                /*float * p_getCOM_PRE_VALUE*//*压气机空气旁通挡板压力最大值*/
                                                                                 );
 void monitorHelp(void);/*主程序运行前的帮助信息*/
 void setMonitorConfigFile(void);/*设定主控程序临界值*/
 void readMonitorConfigFile(void);/*读取主控程序临界值*/
//extern void monitorInit(void);/*监测系统初始化*/

#endif // MONITORCONFIG_H
