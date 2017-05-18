#include "graphicgrid.h"

#include <iostream>

GraphicGrid::GraphicGrid(OccupancyGrid *grid) :
    QGraphicsItem()
{
    this->grid = grid;
}

GraphicGrid::~GraphicGrid()
{
}

QRectF GraphicGrid::boundingRect() const
{
    return QRectF(-250, -250, 250, 250);
}

void GraphicGrid::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    for (double i = -250; i < 250; i++) {
        for (double j = -250; j < 250; j++) {
            if (grid->at(i, j)) {
                double value = grid->at(i, j)->getValue();
                if (value == 1) painter->setBrush(QBrush(Qt::red));
                else if (value == 2) painter->setBrush(QBrush(QColor(0, 255, 255, 127)));
                else if (value == 3) painter->setBrush(QBrush(QColor(255, 0, 255, 127)));
                else if (value == 4) painter->setBrush(QBrush(QColor(255, 255, 0, 127)));
                else if (value == 5) painter->setBrush(QBrush(QColor(0, 255, 255, 127)));
                else if (value == 6) painter->setBrush(QBrush(QColor(255, 0, 255, 127)));
                else if (value == 7) painter->setBrush(QBrush(QColor(255, 255, 0, 127)));
                else if (value == 8) painter->setBrush(QBrush(QColor(0, 255, 255, 127)));
                else if (value == 9) painter->setBrush(QBrush(QColor(255, 0, 255, 127)));
                else if (value == 0) painter->setBrush(QBrush(Qt::white));
                painter->drawRect(5*i, 5*j*-1, 5, 5);
            }
         }
     }
}
