#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QTimer>
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

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(see_pose()));
    timer->start(51);
    connect(ui->widget_map, SIGNAL(my_Range_xy_canged()), this, SLOT(map_Range_xy_cahnged()));
}

void MainWindow::showMsgs(QString msg)
{
  QString showmsg=ui->show_msgs->toPlainText();
  if(showmsg.size()>1000){
    showmsg.clear();
  }
  showmsg.append(msg);
  showmsg.append("\n");
  ui->show_msgs->setText(showmsg);
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
         case Qt::Key_Y:
             ui->widget_map->setOrg_is_bot(true);
             fputc(' ',stdin);
         break;
         case Qt::Key_B:
             ui->widget_map->set_Org_back_bot();\
             showMsgs("space");
         break;
   }

}




MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::see_pose()
{
    if(c_reply!=nullptr)
    {
        try {
            my_nowpose=c_reply->call("get_pose").as<rpc_pose_t>();
            my_nowpose.x=my_nowpose.x*1000;
            my_nowpose.y=my_nowpose.y*1000;
            my_nowpose.theta=(my_nowpose.theta/MY_PI)*180   ;
            ui->widget_map->append_hisory_xys(my_nowpose);
            ui->widget_map->update();
        } catch (int e) {

            return;
        }
        ui->text_nowpose->setText(QString("now_pose:\nx:%1\ny:%2\ntheta:%3").arg(my_nowpose.x).arg(my_nowpose.y).arg(my_nowpose.theta));
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




void MainWindow::on_enble_kp_clicked()
{
    if(ui->enble_kp->text()=="enable_kp")
    {
        my_pid_mode|=0x01<<0;
        ui->enble_kp->setText("disable_kp");
        showMsgs("kp is enable");
    }else{
        my_pid_mode&=~(0x01<<0);
        ui->enble_kp->setText("enable_kp");
        showMsgs("kp is disabled");
    }
     if(c_reply!=nullptr)
     c_reply->call("set_pid_mode",my_pid_mode);
}

void MainWindow::on_enbale_ki_clicked()
{
    if(ui->enbale_ki->text()=="enable_ki")
    {
        my_pid_mode|=0x01<<1;
        ui->enbale_ki->setText("disable_ki");
        showMsgs("ki is enable");
    }else
    {
        my_pid_mode&=~(0x01<<1);
        ui->enbale_ki->setText("enable_ki");
         showMsgs("ki is disabled");
    }
    if(c_reply!=nullptr)
    c_reply->call("set_pid_mode",my_pid_mode);
}

void MainWindow::on_enable_kd_clicked()
{
    if(ui->enable_kd->text()=="enable_kd")
    {
        my_pid_mode|=0x01<<2;
        ui->enable_kd->setText("disable_kd");
        showMsgs("kd is enable");
    }else
    {
        my_pid_mode&=~(0x01<<2);
        ui->enable_kd->setText("enable_kd");
         showMsgs("kd is disabled");
    }
    if(c_reply!=nullptr)
    c_reply->call("set_pid_mode",my_pid_mode);
}

void MainWindow::on_set_kp_returnPressed()
{

    if(ui->set_kp->text()!="")
    {
        my_kpid.kp= ui->set_kp->text().toFloat();
        if(c_reply!=nullptr)
        {
            my_kpid=c_reply->call("set_kpid",my_kpid).as<pid_kpid_t>();
            showMsgs(QString("ok! set_kp %1").arg(my_kpid.kp));
            ui->text_kp->setText(QString("kp:%1").arg(my_kpid.kp));

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
            my_kpid=c_reply->call("set_kpid",my_kpid).as<pid_kpid_t>();
            showMsgs(QString("ok! set_ki %1").arg(my_kpid.ki));
            ui->text_ki->setText(QString("ki:%1").arg(my_kpid.ki));
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
            my_kpid=c_reply->call("set_kpid",my_kpid).as<pid_kpid_t>();
            showMsgs(QString("ok! set_kd %1").arg(my_kpid.kd));
             ui->text_kd->setText(QString("kd:%1").arg(my_kpid.kd));
        }
    }
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
             taget_m= c_reply->call("set_pid_target",taget_m).as<rpc_pose_t>();

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
             taget_m= c_reply->call("set_pid_target",taget_m).as<rpc_pose_t>();

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
             taget_m= c_reply->call("set_pid_target",taget_m).as<rpc_pose_t>();

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
         taget_m= c_reply->call("set_pid_target",taget_m).as<rpc_pose_t>();

         my_target.x=taget_m.x*1000;
         my_target.y=taget_m.y*1000;
         my_target.theta=(taget_m.theta/MY_PI)*180;

         showMsgs(QString("ok! set_taeget ok"));
         ui->text_target_y->setText(QString("target_y:%1").arg(my_target.y));
         ui->text_target_theta->setText(QString("target_theta:%1").arg(my_target.theta));
         ui->text_target_x->setText(QString("target_x:%1").arg(my_target.x));
    }

}


void MainWindow::on_enable_pid_run_clicked()
{
    if(ui->enable_pid_run->text()=="enable_pid_run")
    {
        if(c_reply!=nullptr)
        {
            c_reply->call("enable_do_pid",true);
            ui->enable_pid_run->setText("disable_pid_run");
            showMsgs("pid_run is enabled");
        }
    }else
    {
        if(c_reply!=nullptr)
        {
            c_reply->call("enable_do_pid",false);
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
    ui->set_target_x->setText(QString("%1").arg(my_target.x));
    ui->set_target_y->setText(QString("%1").arg(my_target.y));
    on_button_set_target_clicked();
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
             c_reply->call("set_pid_v",pid_set_v);
             showMsgs(QString("ok! set_pid_v %1").arg(pid_set_v));
             ui->text_pid_v->setText(QString("pid_v:%1").arg(pid_set_v));
        }
    }
}

void MainWindow::on_set_allow_erro_returnPressed()
{
    if(ui->set_allow_erro->text()!="")
    {
        setPid_allow_erro(ui->set_allow_erro->text().toUInt());
        if(c_reply!=nullptr){
            c_reply->call("set_pid_allow_erro",getPid_allow_erro());
            showMsgs(QString("ok! pid_allow_erro_radius %1 mm").arg(getPid_allow_erro()));
            ui->text_allow_erro->setText(QString("近点允许误差%1mm").arg(getPid_allow_erro()));
        }
        ui->widget_map->update();
    }
}

int MainWindow::getPid_allow_erro() const
{
    return pid_allow_erro;
}

void MainWindow::setPid_allow_erro(int value)
{
    pid_allow_erro = value;
}

void MainWindow::on_clean_history_clicked()
{
    ui->widget_map->clear_hisory_xys();
}
