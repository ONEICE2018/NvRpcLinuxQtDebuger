#include "coordinatemap.h"
#include "ui_coordinatemap.h"

#include <QTimer>
#define BOT_L 350  //mm
#define MAX_MOVESPED 10

CoordinateMap::CoordinateMap(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CoordinateMap)
{
    ui->setupUi(this);
    this->installEventFilter(this);
    QCursor cursor;                      // 创建光标对象
    cursor.setShape(Qt::CrossCursor);    // 设置光标形状
    setCursor(cursor);                   // 使用光标
    hisory_xys=*new QList<history_datas>;
    targets_xys=*new QList<rpc_pose_t>;
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(timer_mover()));
    timer->start(66);
    ui->dial_show_hisbot->setVisible(false);
}

CoordinateMap::~CoordinateMap()
{
    delete ui;
}

void CoordinateMap::mouseMoveEvent(QMouseEvent *event)
{
   if(mouse_pressed)
   {
       int x,y;
       x=event->x();
       y=event->y();
       if(x<0){
           x=0;
       }
       if(x>this->width()){
           x=this->width();
       }
       if(y<0){
           y=0;
       }
       if(y>this->height()){
           y=this->height();
       }

       draw_target=true;
       ui->show_xy->setText(QString("x:%1,y:%2").arg(getreal_x(x)).arg(getreal_y(y)));
       rpc_pose_t target;
       target.x=getreal_x(x);
       target.y=getreal_y(y);
       if(!Q_pressed){
           MainWindow::MW->setMy_target(target);
       }else{
           MainWindow::MW->setMy_org_pose(target);
       }
       update();
   }
   if(left_click_to_move)
   {
       int x=event->x();
       int y=event->y();
       int height=this->height();
       int width=this->width();
       if(x<0)
       {
           x=0;
       }
       if(x>width)
       {
           x=width;
       }
       if(y<0)
       {
           y=0;
       }
       if(y>height)
       {
           y=height;
       }
       move_p_x = ((x-width/2.0)/(width/2.0))*MAX_MOVESPED;
       move_p_y = 0-((y-height/2.0)/(height/2.0))*MAX_MOVESPED;
       update();
   }
}

bool CoordinateMap::eventFilter(QObject *target, QEvent *event)
{
    if(event->type()==QEvent::Paint)
    {
        refrash_ui();
    }    
}

void CoordinateMap::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::RightButton)
      {
          mouse_pressed=true;
          draw_target=true;
          ui->show_xy->setText(QString("x:%1,y:%2").arg(getreal_x(event->x())).arg(getreal_y(event->y())));
          rpc_pose_t target;
          target.x=getreal_x(event->x());
          target.y=getreal_y(event->y());

          if(!Q_pressed){
              MainWindow::MW->setMy_target(target);
          }else{
              MainWindow::MW->setMy_org_pose(target);
          }

          update();
    }
    if(event->button() == Qt::LeftButton)
     {
          left_click_to_move=true;

          int x=event->x();
          int y=event->y();
          int height=this->height();
          int width=this->width();
          if(x<0){x=0;}
          if(x>width){x=width;}
          if(y<0){y=0;}
          if(y>height){y=height;}
          move_p_x = ((x-width/2.0)/(width/2.0))*MAX_MOVESPED;
          move_p_y = (0-((y-height/2.0)/(height/2.0)))*MAX_MOVESPED;
          update();
     }
}

void CoordinateMap::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::RightButton)
    {
        mouse_pressed=false;
    }
    if(event->button() == Qt::LeftButton)
    {
         left_click_to_move=false;
    }
}

void CoordinateMap::wheelEvent(QWheelEvent *event)
{
    static int lastvalue;
    if(event->delta() < 0){                    // 当滚轮远离使用者时
       Range_xy+=50;                // 进行缩小
      if(Range_xy>10000){
          Range_xy=10000;
      }
    }else{                                     // 当滚轮向使用者方向旋转时
        Range_xy-=50;               // 进行放大
        if(Range_xy<400){
            Range_xy=400;
        }
    }
    if(Range_xy!=lastvalue)
    {
        update();
        emit my_Range_xy_canged();
    }
    lastvalue=Range_xy;
}

void CoordinateMap::move_view()
{
    if(!org_is_bot&&left_click_to_move)
    {

        refrence_pose.x=MainWindow::MW->getMy_nowpose().x;
        refrence_pose.y=MainWindow::MW->getMy_nowpose().y;
    }

}


void CoordinateMap::refrash_ui()
{
      int line_x0,line_x1,line_y0,line_y1;
      QPainter painter(this);


      float Pxy=(Range_xy*1.0)/this->width()  ;
      int shift_x=this->width()/2.0*Pxy ;
      int shift_y=this->height()/2.0*Pxy;
      if(org_is_bot){
          refrence_pose.x=MainWindow::MW->getMy_nowpose().x;
          refrence_pose.y=MainWindow::MW->getMy_nowpose().y;
          refrence_pose.theta=MainWindow::MW->getMy_nowpose().theta;
      }
      //drawo bot
      line_x0=getdraw_one_x(MainWindow::MW->getMy_nowpose().x);
      line_y0=getdraw_one_y(MainWindow::MW->getMy_nowpose().y);
      ui->dial_show_bot->setGeometry(line_x0-BOT_L/2.0/Pxy+2,line_y0+2-BOT_L/2.0/Pxy,BOT_L/Pxy,BOT_L/Pxy);
      //0~360 ni shizheng 0 zai zui xia
      int t=MainWindow::MW->getMy_nowpose().theta;

      if(t>0){ //0~180
          t=90+t;
      }else if(t>-90){
              t=90+t;
      }else{
           t=450+t;
      }

      ui->dial_show_bot->setValue(t);

      //draw_history_bot
      if(getHisplay_enable())
      {
          ui->hisbot_vw->setVisible(true);
          history_datas iplayhis;
         if(getHisplay_index()<hisory_xys.size())
         {
            iplayhis=hisory_xys[getHisplay_index()];
            //drawo bot
            line_x0=getdraw_one_x(iplayhis.hisory_xys.x);
            line_y0=getdraw_one_y(iplayhis.hisory_xys.y);
            ui->dial_show_hisbot->setGeometry(line_x0-BOT_L/2.0/Pxy+2,line_y0+2-BOT_L/2.0/Pxy,BOT_L/Pxy,BOT_L/Pxy);
            //0~360 ni shizheng 0 zai zui xia
            int t=iplayhis.hisory_xys.theta;

            QString mainstat,cleanstat;
            if(MainWindow::MW->getMachine_main_status().contains(iplayhis.mainstatus))
            {
                mainstat=MainWindow::MW->getMachine_main_status().value(iplayhis.mainstatus);
            }else{
                mainstat=QString("出现了不存在的状态！");
            }
            if(MainWindow::MW->getMachine_clean_status().contains(iplayhis.clean_stat))
            {
                cleanstat=MainWindow::MW->getMachine_clean_status().value(iplayhis.clean_stat);
            }else{
                cleanstat=QString("出现了不存在的状态！");
            }

            QString statu="机器状态:";
            switch(iplayhis.ecinfint>>5 &0x0F)
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


            ui->hisbot_vw->setText(QString("Mainstat:%1 \ncleanstat:%2 \nhis:w %3 v %4 \n%5 ").arg(mainstat).arg(cleanstat).arg(iplayhis.w).arg(iplayhis.v).arg(statu));

            if(t>0){ //0~180
                t=90+t;
            }else if(t>-90){
                    t=90+t;
            }else{
                 t=450+t;
            }

            ui->dial_show_hisbot->setValue(t);
            ui->dial_show_hisbot->setVisible(true);
         }else
         {
             ui->dial_show_hisbot->setVisible(false);
         }

      }else{
          ui->hisbot_vw->setVisible(false);
          ui->dial_show_hisbot->setVisible(false);
      }



      //绘制坐标轴x
      painter.setPen(QPen(Qt::blue,1));//设置画笔形式  或者Qpen pen; pen.setColor(QColor(40, 115, 216)); pen.setWidth(2);
      line_x0=getdraw_one_x(refrence_pose.x-shift_x);
      line_y0=getdraw_one_y(refrence_pose.y);
      line_x1=getdraw_one_x(refrence_pose.x+shift_x);
      line_y1=getdraw_one_y(refrence_pose.y);
      painter.drawLine(line_x0,line_y0,line_x1,line_y1);
      //绘制Y轴
      line_x0=getdraw_one_x(refrence_pose.x);
      line_y0=getdraw_one_y(refrence_pose.y-shift_y);
      line_x1=getdraw_one_x(refrence_pose.x);
      line_y1=getdraw_one_y(refrence_pose.y+shift_y);
      painter.drawLine(line_x0,line_y0,line_x1,line_y1);
      //draw taget
      line_x0=getdraw_one_x(MainWindow::MW->getMy_target().x);
      line_y0=getdraw_one_y(MainWindow::MW->getMy_target().y);

      painter.setPen(QPen(Qt::green,1));
      line_x1=getdraw_one_x(MainWindow::MW->getMy_nowpose().x);
      line_y1=getdraw_one_y(MainWindow::MW->getMy_nowpose().y);
      painter.drawLine(line_x0,line_y0,line_x1,line_y1);

      painter.setPen(QColor(Qt::blue));
      painter.setBrush(QBrush(Qt::yellow));
      painter.drawEllipse(line_x0-4,line_y0-4,8,8);

      //draw taget
      painter.setPen(QPen(Qt::cyan,2));
      line_x1=getdraw_one_x(MainWindow::MW->getMy_org_pose().x);
      line_y1=getdraw_one_y(MainWindow::MW->getMy_org_pose().y);
      painter.drawLine(line_x0,line_y0,line_x1,line_y1);

      painter.setPen(QColor(Qt::color0));
      painter.setBrush(QBrush(Qt::color0));
      painter.drawEllipse(line_x1-4,line_y1-4,8,8);

      //jd pose

      line_x0=getdraw_one_x(MainWindow::MW->getMy_nowpose().x);
      line_y0=getdraw_one_y(MainWindow::MW->getMy_nowpose().y);

      painter.setPen(QPen(Qt::color1,2));
      line_x1=getdraw_one_x(MainWindow::MW->getMy_jd_pos().x);
      line_y1=getdraw_one_y(MainWindow::MW->getMy_jd_pos().y);
      painter.drawLine(line_x0,line_y0,line_x1,line_y1);

      painter.setPen(QColor(Qt::color1));
      painter.setBrush(QBrush(Qt::color1));
      painter.drawEllipse(line_x1-4,line_y1-4,8,8);

      line_x1=getdraw_one_x(MainWindow::MW->getMy_vector_pose().x);
      line_y1=getdraw_one_y(MainWindow::MW->getMy_vector_pose().y);
      painter.drawLine(line_x0,line_y0,line_x1,line_y1);

      painter.setPen(QColor(Qt::color0));
      painter.setBrush(QBrush(Qt::color0));
      painter.drawEllipse(line_x1-4,line_y1-4,8,8);
      //w_vector



      //绘制偏差范围
      painter.setPen(QPen(Qt::black,1));
      painter.setBrush(QBrush(Qt::gray));
      float x0=MainWindow::MW->getMy_nowpose().x; //qi dian
      float y0=MainWindow::MW->getMy_nowpose().y; //qi dian

      float x1=MainWindow::MW->getMy_target().x; //zhong dian
      float y1=MainWindow::MW->getMy_target().y; //zhong dian

      float draw_allowline_erro =  MainWindow::MW->getPid_allow_erro();

      float distance=abs(sqrt(
                             (MainWindow::MW->getMy_target().x-refrence_pose.x)
                             *(MainWindow::MW->getMy_target().x-refrence_pose.x)
                            +(MainWindow::MW->getMy_target().y-refrence_pose.y)
                             *(MainWindow::MW->getMy_target().y-refrence_pose.y)
                            ));

      if(distance > MainWindow::MW->getPid_refrence_distance())
      {
          x1 = x0 + (MainWindow::MW->getPid_refrence_distance()/distance)*(x1 - x0);
          y1 = y0 + (MainWindow::MW->getPid_refrence_distance()/distance)*(y1 - y0);
          draw_allowline_erro = MainWindow::MW->getPid_refrence_allow_erro();
      }


      float etheta=atan2(y1-y0,x1-x0);

      float cx1=x1+ draw_allowline_erro *sin(etheta);//垂线点1
      float cy1=y1- draw_allowline_erro *cos(etheta);//垂线点1
      float cx2=x1- draw_allowline_erro *sin(etheta);//垂线点2
      float cy2=y1+ draw_allowline_erro *cos(etheta);//垂线点2


      line_x0=getdraw_one_x(MainWindow::MW->getMy_nowpose().x);
      line_y0=getdraw_one_y(MainWindow::MW->getMy_nowpose().y);

      line_x1=getdraw_one_x(cx1);
      line_y1=getdraw_one_y(cy1);
      painter.drawLine(line_x0,line_y0,line_x1,line_y1);
      line_x1=getdraw_one_x(cx2);
      line_y1=getdraw_one_y(cy2);
      painter.drawLine(line_x0,line_y0,line_x1,line_y1);

      painter.setPen(QPen(Qt::black,1));
      painter.setBrush(QBrush(Qt::black));
      line_x0=getdraw_one_x(cx1);
      line_y0=getdraw_one_y(cy1);
      painter.drawLine(line_x0,line_y0,line_x1,line_y1);

      mutex_hittory_xys.lock();
      int count_flag=0;
      for(history_datas i: hisory_xys)
          if(hisory_xys.size()>0){
          {
              line_x0=getdraw_one_x(i.hisory_xys.x);
              line_y0=getdraw_one_y(i.hisory_xys.y);
              painter.drawEllipse(line_x0-1,line_y0-1,2,2);
              count_flag++;
              if(count_flag>=2){
                  count_flag=1;
                   painter.drawLine(line_x0,line_y0,line_x1,line_y1);

              }
              line_x1=getdraw_one_x(i.hisory_xys.x);
              line_y1=getdraw_one_y(i.hisory_xys.y);

           }
      }
     mutex_hittory_xys.unlock();

    painter.setPen(QPen(Qt::red,1));
     painter.setBrush(QBrush(Qt::red));

     mutex_targets_xys.lock();
     rpc_pose_t lastpose;
     bool is_not_first=false;
     painter.setPen(QPen(Qt::blue,2));
     for(rpc_pose_t j: targets_xys)
         if(targets_xys.size()>0){
         {

             line_x0=getdraw_one_x(j.x);
             line_y0=getdraw_one_y(j.y);
             painter.drawEllipse(line_x0-5,line_y0-5,10,10);

             if(!is_not_first)
             {
               is_not_first=true;
             }else{

                 line_x1=getdraw_one_x(lastpose.x);
                 line_y1=getdraw_one_y(lastpose.y);
                 painter.drawLine(line_x0,line_y0,line_x1,line_y1);
             }
             lastpose=j;

         }
     }
     mutex_targets_xys.unlock();

     draw_texts();
}



#define TEXT_PY_Y 3
#define FONT_W 85

#define FONT_H 7
void CoordinateMap::draw_texts()
{
    int line_x0,line_y0;
    QPainter painter(this);

    float Pxy=(Range_xy*1.0)/this->width()  ;
    int shift_x=this->width()/2.0*Pxy ;
    int shift_y=this->height()/2.0*Pxy;
    QFont font("宋体", FONT_H, QFont::Bold, true);
    //设置字符间的间距
    font.setLetterSpacing(QFont::AbsoluteSpacing, 2);
    //使用字体
    painter.setFont(font);
    //draw machin (x,y)
    line_x0=getdraw_one_x(refrence_pose.x);
    line_y0=getdraw_one_y(refrence_pose.y);
    painter.drawText(line_x0+BOT_L/2.0/Pxy,line_y0-10-TEXT_PY_Y,
                     QString("%1,%2,%3").arg(refrence_pose.x)
                     .arg(refrence_pose.y)
                     .arg(refrence_pose.theta));

    //show min_y
    line_x0=getdraw_one_x(refrence_pose.x);
    line_y0=getdraw_one_y(refrence_pose.y-shift_y);
    painter.drawText(line_x0,line_y0-TEXT_PY_Y,
                     QString("y_min(%1)").arg(getreal_y(this->height())));
    //show max y
    line_x0=getdraw_one_x(refrence_pose.x);
    line_y0=getdraw_one_y(refrence_pose.y+shift_y);
    painter.drawText(line_x0,line_y0+FONT_H+TEXT_PY_Y,
                     QString("y_max(%1)").arg(getreal_y(0)));

    //show min_x
    line_x0=getdraw_one_x(refrence_pose.x-shift_x);
    line_y0=getdraw_one_y(refrence_pose.y);
    painter.drawText(line_x0,line_y0-TEXT_PY_Y,
                     QString("x_min(%1)").arg(getreal_x(0)));
    //show max x
    line_x0=getdraw_one_x(refrence_pose.x+shift_x);
    line_y0=getdraw_one_y(refrence_pose.y);
    painter.drawText(line_x0-FONT_W,line_y0-TEXT_PY_Y,
                     QString("x_max(%1)").arg(getreal_x(this->width())));

    //show distance
    painter.setPen(QPen(Qt::green,1));
    painter.setBrush(QBrush(Qt::green));
    line_x0=getdraw_one_x(MainWindow::MW->getMy_target().x);
    line_y0=getdraw_one_y(MainWindow::MW->getMy_target().y);

    float distance=abs(sqrt(
                           (MainWindow::MW->getMy_target().x-refrence_pose.x)
                           *(MainWindow::MW->getMy_target().x-refrence_pose.x)
                          +(MainWindow::MW->getMy_target().y-refrence_pose.y)
                           *(MainWindow::MW->getMy_target().y-refrence_pose.y)
                          ));

    painter.drawText(line_x0-FONT_W,line_y0-TEXT_PY_Y,
                     QString("D:%1mm").arg(distance));


}

bool CoordinateMap::getHisplay_enable() const
{
    return hisplay_enable;
}

void CoordinateMap::setHisplay_enable(bool value)
{
    hisplay_enable = value;
}



int CoordinateMap::getHisplay_index() const
{
    return hisplay_index;
}

void CoordinateMap::setHisplay_index(int value)
{
    hisplay_index = value;
}

bool CoordinateMap::getQ_pressed() const
{
    return Q_pressed;
}

void CoordinateMap::setQ_pressed(bool value)
{
    Q_pressed = value;
}

QList<rpc_pose_t> CoordinateMap::getTargets_xys() const
{
    return targets_xys;
}



void CoordinateMap::setOrg_is_bot(bool value)
{
    org_is_bot =!org_is_bot;
    MainWindow::MW->showMsgs(QString("view locked %1").arg(org_is_bot));
    update();
}

void CoordinateMap::set_Org_back_bot()
{
    if(!org_is_bot){
        refrence_pose.x=MainWindow::MW->getMy_nowpose().x;
        refrence_pose.y=MainWindow::MW->getMy_nowpose().y;
        refrence_pose.theta=MainWindow::MW->getMy_nowpose().theta;
        update();
    }
}

void CoordinateMap::timer_mover()
{
    //set referce point
    if(!org_is_bot && left_click_to_move){
        float Pxy=(Range_xy*1.0)/this->width()  ;
        refrence_pose.x+=move_p_x*Pxy;
        refrence_pose.y+=move_p_y*Pxy;
        update();
    }
}

void CoordinateMap::setRefrence_pose(const rpc_pose_t &value)
{
    refrence_pose = value;
}



QList<history_datas> CoordinateMap::getHisory_xys() const
{
    return hisory_xys;
}

int CoordinateMap::getdraw_one_x(int x)
{        
    float Pxy=(Range_xy*1.0)/this->width()  ;
    int shift_x=this->width()/2.0*Pxy ;
    return  (x+shift_x - refrence_pose.x)/Pxy;
}

int CoordinateMap::getdraw_one_y(int y)
{
    float Pxy=(Range_xy*1.0)/this->width()  ;
    int shift_y=this->height()/2.0*Pxy;
    int hight=this->height();
    return  hight-(y+shift_y - refrence_pose.y)/Pxy;
}

int CoordinateMap::getreal_x(int drawx)
{
    int width=this->width();
    float Pxy=(Range_xy*1.0)/width ;
    int shift_x=width/2.0 * Pxy ;
    return  drawx*Pxy+refrence_pose.x-shift_x;
}

int CoordinateMap::getreal_y(int drawy)
{
    float Pxy=(Range_xy*1.0)/this->width()  ;
    int shift_y=this->height()/2.0*Pxy;
    int hight=this->height();
    return (hight-drawy)*Pxy+refrence_pose.y-shift_y;
}

int CoordinateMap::getRange_xy() const
{
    return Range_xy;
}

void CoordinateMap::setRange_xy(int value)
{
    Range_xy = value;
    update();
}
void CoordinateMap::clear_hisory_xys(void){
    mutex_hittory_xys.lock();
    hisory_xys.clear();
    mutex_hittory_xys.unlock();
}
void CoordinateMap::append_hisory_xys(history_datas his){
    mutex_hittory_xys.lock();
    hisory_xys.append(his);
    mutex_hittory_xys.unlock();
}

void CoordinateMap::clear_target_xys()
{
    mutex_targets_xys.lock();
    targets_xys.clear();
    mutex_targets_xys.unlock();
}

void CoordinateMap::append_target_xys(rpc_pose_t tgs)
{
    mutex_targets_xys.lock();
    targets_xys.append(tgs);
    mutex_targets_xys.unlock();
}

