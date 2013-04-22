#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>

#include <opencv2/opencv.hpp>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    videoHandler = new VideoHandler(1);

    connect(&timer, SIGNAL(timeout()), this, SLOT(timeout()));
    timer.start(33);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::timeout(){
    Mat frame = videoHandler->getFrame();

    ui->videoCanvas->setPixmap(QPixmap::fromImage(videoHandler->convert(frame)));
}
