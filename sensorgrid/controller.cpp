#include "controller.h"
#include "bot.h"

#include <iostream>

Controller::Controller(double width, double height, double cellSize, double cellScale) :
    QObject()
{
    grid = new OccupancyGrid(width, height, cellSize, cellScale);
    scene = new SceneGrid((width/2)*-1, (height/2)*-1, width, height, grid);
    view = new QGraphicsView(scene);
    thread = new QThread();
}

Controller::~Controller()
{
    delete bot;
    delete view;
    delete scene;
    delete grid;
    delete thread;
}

void Controller::setBot(Bot *bot)
{
    this->bot = bot;
    this->connect(this->bot, SIGNAL(moving()), this, SLOT(update()));
    this->bot->connect(thread, SIGNAL(started()), this->bot, SLOT(doTeleOp()));
    //this->bot->connect(thread, SIGNAL(started()), this->bot, SLOT(doWallFollowing()));
    this->bot->moveToThread(thread);
}

void Controller::run()
{
    thread->start();
}

void Controller::update()
{
    this->mappingDeadReck();
    this->showView();
}

void Controller::mappingDeadReck()
{
    double limitx = (grid->getWidth()/2),
           limity = (grid->getHeight()/2),
           beginx = limitx*-1,
           beginy = limity*-1;
    double botx = round(bot->getX()/grid->getCellScale()),
           boty = round(bot->getY()/grid->getCellScale()),
           botth = bot->getTh()*-1;

    for (double x = beginx; x < limitx; x++) {
        for (double y = beginy; y < limity; y++) {
            double distance = sqrt(pow((x - limitx) - (botx - limitx), 2) + pow((limity - y) - (limity - boty), 2));
            double angle = round(atan2((limity - y) - (limity - boty), (x - limitx) - (botx - limitx))*180/M_PI);

            if (x == botx && y == boty) {
                grid->assign(x, y, 1, -1);
            } else if (distance <= 25) {
                //std::cout << "bot->getTh()" << bot->getTh() << std::endl;
                if (grid->isAngleAtRange(x, y, 2, botth+90, angle, 15)) {
                    //grid->assign(x, y, 2, 0);
                } else if (grid->isAngleAtRange(x, y, 3, botth+50, angle, 15)) {
                    //grid->assign(x, y, 3, 1);
                } else if (grid->isAngleAtRange(x, y, 4, botth+30, angle, 15)) {
                    //grid->assign(x, y, 4, 2);
                } else if (grid->isAngleAtRange(x, y, 5, botth+10, angle, 15)) {
                    //grid->assign(x, y, 5, 3);
                } else if (grid->isAngleAtRange(x, y, 6, botth-10, angle, 15)) {
                    //grid->assign(x, y, 6, 4);
                } else if (grid->isAngleAtRange(x, y, 7, botth-30, angle, 15)) {
                    //grid->assign(x, y, 7, 5);
                } else if (grid->isAngleAtRange(x, y, 8, botth-50, angle, 15)) {
                    //grid->assign(x, y, 8, 6);
                } else if (grid->isAngleAtRange(x, y, 9, botth-90, angle, 15)) {
                    //grid->assign(x, y, 9, 7);
                } else {
                    grid->assign(x, y, 0, -1);
                }
            } else {
                grid->assign(x, y, 0, -1);
            }
        }
    }
}

void Controller::showView()
{
    if (!view->isVisible()) {
        view->show();
    } else {
        view->viewport()->update();
    }
}
