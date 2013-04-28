#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>

#include <opencv2/opencv.hpp>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->displayGroup->setHidden(true);
    ui->pupilButton->setChecked(true);
    imageProcessor.setDisplayMode(ImageProcessor::Pupil);

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
    Mat image = imageProcessor.getDisplayImage();

    // do stuff on calibration
    switch (calibrator.getState())
    {
    // while calibrating
    case Calibrator::Calibrating:

        // add position info
        calibrator.setPosition(pos);

        break;

    case Calibrator::Calibrated:

        // render quadriliteral
        calibrator.setPosition(pos);
        image = calibrator.drawCalibration(image);

        break;

    default:
        break;
    }

    // set vide
    ui->videoCanvas->setPixmap(QPixmap::fromImage(videoHandler.convert(image)).scaled(320, 240));
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
        ui->displayGroup->setHidden(false);
        ui->pupilButton->setChecked(true);
        imageProcessor.setDisplayMode(ImageProcessor::Pupil);
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
        ui->displayGroup->setHidden(true);
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
        calibrator.startCalibrating();
    }
    else
    {
        if (ui->recordButton->isChecked())
        {
            ui->recordButton->click();
        }
        ui->recordButton->setEnabled(false);

        qDebug("calibrate: OFF");

        // disable calibrating
        calibrator.dismissCalibration();
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

// ----------------------------------------------------------------------------
void MainWindow::equalizedClicked()
{
    qDebug("equalized");
    imageProcessor.setDisplayMode(ImageProcessor::Equalized);
}

void MainWindow::thresholdedClicked()
{
    qDebug("thresholded");
    imageProcessor.setDisplayMode(ImageProcessor::Thresholded);
}

void MainWindow::blobsClicked()
{
    qDebug("blobs");
    imageProcessor.setDisplayMode(ImageProcessor::Blobs);
}

void MainWindow::pupilClicked()
{
    qDebug("pupil");
    imageProcessor.setDisplayMode(ImageProcessor::Pupil);
}
