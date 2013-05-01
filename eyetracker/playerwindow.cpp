#include "playerwindow.h"
#include "ui_playerwindow.h"

#include "player.h"

#include <QApplication>
#include <QDesktopWidget>

PlayerWindow::PlayerWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PlayerWindow)
{
    ui->setupUi(this);

    // fullscreen bg
    ui->bgImage->setGeometry(QApplication::desktop()->screenGeometry(1));
}

PlayerWindow::~PlayerWindow()
{
    delete ui;
}

void PlayerWindow::setPlayer(Player *p_player)
{
    player = p_player;
}

void PlayerWindow::setFrameCount(int num, int sum)
{
    stringstream s;
    s << num << " / " << sum;
    ui->frameCount->setText(QString(s.str().c_str()));
}

void PlayerWindow::setImage(const QImage& image)
{
    ui->bgImage->setPixmap(QPixmap::fromImage(image));
}

void PlayerWindow::reject()
{
    player->stopPlayback();
}
