#include "coordinatemap.h"
#include "ui_coordinatemap.h"


CoordinateMap::CoordinateMap(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CoordinateMap)
{
    ui->setupUi(this);
    this->installEventFilter(this);
}

CoordinateMap::~CoordinateMap()
{
    delete ui;
}

void CoordinateMap::mouseMoveEvent(QMouseEvent *event)
{
   if(mouse_pressed){
    draw_target=true;
    ui->show_xy->setText(QString("x:%1,y:%2").arg(getreal_x(event->x())).arg(getreal_y(event->y())));
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
    if(event->button() == Qt::LeftButton)
      {
          mouse_pressed=true;
          draw_target=true;
          ui->show_xy->setText(QString("x:%1,y:%2").arg(getreal_x(event->x())).arg(getreal_y(event->y())));
          update();
    }
}

void CoordinateMap::mouseReleaseEvent(QMouseEvent *event)
{
    mouse_pressed=false;
}

void CoordinateMap::refrash_ui()
{
      int line_x0,line_x1,line_y0,line_y1;
      QPainter painter(this);

      float Pxy=(Range_xy*1.0)/this->width()  ;
      int shift_x=this->width()/2.0*Pxy ;
      int shift_y=this->height()/2.0*Pxy;
      //绘制坐标轴x
      painter.setPen(QPen(Qt::blue,2));//设置画笔形式  或者Qpen pen; pen.setColor(QColor(40, 115, 216)); pen.setWidth(2);
      line_x0=getdraw_one_x(MainWindow::MW->getMy_target().x-shift_x);
      line_y0=getdraw_one_y(MainWindow::MW->getMy_target().y);
      line_x1=getdraw_one_x(MainWindow::MW->getMy_target().x+shift_x);
      line_y1=getdraw_one_y(MainWindow::MW->getMy_target().y);
      painter.drawLine(line_x0,line_y0,line_x1,line_y1);
      //绘制Y轴
      line_x0=getdraw_one_x(MainWindow::MW->getMy_target().x);
      line_y0=getdraw_one_y(MainWindow::MW->getMy_target().y-shift_y);
      line_x1=getdraw_one_x(MainWindow::MW->getMy_target().x);
      line_y1=getdraw_one_y(MainWindow::MW->getMy_target().y+shift_y);
      painter.drawLine(line_x0,line_y0,line_x1,line_y1);
      draw_texts();

}

#define TEXT_PY_Y 3
#define FONT_W 85

#define FONT_H 7
void CoordinateMap::draw_texts()
{
    int line_x0,line_x1,line_y0,line_y1;
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
    line_x0=getdraw_one_x(MainWindow::MW->getMy_target().x);
    line_y0=getdraw_one_y(MainWindow::MW->getMy_target().y);
    painter.drawText(line_x0,line_y0-TEXT_PY_Y,
                     QString("%1,%2").arg(MainWindow::MW->getMy_target().x).arg(MainWindow::MW->getMy_target().y));

    //show min_y
    line_x0=getdraw_one_x(MainWindow::MW->getMy_target().x);
    line_y0=getdraw_one_y(MainWindow::MW->getMy_target().y-shift_y);
    painter.drawText(line_x0,line_y0-TEXT_PY_Y,
                     QString("y_min(%1)").arg(MainWindow::MW->getMy_target().y-shift_y));
    //show max y
    line_x0=getdraw_one_x(MainWindow::MW->getMy_target().x);
    line_y0=getdraw_one_y(MainWindow::MW->getMy_target().y+shift_y);
    painter.drawText(line_x0,line_y0+FONT_H+TEXT_PY_Y,
                     QString("y_max(%1)").arg(MainWindow::MW->getMy_target().y+shift_y));

    //show min_x
    line_x0=getdraw_one_x(MainWindow::MW->getMy_target().x-shift_x);
    line_y0=getdraw_one_y(MainWindow::MW->getMy_target().y);
    painter.drawText(line_x0,line_y0-TEXT_PY_Y,
                     QString("x_min(%1)").arg(MainWindow::MW->getMy_target().x-shift_y));
    //show max x
    line_x0=getdraw_one_x(MainWindow::MW->getMy_target().x+shift_x);
    line_y0=getdraw_one_y(MainWindow::MW->getMy_target().y);
    painter.drawText(line_x0-FONT_W,line_y0-TEXT_PY_Y,
                     QString("x_max(%1)").arg(MainWindow::MW->getMy_target().x+shift_y));




}

int CoordinateMap::getdraw_one_x(int x)
{        
    float Pxy=(Range_xy*1.0)/this->width()  ;
    int shift_x=this->width()/2.0*Pxy ;
    return  (x+shift_x - MainWindow::MW->getMy_target().x)/Pxy;
}

int CoordinateMap::getdraw_one_y(int y)
{
    float Pxy=(Range_xy*1.0)/this->width()  ;
    int shift_y=this->height()/2.0*Pxy;
    int hight=this->height();
    return  hight-(y+shift_y - MainWindow::MW->getMy_target().y)/Pxy;
}

int CoordinateMap::getreal_x(int drawx)
{
    int width=this->width();
    float Pxy=(Range_xy*1.0)/width ;
    int shift_x=width/2.0 * Pxy ;
    return  drawx*Pxy+MainWindow::MW->getMy_target().x-shift_x;
}

int CoordinateMap::getreal_y(int drawy)
{
    float Pxy=(Range_xy*1.0)/this->width()  ;
    int shift_y=this->height()/2.0*Pxy;
    int hight=this->height();
    return (hight-drawy)*Pxy+MainWindow::MW->getMy_target().y-shift_y;
}


