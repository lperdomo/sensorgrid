#include "scenegrid.h"
#include "keyboard.h"

#include <iostream>


SceneGrid::SceneGrid(qreal x, qreal y, qreal width, qreal height, double cellSize, double cellScale, OccupancyGrid *grid) :
    QGraphicsScene(x, y, width, height)
{
    this->cellSize = cellSize;
    this->cellScale = cellScale;
    this->grid = grid;
    rectBot = NULL;
}

SceneGrid::~SceneGrid()
{
}

double SceneGrid::getCellSize()
{
    return this->cellSize;
}

double SceneGrid::getCellScale()
{
    return this->cellScale;
}

void SceneGrid::drawGrid()
{

}

void SceneGrid::drawForeground(QPainter *painter, const QRectF &rect)
{
    qreal x, y;
    qreal left = int(rect.left()) - (int(rect.left()) % (int(cellSize)));
    qreal top = int(rect.top()) - (int(rect.top()) % (int(cellSize)));

    for (double i = left; i < rect.right(); i++) {
        for (double j = top; j < rect.bottom(); j++) {
            if (grid->at(i, j)) {
                double value = grid->at(i, j)->getValue();
                if (value >= 2 && value <= 9) {
                    if (value == 2) painter->setPen(QPen(QColor(0, 255, 255, 127)));
                    else if (value == 3) painter->setPen(QPen(QColor(255, 0, 255, 127)));
                    else if (value == 4) painter->setPen(QPen(QColor(255, 255, 0, 127)));
                    else if (value == 5) painter->setPen(QPen(QColor(0, 255, 255, 127)));
                    else if (value == 6) painter->setPen(QPen(QColor(255, 0, 255, 127)));
                    else if (value == 7) painter->setPen(QPen(QColor(255, 255, 0, 127)));
                    else if (value == 8) painter->setPen(QPen(QColor(0, 255, 255, 127)));
                    else if (value == 9) painter->setPen(QPen(QColor(255, 0, 255, 127)));
                    QRect tmp(i, j, cellSize, cellSize);
                    painter->fillRect(tmp, painter->brush().style());
                    painter->drawRect(tmp);
                } else if (value == 1) {
                    QRect tmp(i, j, cellSize, cellSize);
                    painter->fillRect(tmp, QBrush(QColor(255, 0, 0, 255)));
                    painter->drawRect(tmp);
                } else if (value == 0) {
                    painter->eraseRect(i, j, cellSize, cellSize);
                }
            }
        }
    }

    QVarLengthArray<QLineF, 100> lines;

    painter->setPen(QPen(Qt::darkGray));
    for (x = left; x < rect.right(); x += cellSize)
        lines.append(QLineF(x, rect.top(), x, rect.bottom()));
    for (y = top; y < rect.bottom(); y += cellSize)
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

void SceneGrid::drawBotRect(qreal x, qreal y)
{
    x = cellSize*x;
    y = cellSize*y;
    if (!rectBot) {
        rectBot = new QGraphicsRectItem(x, y, cellSize, cellSize);
        rectBot->setBrush(QBrush(Qt::red));
    } else {
        this->removeItem(rectBot);
        rectBot->setPos(x, y);
    }
    this->addItem(rectBot);
}

void SceneGrid::drawTrailRect(qreal x, qreal y)
{
    QGraphicsRectItem *rect = new QGraphicsRectItem(x, y, cellSize*2, cellSize*2);
    rectBot->setParentItem(rect);
    rect->setBrush(QBrush(Qt::darkGray));
    this->addItem(rect);
}
