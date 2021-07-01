#include "coordinatemap.h"
#include "ui_coordinatemap.h"

CoordinateMap::CoordinateMap(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CoordinateMap)
{
    ui->setupUi(this);
}

CoordinateMap::~CoordinateMap()
{
    delete ui;
}
