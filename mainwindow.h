#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <rpc/client.h>
#include "rpc/this_handler.h"
#include <thread>
#include <time.h>
#include <unistd.h>
#include <sstream>
#include <iostream>

#define MY_PI 3.141592
struct pid_kpid_t
{
    float kp;                       //比例系数Proportional
    float ki;                       //积分系数Integral
    float kd;                       //微分系数Derivative
    MSGPACK_DEFINE_MAP(kp, ki, kd);
};
struct rpc_pose_t
{
    float x,y,theta;
    MSGPACK_DEFINE_MAP(x, y, theta);
};

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    static MainWindow *MW;
    rpc_pose_t getMy_target() const;
    void setMy_target(const rpc_pose_t &value);

private slots:
    void see_pose();

    void on_connect_rpc_clicked();

    void on_enble_kp_clicked();

    void on_enbale_ki_clicked();

    void on_enable_kd_clicked();

    void on_set_kp_returnPressed();

    void on_set_ki_returnPressed();

    void on_set_kd_returnPressed();

    void on_set_target_x_returnPressed();

    void on_set_target_y_returnPressed();

    void on_button_set_target_clicked();

    void on_set_target_theta_returnPressed();

    void on_enable_pid_run_clicked();

    void on_global_clean_clicked();

    void on_pass_clean_clicked();

    void on_set_rpc_IP_returnPressed();

private:
    Ui::MainWindow *ui;
    rpc::client* c_reply = nullptr;
    rpc_pose_t my_target;
    rpc_pose_t my_nowpose;
    pid_kpid_t my_kpid;
    int my_pid_mode=0x01; //bit 1 enable kp,bit 2 enable ki, bit 3 enable kd
    void showMsgs(QString msg);
    void init();
};

#endif // MAINWINDOW_H
