#include "calibrationwindow.h"
#include "ui_calibrationwindow.h"

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

void CalibrationWindow::setCalibrator(Calibrator *p_calibrator)
{
    calibrator = p_calibrator;
}
