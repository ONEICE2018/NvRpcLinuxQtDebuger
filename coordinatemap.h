#ifndef COORDINATEMAP_H
#define COORDINATEMAP_H

#include <QWidget>

namespace Ui {
class CoordinateMap;
}

class CoordinateMap : public QWidget
{
    Q_OBJECT

public:
    explicit CoordinateMap(QWidget *parent = nullptr);
    ~CoordinateMap();

private:
    Ui::CoordinateMap *ui;
};

#endif // COORDINATEMAP_H
