#include "player.h"
#include "mainwindow.h"

#include <QApplication>
#include <QDesktopWidget>

Player::Player()
{
    connect(&processTimer, SIGNAL(process()), this, SLOT(process()));
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
}

void Player::stopPlayback()
{
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
}
