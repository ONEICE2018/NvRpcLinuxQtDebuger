#include "widget_machine_status.h"
#include "ui_widget_machine_status.h"

widget_machine_status::widget_machine_status(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::widget_machine_status)
{
    ui->setupUi(this);
    this->installEventFilter(this);
}

widget_machine_status::~widget_machine_status()
{
    delete ui;
}

bool widget_machine_status::eventFilter(QObject *target, QEvent *event)
{
    if(event->type()==QEvent::Paint)
    {
        refrash_ui();
    }
}
QString getIR_msg(uint8_t irbits){
  QString irmsg="";
    if((irbits>>0)&0x01)
  {
        irmsg.append("CL");
  }
    if((irbits>>1)&0x01)
  {
        irmsg.append("CR");
  }
    if((irbits)>>2&0x01)
  {
        irmsg.append("WL");
  }
    if((irbits)>>3&0x01)
  {
        irmsg.append("WR");
  }
   return irmsg;

}
void widget_machine_status::refrash_ui()
{
     ui->sensor_lWheelCnt->setText(QString("左轮码盘值:%1").arg(MainWindow::MW->getMy_paneldata().lWheelCnt));
     ui->sensor_rWheelCnt->setText(QString("右轮码盘值:%1").arg(MainWindow::MW->getMy_paneldata().rWheelCnt));
     ui->sensor_s_pitch->setText(QString("x_gyro:%1").arg(MainWindow::MW->getMy_paneldata().x_gyro));
     ui->sensor_s_yaw->setText(QString("y_gyro:%1").arg(MainWindow::MW->getMy_paneldata().y_gyro));
     ui->sensor_s_roll->setText(QString("z_gyro:%1").arg(MainWindow::MW->getMy_paneldata().z_gyro));
     ui->sensor_s_xAcc->setText(QString("xAcc:%1").arg(MainWindow::MW->getMy_paneldata().xacc));
     ui->sensor_s_yAcc->setText(QString("yAcc:%1").arg(MainWindow::MW->getMy_paneldata().yacc));
     ui->sensor_s_zAcc->setText(QString("zAcc:%1").arg(MainWindow::MW->getMy_paneldata().zacc));

     ui->sensor_alarmInt->setText(QString("报警信号:%1").arg(MainWindow::MW->getMy_paneldata().alarmInt));
     ui->sensor_borderDist->setText(QString("沿边距离:%1").arg(MainWindow::MW->getMy_paneldata().borderDist));
     ui->sensor_battInt->setText(QString("电池电量:%1").arg(MainWindow::MW->getMy_paneldata().battInt));

     ui->sensor_v->setText(QString("V:%1").arg(MainWindow::MW->getMy_paneldata().v));
     ui->sensor_w->setText(QString("W:%1").arg(MainWindow::MW->getMy_paneldata().w));
     ui->sensor_q0->setText(QString("q0:%1").arg(MainWindow::MW->getMy_paneldata().q0));
     ui->sensor_q1->setText(QString("q1:%1").arg(MainWindow::MW->getMy_paneldata().q1));
     ui->sensor_q2->setText(QString("q2:%1").arg(MainWindow::MW->getMy_paneldata().q2));
     ui->sensor_q3->setText(QString("q3:%1").arg(MainWindow::MW->getMy_paneldata().q3));

     ui->text_protect_land_enable->setText(QString("地检使能:%1").arg(MainWindow::MW->getMy_panel_data_part2().protect_land_enable));

     ui->text_curr_left_brush->setText(QString("左边刷电流:%1").arg(MainWindow::MW->getMy_panel_data_part2().curr_left_edge_brush));
     ui->text_curr_right_brush->setText(QString("右边刷电流:%1").arg(MainWindow::MW->getMy_panel_data_part2().curr_right_edge_brush));
     ui->text_curr_left_wheel->setText(QString("左轮电流:%1").arg(MainWindow::MW->getMy_panel_data_part2().curr_left_wheel));
     ui->text_curr_right_wheel->setText(QString("右轮电流:%1").arg(MainWindow::MW->getMy_panel_data_part2().curr_right_wheel));
     ui->text_curr_midle_wheel->setText(QString("中扫电流:%1").arg(MainWindow::MW->getMy_panel_data_part2().curr_midle_brush));
     ui->text_curr_mop->setText(QString("拖布电机电流:%1").arg(MainWindow::MW->getMy_panel_data_part2().curr_mop));

     ui->text_curr_left_fan->setText(QString("风机电流:%1").arg(MainWindow::MW->getMy_panel_data_part2().curr_fan));
     ui->text_exestatu_mop_down_button->setText(QString("拖布下微动:%1").arg(MainWindow::MW->getMy_panel_data_part2().executor_state>>0&0x01));
     ui->text_exestatu_mop_up_button->setText(QString("拖布上微动:%1").arg(MainWindow::MW->getMy_panel_data_part2().executor_state>>1&0x01));
     ui->text_exestatu_midle_brush->setText(QString("中扫开启:%1").arg(MainWindow::MW->getMy_panel_data_part2().executor_state>>2&0x01));
     ui->text_exestatu_side_brush->setText(QString("边扫开启:%1").arg(MainWindow::MW->getMy_panel_data_part2().executor_state>>3&0x01));
     ui->text_exestatu_fan->setText(QString("风机开启:%1").arg(MainWindow::MW->getMy_panel_data_part2().executor_state>>4&0x01));
    if(MainWindow::MW->DEVICE_VERSION==DEVICE_T10)
    {
     ui->text_land_left_value->setText(QString("左地检值:%1").arg(MainWindow::MW->getMy_panel_data_part2().left_land_D10_LF));
     ui->text_land_right_value->setText(QString("右地检值:%1").arg(MainWindow::MW->getMy_panel_data_part2().right_land_D10_RF));
     ui->text_land_front_value->setText(QString("前地检值:%1").arg(MainWindow::MW->getMy_panel_data_part2().front_land_D10_LM));
    }else if(MainWindow::MW->DEVICE_VERSION==DEVICE_D10){
     ui->text_land_left_value->setText(QString("左前地检值:%1").arg(MainWindow::MW->getMy_panel_data_part2().left_land_D10_LF));
     ui->text_land_right_value->setText(QString("右前地检值:%1").arg(MainWindow::MW->getMy_panel_data_part2().right_land_D10_RF));
     ui->text_land_front_value->setText(QString("左中地检值:%1").arg(MainWindow::MW->getMy_panel_data_part2().front_land_D10_LM));
     ui->text_left_wheel_setvalue->setText(QString("左码盘设定值:%1").arg((int16_t)MainWindow::MW->getMy_panel_data_part2().left_wheel_setSpeed));
     ui->text_right_wheel_setvalue->setText(QString("右码盘设定值:%1").arg((int16_t)MainWindow::MW->getMy_panel_data_part2().right_wheel_setSpeed));


     //     uint16_t  midle_land_D10_RM; // D10右中地检
//     uint16_t  right_land_D10_LB; //D10左后地检
//     uint16_t  front_land_D10_RB; //D10右后地检
    }


     QStringList alarm_string;


    alarm_string.append("适配器充电过压");      // 0
    alarm_string.append("适配器充电欠压");      //  1
    alarm_string.append("电池过压");      //  2
    alarm_string.append("电池欠压");      //   3
    alarm_string.append("风机过流");      //   4
    alarm_string.append("风机开路");      //    5
    alarm_string.append("中扫电机过流");      //  6
    alarm_string.append("中扫电机开路");      //   7
    alarm_string.append("边扫电机过流");      //    8
    alarm_string.append("边扫电机开路");      //   9
    alarm_string.append("左轮电机过流");      //  10
    alarm_string.append("左轮电机开路");      //  11
    alarm_string.append("右轮电机过流");      //   12
    alarm_string.append("右轮电机开路");      //  13
    alarm_string.append("升降电机过流");      //   14
    alarm_string.append("升降电机开路");      //    15
    alarm_string.append("右轮离地");      //16
    alarm_string.append("左轮离地");      //    17
    alarm_string.append("尘盒未盖好");      //    18
    alarm_string.append("沿墙传感器故障");      //   19
    alarm_string.append("无尘袋工作");      //    20
    alarm_string.append("雷达保护盖(T10无)");      //    21
    alarm_string.append("充电失败");      //    22
    alarm_string.append("清洗失败");      //    23
    alarm_string.append("拖布抬升错误");      //   24
    alarm_string.append("拖布放下错误");      //    25
    alarm_string.append("伸缩收缩报错(T10无)");       //    26
    alarm_string.append("伸缩延伸报错(T10无)");       //    27
    alarm_string.append("上基座失败");      //    28
    alarm_string.append("红外丢失");     //29
    if(MainWindow::MW->DEVICE_VERSION==DEVICE_D10){
    alarm_string.append("Zigbee在网L");     //30
    alarm_string.append("Zigbee在网H");     //31
    }else{
        alarm_string.append("清水箱错误");     //30
        alarm_string.append("污水箱错误");     //31
    }
     QString alarm_msg;
     for(int i=0;i<alarm_string.size();i++)
     {
         alarm_msg.append(alarm_string[i]);
        if(MainWindow::MW->getMy_paneldata().alarmInt>>i & 0x01)
         {
            alarm_msg.append(" 1\n");
         }else{
            alarm_msg.append(" 0\n");
        }
     }
     ui->text_alarm_msgs->setText(alarm_msg);
     //show_msgs(alarm_msg);
    if(MainWindow::MW->getDEVICE_VERSION()==DEVICE_T10)
    {
        ui->sensor_eCInfrInt_0->setText(QString("右地检 %1").arg(MainWindow::MW->getMy_paneldata().eCInfrInt>>0 &0x01 ));
        ui->sensor_eCInfrInt_1->setText(QString("左地检 %1").arg(MainWindow::MW->getMy_paneldata().eCInfrInt>>1 &0x01 ));
        ui->sensor_eCInfrInt_2->setText(QString("前中地检 %1").arg(MainWindow::MW->getMy_paneldata().eCInfrInt>>2 &0x01 ));
        ui->sensor_eCInfrInt_4->setText(QString("左撞板 %1").arg(MainWindow::MW->getMy_paneldata().eCInfrInt>>7 &0x01 ));
        ui->sensor_eCInfrInt_5->setText(QString("右撞板 %1").arg(MainWindow::MW->getMy_paneldata().eCInfrInt>>6 &0x01 ));
        ui->sensor_eCInfrInt_7->setText(QString("机器状态 %1").arg(MainWindow::MW->getMy_paneldata().eCInfrInt>>8 &0x0F ));
        ui->sensor_eCInfrInt_12->setText(QString("右后红外 %1").arg(MainWindow::MW->getMy_paneldata().eCInfrInt>>12 &0x01 ));
        ui->sensor_eCInfrInt_13->setText(QString("左后红外 %1").arg(MainWindow::MW->getMy_paneldata().eCInfrInt>>13 &0x01 ));
        ui->sensor_eCInfrInt_14->setText(QString("右前红外 %1").arg(MainWindow::MW->getMy_paneldata().eCInfrInt>>14 &0x01 ));
    }else
    if(MainWindow::MW->getDEVICE_VERSION()==DEVICE_D10)
    {
        ui->sensor_eCInfrInt_0->setText(QString("机器状态 %1").arg(MainWindow::MW->getMy_paneldata().eCInfrInt>>28 &0x0F ));

        ui->sensor_eCInfrInt_1->setText(QString("左撞板 %1").arg(MainWindow::MW->getMy_panel_data_part2().d10_bump_bits>>6 &0x01 ));
        ui->sensor_eCInfrInt_2->setText(QString("中撞板 %1").arg(MainWindow::MW->getMy_panel_data_part2().d10_bump_bits>>7 &0x01 ));
        ui->sensor_eCInfrInt_3->setText(QString("右撞板 %1").arg(MainWindow::MW->getMy_panel_data_part2().d10_bump_bits>>8 &0x01 ));

        //LZ 0 LY 5 QZ 10 QY 15
        QString IR_state="";
        uint8_t irbits=0;
        IR_state.append("前红外:");
        irbits= ((MainWindow::MW->getMy_paneldata().eCInfrInt>>0 &0x01) <<0)|
                ((MainWindow::MW->getMy_paneldata().eCInfrInt>>5 &0x01) <<1)|
                ((MainWindow::MW->getMy_paneldata().eCInfrInt>>10 &0x01)<<2)|
                ((MainWindow::MW->getMy_paneldata().eCInfrInt>>15 &0x01)<<3);
        IR_state.append( getIR_msg(irbits));
        ui->sensor_eCInfrInt_4->setText(IR_state);
        IR_state.clear();
        IR_state.append("左红外:");
        irbits= ((MainWindow::MW->getMy_paneldata().eCInfrInt>>1 &0x01) <<0)|
                ((MainWindow::MW->getMy_paneldata().eCInfrInt>>6 &0x01) <<1)|
                ((MainWindow::MW->getMy_paneldata().eCInfrInt>>11 &0x01)<<2)|
                ((MainWindow::MW->getMy_paneldata().eCInfrInt>>16 &0x01)<<3);
        IR_state.append( getIR_msg(irbits));
        ui->sensor_eCInfrInt_5->setText(IR_state);
        IR_state.clear();
        IR_state.append("右红外:");
        irbits= ((MainWindow::MW->getMy_paneldata().eCInfrInt>>2 &0x01) <<0)|
                ((MainWindow::MW->getMy_paneldata().eCInfrInt>>7 &0x01) <<1)|
                ((MainWindow::MW->getMy_paneldata().eCInfrInt>>12 &0x01)<<2)|
                ((MainWindow::MW->getMy_paneldata().eCInfrInt>>17 &0x01)<<3);
        IR_state.append( getIR_msg(irbits));
        ui->sensor_eCInfrInt_6->setText(IR_state);
        IR_state.clear();
        IR_state.append("左后红外:");
        irbits= ((MainWindow::MW->getMy_paneldata().eCInfrInt>>3 &0x01) <<0)|
                ((MainWindow::MW->getMy_paneldata().eCInfrInt>>8 &0x01) <<1)|
                ((MainWindow::MW->getMy_paneldata().eCInfrInt>>13 &0x01)<<2)|
                ((MainWindow::MW->getMy_paneldata().eCInfrInt>>18 &0x01)<<3);
        IR_state.append( getIR_msg(irbits));
        ui->sensor_eCInfrInt_7->setText(IR_state);
        IR_state.clear();
        IR_state.append("右后红外:");
        irbits= ((MainWindow::MW->getMy_paneldata().eCInfrInt>>4 &0x01) <<0)|
                ((MainWindow::MW->getMy_paneldata().eCInfrInt>>9 &0x01) <<1)|
                ((MainWindow::MW->getMy_paneldata().eCInfrInt>>14 &0x01)<<2)|
                ((MainWindow::MW->getMy_paneldata().eCInfrInt>>19 &0x01)<<3);
        IR_state.append( getIR_msg(irbits));
        ui->sensor_eCInfrInt_8->setText(IR_state);

        ui->sensor_eCInfrInt_9 ->setText(QString("左前地检 %1").arg((MainWindow::MW->getMy_panel_data_part2().d10_bump_bits>>0 &0x01) ));
        ui->sensor_eCInfrInt_10->setText(QString("右前地检 %1").arg((MainWindow::MW->getMy_panel_data_part2().d10_bump_bits>>1 &0x01) ));
        ui->sensor_eCInfrInt_11->setText(QString("左中地检 %1").arg((MainWindow::MW->getMy_panel_data_part2().d10_bump_bits>>2 &0x01) ));
        ui->sensor_eCInfrInt_12->setText(QString("右中地检 %1").arg((MainWindow::MW->getMy_panel_data_part2().d10_bump_bits>>3 &0x01) ));
        ui->sensor_eCInfrInt_13->setText(QString("左后地检 %1").arg((MainWindow::MW->getMy_panel_data_part2().d10_bump_bits>>4 &0x01) ));
        ui->sensor_eCInfrInt_14->setText(QString("右后地检 %1").arg((MainWindow::MW->getMy_panel_data_part2().d10_bump_bits>>5 &0x01) ));

    }

     QString statu="机器状态:";
     uint8_t mstate=0;
      if(MainWindow::MW->getDEVICE_VERSION()==DEVICE_D10){
          mstate=MainWindow::MW->getMy_paneldata().eCInfrInt>>28;

      }else{
         mstate=MainWindow::MW->getMy_paneldata().eCInfrInt>>5 &0x0F;
      }
     switch(mstate)
     {
         case 0:statu.append("待机中");break;
         case 1:statu.append("休眠");break;
         case 2:statu.append("工作");break;
         case 3:statu.append("暂停态");break;
         case 4:statu.append("回充路上");break;
         case 5:statu.append("充电中");break;
         case 6:statu.append("充电完成");break;
         case 7:statu.append("回洗路上");break;
         case 8:statu.append("清洗中");break;
         case 9:statu.append("清洗完成");break;
         case 10:statu.append("清理路上");break;
         case 11:statu.append("清理中");break;
         case 12:statu.append("清理完成");break;
         case 13:statu.append("故障");break;
         case 14:statu.append("遥控");break;
         case 15:statu.append("关机");break;
         case 16:statu.append("跑机模式");break;

     }
     ui->sensor_machine_now_statu->setText(statu);
     ui->sensor_system_time->setText(QString("机器运行时间:%1").arg(QString::fromStdString(MainWindow::MW->getMy_paneldata().now_system_time)));
}

void widget_machine_status::show_msgs(QString msg)
{
//        static int msgcount=0;
//        msgcount++;
//        if(msgcount>2){
//            ui->showmsgs->clear();
//            msgcount=0;
//        }
//      QString showmsg=ui->showmsgs->toPlainText();
//      if(showmsg.size()>1000)
//      {
//        showmsg.clear();
//      }
//      showmsg.append(msg);
//      showmsg.append("\n");
//      ui->showmsgs->setText(showmsg);

}
