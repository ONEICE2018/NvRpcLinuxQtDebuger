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
#include <QKeyEvent>
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

    rpc_pose_t getMy_nowpose() const;
    void setMy_nowpose(const rpc_pose_t &value);

    float getPid_allow_erro() const;
    void setPid_allow_erro(float value);
    void showMsgs(QString msg);
    float getPid_refrence_distance() const;
    void setPid_refrence_distance(float value);

    float getPid_refrence_allow_erro() const;
    void setPid_refrence_allow_erro(float value);

    float getPid_S_allow_diserro() const;
    void setPid_S_allow_diserro(float value);

    float getPid_braking_dis() const;
    void setPid_braking_dis(float value);

    float getPid_min_w() const;
    void setPid_min_w(float value);

    rpc_pose_t getMy_org_pose() const;
    void setMy_org_pose(const rpc_pose_t &value);

    rpc_pose_t getMy_jd_pos() const;
    void setMy_jd_pos(const rpc_pose_t &value);

    rpc_pose_t getMy_vector_pose() const;

protected:
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

private slots:
    void see_pose();
    void map_Range_xy_cahnged();
    void on_connect_rpc_clicked();



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

    void on_set_map_x_range_returnPressed();

    void on_set_pid_v_returnPressed();

    void on_set_allow_erro_returnPressed();

    void on_clean_history_clicked();

    void on_set_far_allow_erro_returnPressed();

    void on_set_far_refdis_returnPressed();

    void on_set_s_allow_diserro_returnPressed();

    void on_set_braking_dis_returnPressed();

    void on_clean_targets_clicked();

    void on_set_min_w_returnPressed();

    void on_set_org_x_returnPressed();

    void on_set_org_y_returnPressed();

    void on_button_set_org_clicked();

    void on_set_dkp_returnPressed();

    void on_set_dki_returnPressed();

    void on_set_dkd_returnPressed();

private:
    Ui::MainWindow *ui;
    rpc::client* c_reply = nullptr;
    rpc_pose_t my_target;
    rpc_pose_t my_org_pose;
    rpc_pose_t my_nowpose;
    rpc_pose_t my_jd_pos;
    rpc_pose_t my_vector_pose;
    pid_kpid_t my_kpid;

    pid_kpid_t my_dpid;

    float pid_set_v;
    int my_pid_mode=0x01; //bit 1 enable kp,bit 2 enable ki, bit 3 enable kd
    float pid_allow_erro{0.01};

    float pid_refrence_distance{1};
    float pid_refrence_allow_erro{0.01};
    float pid_S_allow_diserro{0.01};
    float pid_braking_dis{0.15};
    float pid_min_w{60};
    void init();

};

#endif // MAINWINDOW_H
