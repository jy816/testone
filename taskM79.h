#ifndef TASKM79_H
#define TASKM79_H

#include <QObject>
#include "Drv.h"

class taskM79 : public QObject
{
    Q_OBJECT
public:
    explicit taskM79(QObject *parent = 0);
    void taskM79_slot();
    ~taskM79();
    SAMPLEDATA tmp_data_m79;//用于临时存储监测数据
signals:

public slots:

};

#endif // TASKM79_H
