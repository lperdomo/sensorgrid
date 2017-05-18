#ifndef GRAPHICGRID_H
#define GRAPHICGRID_H

#include <QGraphicsItem>
#include <QPainter>
#include "occupancygrid.h"

class GraphicGrid : public QGraphicsItem
{
public:
    GraphicGrid(OccupancyGrid *grid);
    ~GraphicGrid();
    QRectF boundingRect() const;
protected:
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = 0);
private:
    OccupancyGrid *grid;
};

#endif
