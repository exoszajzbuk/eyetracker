#include "calibrationwindow.h"
#include "ui_calibrationwindow.h"

#include <QDebug>

CalibrationWindow::CalibrationWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CalibrationWindow)
{
    ui->setupUi(this);
}

CalibrationWindow::~CalibrationWindow()
{
    delete ui;
}

void CalibrationWindow::targetClicked()
{
    qDebug("target clicked");
}
