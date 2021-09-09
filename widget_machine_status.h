#ifndef WIDGET_MACHINE_STATUS_H
#define WIDGET_MACHINE_STATUS_H

#include <QWidget>
#include <mainwindow.h>
namespace Ui {
class widget_machine_status;
}

class widget_machine_status : public QWidget
{
    Q_OBJECT

public:
    explicit widget_machine_status(QWidget *parent = nullptr);
    ~widget_machine_status();
protected:
    bool eventFilter(QObject *target, QEvent *event);




private:
    Ui::widget_machine_status *ui;
    void refrash_ui();
    void show_msgs(QString msg);
};



#endif // WIDGET_MACHINE_STATUS_H
