#include "occupancygrid.h"
#include <iostream>

OccupancyGrid::OccupancyGrid(double width, double height, double cellSize, double cellScale) :
    matrix(width+1, std::vector<OccupancyGridCell*>(height+1, NULL)),
    QObject()
{
    this->width = width;
    this->height = height;
    this->cellSize = cellSize;
    this->cellScale = cellScale;
}

OccupancyGrid::~OccupancyGrid()
{
}

double OccupancyGrid::getWidth()
{
    return width;
}

double OccupancyGrid::getHeight()
{
    return height;
}

double OccupancyGrid::getCellSize()
{
    return this->cellSize;
}

double OccupancyGrid::getCellScale()
{
    return this->cellScale;
}

OccupancyGridCell *OccupancyGrid::at(double x, double y)
{
    x = x + (width/2);
    y = (height/2) - y;
    return matrix[x][y];
}

void OccupancyGrid::assign(double x, double y, double value, int closestSensor)
{
    x = x + (width/2);
    y = (height/2) - y;
    if (!matrix[x][y]) {
        matrix[x][y] = new OccupancyGridCell(value, closestSensor);
    } else {
        matrix[x][y]->setValue(value);
        matrix[x][y]->setClosestSensor(closestSensor);
    }
}

bool OccupancyGrid::isAngleAtRange(double x, double y, double value, double angle1, double angle2, double range)
{
    x = x + (width/2);
    y = (height/2) - y;
    int difference = angle1 - angle2;
    int clockwise = abs(difference % 360),
        counterclockwise = 360 - abs(difference % 360),
        closest = 0;
    if (clockwise <= range || counterclockwise <= range) {
        if (clockwise < counterclockwise) {
            closest = clockwise;
        } else {
            closest = counterclockwise;
        }
        if (!matrix[x][y]) {
            matrix[x][y] = new OccupancyGridCell(value, closest);
            return true;
        } else {
            if (matrix[x][y]->getClosestSensor() > closest) {
                matrix[x][y]->setValue(value);
                matrix[x][y]->setClosestSensor(closest);
                return true;
            }
        }
    }
    return false;
}

OccupancyGridCell::OccupancyGridCell(double value, int closestSensor) :
    QObject()
{
    this->value = value;
    this->closestSensor = closestSensor;
}

void OccupancyGridCell::setValue(double value)
{
    this->value = value;
}

double OccupancyGridCell::getValue()
{
    return value;
}

void OccupancyGridCell::setClosestSensor(int closestSensor)
{
    this->closestSensor = closestSensor;
}

int OccupancyGridCell::getClosestSensor()
{
    return closestSensor;
}
