#include "RelayDelay.h"
#include"Drv.h"
#include"MonitorFunction.h"
#include <unistd.h>
#include<stdio.h>
#include<QDebug>
#include<QWaitCondition>

extern volatile BOOL IF_SELFTEST;
extern volatile BOOL IF_STAY_2;
extern  volatile BOOL IF_STAY_10;

RelayDelay::RelayDelay(QObject *parent) :
    QThread(parent)
{
    RelayDelay::setTerminationEnabled(true);
}

void RelayDelay::run()//新线程入口
{
               if(!IF_SELFTEST)//如果不是自检状态
               {
                    /*停机有效睡10S*/
                   relaySet(EMC_STOP,RELAY_STATUS_NO);//停机有效
                   while(IF_STAY_10)//标志IF_STAY_10有效就一直保持停机有效状态
                   {}
                   sleep(10);//标志无效后，开始延时10S
                   relaySet(EMC_STOP,RELAY_STATUS_NC);//然后停机信号无效
               }
               else//如果是自检状态
               {
                   relaySet(EMC_STOP,RELAY_STATUS_NO);
                   relaySet(OIL1_ALRM_CH,RELAY_STATUS_NO);
                   relaySet(OIL2_ALRM_CH,RELAY_STATUS_NO);
                   relaySet(SPD1_ALRM_CH,RELAY_STATUS_NO);
                   relaySet(SPD2_ALRM_CH,RELAY_STATUS_NO);
                   relaySet(CHUANZHEN_CH,RELAY_STATUS_NO);
                   relaySet(SELFTEST_CH,RELAY_STATUS_NO);
                   relaySet(PANCHE_DIS_CH,RELAY_STATUS_NO);
                   while(IF_STAY_2)//标志IF_STAY_2有效就一直保持自检的状态
                   {}
                   sleep(2);//标志无效后，开始延时2S
                   relaySet(EMC_STOP,RELAY_STATUS_NC);
                   relaySet(OIL1_ALRM_CH,RELAY_STATUS_NC);
                   relaySet(OIL2_ALRM_CH,RELAY_STATUS_NC);
                   relaySet(SPD1_ALRM_CH,RELAY_STATUS_NC);
                   relaySet(SPD2_ALRM_CH,RELAY_STATUS_NC);
                   relaySet(CHUANZHEN_CH,RELAY_STATUS_NC);
                   relaySet(PANCHE_DIS_CH,RELAY_STATUS_NC);
                   relaySet(SELFTEST_CH,RELAY_STATUS_NC);//自检信号灯灭
                   IF_SELFTEST=false;//自检状态结束
               }
}


