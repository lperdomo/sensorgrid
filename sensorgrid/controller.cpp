#include "controller.h"
#include "bot.h"

#include <iostream>

Controller::Controller(double width, double height, double cellSize, double cellScale) :
    QObject()
{
    grid = new OccupancyGrid(width, height);
    scene = new SceneGrid((width/2)*-1, (height/2)*-1, width, height, cellSize, cellScale, grid);
    view = new QGraphicsView(scene);
    thread = new QThread();
}

Controller::~Controller()
{
    delete bot;
}

void Controller::setBot(Bot *bot)
{
    this->bot = bot;
    this->connect(this->bot, SIGNAL(moving()), this, SLOT(updateGrid()));
    this->bot->connect(thread, SIGNAL(started()), this->bot, SLOT(doTeleOp()));
    //this->bot->connect(thread, SIGNAL(started()), this->bot, SLOT(doWallFollowing()));
    this->bot->moveToThread(thread);
}

void Controller::runBot()
{
    //scene->drawGrid();
    thread->start();
}

void Controller::updateGrid()
{
    //this->updateBotCell();
    this->mappingDeadReck();
    scene->drawGrid();
    this->drawBotView();
}

void Controller::updateBotCell()
{
    double scale = scene->getCellScale();
    int x = round(bot->getX()/scale), y = round(bot->getY()/scale);
    std::cout << "x" << x << " y" << y << std::endl;
    //x = round(x/size);
    //y = round(y/size);
    grid->assign(x, y, 1);
    scene->drawBotRect(x, y*-1);
}

void Controller::mappingDeadReck()
{
    //bot->getClosestSonarNumber();
    //bot->getClosestSonarRange()
    double limitx = (grid->getWidth()/2);
    double limity = (grid->getHeight()/2);
    double beginx = limitx*-1;
    double beginy = limity*-1;
    double botx = round(bot->getX()/scene->getCellScale());
    double boty = round(bot->getY()/scene->getCellScale());
    for (double x = beginx; x < limitx; x++) {
        for (double y = beginy; y < limity; y++) {
            //int angle = bot->getTh()+round(atan2((limity - y) - (limity - boty), (x - limitx) - (botx - limitx))*180/M_PI);
            double angle = round(atan2((limity - y) - (limity - boty), (x - limitx) - (botx - limitx))*180/M_PI);
            double distance = sqrt(pow((x - limitx) - (botx - limitx), 2) + pow((limity - y) - (limity - boty), 2));
            //int angle = bot->getTh()+round(atan2((limity - y)*5 - boty, (x - limitx)*5 - botx)*180/M_PI);
            //double distance = sqrt(pow((x - limitx)*5 - botx, 2) + pow((limity - y)*5 - boty, 2));
            //if (angle < -105) angle
            /*if (angle > 105) {
                angle %= 105;
            }*/
            /*if (angle > 269) {
                angle -= bot->getTh();
            } else {
                angle += bot->getTh();
            }*/
            if (x == botx && y == boty) {
                grid->assign(x, y, 1);
            } else if (distance <= 25) {
                std::cout << "bot->getTh()" << bot->getTh() << " angle" << angle << std::endl;
                //double temp = bot->getTh()-angle;
                if (angle <= 105 && angle >= 75) {
                    grid->assignRotate(x, y, angle, botx, boty, 2);
                    //grid->assign(x, y , 2);//90
                } else if (angle <= 65 && angle >= 35) {
                    grid->assignRotate(x, y, angle, botx, boty, 3);
                    //grid->assign(x, y , 3);//50
                } else if (angle <= 45 && angle >= 15) {
                    grid->assignRotate(x, y, angle, botx, boty, 4);
                    //grid->assign(x, y , 4);//30
                } else if (angle <= 25 && angle >= -5) {
                    grid->assignRotate(x, y, angle, botx, boty, 5);
                    //grid->assign(x, y , 5);//10
                } else if (angle <= 5 && angle >= -25) {
                    grid->assignRotate(x, y, angle, botx, boty, 6);
                    //grid->assign(x, y , 6);//-10
                } else if (angle <= -15 && angle >= -45) {
                    grid->assignRotate(x, y, angle, botx, boty, 7);
                    //grid->assign(x, y , 7);//-30
                } else if (angle <= -35 && angle >= -65) {
                    grid->assignRotate(x, y, angle, botx, boty, 8);
                    //grid->assign(x, y , 8);//-50
                } else if (angle <= -75 && angle >= -105) {
                    grid->assignRotate(x, y, angle, botx, boty, 9);
                    //grid->assign(x, y , 9);//-90
                } else {
                    grid->assignRotate(x, y, angle, botx, boty, 0);
                    //grid->assign(x, y , 0);
                }
            } else {
                grid->assignRotate(x, y, angle, botx, boty, 0);
                //grid->assign(x, y , 0);
            }
            //std::cout << distance << std::endl;
            //std::cout << angle << std::endl;
            /*std::cout << "lslslslslsl" << std::endl;
            int angle = round(atan2(((float)grid.getWidth()/2 - y)*scene.getCellSize()-bot->getY(),
                                       ((float)x - grid.getHeight()/2)*scene.getCellSize()-bot->getX())
                                 *180/M_PI)+90-bot->getTh();
            std::cout << "ppppppppppppppp" << std::endl;
            double distance = sqrt(pow((x - grid.getWidth()/2)*scene.getCellSize()-bot->getX(), 2)
                                   +pow((grid.getHeight()/2 - y)*scene.getCellSize()-bot->getY(), 2));
            std::cout << "eoeoeoe" << std::endl;
            if (angle < 0) angle += 360;
            if (angle > 360) angle %= 360;
            if (angle > 0 && angle <= 180) {
                double reading = (double)bot->getLaserRange(180-angle);
                if (reading+100 < distance) {
                    std::cout << "zxczxczx x" << x << " y" << y << std::endl;
                    grid.assign(x, y, 0);
                } else if (reading-100 > distance) {
                    std::cout << "rtyrtyryrt x" << x << " y" << y << std::endl;
                    grid.assign(x, y, 3);
                }
            } else {
                std::cout << "ogfkdogkof x" << x << " y" << y << std::endl;
                grid.assign(x, y, 0);
            }*/
        }
    }
}

void Controller::drawBotView()
{
    if (!view->isVisible()) {
        view->show();
    }
    view->viewport()->update();
}
