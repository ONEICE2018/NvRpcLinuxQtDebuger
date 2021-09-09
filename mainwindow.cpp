#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "rpc/client.h"
#include <QTimer>
#include <QtDebug>
MainWindow *MainWindow::MW;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    MW=this;
    init();

}



void MainWindow::init()
{
    pid_set_v=0;
    my_kpid.kp=1;
    my_kpid.ki=1;
    my_kpid.kd=1;

    my_target.x=0;
    my_target.y=0;
    my_target.theta=0;

    my_nowpose.x=0;
    my_nowpose.y=0;
    my_nowpose.theta=60;

    my_vw.w=0;
    my_vw.v=0;
    ui->text_set_W->setText(QString("set_W:%1").arg(my_vw.w));
    ui->text_set_V->setText(QString("set_V:%1").arg(my_vw.v));
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(see_pose()));
    timer->start(51);

    QTimer *playhis_timer = new QTimer(this);
    connect(playhis_timer, SIGNAL(timeout()), this, SLOT(draw_hisbot()));
    playhis_timer->start(1);

    connect(ui->widget_map, SIGNAL(my_Range_xy_canged()), this, SLOT(map_Range_xy_cahnged()));

    ///CMDs
    set_string_cmds =new QMap<QString,int>;

    //set_string_cmds->insert("",E_MOVE                              );

    set_string_cmds->insert("读取版本",E_REQUEST_VERSION         );
    set_string_cmds->insert("放下拖布",E_MOPDOWN                    );
    set_string_cmds->insert("抬起拖布",E_MOPUP                      );
    set_string_cmds->insert("风机0档",E_FAN_CLOSE                  );
    set_string_cmds->insert("风机1档",E_FAN_OPEN_LOW                );
    set_string_cmds->insert("风机2档",E_FAN_OPEN_MIDDLE             );
    set_string_cmds->insert("风机3档",E_FAN_OPEN_HIGHT              );
    set_string_cmds->insert("风机4档",E_FAN_OPEN_STRONGLY           );
    set_string_cmds->insert("中扫0档",E_MIDDLE_SWEEP_CLOSE                );
    set_string_cmds->insert("中扫1档",E_MIDDLE_SWEEP_LOW                 );
    set_string_cmds->insert("中扫2档",E_MIDDLE_SWEEP_MIDDLE               );
    set_string_cmds->insert("中扫3档",E_MIDDLE_SWEEP_HIGHT                );
    set_string_cmds->insert("边扫0档",E_EDGE_SWEEP_CLOSE              );
    set_string_cmds->insert("边扫1档",E_EDGE_SWEEP_LOW                  );
    set_string_cmds->insert("边扫2档",E_EDGE_SWEEP_MIDDLE                 );
    set_string_cmds->insert("边扫3档",E_EDGE_SWEEP_HIGHT                  );



    //set_string_cmds->insert("上位机控制",E_PERMISSION_NULL            );// ,    //上位机控制
    set_string_cmds->insert("回充",E_PERMISSION_RECHARGE        );// ,//回充
    set_string_cmds->insert("回洗",E_PERMISSION_BACKWASH        );// ,//回洗
    set_string_cmds->insert("关机",E_PERMISSION_SHUTDOWN        );// ,//关机
    set_string_cmds->insert("待机",E_WORK_MOLD_STANBY           );// ,//待机 0
    set_string_cmds->insert("休眠",E_WORK_MOLD_SLEEP            );// ,//休眠 1
    set_string_cmds->insert("清扫",E_WORK_MOLD_CLEAN            );// ,//清扫 2
    set_string_cmds->insert("暂停",E_WORK_MOLD_PAUSE            );// ,// 暂停 3
    set_string_cmds->insert("异常",E_WORK_MOLD_ABNORMAL         );// ,//异常 6
    set_string_cmds->insert("遥控",E_WORK_MOLD_REMOTE_CONTROL   );// ,//遥控 7
    set_string_cmds->insert("回基座失败",E_WORK_MOLD_RECHARGE_FAULT   );// , //回基座失败 8
    set_string_cmds->insert("跑机模式",E_WORK_MOLD_RUNING_MACHINE   );// , //跑机模式 9
    set_string_cmds->insert("配网成功",E_WIFI_CONFIG_S       );// ,     //配网成功 10
    set_string_cmds->insert("配网失败",E_WIFI_CONFIG_F    );//  //配网失败 11


    set_string_cmds->insert("网络异常",E_WIFI_ERR);       //网络异常
    set_string_cmds->insert("地检保护使能",E_MODULE_LAND_PROTECT_ENABLE);
    set_string_cmds->insert("地检保护禁止",E_MODULE_LAND_PROTECT_DISABLE);
    set_string_cmds->insert("读取时间",E_REQU_TIME);

    machine_main_status.insert( ESTATE_INIT,                               "ESTATE_INIT");
    machine_main_status.insert( ESTATE_IDLE,			                     "空闲");
    machine_main_status.insert( ESTATE_CLEAN,			                     "清扫中");
    machine_main_status.insert( ESTATE_PAUSE_CLEAN,                        "清扫暂停,恢复不需要重定位");
    machine_main_status.insert( ESTATE_BACK_CHARGE,                        "回冲中");
    machine_main_status.insert( ESTATE_BACK_WASH,                          "回洗中");
    machine_main_status.insert( ESTATE_VOICE_COME_HERE,                    "语音来这里");
    machine_main_status.insert( ESTATE_APP_CONTROL,                        "app遥控模式");
    machine_main_status.insert( ESTATE_LOW_BATTERY_BACK_CHARGE,            "低电回冲中");
    machine_main_status.insert( ESTATE_OTA,                                "ota中");
    machine_main_status.insert( ESTATE_CHAGRE,                             "充电中");
    machine_main_status.insert( ESTATE_CHAGRE_END,                         "充电完成");
    machine_main_status.insert( ESTATE_CHAGRE_PAUSE,                       "回冲暂停");
    machine_main_status.insert( ESTATE_DORMANCY,		                     "休眠");
    machine_main_status.insert( ESTATE_ERROR,                              "错误中");
    machine_main_status.insert( ESTATE_WASH,                               "清洗中");
    machine_main_status.insert( ESTATE_TASk_DORMANCY,                      "有任务休眠");
    machine_main_status.insert( ESTATE_BACK_BASE_ERR,                      "ESTATE_BACK_BASE_ERR");
    machine_main_status.insert( ESTATE_WASH_PAUSE,                         "回洗暂停");
    machine_main_status.insert( ESTATE_LOW_BATTERY_BACK_CHARGE_PAUSE,      "低电回冲暂停中");
    machine_main_status.insert( ESTATE_OTHER                        ,      "ESTATE_OTHER");

    machine_clean_status.insert(CLEAN_STATE_NULL ,   "未清扫");
    machine_clean_status.insert(CLEAN_STATE_AUTO ,   "全局清扫");
    machine_clean_status.insert(CLEAN_STATE_RECT ,   "划区清扫");
    machine_clean_status.insert(CLEAN_STATE_LOCATION,   "定点清扫");
    machine_clean_status.insert(CLEAN_STATE_SITU,       "原定定点");
    machine_clean_status.insert(CLEAN_STATE_RESERVATION, "预约清扫");

}

rpc_panel_data MainWindow::getMy_paneldata() const
{
    return my_paneldata;
}

void MainWindow::showMsgs(QString msg)
{
  static int msgcount=0;
  msgcount++;
  if(msgcount>2)
  {
        ui->show_msgs->clear();
        msgcount=0;
  }
  QString showmsg=ui->show_msgs->toPlainText();
  if(showmsg.size()>1000)
  {
    showmsg.clear();
  }
  showmsg.append(msg);
  showmsg.append("\n");
  ui->show_msgs->setText(showmsg);
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
 if(!event->isAutoRepeat()){
    switch (event->key())
    {
         case Qt::Key_Y:
             ui->widget_map->setOrg_is_bot(true);
             fputc(' ',stdin);
         break;
         case Qt::Key_B:
             ui->widget_map->set_Org_back_bot();
             showMsgs("space");
            break;
        case Qt::Key_Q:
            ui->widget_map->setQ_pressed(true);
              showMsgs("start set_org");
        break;
        case Qt::Key_W:
             on_remote_cotrol_front_pressed();
        break;

        case Qt::Key_S:
        on_remote_cotrol_back_pressed();

        break;
        case Qt::Key_A:
            on_remote_cotrol_left_pressed();
        break;
        case Qt::Key_D:
            on_remote_cotrol_right_pressed();
        break;

   }
    }
//   QWidget::keyPressEvent(event);
}

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    if(!event->isAutoRepeat()){
    switch (event->key())
    {
        case Qt::Key_Q:
         if(ui->widget_map->getQ_pressed()){
            ui->widget_map->setQ_pressed(false);
              showMsgs("start set_target");
         }
        break;
    case Qt::Key_W:
         on_remote_cotrol_front_released();
        break;
    case Qt::Key_S:
        on_remote_cotrol_back_released();
    break;
    case Qt::Key_A:
         on_remote_cotrol_left_released();
        break;
    case Qt::Key_D:
         on_remote_cotrol_right_released();
    break;
   }
    }
   // QWidget::keyReleaseEvent(event);
}




MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::see_pose()
{

    if(c_reply!=nullptr)

    {
    //rpc::client::connection_state state=c_reply->get_connection_state();


        try {
            //now
            my_paneldata=c_reply->call("get_onepanel_datas").as<rpc_panel_data>();
            my_panel_data_part2=c_reply->call("get_onepanel_datas_part2").as<rpc_panel_data_part2>();

                  my_paneldata.pid_nowpose.x =      my_paneldata.pid_nowpose.x*1000.0   ;
              my_paneldata.pid_vector_pose.x =  my_paneldata.pid_vector_pose.x*1000.0   ;
              my_paneldata.pid_target_pose.x =  my_paneldata.pid_target_pose.x*1000.0   ;
                 my_paneldata.pid_org_pose.x =     my_paneldata.pid_org_pose.x*1000.0   ;
            my_paneldata.pid_jiaodian_pose.x =my_paneldata.pid_jiaodian_pose.x*1000.0   ;

                  my_paneldata.pid_nowpose.y =      my_paneldata.pid_nowpose.y*1000.0   ;
              my_paneldata.pid_vector_pose.y =  my_paneldata.pid_vector_pose.y*1000.0   ;
              my_paneldata.pid_target_pose.y =  my_paneldata.pid_target_pose.y*1000.0   ;
                 my_paneldata.pid_org_pose.y =     my_paneldata.pid_org_pose.y*1000.0   ;
            my_paneldata.pid_jiaodian_pose.y =my_paneldata.pid_jiaodian_pose.y*1000.0   ;

                  my_paneldata.pid_nowpose.theta =      my_paneldata.pid_nowpose.theta/MY_PI*180.0   ;
              my_paneldata.pid_vector_pose.theta =  my_paneldata.pid_vector_pose.theta/MY_PI*180.0   ;
              my_paneldata.pid_target_pose.theta =  my_paneldata.pid_target_pose.theta/MY_PI*180.0   ;
                 my_paneldata.pid_org_pose.theta =     my_paneldata.pid_org_pose.theta/MY_PI*180.0   ;
            my_paneldata.pid_jiaodian_pose.theta =my_paneldata.pid_jiaodian_pose.theta/MY_PI*180.0   ;


            my_paneldata.pid_arrive_erro_dis =my_paneldata.pid_arrive_erro_dis*1000;
            //now_poses
            my_nowpose=my_paneldata.pid_nowpose;
            history_datas myhisdata;
            if(ui->widget_map->getHisory_xys().size()>0 &&
                    (ui->widget_map->getHisory_xys().last().hisory_xys.x!=my_nowpose.x ||
                    ui->widget_map->getHisory_xys().last().hisory_xys.y!=my_nowpose.y ||
                    ui->widget_map->getHisory_xys().last().hisory_xys.theta!=my_nowpose.theta
                     ))
            {
                myhisdata.hisory_xys=my_nowpose                    ;
                myhisdata.mainstatus=my_panel_data_part2.mainStat  ;
                myhisdata.clean_stat=my_panel_data_part2.cleanStat ;
                myhisdata.v=my_paneldata.v;
                myhisdata.w=my_paneldata.w;
                myhisdata.ecinfint=my_paneldata.eCInfrInt;
                ui->widget_map->append_hisory_xys(myhisdata);
            }else if(ui->widget_map->getHisory_xys().size()<=0){
                 ui->widget_map->append_hisory_xys(myhisdata);
            }

            //targets
            rpc_pose_t target_poses;
            target_poses=my_paneldata.pid_target_pose;
            if(ui->widget_map->getTargets_xys().size()>0 &&
                    (ui->widget_map->getTargets_xys().last().x !=target_poses.x ||
                    ui->widget_map->getTargets_xys().last().y !=target_poses.y))
            {
                showMsgs(QString("get now target! %1 %2").arg(target_poses.x).arg(target_poses.y));
                ui->widget_map->append_target_xys(target_poses);
            }else if(ui->widget_map->getTargets_xys().size()<=0){
                ui->widget_map->append_target_xys(target_poses);
            }

            //jiaodian
            my_jd_pos=my_paneldata.pid_jiaodian_pose;
            //org
            my_org_pose = my_paneldata.pid_org_pose;
            //vector
            my_vector_pose =my_paneldata.pid_vector_pose;

            ui->widget_map->update();

            ui->widget_machine_statu->update();


        } catch (int e) {

            return;
        }

        ref_ui_main();

    }

}

void MainWindow::draw_hisbot()
{
    static int ms_count=0;
    if(ui->widget_map->getHisplay_enable())
    {
        ms_count++;

        if(ms_count>play_time_interval){
            int index=ui->widget_map->getHisplay_index();
            index++;
            if(ui->hisbot_slider_seter->maximum()<=ui->widget_map->getHisory_xys().size())
            {
                 if(index>=ui->hisbot_slider_seter->maximum())
                 {
                     index = index-1;
                 }
                 ui->widget_map->setHisplay_index(index);
                 ui->hisbot_slider_seter->setValue(ui->widget_map->getHisplay_index());
                 ui->text_hisbot_nowpoint->setText(QString("%1").arg(index));
                 ui->widget_map->update();
            }
            else
            {
                ui->hisbot_slider_seter->setMaximum(ui->widget_map->getHisory_xys().size());
                ui->text_hisbot_totalpoint->setText(QString("Total points:%1").arg(ui->hisbot_slider_seter->maximum()));
            }
            ms_count=0;
        }

    }else{
        ms_count = 0;
    }
}

void MainWindow::ref_ui_main()
{
    QString mainstat,cleanstat;
    if(machine_main_status.contains(my_panel_data_part2.mainStat))
    {
        mainstat=machine_main_status.value(my_panel_data_part2.mainStat);
    }else{
        mainstat=QString("出现了不存在的状态！");
    }
    if(machine_clean_status.contains(my_panel_data_part2.cleanStat))
    {
        cleanstat=machine_clean_status.value(my_panel_data_part2.cleanStat);
    }else{
        cleanstat=QString("出现了不存在的状态！");
    }
    ui->text_machine_status->setText(QString("Mainstat:%1,cleanstat:%2").arg(mainstat).arg(cleanstat));

    ui->text_nowpose->setText(QString("now_pose:\nx:%1\ny:%2\ntheta:%3").arg(my_paneldata.pid_nowpose.x/1000.0).arg(my_paneldata.pid_nowpose.y/1000.0).arg(my_nowpose.theta));
    ui->text_target_x->setText(QString("target_x:%1").arg(my_paneldata.pid_target_pose.x));
    ui->text_target_y->setText(QString("target_y:%1").arg(my_paneldata.pid_target_pose.y));
    ui->text_target_theta->setText(QString("target_theta:%1").arg(my_paneldata.pid_target_pose.theta));

    ui->text_org_x->setText(QString("target_x:%1").arg(my_paneldata.pid_org_pose.x));
    ui->text_org_y->setText(QString("target_y:%1").arg(my_paneldata.pid_org_pose.y));

    ui->text_kp->setText(QString("角度kp:%1").arg(my_paneldata.pid_angle_kpid.kp));
    ui->text_ki->setText(QString("角度ki:%1").arg(my_paneldata.pid_angle_kpid.ki));
    ui->text_kd->setText(QString("角度kd:%1").arg(my_paneldata.pid_angle_kpid.kd));

    ui->text_dkp->setText(QString("距离kp:%1").arg(my_paneldata.pid_dis_kpid.kp));
    ui->text_dki->setText(QString("距离ki:%1").arg(my_paneldata.pid_dis_kpid.ki));
    ui->text_dkd->setText(QString("距离kd:%1").arg(my_paneldata.pid_dis_kpid.kd));

    if(my_paneldata.pid_tester_run)
    {
        ui->enable_pid_run->setText("disable_pid_run");
    }else
    {
        ui->enable_pid_run->setText("enable_pid_run");
    }
    ui->text_pid_v->setText(QString("pid_v:%1").arg(my_paneldata.pid_set_v));
    ui->text_s_allow_diserro->setText(QString("允许到点误差距离%1mm").arg(my_paneldata.pid_arrive_erro_dis));
    ui->text_min_w->setText(QString(" 最小调节角速度:%1(0.01°/s)").arg(my_paneldata.pid_min_w));

    ui->text_Along_kp->setText(QString("沿角kp:%1").arg(my_panel_data_part2.along_angle_kpid.kp));
    ui->text_Along_ki->setText(QString("沿角ki:%1").arg(my_panel_data_part2.along_angle_kpid.ki));
    ui->text_Along_kd->setText(QString("沿角kd:%1").arg(my_panel_data_part2.along_angle_kpid.kd));

    ui->text_Along_dkp->setText(QString("沿距kp:%1").arg(my_panel_data_part2.along_dis_kpid.kp));
    ui->text_Along_dki->setText(QString("沿距ki:%1").arg(my_panel_data_part2.along_dis_kpid.ki));
    ui->text_Along_dkd->setText(QString("沿距kd:%1").arg(my_panel_data_part2.along_dis_kpid.kd));

    ui->text_along_refdis->setText(QString("A_refdis:%1").arg(my_panel_data_part2.along_refrence_dis));
    ui->text_along_allow_errdis->setText(QString("A_errdis:%1").arg(my_panel_data_part2.along_allow_errodis));


    if(my_panel_data_part2.along_pidtester_enable)
    {
       ui->enable_along_pid->setText("disable_alongPID");
    }else
    {
        ui->enable_along_pid->setText("enable_alongPID");
    }

}

void MainWindow::map_Range_xy_cahnged()
{
     ui->set_map_x_range->setText(QString("%1").arg(ui->widget_map->getRange_xy()));
     ui->show_map_x_range->setText(QString("range_x:%1mm").arg( ui->widget_map->getRange_xy()));
}




void MainWindow::on_connect_rpc_clicked()
{
     QString IP=ui->set_rpc_IP->text();
     std::string ipstr=IP.toStdString();
    if(ui->connect_rpc->text()=="con_rpc")
    {
        if(IP==""){
            showMsgs("ERROR IP set erro");
            return;
        }
        try {
          c_reply=new rpc::client(ipstr,50001);
          rpc::client::connection_state state=c_reply->get_connection_state();
           if(c_reply!=nullptr&&c_reply->get_connection_state()==rpc::client::connection_state::disconnected){
             c_reply=nullptr;
             ui->connect_rpc->setText("con_rpc");
           }
        } catch (int e) {
          showMsgs("ERROR rpc connect failed");
        }


        if(c_reply==nullptr)
        {
            showMsgs("ERROR connetc rpc erro");
            return ;
        }

        ui->connect_rpc->setText("rpc_discon");
    }else
    {
        c_reply=nullptr;
        ui->connect_rpc->setText("con_rpc");
    }
}




void MainWindow::on_set_kp_returnPressed()
{

    if(ui->set_kp->text()!="")
    {
        my_kpid.kp= ui->set_kp->text().toFloat();
        if(c_reply!=nullptr)
        {
            my_kpid=c_reply->call("set_pid_angle_kpid",my_kpid).as<rpc_pid_kpid>();
            showMsgs(QString("ok! set_kp %1").arg(my_kpid.kp));
            ui->text_kp->setText(QString("角度kp:%1").arg(my_kpid.kp));

        }
    }
}

void MainWindow::on_set_ki_returnPressed()
{
    if(ui->set_ki->text()!="")
    {
        my_kpid.ki= ui->set_ki->text().toFloat();
        if(c_reply!=nullptr)
        {
            my_kpid=c_reply->call("set_pid_angle_kpid",my_kpid).as<rpc_pid_kpid>();
            showMsgs(QString("ok! set_ki %1").arg(my_kpid.ki));
            ui->text_ki->setText(QString("角度ki:%1").arg(my_kpid.ki));
        }
    }
}

void MainWindow::on_set_kd_returnPressed()
{
    if(ui->set_kd->text()!="")
    {
        my_kpid.kd= ui->set_kd->text().toFloat();
        if(c_reply!=nullptr)
        {
            my_kpid=c_reply->call("set_pid_angle_kpid",my_kpid).as<rpc_pid_kpid>();
            showMsgs(QString("ok! set_kd %1").arg(my_kpid.kd));
             ui->text_kd->setText(QString("角度kd:%1").arg(my_kpid.kd));
        }
    }
}

void MainWindow::on_set_dkp_returnPressed()
{
    if(ui->set_dkp->text()!="")
    {
        my_dpid.kp= ui->set_dkp->text().toFloat();
        if(c_reply!=nullptr)
        {
            my_dpid=c_reply->call("set_pid_dis_kpid",my_dpid).as<rpc_pid_kpid>();
            showMsgs(QString("ok! set_pid_dis_kpid %1").arg(my_dpid.kp));
            ui->text_dkp->setText(QString("距离kp:%1").arg(my_dpid.kp));

        }
    }

}

void MainWindow::on_set_dki_returnPressed()
{
    if(ui->set_dki->text()!="")
    {
        my_dpid.ki= ui->set_dki->text().toFloat();
        if(c_reply!=nullptr)
        {
            my_dpid=c_reply->call("set_pid_dis_kpid",my_dpid).as<rpc_pid_kpid>();
            showMsgs(QString("ok! set_pid_dis_kpid %1").arg(my_dpid.ki));
            ui->text_dki->setText(QString("距离ki:%1").arg(my_dpid.ki));
        }
    }

}

void MainWindow::on_set_dkd_returnPressed()
{

    if(ui->set_dkd->text()!="")
    {
        my_dpid.kd= ui->set_dkd->text().toFloat();
        if(c_reply!=nullptr)
        {
            my_dpid=c_reply->call("set_pid_dis_kpid",my_dpid).as<rpc_pid_kpid>();
            showMsgs(QString("ok! set_pid_dis_kpid %1").arg(my_dpid.kd));
            ui->text_dkd->setText(QString("距离kd:%1").arg(my_dpid.kd));

        }
    }
}


void MainWindow::on_set_Along_kp_returnPressed()
{
    if(ui->set_Along_kp->text()!="")
    {
        along_angle_kpid.kp= ui->set_Along_kp->text().toFloat();
        if(c_reply!=nullptr)
        {
            along_angle_kpid=c_reply->call("set_along_angle_kpid",along_angle_kpid).as<rpc_pid_kpid>();
            showMsgs(QString("ok! along_angle_kpid.kp %1").arg(along_angle_kpid.kp));
            ui->text_Along_kp->setText(QString("沿角kp:%1").arg(along_angle_kpid.kp));
        }
    }

}

void MainWindow::on_set_Along_ki_returnPressed()
{
    if(ui->set_Along_ki->text()!="")
    {
        along_angle_kpid.ki= ui->set_Along_ki->text().toFloat();
        if(c_reply!=nullptr)
        {
            along_angle_kpid=c_reply->call("set_along_angle_kpid",along_angle_kpid).as<rpc_pid_kpid>();
            showMsgs(QString("ok! along_angle_kpid.ki %1").arg(along_angle_kpid.ki));
            ui->text_Along_ki->setText(QString("沿角ki:%1").arg(along_angle_kpid.ki));
        }
    }
}

void MainWindow::on_set_Along_kd_returnPressed()
{
    if(ui->set_Along_kd->text()!="")
    {
        along_angle_kpid.kd= ui->set_Along_kd->text().toFloat();
        if(c_reply!=nullptr)
        {
            along_angle_kpid=c_reply->call("set_along_angle_kpid",along_angle_kpid).as<rpc_pid_kpid>();
            showMsgs(QString("ok! along_angle_kpid.kd %1").arg(along_angle_kpid.kd));
            ui->text_Along_kd->setText(QString("沿角kd:%1").arg(along_angle_kpid.kd));
        }
    }
}

void MainWindow::on_set_Along_dkp_returnPressed()
{
    if(ui->set_Along_dkp->text()!="")
    {
        along_dis_kpid.kp= ui->set_Along_dkp->text().toFloat();
        if(c_reply!=nullptr)
        {
            along_dis_kpid=c_reply->call("set_along_dis_kpid",along_dis_kpid).as<rpc_pid_kpid>();
            showMsgs(QString("ok! set_along_dis_kpid.kp %1").arg(along_dis_kpid.kp));
            ui->text_Along_dkp->setText(QString("沿距kp:%1").arg(along_dis_kpid.kp));
        }
    }
}

void MainWindow::on_set_Along_dki_returnPressed()
{
    if(ui->set_Along_dki->text()!="")
    {
        along_dis_kpid.ki= ui->set_Along_dki->text().toFloat();
        if(c_reply!=nullptr)
        {
            along_dis_kpid=c_reply->call("set_along_dis_kpid",along_dis_kpid).as<rpc_pid_kpid>();
            showMsgs(QString("ok! set_along_dis_kpid.ki %1").arg(along_dis_kpid.ki));
            ui->text_Along_dki->setText(QString("沿距ki:%1").arg(along_dis_kpid.ki));
        }
    }
}

void MainWindow::on_set_Along_dkd_returnPressed()
{
    if(ui->set_Along_dkd->text()!="")
    {
        along_dis_kpid.kd= ui->set_Along_dkd->text().toFloat();
        if(c_reply!=nullptr)
        {
            along_dis_kpid=c_reply->call("set_along_dis_kpid",along_dis_kpid).as<rpc_pid_kpid>();
            showMsgs(QString("ok! set_along_dis_kpid.kd %1").arg(along_dis_kpid.kd));
            ui->text_Along_dkd->setText(QString("沿距kd:%1").arg(along_dis_kpid.kd));
        }
    }
}






rpc_pose_t MainWindow::getMy_vector_pose() const
{
    return my_vector_pose;
}


void MainWindow::on_set_target_x_returnPressed()
{
    if(ui->set_target_x->text()!="")
    {
        my_target.x = ui->set_target_x->text().toFloat();
        rpc_pose_t taget_m;
        taget_m.x=my_target.x/1000;
        taget_m.y=my_target.y/1000;
        taget_m.theta=(my_target.theta/180.0)*MY_PI;
        if(c_reply!=nullptr)
        {
             taget_m= c_reply->call("set_pid_target_pose",taget_m).as<rpc_pose_t>();

             my_target.x=taget_m.x*1000;
             my_target.y=taget_m.y*1000;
             my_target.theta=(taget_m.theta/MY_PI)*180;
             showMsgs(QString("ok! set_taeget_x %1").arg(my_target.x));
             ui->text_target_x->setText(QString("target_x:%1").arg(my_target.x));
        }
    }
}

void MainWindow::on_set_target_y_returnPressed()
{
    if(ui->set_target_y->text()!="")
    {
        my_target.y = ui->set_target_y->text().toFloat();
        //ZH
        rpc_pose_t taget_m;
        taget_m.x=my_target.x/1000;
        taget_m.y=my_target.y/1000;
        taget_m.theta=(my_target.theta/180.0)*MY_PI;

        if(c_reply!=nullptr)
        {
             taget_m= c_reply->call("set_pid_target_pose",taget_m).as<rpc_pose_t>();

             my_target.x=taget_m.x*1000;
             my_target.y=taget_m.y*1000;
             my_target.theta=(taget_m.theta/MY_PI)*180;

             showMsgs(QString("ok! set_taeget_y %1").arg(my_target.y));
             ui->text_target_y->setText(QString("target_y:%1").arg(my_target.y));
        }
    }
}




void MainWindow::on_set_target_theta_returnPressed()
{
    if(ui->set_target_theta->text()!="")
    {
        
        my_target.theta = ui->set_target_theta->text().toFloat();
        //ZH
        rpc_pose_t taget_m;
        taget_m.x=my_target.x/1000;
        taget_m.y=my_target.y/1000;
        taget_m.theta=(my_target.theta/180.0)*MY_PI;

        if(c_reply!=nullptr)
        {
             taget_m= c_reply->call("set_pid_target_pose",taget_m).as<rpc_pose_t>();

             my_target.x=taget_m.x*1000;
             my_target.y=taget_m.y*1000;
             my_target.theta=(taget_m.theta/MY_PI)*180;
             showMsgs(QString("ok! set_taeget_theta %1").arg(my_target.theta));
             ui->text_target_theta->setText(QString("target_theta:%1").arg(my_target.theta));
        }
    }
}

void MainWindow::on_button_set_target_clicked()
{
    if(ui->set_target_theta->text()!="")
    {
        my_target.theta = ui->set_target_theta->text().toFloat();
    }
    if(ui->set_target_y->text()!="")
    {
        my_target.y = ui->set_target_y->text().toFloat();
    }
    if(ui->set_target_x->text()!="")
    {
        my_target.x = ui->set_target_x->text().toFloat();
    }

    //ZH
    rpc_pose_t taget_m;
    taget_m.x=my_target.x/1000;
    taget_m.y=my_target.y/1000;
    taget_m.theta=(my_target.theta/180.0)*MY_PI;


    if(c_reply!=nullptr)
    {
         taget_m= c_reply->call("set_pid_target_pose",taget_m).as<rpc_pose_t>();

         my_target.x=taget_m.x*1000;
         my_target.y=taget_m.y*1000;
         my_target.theta=(taget_m.theta/MY_PI)*180;

         showMsgs(QString("ok! set_taeget ok"));
         ui->text_target_y->setText(QString("target_y:%1").arg(my_target.y));
         ui->text_target_theta->setText(QString("target_theta:%1").arg(my_target.theta));
         ui->text_target_x->setText(QString("target_x:%1").arg(my_target.x));
    }

}


void MainWindow::on_set_org_x_returnPressed()
{

  on_button_set_org_clicked();
}

void MainWindow::on_set_org_y_returnPressed()
{
   on_button_set_org_clicked();
}

rpc_pose_t MainWindow::getMy_org_pose() const
{
    return my_org_pose;
}

void MainWindow::setMy_org_pose(const rpc_pose_t &value)
{
    my_org_pose = value;

    //ZH
    rpc_pose_t org_m;
    org_m.x=my_org_pose.x/1000;
    org_m.y=my_org_pose.y/1000;
    org_m.theta=0;
      showMsgs(QString("ok! my_org_pose_x %1,my_org_pose_y %2").arg(my_org_pose.x).arg(my_org_pose.y));
    if(c_reply!=nullptr)
    {
         org_m= c_reply->call("set_pid_org_pose",org_m).as<rpc_pose_t>();
         my_org_pose.x=org_m.x*1000;
         my_org_pose.y=org_m.y*1000;
         showMsgs(QString("ok! my_org_pose_x %1,my_org_pose_y %2").arg(my_org_pose.x).arg(my_org_pose.y));
         ui->text_org_x->setText(QString("org_x:%1").arg(my_org_pose.x));
         ui->text_org_y->setText(QString("org_y:%1").arg(my_org_pose.y));
    }

}
void MainWindow::on_button_set_org_clicked()
{
    if(ui->set_org_x->text()!="")
    {
        my_org_pose.x = ui->set_org_x->text().toFloat();

    }
    if(ui->set_org_y->text()!="")
    {
        my_org_pose.y = ui->set_org_y->text().toFloat();

    }

    //ZH
    rpc_pose_t org_m;
    org_m.x=my_org_pose.x/1000;
    org_m.y=my_org_pose.y/1000;
    org_m.theta=0;

    if(c_reply!=nullptr)
    {
         org_m= c_reply->call("set_pid_org_pose",org_m).as<rpc_pose_t>();
         my_org_pose.x=org_m.x*1000;
         my_org_pose.y=org_m.y*1000;
         showMsgs(QString("ok! my_org_pose_x %1,my_org_pose_y %2").arg(my_org_pose.x).arg(my_org_pose.y));
         ui->text_org_x->setText(QString("org_x:%1").arg(my_org_pose.x));
         ui->text_org_y->setText(QString("org_y:%1").arg(my_org_pose.y));
    }

}

rpc_pose_t MainWindow::getMy_jd_pos() const
{
    return my_jd_pos;
}

void MainWindow::setMy_jd_pos(const rpc_pose_t &value)
{
    my_jd_pos = value;
}




void MainWindow::on_enable_pid_run_clicked()
{
    if(ui->enable_pid_run->text()=="enable_pid_run")
    {
        if(c_reply!=nullptr)
        {
            c_reply->call("set_pid_tester_run",true);
            ui->enable_pid_run->setText("disable_pid_run");
            showMsgs("pid_run is enabled");
        }
    }else
    {
        if(c_reply!=nullptr)
        {
            c_reply->call("set_pid_tester_run",false);
            ui->enable_pid_run->setText("enable_pid_run");
            showMsgs("pid_run is disabled");
        }
    }

}

void MainWindow::on_global_clean_clicked()
{
    if(c_reply!=nullptr)
    {
        c_reply->call("CommandGlobalClean");
    }
}

void MainWindow::on_pass_clean_clicked()
{
    if(c_reply!=nullptr)
    {
        c_reply->call("CommandPause");
    }
}

void MainWindow::on_set_rpc_IP_returnPressed()
{
    on_connect_rpc_clicked();
}

rpc_pose_t MainWindow::getMy_target() const
{
    return my_target;
}

void MainWindow::setMy_target(const rpc_pose_t &value)
{
    my_target.x=value.x;
    my_target.y= value.y;

    //ZH
    rpc_pose_t taget_m;
    taget_m.x=my_target.x/1000;
    taget_m.y=my_target.y/1000;
    if(c_reply!=nullptr)
    {
         taget_m= c_reply->call("set_pid_target_pose",taget_m).as<rpc_pose_t>();

         my_target.x=taget_m.x*1000;
         my_target.y=taget_m.y*1000;
         my_target.theta=(taget_m.theta/MY_PI)*180;

         showMsgs(QString("ok! set_taeget ok"));
         ui->text_target_y->setText(QString("target_y:%1").arg(my_target.y));
         ui->text_target_theta->setText(QString("target_theta:%1").arg(my_target.theta));
         ui->text_target_x->setText(QString("target_x:%1").arg(my_target.x));
    }

}

void MainWindow::on_set_map_x_range_returnPressed()
{
    if(ui->set_map_x_range->text()!="")
    {
        ui->widget_map->setRange_xy(ui->set_map_x_range->text().toInt());
        ui->show_map_x_range->setText(QString("range_x:%1mm").arg( ui->widget_map->getRange_xy()));
    }


}

rpc_pose_t MainWindow::getMy_nowpose() const
{
    return my_nowpose;
}

void MainWindow::setMy_nowpose(const rpc_pose_t &value)
{
    my_nowpose = value;
}

void MainWindow::on_set_pid_v_returnPressed()
{
    if(ui->set_pid_v->text()!="")
    {
        pid_set_v= ui->set_pid_v->text().toFloat();
        if(c_reply!=nullptr)
        {
             c_reply->call("set_pid_set_v",pid_set_v);
             showMsgs(QString("ok! set_pid_v %1").arg(pid_set_v));
             ui->text_pid_v->setText(QString("pid_v:%1").arg(pid_set_v));
        }
    }
}


float MainWindow::getPid_allow_erro() const
{
    return pid_allow_erro;
}

void MainWindow::setPid_allow_erro(float value)
{
    pid_allow_erro = value;
}

void MainWindow::on_clean_history_clicked()
{
    ui->widget_map->clear_hisory_xys();
}



void MainWindow::on_set_s_allow_diserro_returnPressed()
{
    if(ui->set_s_allow_diserro->text()!="")
    {
        setPid_S_allow_diserro(ui->set_s_allow_diserro->text().toFloat());
        if(c_reply!=nullptr)
        {
            setPid_S_allow_diserro(1000.0*c_reply->call("set_pid_arrive_erro_dis",getPid_S_allow_diserro()/1000.0).as<float>());
            showMsgs(QString("ok! pid_refrence_distance %1 mm").arg(getPid_S_allow_diserro()));
            ui->text_s_allow_diserro->setText(QString("允许到点误差距离%1mm").arg(getPid_S_allow_diserro()));
        }
        ui->widget_map->update();
    }
}

void MainWindow::on_set_min_w_returnPressed()
{
    if(ui->set_min_w->text()!="")
    {
        setPid_min_w(ui->set_min_w->text().toFloat());
        if(c_reply!=nullptr)
        {
            setPid_min_w(c_reply->call("set_pid_min_w",getPid_min_w()).as<float>());
            showMsgs(QString("ok! setPid_min_w %1 mm").arg(getPid_min_w()));
            ui->text_min_w->setText(QString(" sha che 距离%1mm").arg(getPid_min_w()));
        }
        ui->widget_map->update();
    }

}

float MainWindow::getPid_S_allow_diserro() const
{
    return pid_S_allow_diserro;
}

void MainWindow::setPid_S_allow_diserro(float value)
{
    pid_S_allow_diserro = value;
}

float MainWindow::getPid_refrence_allow_erro() const
{
    return pid_refrence_allow_erro;
}

void MainWindow::setPid_refrence_allow_erro(float value)
{
    pid_refrence_allow_erro = value;
}

float MainWindow::getPid_refrence_distance() const
{
    return pid_refrence_distance;
}

void MainWindow::setPid_refrence_distance(float value)
{
    pid_refrence_distance = value;
}





float MainWindow::getPid_braking_dis() const
{
    return pid_braking_dis;
}

void MainWindow::setPid_braking_dis(float value)
{
    pid_braking_dis = value;
}

void MainWindow::on_clean_targets_clicked()
{
    ui->widget_map->clear_target_xys();
}



float MainWindow::getPid_min_w() const
{
    return pid_min_w;
}

void MainWindow::setPid_min_w(float value)
{
    pid_min_w = value;
}


void MainWindow::on_set_cmds_itemClicked(QListWidgetItem *item)
{
  //showMsgs(item->text());

  if(c_reply!=nullptr)
  {
     if(!set_string_cmds->contains(item->text()))
     {
         return;
     }
     int cmd = set_string_cmds->value(item->text());
     QString set_cmdis=QString("cmd set is %1 ").arg(cmd);
     int getcmd=c_reply->call("set_slave_cmd",cmd).as<int>();
     set_cmdis.append(QString("     get cmd is %1").arg(getcmd));
     showMsgs(set_cmdis);
  }
}

void MainWindow::on_set_V_returnPressed()
{
    if(ui->set_V->text()!="")
    {
        my_vw.v = ui->set_V->text().toUInt();
        if(my_vw.v>300){
            my_vw.v=300;
        }
        ui->text_set_V->setText(QString("set_V:%1").arg(my_vw.v));
    }
}

void MainWindow::on_set_W_returnPressed()
{
    if(ui->set_W->text()!="")
    {

        my_vw.w = ui->set_W->text().toUInt();
        if(my_vw.w>1000)
        {
            my_vw.w=1000;
        }
        ui->text_set_W->setText(QString("set_W:%1").arg(my_vw.w));
    }
}

void MainWindow::on_button_set_VW_clicked()
{
    if(c_reply!=nullptr)
    {

        my_vw = c_reply->call("remote_control_set_my_vw",my_vw).as<my_set_vw>();
        ui->text_set_W->setText(QString("set_W:%1").arg(my_vw.w));
        ui->text_set_V->setText(QString("set_V:%1").arg(my_vw.v));
    }
}
QString press_style="background-color: rgb(115, 210, 22);";
QString reless_style="";
my_set_vw temp_vw{0,0};
void MainWindow::on_remote_cotrol_front_pressed()
{
    if(c_reply!=nullptr){
    temp_vw.v=my_vw.v;
    c_reply->call("remote_control_set_my_vw",temp_vw).as<my_set_vw>();
    showMsgs(QString("front=%1").arg(temp_vw.v));
    }
    ui->remote_cotrol_front->setStyleSheet(press_style);
}

void MainWindow::on_remote_cotrol_front_released()
{
     if(c_reply!=nullptr){
    temp_vw.v=0;
    c_reply->call("remote_control_set_my_vw",temp_vw).as<my_set_vw>();
     showMsgs(QString("front=%1").arg(temp_vw.v));
    }
    ui->remote_cotrol_front->setStyleSheet(reless_style);
}


void MainWindow::on_remote_cotrol_left_pressed()
{
     if(c_reply!=nullptr){
    temp_vw.w=0-my_vw.w;
    c_reply->call("remote_control_set_my_vw",temp_vw).as<my_set_vw>();
    showMsgs(QString("left=%1").arg(temp_vw.w));
   }
   ui->remote_cotrol_left->setStyleSheet(press_style);
}


void MainWindow::on_remote_cotrol_left_released()
{
    if(c_reply!=nullptr){
    temp_vw.w=0;
    c_reply->call("remote_control_set_my_vw",temp_vw).as<my_set_vw>();
    showMsgs(QString("left=%1").arg(temp_vw.w));
    }
      ui->remote_cotrol_left->setStyleSheet(reless_style);
}
void MainWindow::on_remote_cotrol_right_pressed()
{
  if(c_reply!=nullptr){
    temp_vw.w=my_vw.w;
    c_reply->call("remote_control_set_my_vw",temp_vw).as<my_set_vw>();
    showMsgs(QString("rigt=%1").arg(temp_vw.w));
  }
   ui->remote_cotrol_right->setStyleSheet(press_style);
}

void MainWindow::on_remote_cotrol_right_released()
{
 if(c_reply!=nullptr){
    temp_vw.w=0;
    c_reply->call("remote_control_set_my_vw",temp_vw).as<my_set_vw>();
    showMsgs(QString("rigt=%1").arg(temp_vw.w));
 }
 ui->remote_cotrol_right->setStyleSheet(reless_style);
}

void MainWindow::on_remote_cotrol_back_pressed()
{
 if(c_reply!=nullptr){
    temp_vw.v=0-my_vw.v;
    c_reply->call("remote_control_set_my_vw",temp_vw).as<my_set_vw>();
    showMsgs(QString("back=%1").arg(temp_vw.v));
 }
  ui->remote_cotrol_back->setStyleSheet(press_style);
 }

void MainWindow::on_remote_cotrol_back_released()
{
  if(c_reply!=nullptr){
    temp_vw.v=0;
    c_reply->call("remote_control_set_my_vw",temp_vw).as<my_set_vw>();
     showMsgs(QString("back=%1").arg(temp_vw.v));
    }
    ui->remote_cotrol_back->setStyleSheet(reless_style);
  }

void MainWindow::on_button_hisbot_play_clicked()
{
    if(ui->button_hisbot_play->text()=="play_hisbot_enable")
    {
        ui->widget_map->setHisplay_enable(true);
        ui->button_hisbot_play->setText("play_hisbot_disable");
        ui->button_hisbot_play->setStyleSheet("background-color: rgb(255, 100,100);");
        ui->clean_history->setVisible(false);
        ui->clean_targets->setVisible(false);
        ui->hisbot_slider_seter->setRange(0,ui->widget_map->getHisory_xys().size());
        ui->text_hisbot_totalpoint->setText(QString("Total points:%1").arg(ui->hisbot_slider_seter->maximum()));
    }else
    {
        ui->widget_map->setHisplay_enable(false);
        ui->button_hisbot_play->setText("play_hisbot_enable");
        ui->button_hisbot_play->setStyleSheet("");
        ui->clean_history->setVisible(true);
        ui->clean_targets->setVisible(true);
    }
}

void MainWindow::on_hisbot_slider_seter_sliderMoved(int position)
{
 ui->text_hisbot_nowpoint->setText(QString("%1").arg(position));
 if(ui->hisbot_slider_seter->maximum()<=ui->widget_map->getHisory_xys().size())
 {
     if(position<ui->hisbot_slider_seter->maximum())
     {
      ui->widget_map->setHisplay_index(position);
     }
 }
 else
 {
     ui->hisbot_slider_seter->setMaximum(ui->widget_map->getHisory_xys().size());
     ui->text_hisbot_totalpoint->setText(QString("Total points:%1").arg(ui->hisbot_slider_seter->maximum()));
 }

}

void MainWindow::on_set_hisplay_time_interval_returnPressed()
{
    if(ui->set_hisplay_time_interval->text()==""){
        return;
    }
   if(ui->set_hisplay_time_interval->text().toUInt()>=1){
       play_time_interval=ui->set_hisplay_time_interval->text().toUInt();
   }
}

void MainWindow::get_syc_time(time_struct syc)
{
    QString timestr=QString("%1-%2-%3 %4:%5:%6").arg(syc.year).arg(syc.mon).arg(syc.day).arg(syc.hour).arg(syc.min).arg(syc.sec);
    if(c_reply!=nullptr)
    {
        std::string gtime=c_reply->call("syc_slave_time",timestr.toStdString()).as<std::string>();
        showMsgs(QString::fromLocal8Bit(gtime.c_str()));
       ui->text_syc_time->setText(QString::fromLocal8Bit(gtime.c_str()));
    }

}

BOTVERSION MainWindow::getDEVICE_VERSION() const
{
    return DEVICE_VERSION;
}

QMap<int, QString> MainWindow::getMachine_clean_status() const
{
    return machine_clean_status;
}

QMap<int, QString> MainWindow::getMachine_main_status() const
{
    return machine_main_status;
}

void MainWindow::on_set_time_year_returnPressed()
{

    if(ui->set_time_year->text()!="")
    {
        my_syc_time.year = ui->set_time_year->text().toUInt();
        get_syc_time(my_syc_time);

     }
}



void MainWindow::on_set_time_mon_returnPressed()
{
    if(ui->set_time_mon->text()!="")
     {
        my_syc_time.mon = ui->set_time_mon->text().toUInt();
        get_syc_time(my_syc_time);

     }

}

void MainWindow::on_set_time_day_returnPressed()
{
    if(ui->set_time_day->text()!="")
     {
        my_syc_time.day = ui->set_time_day->text().toUInt();
        get_syc_time(my_syc_time);

     }
}

void MainWindow::on_set_time_hour_returnPressed()
{
    if(ui->set_time_hour->text()!="")
     {
        my_syc_time.hour = ui->set_time_hour->text().toUInt();
        get_syc_time(my_syc_time);

     }
}

void MainWindow::on_set_time_min_returnPressed()
{
    if(ui->set_time_min->text()!="")
     {
        my_syc_time.min = ui->set_time_min->text().toUInt();
        get_syc_time(my_syc_time);

     }
}

void MainWindow::on_set_time_sec_returnPressed()
{
    if(ui->set_time_sec->text()!="")
     {
        my_syc_time.sec = ui->set_time_sec->text().toUInt();
        get_syc_time(my_syc_time);
     }
}

void MainWindow::on_set_along_refdis_returnPressed()
{
    if(ui->set_along_refdis->text()!="")
     {
        along_refrence_dis = ui->set_along_refdis->text().toFloat();
        if(c_reply!=nullptr)
        {
           along_refrence_dis=c_reply->call("set_along_refrence_dis",along_refrence_dis).as<float>();

           ui->text_along_refdis->setText(QString("A_refdis:%1").arg(along_refrence_dis));
        }
     }
}

void MainWindow::on_set_along_allow_errdis_returnPressed()
{
    if(ui->set_along_allow_errdis->text()!="")
     {
        along_allow_errodis = ui->set_along_allow_errdis->text().toFloat();
        if(c_reply!=nullptr)
        {
           along_allow_errodis=c_reply->call("set_along_allow_errodis",along_allow_errodis).as<float>();

           ui->text_along_allow_errdis->setText(QString("A_errdis:%1").arg(along_allow_errodis));
        }
     }
}

void MainWindow::on_enable_along_pid_clicked()
{

    if(ui->enable_along_pid->text()=="enable_alongPID")
    {
        if(c_reply!=nullptr)
        {
            bool enable=c_reply->call("set_along_pidtester_enable",true).as<bool>();
            if(enable)
            {
                ui->enable_along_pid->setText("disable_alongPID");
            }
        }
    }
    else
    {
        bool enable=c_reply->call("set_along_pidtester_enable",false).as<bool>();
        if(!enable)
        {
            ui->enable_along_pid->setText("enable_alongPID");
        }
    }
}

void MainWindow::on_comboBox_botversion_editTextChanged(const QString &arg1)
{
    if(arg1=="DEVICE_T10"){
        DEVICE_VERSION=DEVICE_T10;
    }else
    if(arg1=="DEVICE_D10"){
        DEVICE_VERSION=DEVICE_D10;
    }
    ui->widget_map->update();
    ui->widget_machine_statu->update();
}

rpc_panel_data_part2 MainWindow::getMy_panel_data_part2() const
{
    return my_panel_data_part2;
}


