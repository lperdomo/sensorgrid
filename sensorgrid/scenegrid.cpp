#include "scenegrid.h"
#include "keyboard.h"

#include <math.h>

#include <iostream>

SceneGridItem::SceneGridItem(OccupancyGrid *grid) :
    QGraphicsItem()
{
    this->grid = grid;
}

SceneGridItem::~SceneGridItem()
{
}

QRectF SceneGridItem::boundingRect() const
{
    return QRectF((grid->getWidth()/2)*-1, (grid->getHeight()/2)*-1, grid->getWidth()/2, grid->getHeight()/2);
}

void SceneGridItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    QRectF rect = boundingRect();
    double size = grid->getCellSize();
    for (double x = rect.left(); x < rect.width(); x++) {
        for (double y = rect.top(); y < rect.height(); y++) {
            if (grid->at(x, y)) {
                double value = grid->at(x, y)->getValue();
                if (value == 1) painter->setBrush(QBrush(Qt::red));
                else if (value == 2) painter->setBrush(QBrush(QColor(0, 255, 255, 127)));
                else if (value == 3) painter->setBrush(QBrush(QColor(255, 0, 255, 127)));
                else if (value == 4) painter->setBrush(QBrush(QColor(255, 255, 0, 127)));
                else if (value == 5) painter->setBrush(QBrush(QColor(255, 0, 0, 127)));
                else if (value == 6) painter->setBrush(QBrush(QColor(0, 255, 0, 127)));
                else if (value == 7) painter->setBrush(QBrush(QColor(0, 0, 255, 127)));
                else if (value == 8) painter->setBrush(QBrush(QColor(127, 127, 127, 127)));
                else if (value == 9) painter->setBrush(QBrush(QColor(127, 127, 0, 127)));
                else if (value == 0) painter->setBrush(QBrush(Qt::white));
                painter->drawRect(size*x, size*y*-1, size, size);
            }
        }
    }
}

SceneGrid::SceneGrid(qreal x, qreal y, qreal width, qreal height, OccupancyGrid *grid) :
    QGraphicsScene(x, y, width, height)
{
    gridItem = new SceneGridItem(grid);
    this->addItem(gridItem);
}

SceneGrid::~SceneGrid()
{
}

void SceneGrid::drawForeground(QPainter *painter, const QRectF &rect)
{
    double size = gridItem->grid->getCellSize();
    qreal x, y;
    qreal left = int(rect.left()) - (int(rect.left()) % (int(size)));
    qreal top = int(rect.top()) - (int(rect.top()) % (int(size)));

    QVarLengthArray<QLineF, 100> lines;

    painter->setPen(QPen(Qt::darkGray));
    for (x = left; x < rect.right(); x += size)
        lines.append(QLineF(x, rect.top(), x, rect.bottom()));
    for (y = top; y < rect.bottom(); y += size)
        lines.append(QLineF(rect.left(), y, rect.right(), y));

    painter->drawLines(lines.data(), lines.size());

    painter->setPen(QPen(Qt::black));
    painter->drawLine(QLineF(0, rect.top(), 0, rect.bottom()));
    painter->drawLine(QLineF(rect.left(), 0, rect.right(), 0));
}

void SceneGrid::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Up) {
        Keyboard::getInstance()->setArrowUp(true);
    } else if (event->key() == Qt::Key_Down) {
        Keyboard::getInstance()->setArrowDown(true);
    } else if (event->key() == Qt::Key_Left) {
        Keyboard::getInstance()->setArrowLeft(true);
    } else if (event->key() == Qt::Key_Right) {
        Keyboard::getInstance()->setArrowRight(true);
    }
}

void SceneGrid::keyReleaseEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Up) {
        Keyboard::getInstance()->setArrowUp(false);
    } else if (event->key() == Qt::Key_Down) {
        Keyboard::getInstance()->setArrowDown(false);
    } else if (event->key() == Qt::Key_Left) {
        Keyboard::getInstance()->setArrowLeft(false);
    } else if (event->key() == Qt::Key_Right) {
        Keyboard::getInstance()->setArrowRight(false);
    }
}
