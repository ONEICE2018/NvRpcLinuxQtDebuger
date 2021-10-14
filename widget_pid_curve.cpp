#include "widget_pid_curve.h"
#include "ui_widget_pid_curve.h"
#include <mainwindow.h>
uint16_t seetime=100;
widget_pid_curve::widget_pid_curve(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::widget_pid_curve)
{
    ui->setupUi(this);
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
    mDataTimer.start(20);

}

widget_pid_curve::~widget_pid_curve()
{
    delete ui;
}

void widget_pid_curve::timerSlot()
{
    // calculate and add a new data point to each graph:
    mGraph1->addData(mGraph1->dataCount(),MainWindow::MW->getMy_paneldata().borderDist);//曲线3
    mGraph2->addData(mGraph2->dataCount(), 55);//曲线4
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
