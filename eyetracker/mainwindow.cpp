#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "sessionitemwidget.h"

#include <QDebug>

#include <opencv2/opencv.hpp>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // custom states
    ui->displayGroup->setHidden(true);
    ui->pupilButton->setChecked(true);
    imageProcessor.setDisplayMode(ImageProcessor::Pupil);

    // links
    calibrator.setMainWindow(this);
    recorder.setMainWindow(this);

    // local status widget
    localStatus = new QLabel(this);
    localStatus->setHidden(true);
    ui->statusBar->addPermanentWidget(localStatus);

    // screen status widget
    screenStatus = new QLabel(this);
    screenStatus->setHidden(true);
    ui->statusBar->addPermanentWidget(screenStatus);

    // refresh sessionlist
    refreshSessionList();

    // timer
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
    Point2f localPos = imageProcessor.process(frame);
    Mat image = imageProcessor.getDisplayImage();

    // screen positions
    Point2f screenPos;
    double relativeX;
    double relativeY;

    // draw status
    localStatus->setText("Local - "+calibrator.getPositionString(localPos, localPos));

    // ------------------
    // CALIBRATION STATES
    switch (calibrator.getState())
    {
    // while calibrating
    case Calibrator::Calibrating:

        // add position info
        calibrator.setPosition(localPos);

        break;

    case Calibrator::Calibrated:

        // render quadriliteral
        image = calibrator.drawCalibrationPoly(image);

        // calculate position
        screenPos = calibrator.calculatePosition(localPos, &relativeX, &relativeY);

        // status
        screenStatus->setHidden(false);
        screenStatus->setText("Screen - "+calibrator.getPositionString(localPos, screenPos));

        break;

    default:
        screenStatus->setHidden(true);
        break;
    }

    // ----------------
    // RECORDING STATES
    switch (recorder.getState())
    {
    // while recording
    case Recorder::Recording:
        recorder.recordPoint(screenPos);
        break;

    default:
        // do nothing
        break;
    }

    // set video
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
        localStatus->setHidden(false);
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
        localStatus->setHidden(true);
    }

    return;
}

void MainWindow::calibrateToggled(bool state)
{
    // re-set state
    //ui->calibrateButton->setChecked(state);

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
    // re-set state
    //ui->recordButton->setChecked(state);

    if (state)
    {
        qDebug("record: ON");

        if (QApplication::desktop()->screenCount() == 1)
        {
            showMinimized();
            QTimer::singleShot(1000, this, SLOT(startRecording()));
        }
        else
        {
            // start immediately
            startRecording();
        }
    }
    else
    {
        qDebug("record: OFF");

        if (QApplication::desktop()->screenCount() == 1)
        {
            setWindowState( (windowState() & ~Qt::WindowMinimized) | Qt::WindowActive);
            activateWindow();
        }

        recorder.stopRecording();
        refreshSessionList();
    }

    return;
}

void MainWindow::startRecording()
{
    recorder.startRecording();
}

void MainWindow::toggleCalibrate(bool state)
{
    ui->calibrateButton->setChecked(state);
}

void MainWindow::toggleRecord(bool state)
{
    ui->recordButton->setChecked(state);
}

// ----------------------------------------------------------------------------
void MainWindow::listItemSelected()
{
    ui->playButton->setEnabled(true);
    ui->heatMapButton->setEnabled(true);
}

void MainWindow::playClicked()
{
    qDebug("play");

    QModelIndexList indexes = ui->listWidget->selectionModel()->selectedIndexes();
    cout << indexes.at(0).row() << endl;

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

// ----------------------------------------------------------------------------
void MainWindow::refreshSessionList()
{
    // initialize sessions
    findSessions();

    // clear list
    ui->listWidget->clear();

    // populate
    for (unsigned int i=0; i<sessions.size(); ++i)
    {
        Session session = sessions.at(i);

        SessionItemWidget *sessionItem = new SessionItemWidget(session);
        QListWidgetItem *listItem = new QListWidgetItem();
        listItem->setSizeHint(QSize(210,50));
        ui->listWidget->addItem(listItem);
        ui->listWidget->setItemWidget(listItem, sessionItem);
    }
}

void MainWindow::findSessions()
{
    sessions.clear();

    vector<string> names = Session::getNames();

    for (unsigned int i=0; i<names.size(); ++i)
    {
        Session* session = new Session();
        session->load(names.at(i));
        sessions.push_back(*session);
    }
}
