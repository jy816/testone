#ifndef INPUTDIALOG_H
#define INPUTDIALOG_H
#include<QTextStream>
#include<QFile>
#include <QDialog>
struct user_def_val{
    float	IN_OIL1_LMT; 		/*滑油压力低I值报警值**************/
    float	IN_OIL2_LMT;			/*滑油压力低II值报警值*************/
    float	IN_SPD1_LMT;		/*超速I值报警值********************/
    float	IN_SPD2_LMT;		/*超速II值报警值*******************/
    float	IN_PROFIBUS_SLAVE ;        /*气轮机转速设定值*****************/
};
namespace Ui {
    class inputDialog;
}

class inputDialog : public QDialog
{
    Q_OBJECT

public:
    explicit inputDialog(QWidget *parent = 0);
    ~inputDialog();
     user_def_val UserDefVal;

private slots:
    void on_etrButton_clicked();
    void TimerOut();
    void on_pushButton_clicked();
    //void on_pushButton_savequit_clicked();

protected:
    void mouseMoveEvent(QMouseEvent *p);
private:
    Ui::inputDialog *ui;
    QTimer *timer_1S;
};

#endif // INPUTDIALOG_H
