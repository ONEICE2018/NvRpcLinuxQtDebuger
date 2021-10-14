#ifndef WIDGET_PID_CURVE_H
#define WIDGET_PID_CURVE_H

#include <QWidget>
#include "qcustomplot.h"
#include "axistag.h"

namespace Ui {
class widget_pid_curve;
}

class widget_pid_curve : public QWidget
{
    Q_OBJECT

public:
    explicit widget_pid_curve(QWidget *parent = nullptr);
    ~widget_pid_curve();  
private slots:
    void timerSlot();
    void on_set_see_time_returnPressed();

private:
     Ui::widget_pid_curve *ui;
    QPointer<QCPGraph> mGraph1;
    QPointer<QCPGraph> mGraph2;
    AxisTag *mTag1;
    AxisTag *mTag2;
    QTimer mDataTimer;
};

#endif // WIDGET_PID_CURVE_H
