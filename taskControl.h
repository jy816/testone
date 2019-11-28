#ifndef TASK_CONTROL_H
#define TASK_CONTROL_H

#include <QObject>
#include"RelayDelay.h"

class task_control : public QObject
{
    Q_OBJECT
public:
    explicit task_control(QObject *parent = 0);
    void taskControl_slot();

    ~task_control();
private:
       RelayDelay delay_qthread;
signals:

public slots:


};

#endif // TASK_CONTROL_H
