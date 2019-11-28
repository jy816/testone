#ifndef JIANCE_WIDGET_H
#define JIANCE_WIDGET_H

#include <QWidget>
#include<directoryviewer.h>
#include"Drv.h"
#include <QTimer>
#include "inputdialog.h"
#include "Drv.h"

namespace Ui {
    class Jiance_Widget;
}

class Jiance_Widget : public QWidget
{
    Q_OBJECT

public:
    //explicit Jiance_Widget(sh_val InToSh,QWidget *parent = 0);
    explicit Jiance_Widget(QWidget *parent = 0);
    void ebi_RDWR();
    bool openmyfile(QString);
    bool saveFile(const QString &fileName);
    QString data;
    QString filename;
    ~Jiance_Widget();
    SAMPLEDATA tmp_data_widget;//临时存储监测数据

private:
    Ui::Jiance_Widget *ui;
    QTimer *timer_1000;

protected:
 void paintEvent(QPaintEvent *);/*重绘事件*/

private slots:
    void timerUpdate();
};


#endif // JIANCE_WIDGET_H
