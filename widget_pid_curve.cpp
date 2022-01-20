#include "widget_pid_curve.h"
#include "ui_widget_pid_curve.h"
#include <mainwindow.h>
uint16_t seetime=100;
widget_pid_curve::widget_pid_curve(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::widget_pid_curve)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose,false);
    init();
    ui->text_set_see_time->setText(QString("监控总时长:%1(ms)").arg(seetime));
    setWindowTitle(tr("实时曲线"));
    // configure plot to have two right axes:
    ui->customPlot->yAxis->setTickLabels(false);
    connect(ui->customPlot->yAxis2, SIGNAL(rangeChanged(QCPRange)),  ui->customPlot->yAxis, SLOT(setRange(QCPRange))); // left axis only mirrors inner right axis
    ui->customPlot->yAxis2->setVisible(true);
    ui->customPlot->axisRect()->addAxis(QCPAxis::atRight);
    ui->customPlot->axisRect()->axis(QCPAxis::atRight, 0)->setPadding(30); // add some padding to have space for tags
    ui->customPlot->axisRect()->axis(QCPAxis::atRight, 1)->setPadding(30); // add some padding to have space for tags
    // create graphs:
    mGraph1 =ui->customPlot->addGraph(ui->customPlot->xAxis, ui->customPlot->axisRect()->axis(QCPAxis::atRight, 0));
    mGraph2 = ui->customPlot->addGraph(ui->customPlot->xAxis, ui->customPlot->axisRect()->axis(QCPAxis::atRight, 1));
    mGraph1->setPen(QPen(QColor(250, 120, 0)));
    mGraph2->setPen(QPen(QColor(0, 180, 60)));
    // create tags with newly introduced AxisTag class (see axistag.h/.cpp):
    mTag1 = new AxisTag(mGraph1->valueAxis());
    mTag1->setPen(mGraph1->pen());
    mTag2 = new AxisTag(mGraph2->valueAxis());
    mTag2->setPen(mGraph2->pen());
    connect(&mDataTimer, SIGNAL(timeout()), this, SLOT(timerSlot()));
    mDataTimer.start(10);

}

widget_pid_curve::~widget_pid_curve()
{
    delete ui;
}

void widget_pid_curve::init(){
    curve_list.append("速度曲线");
    curve_list.append("机器方向角");
    curve_list.append("IMU_q0");
    curve_list.append("沿边距离");
    curve_list.append("左轮码盘值");
    curve_list.append("左轮码设定值");
    curve_list.append("右轮码盘值");
    curve_list.append("右轮码盘设定值");
    curve_list.append("沿边adc");
    curve_list.append("当前W");
    curve_list.append("沿边参考值");
    ui->comboBox_line1->addItems(curve_list);
    ui->comboBox_line2->addItems(curve_list);
}
double widget_pid_curve::line_find_data(QString key)
{
    if(key=="速度曲线"){
         return   MainWindow::MW->getMy_paneldata().v;

    }
    if(key=="机器方向角"){
        return   MainWindow::MW->getMy_paneldata().pid_nowpose.theta;
    }
    if(key=="IMU_q0"){
        return   MainWindow::MW->getMy_paneldata().q0;
    }
   if(key=="沿边距离"){
       return   MainWindow::MW->getMy_paneldata().borderDist;
   }
   if(key=="左轮码盘值"){
       return   MainWindow::MW->getMy_paneldata().lWheelCnt;
   }
   if(key=="右轮码盘值"){
       return   MainWindow::MW->getMy_paneldata().rWheelCnt;
   }
   if(key=="左轮码设定值"){

       return   (int16_t)MainWindow::MW->getMy_panel_data_part2().left_wheel_setSpeed; }
   if(key=="右轮码盘设定值"){
        return   (int16_t)MainWindow::MW->getMy_panel_data_part2().right_wheel_setSpeed;
   }
   if(key=="沿边adc"){
     return (uint16_t)MainWindow::MW->getMy_panel_data_part2().front_land_D10_LM;
   }
   if(key=="当前W"){
        return (int16_t)MainWindow::MW->getMy_paneldata().w;
   }
   if(key=="沿边参考值"){
        return (int16_t)MainWindow::MW->getMy_panel_data_part2().along_refrence_dis;
   }
   return 0;
}


void widget_pid_curve::timerSlot()
{
    // calculate and add a new data point to each graph:
    mGraph1->addData(mGraph1->dataCount(),line_find_data(ui->comboBox_line1->currentText()));//曲线3
    mGraph2->addData(mGraph2->dataCount(),line_find_data(ui->comboBox_line2->currentText()));//曲线4
    // make key axis range scroll with the data:
    ui->customPlot->xAxis->rescale();
    mGraph1->rescaleValueAxis(true, true);
    mGraph2->rescaleValueAxis(true, true);
    ui->customPlot->xAxis->setRange(ui->customPlot->xAxis->range().upper,seetime, Qt::AlignRight);
    // update the vertical axis tag positions and texts to match the rightmost data point of the graphs:
    double graph1Value = mGraph1->dataMainValue(mGraph1->dataCount()-1);
    double graph2Value = mGraph2->dataMainValue(mGraph2->dataCount()-1);
    mTag1->updatePosition(graph1Value);
    mTag2->updatePosition(graph2Value);
    mTag1->setText(QString::number(graph1Value, 'f', 2));
    mTag2->setText(QString::number(graph2Value, 'f', 2));
    ui->customPlot->replot();
}

void widget_pid_curve::on_set_see_time_returnPressed()
{
  if(ui->set_see_time->text().toInt()>5){
     seetime=ui->set_see_time->text().toInt();
     ui->text_set_see_time->setText(QString("监控总时长:%1(ms)").arg(seetime));
  }
}

void widget_pid_curve::on_comboBox_line1_currentTextChanged(const QString &arg1)
{
   // mGraph1.clear();

}

void widget_pid_curve::on_comboBox_line2_currentIndexChanged(const QString &arg1)
{
   // mGraph2.clear();
}
