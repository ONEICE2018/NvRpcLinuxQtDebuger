#ifndef COORDINATEMAP_H
#define COORDINATEMAP_H

#include <QMutex>
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


    int getRange_xy() const;
    void setRange_xy(int value);
    QList<rpc_pose_t> getHisory_xys() const;
    void clear_hisory_xys(void);
    void append_hisory_xys(rpc_pose_t his);

    void clear_target_xys(void);
    void append_target_xys(rpc_pose_t tgs);

    void setRefrence_pose(const rpc_pose_t &value);

    void setOrg_is_bot(bool value);
    void set_Org_back_bot(void);

    QList<rpc_pose_t> getTargets_xys() const;

    bool getQ_pressed() const;
    void setQ_pressed(bool value);

signals:
    void my_Range_xy_canged();
    void tagrget_changed();

private slots:
    void timer_mover();
protected:
    void mouseMoveEvent(QMouseEvent* event);
    bool eventFilter(QObject *target, QEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);



private:
    Ui::CoordinateMap *ui;
    void refrash_ui(void);
    int getdraw_one_x(int x);
    int getdraw_one_y(int y);
    int getreal_x(int drawx);
    int getreal_y(int drawy);
    void move_view(void);
    bool draw_target{false};
    int  Range_xy=4000;
    void draw_texts();
    bool mouse_pressed{false};
    bool Q_pressed{false};
    bool left_click_to_move{false};

    QMutex mutex_hittory_xys;
    QList<rpc_pose_t> hisory_xys;// you ge keng
    QMutex mutex_targets_xys;
    QList<rpc_pose_t> targets_xys;
    bool org_is_bot{true};
    rpc_pose_t refrence_pose;


    int move_p_x{1};
    int move_p_y{1};


};

#endif // COORDINATEMAP_H
