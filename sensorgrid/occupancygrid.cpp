#include "occupancygrid.h"
#include <iostream>

OccupancyGrid::OccupancyGrid(double width, double height) :
    matrix(width+1, std::vector<OccupancyGridCell*>(height+1, NULL)),
    QObject()
{
    this->width = width;
    this->height = height;
}

OccupancyGrid::~OccupancyGrid()
{
}

OccupancyGridCell *OccupancyGrid::at(double x, double y)
{
    x = x + (width/2);
    y = (height/2) - y;
    return matrix[x][y];
}

void OccupancyGrid::assign(double x, double y, double value)
{
    x = x + (width/2);
    y = (height/2) - y;
    if (!matrix[x][y]) {
        matrix[x][y] = new OccupancyGridCell(value);
    } else {
        matrix[x][y]->setValue(value);
    }
}

void OccupancyGrid::assignRotate(double x, double y, double angle, double pivotx, double pivoty, double value)
{
    std::cout << "eeeeeeeeeeeeeeee x" << x << " y" << y << " angle" << angle << std::endl;
    int tx = cos(angle) * (x-pivotx) - sin(angle) * (y-pivoty) + pivotx;
    int ty = sin(angle) * (x-pivotx) + cos(angle) * (y-pivoty) + pivoty;
    std::cout << "tx" << tx << " ty" << ty << std::endl;
    assign(tx
           , ty, value);
    std::cout << "dddddddddddddddd" << std::endl;
}


double OccupancyGrid::getWidth()
{
    return width;
}

double OccupancyGrid::getHeight()
{
    return height;
}

OccupancyGridCell::OccupancyGridCell(double value) :
    QObject()
{
    this->value = value;
}

void OccupancyGridCell::setValue(double value)
{
    this->value = value;
}

double OccupancyGridCell::getValue()
{
    return value;
}
