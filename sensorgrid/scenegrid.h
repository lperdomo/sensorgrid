#ifndef SCENEGRID_H
#define SCENEGRID_H

#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QPainter>
#include <QKeyEvent>

#include "occupancygrid.h"

class SceneGrid : public QGraphicsScene
{
public:
    SceneGrid(qreal x, qreal y, qreal width, qreal height, OccupancyGrid *grid, double cellSize, double cellScale);
    ~SceneGrid();
    void drawBotRect(qreal x, qreal y);
    void drawTrailRect(qreal x, qreal y);
    double getCellSize();
    double getCellScale();
private:
    double cellSize;
    double cellScale;
    OccupancyGrid *grid;
    QGraphicsRectItem *rectBot;
protected:
    void drawForeground(QPainter *painter, const QRectF &rect);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
};

#endif // SCENEGRID_H
