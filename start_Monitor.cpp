#include "start_Monitor.h"
#include<QTimer>
#include<QDebug>
#include<QObject>
#include"sem_com.h"

extern int semSyncMon;
extern int semSyncAcq;
extern int semSyncM79;
static int nTimerIntCount=0;

start_Monitor::start_Monitor(QObject *parent) :
    QObject(parent)
{
    qDebug()<<"show before";
    QTimer *timer_10 = new QTimer();
    connect(timer_10, SIGNAL(timeout()), this, SLOT(timer_update()),Qt::QueuedConnection);
//     connect(timer_1000, SIGNAL(timeout()), &w, SLOT(timerUpdate()));
      connect(this,SIGNAL(Time_out()),&w,SLOT(timerUpdate()),Qt::QueuedConnection);
//     w.setWindowTitle("H-I-T-A-T-C-I");
//     w.show();
     qDebug()<<"show after";
//     sleep(5);
     timer_10->start(10);
}

void start_Monitor::timer_update()
{
//    sem_v(semSyncAcq);
//    if(nTimerIntCount == 100)
//    {
//        sem_v(semSyncM79);
//        emit(Time_out());
////        sem_v(semSyncMon);
//        nTimerIntCount = 0;
//    }
//    nTimerIntCount ++;
}

void start_Monitor::start_Monitor_slot()
{
//    while(1)
//    {
//        sem_p(semSyncMon);
//        emit(Time_out());
//    }
}

//void start_Monitor::run()
//{
//    exec();
//}
