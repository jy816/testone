#ifndef TASK_ACQUIRE_H
#define TASK_ACQUIRE_H

#include <QObject>
#include"jiance_widget.h"
class task_Acquire : public QObject
{
    Q_OBJECT
public:
    explicit task_Acquire(QObject *parent = 0);
    void taskAcquire_slot();
    ~task_Acquire();
public slots:

protected:

signals:


private:

};
#endif // TASK_ACQUIRE_H
