#ifndef START_MONITOR_H
#define START_MONITOR_H

#include <QObject>
#include"jiance_widget.h"
class start_Monitor : public QObject
{
    Q_OBJECT
public:
    explicit start_Monitor(QObject *parent = 0);
    void start_Monitor_slot();
signals:
    void Time_out();

protected:
//     void run();
public slots:
     void timer_update();

private:
     Jiance_Widget w;
};

#endif // START_MONITOR_H
