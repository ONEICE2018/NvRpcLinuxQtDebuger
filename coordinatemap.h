#ifndef COORDINATEMAP_H
#define COORDINATEMAP_H

#include <QWidget>
#include "QMoveEvent"
#include "QPainter"
#include "mainwindow.h"
namespace Ui {
class CoordinateMap;
}

class CoordinateMap : public QWidget
{
    Q_OBJECT

public:
    explicit CoordinateMap(QWidget *parent = nullptr);
    ~CoordinateMap();
protected:
    void mouseMoveEvent(QMouseEvent* event);
    bool eventFilter(QObject *target, QEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
private:
    Ui::CoordinateMap *ui;
    void refrash_ui(void);
    int getdraw_one_x(int x);
    int getdraw_one_y(int y);
    int getreal_x(int drawx);
    int getreal_y(int drawy);
    bool draw_target{false};
    int  Range_xy=400;
    void draw_texts();
    bool mouse_pressed{false};


};

#endif // COORDINATEMAP_H
