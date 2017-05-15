#include "controller.h"
#include "bot.h"

#include <iostream>

Controller::Controller(double width, double height, double cellSize, double cellScale) :
    grid(width, height),
    scene((width/2)*-1, (height/2)*-1, width, height, cellSize, cellScale),
    view(&scene),
    QObject()
{
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
    thread->start();
}

void Controller::updateGrid()
{
    double size = scene.getCellSize(), scale = scene.getCellScale();
    int x = round(bot->getX()/scale), y = round(bot->getY()/scale);
    std::cout << "x" << x << " = " << size*(round(x/size))
              << " y" << y << " = " << size*(round(y/size)) << std::endl;
    x = round(x/size);
    y = round(y/size);
    this->updateBotCell(x, y);
    //this->mappingDeadReck();
    this->drawBotView();
}

void Controller::updateBotCell(double x, double y)
{
    double size = scene.getCellSize();
    grid.assign(x, y, 1);
    scene.drawBotRect(size*x, size*y*-1);
}

void Controller::updateTrailCell(double x, double y)
{
    if (grid.at(x, y) != NULL) {
        double size = scene.getCellSize();
        grid.assign(x, y, 2);
        scene.drawTrailRect(size*x, size*y*-1);
    }
}

void Controller::mappingDeadReck()
{
    for (double x = 0; x < grid.getWidth(); x++) {
        for (double y = 0; y < grid.getHeight(); y++) {
            std::cout << "lslslslslsl" << std::endl;
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
            }
        }
    }
}

void Controller::drawBotView()
{
    if (!view.isVisible()) {
        view.show();
    }
    view.update();
}
