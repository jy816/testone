#include "inputdialog.h"
#include "ui_inputdialog.h"
#include <QTimer>
#include<QMessageBox>
#include<QDebug>
#include<QString>
#include <qtextcodec.h>
#include<QTextStream>
#include<QFile>
#include<QFileDialog>
#include<QDateTime>
#include <unistd.h>
#include<iostream>
using namespace std;
inputDialog::inputDialog(QWidget *parent) :
    QDialog(parent),
ui(new Ui::inputDialog)
 {
        QString str= QString("/home/newdata");
        QTextCodec *codec = QTextCodec::codecForName("GB2312");
        this->setMouseTracking(true);//打开鼠标跟踪，不然只监测鼠标摁下时的移动
        ui->setupUi(this);
        this->setWindowTitle(tr("CONFIG"));//设置对话窗口的标题
        timer_1S = new QTimer();
        connect(timer_1S, SIGNAL(timeout()), this, SLOT(TimerOut()));//定时器timer_1s到时后发出timeout()信号，inputDialog类调用函数TimerOut()响应
        qDebug("File !\n");
        if( QFile::exists(str) )//如果newdata.dat文件存在，该文件存储的是用户自定义参数
        {
             qDebug("File exist!\n");
             QFile file_open(str);//打开newdata.dat文件
             if (!file_open.open(QIODevice::ReadOnly | QIODevice::Text))//只读，读取时，将文件中的换行符由"\r\n"，转换成"\n"
             {
                qDebug("File open failed!");
             }
             qDebug("File open success!\n");
             QTextStream data_in(&file_open);//把newdata.dat文件里的数据读进来
             UserDefVal.IN_OIL1_LMT = data_in.readLine().trimmed().toFloat();//readLine逐行读取  trimmed()过滤掉字符串两端的空白符
             UserDefVal.IN_OIL2_LMT = data_in.readLine().trimmed().toFloat();
             UserDefVal.IN_SPD1_LMT = data_in.readLine().trimmed().toFloat();
             UserDefVal.IN_SPD2_LMT = data_in.readLine().trimmed().toFloat();
             UserDefVal.IN_PROFIBUS_SLAVE = data_in.readLine().trimmed().toInt();
        }
         else//newdata.dat文件不存在，意味着目前没有用户自定义参数，就使用默认参数
         {
		 UserDefVal.IN_OIL1_LMT =0.072;
		 UserDefVal.IN_OIL2_LMT = 0.05;
		 UserDefVal.IN_SPD1_LMT = 11000;
		 UserDefVal.IN_SPD2_LMT = 13000;
		 UserDefVal.IN_PROFIBUS_SLAVE = 5;
		 }
         ui->lineEdit_oil_i->setPlaceholderText ("value: " + QString::number(UserDefVal.IN_OIL1_LMT));//把参数值显示在文本框上
         ui->lineEdit_oil_ii->setPlaceholderText ("value: " + QString::number(UserDefVal.IN_OIL2_LMT));
         ui->lineEdit_spd_i->setPlaceholderText ("value: " + QString::number(UserDefVal.IN_SPD1_LMT));
         ui->lineEdit_spd_ii->setPlaceholderText ("value: " + QString::number(UserDefVal.IN_SPD2_LMT));
         ui->profibus_num->setPlaceholderText ("value: " + QString::number(UserDefVal.IN_PROFIBUS_SLAVE));
         timer_1S->start(1000);//定时周期为1s，10个定时周期之后自动进入监测界面

  }

inputDialog::~inputDialog()
{
        delete ui;
}
/*监测到鼠标在对话框窗口移动，就执行下面的函数*/
void inputDialog::mouseMoveEvent(QMouseEvent *p)
{
    ui->timer_state->setText("-");
     timer_1S->stop();//计时器计时停止
}
/*"默认参数"按键按下时的响应函数*/
void inputDialog::on_etrButton_clicked()
{
         timer_1S->stop();//计时停止
         UserDefVal.IN_OIL1_LMT =0.072;
		 UserDefVal.IN_OIL2_LMT = 0.06;
		 UserDefVal.IN_SPD1_LMT = 11000;
		 UserDefVal.IN_SPD2_LMT = 13000;
		 UserDefVal.IN_PROFIBUS_SLAVE = 5;//采用默认设置
         accept();//槽函数自己处理
         delete ui;
         this->close();
}
/*1S计时器计时到期执行该函数   不到10S显示倒计时数字   到10S没有操作进入监测界面*/
void inputDialog::TimerOut()
{
    static int cnt = 0;
    cnt++;
    if(cnt<10)
    {
        ui->timer_state->setText(QString::number(10-cnt));//用于界面上显示倒计时的数字
    }
    else
    {
        timer_1S->stop();
        //10S倒计时结束还没有操作，直接进入监测界面。
        this->accept();//槽函数自己处理
        this->close();
    }
}

void inputDialog::on_pushButton_clicked()//点击启动应用程序按钮，设置值存入设置值的类并写入文件
{
    //char filename[] = { "newdata" }
    QString str= QString("/home/newdata");
    if(ui->lineEdit_oil_i->text().trimmed() == tr("") ||  ui->lineEdit_oil_ii->text().trimmed() == tr("") || ui->lineEdit_spd_i->text() == tr("") || ui->lineEdit_spd_ii->text().trimmed() == tr("") ||
            ui->profibus_num->text().trimmed() == tr(""))//如果填写的输入框参数至少有一个是空白着的
    {
            if(ui->lineEdit_oil_i->text().trimmed() != tr(""))
                { UserDefVal.IN_OIL1_LMT = ui->lineEdit_oil_i->text().trimmed().toFloat();  }
            if(ui->lineEdit_oil_ii->text().trimmed() != tr(""))
                { UserDefVal.IN_OIL2_LMT = ui->lineEdit_oil_ii->text().trimmed().toFloat(); }
            if(ui->lineEdit_spd_i->text().trimmed() != tr(""))
                { UserDefVal.IN_SPD1_LMT = ui->lineEdit_spd_i->text().trimmed().toFloat(); }
            if(ui->lineEdit_spd_ii->text().trimmed() != tr(""))
                { UserDefVal.IN_SPD2_LMT = ui->lineEdit_spd_ii->text().trimmed().toFloat(); }
            if(ui->profibus_num->text().trimmed() != tr(""))
                { UserDefVal.IN_PROFIBUS_SLAVE = ui->profibus_num ->text().trimmed().toInt(); }
            QFile file1(str);
            if(!file1.open(QIODevice::WriteOnly | QFile::Truncate))//打开newdata.dat文件
            {
                qDebug("File open failed!");
             }
            else//覆盖写入新的配置值
            {
                QTextStream out(&file1);
                cout<<UserDefVal.IN_OIL1_LMT<<endl<<UserDefVal.IN_OIL2_LMT<<endl<<UserDefVal.IN_SPD1_LMT<<endl<<UserDefVal.IN_SPD2_LMT<<endl<<UserDefVal.IN_PROFIBUS_SLAVE<<endl;
                //把配置值写入文件
				out <<UserDefVal.IN_OIL1_LMT<<endl<<UserDefVal.IN_OIL2_LMT<<endl<<UserDefVal.IN_SPD1_LMT<<endl<<UserDefVal.IN_SPD2_LMT<<endl<<UserDefVal.IN_PROFIBUS_SLAVE<<endl;
              //  if(!file1.flush())
                //    printf("flush failed\n");
                file1.close();
            }
    }
    else//如果填写的输入框参数都填写完毕了
    {
            /*读入输入参数*/
              UserDefVal.IN_OIL1_LMT = ui->lineEdit_oil_i->text().trimmed().toFloat();
			  UserDefVal.IN_OIL2_LMT = ui->lineEdit_oil_ii->text().trimmed().toFloat();
              UserDefVal.IN_SPD1_LMT = ui->lineEdit_spd_i->text().trimmed().toFloat();
			  UserDefVal.IN_SPD2_LMT = ui->lineEdit_spd_ii->text().trimmed().toFloat();
              UserDefVal.IN_PROFIBUS_SLAVE = ui->profibus_num ->text().trimmed().toInt();
              QFile file1(str);
              if(!file1.open(QIODevice::WriteOnly | QFile::Truncate))//打开newdata.dat文件
              {
                  qDebug("File open failed!");
               }
              else//覆盖写入新的配置值
              {
                  QTextStream out(&file1);
                  cout<<UserDefVal.IN_OIL1_LMT<<endl<<UserDefVal.IN_OIL2_LMT<<endl<<UserDefVal.IN_SPD1_LMT<<endl<<UserDefVal.IN_SPD2_LMT<<endl<<UserDefVal.IN_PROFIBUS_SLAVE<<endl;
                  //把配置值写入文件，<<  写入
				  out <<UserDefVal.IN_OIL1_LMT<<endl<<UserDefVal.IN_OIL2_LMT<<endl<<UserDefVal.IN_SPD1_LMT<<endl<<UserDefVal.IN_SPD2_LMT<<endl<<UserDefVal.IN_PROFIBUS_SLAVE<<endl;
                //  if(!file1.flush())
                    //  printf("flush failed\n");
                  file1.close();
              }
    }
    system("sync");//sync命令可用来强制将内存缓冲区中的数据立即写入磁盘中
    this->accept();//槽函数自己处理
    this->close();
}
