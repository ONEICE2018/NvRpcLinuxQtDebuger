#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "widget_pid_curve.h"
#include <rpc/client.h>
#include "rpc/this_handler.h"
#include <thread>
#include <time.h>
#include <unistd.h>
#include <sstream>
#include <iostream>
#include <QKeyEvent>
#include "ui_coordinatemap.h"
#include "QListWidget"


#define MY_PI 3.141592
enum BOTVERSION
{
    DEVICE_T10,
    DEVICE_D10
};
struct rpc_pid_kpid
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

struct my_set_vw
{
   int v;
   int w;
   MSGPACK_DEFINE_MAP(v, w);
};

struct time_struct{
  int year;
  int mon;
  int day;
  int hour;
  int min;
  int sec;

};


struct rpc_panel_data
{
    //>>>
    int16_t lWheelCnt;
    int16_t rWheelCnt;
    float x_gyro;
    float y_gyro;
    float z_gyro;
    float xacc;
    float yacc;
    float zacc;
    uint32_t eCInfrInt;
    uint32_t alarmInt ;
    uint8_t borderDist;
    uint8_t battInt ;
    int16_t v;
    int16_t w;
    int16_t q0;
    int16_t q1;
    int16_t q2;
    int16_t q3;
    //<<<<<<<<<<<<<<<LinePID
    rpc_pid_kpid pid_angle_kpid;      ///<
    rpc_pid_kpid pid_dis_kpid;        ///<

    rpc_pose_t pid_nowpose;
    rpc_pose_t pid_vector_pose;
    rpc_pose_t pid_target_pose;       ///<
    rpc_pose_t pid_org_pose;          ///<
    rpc_pose_t pid_jiaodian_pose;     ///<
    bool  pid_tester_run;             ///<
    float pid_set_v;                  ///<
    float pid_arrive_erro_dis;        ///<
    float pid_min_w;                  ///
    std::string now_system_time;

    MSGPACK_DEFINE_MAP(lWheelCnt,rWheelCnt,x_gyro,y_gyro,z_gyro,xacc,yacc,zacc,eCInfrInt,alarmInt,borderDist,battInt,v,w,q0,q1,q2,q3
                      ,pid_angle_kpid,pid_dis_kpid
                      ,pid_nowpose,pid_vector_pose
                      ,pid_target_pose,pid_org_pose,pid_jiaodian_pose
                      ,pid_tester_run,pid_set_v,pid_arrive_erro_dis,pid_min_w
                      ,now_system_time
                      );
};

struct rpc_panel_data_part2{
    //<<<<<<<< AlongPID
        rpc_pid_kpid along_angle_kpid;      ///<
        rpc_pid_kpid along_dis_kpid;        ///<
        float along_refrence_dis;
        float along_allow_errodis;
        bool along_pidtester_enable;
        int16_t mainStat;
        int16_t cleanStat;
        uint8_t   curr_left_edge_brush;
        uint8_t   curr_right_edge_brush;
        uint8_t   curr_left_wheel;
        uint8_t   curr_right_wheel;
        uint8_t   curr_midle_brush;
        uint8_t   curr_mop;
        uint8_t   curr_fan;
        uint8_t   protect_land_enable;
        uint8_t  executor_state;
        uint16_t  left_land_D10_LF;
        uint16_t  right_land_D10_RF;
        uint16_t  front_land_D10_LM;
        uint16_t  left_wheel_setSpeed  ;
        uint16_t  right_wheel_setSpeed ;
        uint16_t  midle_land_D10_RM;   // D10右中地检
        uint16_t  right_land_D10_LB; //D10左后地检
        uint16_t  front_land_D10_RB; //D10右后地检
        uint16_t d10_bump_bits;

        //<<<<<<<<<<<<<<<
        MSGPACK_DEFINE_MAP(along_angle_kpid,along_dis_kpid,along_refrence_dis,along_allow_errodis,along_pidtester_enable
                           ,along_pidtester_enable,mainStat,cleanStat
                           ,curr_left_edge_brush,curr_right_edge_brush,curr_left_wheel,curr_right_wheel
                           ,curr_midle_brush,curr_mop,curr_fan,protect_land_enable,executor_state,left_land_D10_LF,right_land_D10_RF
                           ,front_land_D10_LM,left_wheel_setSpeed,right_wheel_setSpeed,midle_land_D10_RM,right_land_D10_LB,front_land_D10_RB
                          ,d10_bump_bits);

};

//master to slave
enum CMToSCMD //master to slave command
{
//    E_MOVE = 1,//下发 V W，移动速度
//    E_MOPDOWN,		//放下拖布
//    E_MOPUP,        //抬起拖布
//    E_EDGE_SWEEP_LOW,//边扫开
//    E_EDGE_SWEEP_MIDDLE, //T10
//    E_EDGE_SWEEP_HIGHT,  //T10
//    E_EDGE_SWEEP_CLOSE,//边扫关
//    E_FAN_OPEN_LOW = 8,   //风机 低
//    E_FAN_OPEN_MIDDLE, //风机中
//    E_FAN_OPEN_HIGHT, //风机 高
//    E_FAN_OPEN_STRONGLY, //风机 强劲
//    E_FAN_CLOSE,		//风机 关
//    E_MIDDLE_SWEEP_LOW, //中扫 开
//    E_MIDDLE_SWEEP_MIDDLE, //T10
//    E_MIDDLE_SWEEP_HIGHT,  //T10
//    E_MIDDLE_SWEEP_CLOSE,//中扫 关
//    E_APP_START_CLEAN_MOP=17, //APP控制洗拖布
//    E_REQUEST_VERSION, //请求下位机版本号
//    E_REQU_TIME, //请求下位机的时间
//    E_SYNC_TIME, //将最新时间同步给下位机
//    E_WORK_MOLD_STANBY=21,//待机 0
//    E_WORK_MOLD_SLEEP,//休眠 1
//    E_WORK_MOLD_CLEAN,//清扫 2
//    E_WORK_MOLD_PAUSE ,// 暂停 3
//    E_PERMISSION_RECHARGE,//回充下发控制权 4
//    E_PERMISSION_BACKWASH,//回洗下发控制权 5
//    E_WORK_MOLD_ABNORMAL ,//异常 6
//    E_WORK_MOLD_REMOTE_CONTROL,//遥控 7
//    E_WORK_MOLD_RECHARGE_FAULT, //回基座失败 8
//    E_PERMISSION_SHUTDOWN,//关机 9
//    E_WORK_MOLD_RUNING_MACHINE, //跑机模式 10
//    E_WIFI_CONFIG_S, //配望成功 11
//    E_WIFI_CONFIG_F, //配望失败 12
//    E_WIFI_ERR,       //网络异常13
//    E_MODULE_LAND_PROTECT_ENABLE,
//    E_MODULE_LAND_PROTECT_DISABLE,
//    E_ACK_EXECUTOR_STATUS
    E_MOVE = 1,//下发 V W，移动速度
    E_MOPDOWN,		//放下拖布
    E_MOPUP,        //抬起拖布

    E_EDGE_SWEEP_LOW,//边扫开
    E_EDGE_SWEEP_MIDDLE, //T10
    E_EDGE_SWEEP_HIGHT,  //T10
    E_EDGE_SWEEP_CLOSE,//边扫关

    E_FAN_OPEN_LOW = 8,   //风机 低
    E_FAN_OPEN_MIDDLE, //风机中
    E_FAN_OPEN_HIGHT, //风机 高
    E_FAN_OPEN_STRONGLY, //风机 强劲
    E_FAN_CLOSE,		//风机 关

    E_MIDDLE_SWEEP_LOW, //中扫 开
    E_MIDDLE_SWEEP_MIDDLE, //T10
    E_MIDDLE_SWEEP_HIGHT,  //T10
    E_MIDDLE_SWEEP_CLOSE,//中扫 关

    E_APP_START_CLEAN_MOP=17, //APP控制洗拖布
    E_REQUEST_VERSION, //请求下位机版本号

    E_REQU_TIME, //请求下位机的时间
    E_SYNC_TIME, //将最新时间同步给下位机

    E_WORK_MOLD_STANBY=21,//待机 0
    E_WORK_MOLD_SLEEP,//休眠 1
    E_WORK_MOLD_CLEAN,//清扫 2
    E_WORK_MOLD_PAUSE ,// 暂停 3
    E_PERMISSION_RECHARGE,//回充下发控制权 4
    E_PERMISSION_BACKWASH,//回洗下发控制权 5
    E_WORK_MOLD_ABNORMAL ,//异常 6
    E_PERMISSION_BACKDUMP,     //回倒下发控制权7
    E_WORK_MOLD_RECHARGE_FAULT, //回基座失败 8
    E_PERMISSION_SHUTDOWN,//关机 9
    E_WORK_MOLD_RUNING_MACHINE, //跑机模式 10


    E_WIFI_CONFIG_S, //配望成功 11
    E_WIFI_CONFIG_F, //配望失败 12
    E_WIFI_ERR,       //网络异常
    E_ACK_EXECUTOR_STATUS,
    E_SET_OFFSET_SETTING,//产检校准传参
    E_SET_ODOMETRY,      //里程校准

    E_TELL_IS_SLIPING, //告诉下位机打滑了

    E_PERMISSION_GRAB_DUSTBOX, //取尘盒下发控制权
    E_PERMISSION_PUT_DOWN_MOP, //放下拖布下发控制权
    E_PERMISSION_GRAB_MOP,     //取拖布下发控制权
    E_PERMISSION_UPWARD,       //上基座下发控制权

    E_ZIGBEE_CONFIG,       //
    E_MOP_PUT,
    E_MOP_GET
};

//master to slave
enum CMToSCMD_D10 //master to slave command
{
    E_D10_MOVE = 1,//下发 V W，移动速度
    E_D10_MOPDOWN,		//放下拖布
    E_D10_MOPUP,        //抬起拖布
    E_D10_EDGE_SWEEP_LOW,//边扫开
    E_D10_EDGE_SWEEP_MIDDLE,
    E_D10_EDGE_SWEEP_HIGHT,
    E_D10_EDGE_SWEEP_CLOSE,//边扫关
    E_D10_FAN_OPEN_LOW = 8,   //风机 低
    E_D10_FAN_OPEN_MIDDLE, //风机中
    E_D10_FAN_OPEN_HIGHT, //风机 高
    E_D10_FAN_OPEN_STRONGLY, //风机 强劲
    E_D10_FAN_CLOSE,		//风机 关
    E_D10_MIDDLE_SWEEP_LOW, //中扫 开
    E_D10_MIDDLE_SWEEP_MIDDLE, //T10
    E_D10_MIDDLE_SWEEP_HIGHT,  //T10
    E_D10_MIDDLE_SWEEP_CLOSE,//中扫 关
    E_D10_APP_START_CLEAN_MOP=17, //APP控制洗拖布
    E_D10_REQUEST_VERSION, //请求下位机版本号
    E_D10_REQU_TIME, //请求下位机的时间
    E_D10_SYNC_TIME, //将最新时间同步给下位机
    E_D10_WORK_MOLD_STANBY=21,//待机 0
    E_D10_WORK_MOLD_SLEEP,//休眠 1
    E_D10_WORK_MOLD_CLEAN,//清扫 2
    E_D10_WORK_MOLD_PAUSE ,// 暂停 3
    E_D10_PERMISSION_RECHARGE,//回充下发控制权 4
    E_D10_PERMISSION_BACKWASH,//回洗下发控制权 5
    E_D10_WORK_MOLD_ABNORMAL ,//异常 6
    E_D10_WORK_MOLD_REMOTE_CONTROL,//遥控 7
    E_D10_WORK_MOLD_RECHARGE_FAULT, //回基座失败 8
    E_D10_PERMISSION_SHUTDOWN,//关机 9
    E_D10_WORK_MOLD_RUNING_MACHINE, //跑机模式 10
    E_D10_WIFI_CONFIG_S, //配望成功 11
    E_D10_WIFI_CONFIG_F, //配望失败 12
    E_D10_WIFI_ERR,       //网络异常
    E_D10_ACK_EXECUTOR_STATUS,
    E_D10_SET_OFFSET_SETTING,//产检校准传参
    E_D10_SET_ODOMETRY,      //里程校准
    E_D10_TELL_IS_SLIPING, //告诉下位机打滑了
    E_D10_PERMISSION_BACKDUMP,     //回倒下发控制权
    E_D10_PERMISSION_GRAB_DUSTBOX, //取尘盒下发控制权
    E_D10_PERMISSION_PUT_DOWN_MOP, //放下拖布下发控制权
    E_D10_PERMISSION_GRAB_MOP,     //取拖布下发控制权
    E_D10_PERMISSION_UPWARD,       //上基座下发控制权
    E_D10_ZIGBEE_CONFIG       //


};//下发到下位机的控制指令和状态列表


enum EMainState
{
    ESTATE_INIT = 0,
    ESTATE_IDLE,			//空闲
    ESTATE_CLEAN,			//清扫中
    ESTATE_PAUSE_CLEAN,     //清扫暂停,恢复不需要重定位
    ESTATE_BACK_CHARGE,     //回冲中
    ESTATE_BACK_WASH,       //回洗中
    ESTATE_VOICE_COME_HERE, //语音来这里
    ESTATE_APP_CONTROL,     //app遥控模式
    ESTATE_LOW_BATTERY_BACK_CHARGE,     //低电回冲中
    ESTATE_OTA,             //ota中
    ESTATE_CHAGRE,          //充电中
    ESTATE_CHAGRE_END,      //充电完成
    ESTATE_CHAGRE_PAUSE,    //回冲暂停
    ESTATE_DORMANCY,		//休眠
    ESTATE_ERROR,           //错误中
    ESTATE_WASH,            //清洗中
    ESTATE_TASk_DORMANCY,   //有任务休眠
    ESTATE_BACK_BASE_ERR,
    ESTATE_WASH_PAUSE,      //回洗暂停
    ESTATE_LOW_BATTERY_BACK_CHARGE_PAUSE,     //低电回冲暂停中
    ESTATE_OTHER
};
typedef EMainState E_MASTER_STATE;

enum CLEAN_STATE
{
    CLEAN_STATE_NULL = 0,   //未清扫
    CLEAN_STATE_AUTO = 1,   //全局清扫
    CLEAN_STATE_RECT = 2,   //划区清扫
    CLEAN_STATE_LOCATION = 3,   //定点清扫
    CLEAN_STATE_SITU = 4,       //原定定点
    CLEAN_STATE_RESERVATION = 5 //预约清扫
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

    rpc_panel_data getMy_paneldata() const;



    QMap<int, QString> getMachine_main_status() const;

    QMap<int, QString> getMachine_clean_status() const;

    BOTVERSION getDEVICE_VERSION() const;

    rpc_panel_data_part2 getMy_panel_data_part2() const;
    BOTVERSION DEVICE_VERSION{DEVICE_D10};
protected:
    virtual  void keyPressEvent(QKeyEvent *event);
   virtual void keyReleaseEvent(QKeyEvent *event);

private slots:
    void roate_test_time_out();
    void see_pose();
    void draw_hisbot();
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

    void on_clean_history_clicked();

    void on_set_s_allow_diserro_returnPressed();


    void on_clean_targets_clicked();

    void on_set_min_w_returnPressed();

    void on_set_org_x_returnPressed();

    void on_set_org_y_returnPressed();

    void on_button_set_org_clicked();

    void on_set_dkp_returnPressed();

    void on_set_dki_returnPressed();

    void on_set_dkd_returnPressed();

    void on_set_cmds_itemClicked(QListWidgetItem *item);


    void on_set_V_returnPressed();

    void on_set_W_returnPressed();

    void on_button_set_VW_clicked();

    void on_remote_cotrol_front_pressed();

    void on_remote_cotrol_front_released();

    void on_remote_cotrol_left_pressed();

    void on_remote_cotrol_left_released();

    void on_remote_cotrol_right_pressed();

    void on_remote_cotrol_right_released();

    void on_remote_cotrol_back_pressed();

    void on_remote_cotrol_back_released();

    void on_button_hisbot_play_clicked();

    void on_hisbot_slider_seter_sliderMoved(int position);

    void on_set_hisplay_time_interval_returnPressed();

    void on_set_Along_kp_returnPressed();

    void on_set_Along_ki_returnPressed();

    void on_set_Along_kd_returnPressed();

    void on_set_Along_dkp_returnPressed();

    void on_set_Along_dki_returnPressed();

    void on_set_Along_dkd_returnPressed();

    void on_set_time_year_returnPressed();

    void on_set_time_sec_returnPressed();

    void on_set_time_mon_returnPressed();

    void on_set_time_day_returnPressed();

    void on_set_time_hour_returnPressed();

    void on_set_time_min_returnPressed();

    void on_set_along_refdis_returnPressed();

    void on_set_along_allow_errdis_returnPressed();

    void on_enable_along_pid_clicked();

    void on_comboBox_botversion_editTextChanged(const QString &arg1);



    void on_set_angle_returnPressed();

    void on_button_roate_test_clicked();

    void on_set_roate_test_r_returnPressed();

    void on_goto_realtime_widget_clicked();

    void on_comboBox_botversion_currentTextChanged(const QString &arg1);

private:
    Ui::MainWindow *ui;
    rpc::client* c_reply = nullptr;
    rpc_pose_t my_target;
    rpc_pose_t my_org_pose;
    rpc_pose_t my_nowpose;
    rpc_pose_t my_jd_pos;
    rpc_pose_t my_vector_pose;
    rpc_pid_kpid my_kpid;
    rpc_pid_kpid my_dpid;

    //alongPID
    rpc_pid_kpid along_angle_kpid{0,0,0};
    rpc_pid_kpid along_dis_kpid{0,0,0};
    float along_refrence_dis;
    float along_allow_errodis;
    bool along_pidtester_enable;

    float pid_set_v;
    int my_pid_mode=0x01; //bit 1 enable kp,bit 2 enable ki, bit 3 enable kd
    float pid_allow_erro{0.01};

    float pid_refrence_distance{1};
    float pid_refrence_allow_erro{0.01};
    float pid_S_allow_diserro{0.01};
    float pid_braking_dis{0.15};
    float pid_min_w{60};
    rpc_panel_data my_paneldata;
    rpc_panel_data_part2 my_panel_data_part2;
    void init();
    void version_init(void);
    void ref_ui_main();

    void play_hisbot();
    int play_time_interval{50};

    QMap<QString,int> *set_string_cmds;
    my_set_vw my_vw;
    int rotate_test_angle=0;
    uint16_t roate_test_r=0;
    time_struct my_syc_time{2021,7,21,17,0,0};
    void get_syc_time(time_struct syc);
    QMap<int,QString> machine_main_status;
    QMap<int,QString> machine_clean_status;


    widget_pid_curve *my_widget_pid_curve=nullptr;

};

#endif // MAINWINDOW_H
