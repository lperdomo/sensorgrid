#ifndef OCCUPANCYGRID_H
#define OCCUPANCYGRID_H

#include <QObject>
#include <math.h>

class OccupancyGridCell : public QObject
{
    Q_OBJECT
public:
    explicit OccupancyGridCell(double value);
    void setValue(double value);
    double getValue();
private:
    double value;
signals:

public slots:

};

class OccupancyGrid : public QObject
{
    Q_OBJECT
public:
    explicit OccupancyGrid(double width, double height);
    ~OccupancyGrid();
    OccupancyGridCell *at(double x, double y);
    void assign(double x, double y, double value);
    void assignRotate(double x, double y, double angle, double pivotx, double pivoty, double value);
    double getWidth();
    double getHeight();
private:
    std::vector<std::vector<OccupancyGridCell*> > matrix;
    double width;
    double height;
signals:

public slots:

};

#endif // OCCUPANCYGRID_H
