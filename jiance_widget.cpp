#include "jiance_widget.h"
#include "ui_jiance_widget.h"
#include "directoryviewer.h"
//#include"taskAcquire.h"
//#include"taskControl.h"
#include"MonitorConfig.h"
#include"MonitorFunction.h"
#include"Drv.h"
#include <QMetaType>
#include<QThread>
#include <QTimer>
#include<QFile>
#include<QtDebug>
#include<QFileDialog>
#include<QTextStream>
#include <QDateTime>
#include<QMessageBox>
#include<QVBoxLayout>
#include<QLabel>
#include<QPainter>
#include<QSizePolicy>
#include<QList>
#include<QStringList>
#include<QVector>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<sys/types.h>
#include<semaphore.h>
#include"sem_com.h"


extern SAMPLEDATA            globalSampleData;/*�ɼ���������******/
static int                   loop = 0;
extern int                   shmid;//ȥ������������shmid


Jiance_Widget::Jiance_Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Jiance_Widget)
{
    ui->setupUi(this);
    timer_1000 = new QTimer();//�½���ʱ��1S����
    this->setWindowTitle(tr("HIT_ATCI_MONITOR"));
    connect(timer_1000, SIGNAL(timeout()), this, SLOT(timerUpdate()),Qt::QueuedConnection);//��ʱ��1S��ʱ������timeout()�źţ�Jiance_Widget��ִ��timerUpdate()����;QueuedConnection���ڽ����߳���ִ��
    memset(&tmp_data_widget,0,sizeof(SAMPLEDATA) );
    timer_1000->start(1000);//1000ms
}
/*��ʾ�ú���*/
void Jiance_Widget::paintEvent(QPaintEvent *e)
{

    QPainter p(this);
    p.setBrush(QColor("#FFFFFF"));
    p.setPen(QColor("#FFFFFF"));
    p.drawRoundedRect(0, 0, width() - 1, height() - 1, 20, 20);
}

void Jiance_Widget::timerUpdate()
{
        timer_1000->stop();
       //��ȡϵͳ���ڵ�ʱ��
       QDateTime time = QDateTime::currentDateTime();
       //����ϵͳʱ����ʾ��ʽ
       QString str = time.toString("yyyy-MM-dd hh:mm:ss dddd");
       //�ڱ�ǩ����ʾʱ��
       ui->timeLineEdit->setText(str);
       int	 i=0;
       void *shared_memory = NULL;
        //���ձ�ǩ�ؼ�QLabel�����ư����Ƿ��࣬���ڱ�д����
            QList<QLabel *> allLabels_wendu = findChildren<QLabel *>(QRegExp("wendu_*"));
            QList<QLabel *> allLabels_yaqiang = findChildren<QLabel *>(QRegExp("yaqiang_*"));
            QList<QLabel *> allLabels_qita = findChildren<QLabel *>(QRegExp("qita_*"));
            QList<QLabel *> allLabels_output ;//= findChildren<QLabel *>(QRegExp("output_*"))
            QList<QLabel *> allLabels_input = findChildren<QLabel *>(QRegExp("input_*"));
            allLabels_output.append(ui->output_0); allLabels_output.append(ui->output_1); allLabels_output.append(ui->output_2); allLabels_output.append(ui->output_3);
            allLabels_output.append(ui->output_4); allLabels_output.append(ui->output_5); allLabels_output.append(ui->output_6); allLabels_output.append(ui->output_7);
            //��ʾ��ǰ���ò���
            ui->OIL1LMT->setText(QString::number(dev_val::OIL1_LMT));ui->OIL2LMT->setText(QString::number(dev_val::OIL2_LMT));
            ui->Speed1LMT->setText(QString::number(dev_val::SPD1_LMT));ui->Speed2LMT->setText(QString::number(dev_val::SPD2_LMT));
            ui->PROFIBUS->setText(QString::number(dev_val::PROFIBUS_NUM));
            //�ӹ����ڴ��ȡ����
            shared_memory = shmat(shmid,(void*)0,0);
             if(shared_memory == (void*)-1)
             {
                 qDebug()<<"shmat";
             }
             memcpy(&tmp_data_widget.adVal[0],shared_memory,sizeof(SAMPLEDATA));//�����ڴ��е����ݴ浽tmp_data_widget
            //��ʾģ��������
             for(i=0;i<20;i++)
             {
                       if(i<11)
                       {
                          // allLabels_wendu[i]->setText(QString::number(tmp_data_widget.adVal[i]).mid(0,5).trimmed());
                           allLabels_wendu[i]->setText(QString::number(tmp_data_widget.adVal[i],'f',2));//11���¶�
                       }
                       else if(i>10&&i<17)
                       {
                           allLabels_yaqiang[i-11]->setText(QString::number(tmp_data_widget.adVal[i],'f',3));//6��ѹǿ
                       }
//                       else if(i == 18)
//                       {
//                           allLabels_qita[i-17]->setText(QString::number(tmp_data_widget.adVal[i],'f',1));
//                       }
                       else
                       {
                           allLabels_qita[i-17]->setText(QString::number(tmp_data_widget.adVal[i],'f',3));//3������
                       }
             }
             //��ʾ����������
             for(i=0;i<allLabels_input.count();i++)
             {
                 if(!tmp_data_widget.diVal[i])
                 {
                     allLabels_input[i]->setStyleSheet(QString("QLabel{background-color: qradialgradient(spread:pad, cx:0.5, cy:0.5, radius:0.5, fx:0.5, fy:0.5, stop:0 rgba(0, 255, 0, 255), stop:0.518182 rgba(0, 255, 0, 255), stop:0.522685 rgba(255, 255, 255, 255), stop:1 rgba(255, 255, 255, 255));}"));
                 }
                 else
                 {
                     allLabels_input[i]->setStyleSheet(QString("QLabel{background-color: qradialgradient(spread:pad, cx:0.5, cy:0.5, radius:0.5, fx:0.5, fy:0.5, stop:0 rgba(255, 0, 0, 255), stop:0.479904 rgba(255, 0, 0, 255), stop:0.522685 rgba(255, 255, 255, 255), stop:1 rgba(255, 255, 255, 255));}"));
                 }
             }
             //��ʾ����������
             for(i=0;i<allLabels_output.count();i++)
             {
                 if(!tmp_data_widget.swVal[i])
                 {
                     allLabels_output[i]->setStyleSheet(QString("QLabel{background-color: qradialgradient(spread:pad, cx:0.5, cy:0.5, radius:0.5, fx:0.5, fy:0.5, stop:0 rgba(0, 255, 0, 255), stop:0.518182 rgba(0, 255, 0, 255), stop:0.522685 rgba(255, 255, 255, 255), stop:1 rgba(255, 255, 255, 255));}"));
                 }
                 else
                 {
                     allLabels_output[i]->setStyleSheet(QString("QLabel{background-color: qradialgradient(spread:pad, cx:0.5, cy:0.5, radius:0.5, fx:0.5, fy:0.5, stop:0 rgba(255, 0, 0, 255), stop:0.479904 rgba(255, 0, 0, 255), stop:0.522685 rgba(255, 255, 255, 255), stop:1 rgba(255, 255, 255, 255));}"));
                 }
             }
             shmdt(shared_memory);
             timer_1000->start();
 }

Jiance_Widget::~Jiance_Widget()
{
    delete ui;
}
