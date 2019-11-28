#ifndef DELAY10MS_H
#define DELAY10MS_H

#include <QThread>

class RelayDelay : public QThread
{
    Q_OBJECT
public:
    explicit RelayDelay(QObject *parent = 0);
protected:
     void run();
signals:

public slots:


};

#endif // DELAY10MS_H
