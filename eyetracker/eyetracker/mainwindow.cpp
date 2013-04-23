#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>

#include <opencv2/opencv.hpp>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(&timer, SIGNAL(timeout()), this, SLOT(timeout()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

// render loop
void MainWindow::timeout()
{
    Mat frame = videoHandler.getFrame();
    Point2f pos = imageProcessor.process(frame);
    ui->videoCanvas->setPixmap(QPixmap::fromImage(videoHandler.convert(imageProcessor.getPupil())).scaled(320, 240));
}

// ----------------------------------------------------------------------------
// general slots
void MainWindow::exitClicked()
{
    qDebug("exit");
    QApplication::quit();
}

void MainWindow::aboutClicked()
{
    qDebug("about");
}

// ----------------------------------------------------------------------------
// record slots
void MainWindow::startToggled(bool state)
{
    if (state)
    {
        ui->calibrateButton->setEnabled(true);

        // switching the video ON
        qDebug("video: ON");
        videoHandler.start(1);
        timer.start(100);
        ui->videoCanvas->setHidden(false);
    }
    else
    {
        if (ui->calibrateButton->isChecked())
        {
            ui->calibrateButton->click();
        }
        ui->calibrateButton->setEnabled(false);

        // switching the video OFF
        qDebug("video: OFF");
        ui->videoCanvas->setHidden(true);
        timer.stop();
        videoHandler.stop();
    }

    return;
}

void MainWindow::calibrateToggled(bool state)
{
    if (state)
    {
        ui->recordButton->setEnabled(true);

        qDebug("calibrate: ON");
    }
    else
    {
        if (ui->recordButton->isChecked())
        {
            ui->recordButton->click();
        }
        ui->recordButton->setEnabled(false);

        qDebug("calibrate: OFF");
    }

    return;
}

void MainWindow::recordToggled(bool state)
{
    if (state)
    {
        qDebug("record: ON");
    }
    else
    {
        qDebug("record: OFF");
    }

    return;
}

// ----------------------------------------------------------------------------
void MainWindow::replayClicked()
{
    qDebug("replay");

    return;
}

void MainWindow::heatMapClicked()
{
    qDebug("heat map");

    return;
}