#include "player.h"
#include "mainwindow.h"
#include "helper.h"

#include <QObject>
#include <QApplication>
#include <QDesktopWidget>

Player::Player(QWidget *parent) : QObject(parent)
{
    connect(&processTimer, SIGNAL(timeout()), this, SLOT(process()));
}

Player::~Player()
{
}

void Player::setMainWindow(MainWindow *p_mainWindow)
{
    mainWindow = p_mainWindow;
}

void Player::startPlayback(const Session &p_session)
{
    session = p_session;

    QRect screen = QApplication::desktop()->screenGeometry(1);

    window = new PlayerWindow();
    window->setPlayer(this);
    window->move(screen.center());
    window->showFullScreen();

    background = Helper::qimage2mat(session.getImage().toImage());
    frame = 0;

    // start timer
    processTimer.start(MainWindow::PROCESS_TIMEOUT);
}

void Player::stopPlayback()
{
    processTimer.stop();

    if (window != NULL)
    {
        window->close();
        delete window;
        window = NULL;
    }
}

// render loop
void Player::process()
{
    qDebug("rendering");

    background.copyTo(image);

    Point p = session.getPoints().at(frame);

    Scalar pointColor(0, 0, 255);
    Scalar lineColor(0, 255, 0);

    // render current point
    circle(image, p, 2, pointColor, 3);
    circle(image, p, 10, pointColor, 1);
    circle(image, p, 15, pointColor, 1);
    circle(image, p, 20, pointColor, 1);

    // render trail
    if (frame > 4)
    {
        Point p1 = session.getPoints().at(frame-1);
        Point p2 = session.getPoints().at(frame-2);
        Point p3 = session.getPoints().at(frame-3);
        Point p4 = session.getPoints().at(frame-4);

        line(image, p, p1, lineColor, 2);
        line(image, p1, p2, lineColor, 2);
        line(image, p2, p3, lineColor, 2);
        line(image, p3, p4, lineColor, 2);

        circle(image, p1, 2, pointColor, 3);
        circle(image, p2, 2, pointColor, 3);
        circle(image, p3, 2, pointColor, 3);
        circle(image, p4, 2, pointColor, 3);
    }

    // reset frame
    if (frame == session.getPoints().size()-1)
        frame = 0;
    else
        frame++;

    // set image
    window->setFrameCount(frame+1, session.getPoints().size());
    window->setImage(Helper::mat2qimage(image));
}
