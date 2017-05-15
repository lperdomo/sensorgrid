#include "occupancygrid.h"
#include <iostream>

OccupancyGrid::OccupancyGrid(double width, double height) :
    matrix(width, std::vector<OccupancyGridCell*>(height, NULL)),
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
    return matrix[x][y];
}

void OccupancyGrid::assign(double x, double y, double value)
{
    std::cout << " assign x" << x << " y" << y << std::endl;
    if (!matrix[x][y]) {
        matrix[x][y] = new OccupancyGridCell(value);
    } else {
        matrix[x][y]->setValue(value);
    }
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
