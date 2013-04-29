#include "recordingwindow.h"
#include "ui_recordingwindow.h"

#include "recorder.h"
#include "mainwindow.h"

RecordingWindow::RecordingWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RecordingWindow)
{
    ui->setupUi(this);

    image = QPixmap::grabWindow(QApplication::desktop()->winId());
    ui->bgImage->setGeometry(QApplication::desktop()->screenGeometry(1));
    ui->bgImage->setPixmap(image);
}

RecordingWindow::~RecordingWindow()
{
    delete ui;
}

void RecordingWindow::setRecorder(Recorder* p_recorder)
{
    recorder = p_recorder;
}

void RecordingWindow::reject()
{
    recorder->getMainWindow()->toggleRecord(false);
}

QPixmap RecordingWindow::getImage()
{
    return image;
}
