#ifndef OCCUPANCYGRID_H
#define OCCUPANCYGRID_H

#include <QObject>
#include <math.h>

class OccupancyGridCell : public QObject
{
    Q_OBJECT
public:
    explicit OccupancyGridCell(double value, int closestSensor);
    void setValue(double value);
    double getValue();
    void setClosestSensor(int closestSensor);
    int getClosestSensor();
private:
    double value;
    int closestSensor;
signals:

public slots:

};

class OccupancyGrid : public QObject
{
    Q_OBJECT
public:
    explicit OccupancyGrid(double width, double height, double cellSize, double cellScale);
    ~OccupancyGrid();
    double getWidth();
    double getHeight();
    double getCellSize();
    double getCellScale();
    OccupancyGridCell *at(double x, double y);
    void assign(double x, double y, double value, int closestSensorId);
    bool isAngleAtRange(double x, double y, double value, double angle1, double angle2, double range);
private:
    std::vector<std::vector<OccupancyGridCell*> > matrix;
    double width;
    double height;
    double cellSize;
    double cellScale;
signals:

public slots:

};

#endif // OCCUPANCYGRID_H
