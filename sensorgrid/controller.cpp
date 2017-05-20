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
    this->updateBotOnGrid();
    this->showView();
}

void Controller::updateBotOnGrid()
{
    double botx = round(bot->getX()/grid->getCellScale()),
           boty = round(bot->getY()/grid->getCellScale()),
           botth = bot->getTh()*-1;
    double limitx = (botx+125 > (grid->getWidth()/2) ? (grid->getWidth()/2)-botx : botx+125),
           limity = (botx+125 > (grid->getHeight()/2) ? (grid->getHeight()/2)-botx : botx+125),
           beginx = (limitx)*-1,
           beginy = (limity)*-1;

    for (double x = beginx; x < limitx; x++) {
        for (double y = beginy; y < limity; y++) {
            double distance = sqrt(pow((x - limitx) - (botx - limitx), 2) + pow((limity - y) - (limity - boty), 2));
            double angle = round(atan2((limity - y) - (limity - boty), (x - limitx) - (botx - limitx))*180/M_PI);

            if (x == botx && y == boty) {
                grid->assign(x, y, 1);
            } else if (distance <= 25) {
                //std::cout << "bot->getTh()" << bot->getTh() << std::endl;
                if (grid->isAngleAtRange(botth+90, angle, 15)) {
                    grid->assign(x, y, 2);
                } else if (grid->isAngleAtRange(botth+50, angle, 15) && !grid->isAngleAtRange(botth+30, angle, 10)) {
                    grid->assign(x, y, 3);
                } else if (grid->isAngleAtRange(botth+40, angle, 0.5)) {
                    grid->assign(x, y, 4);
                } else if (grid->isAngleAtRange(botth+30, angle, 15) && !grid->isAngleAtRange(botth+10, angle, 10)) {
                    grid->assign(x, y, 5);
                } else if (grid->isAngleAtRange(botth+20, angle, 0.5)) {
                    grid->assign(x, y, 6);
                } else if (grid->isAngleAtRange(botth+10, angle, 15) && !grid->isAngleAtRange(botth-10, angle, 10)) {
                    grid->assign(x, y, 7);
                } else if (grid->isAngleAtRange(botth, angle, 0.5)) {
                    grid->assign(x, y, 8);
                } else if (grid->isAngleAtRange(botth-10, angle, 15) && !grid->isAngleAtRange(botth-30, angle, 10)) {
                    grid->assign(x, y, 9);
                } else if (grid->isAngleAtRange(botth-20, angle, 0.5)) {
                    grid->assign(x, y, 10);
                } else if (grid->isAngleAtRange(botth-30, angle, 15) && !grid->isAngleAtRange(botth-50, angle, 10)) {
                    grid->assign(x, y, 11);
                } else if (grid->isAngleAtRange(botth-40, angle, 0.5)) {
                    grid->assign(x, y, 12);
                } else if (grid->isAngleAtRange(botth-50, angle, 15)) {
                    grid->assign(x, y, 13);
                } else if (grid->isAngleAtRange(botth-90, angle, 15)) {
                    grid->assign(x, y, 14);
                } else {
                    grid->assign(x, y, 0);
                }
            } else {
                grid->assign(x, y, 0);
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
