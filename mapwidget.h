#ifndef MAPWIDGET_H
#define MAPWIDGET_H

#include <QGraphicsView>
#include <QMouseEvent>

class MapWidget : public QGraphicsView
{
    Q_OBJECT
protected:
    void mouseMoveEvent (QMouseEvent *event);

};

#endif // MAPWIDGET_H

