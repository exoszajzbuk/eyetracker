#include "calibrator.h"

#include <QApplication>
#include <QDesktopWidget>
#include <QDebug>

Calibrator::Calibrator()
{
    state = None;
}

void Calibrator::startCalibrating()
{
    qDebug("start calibrating");

    // create window
    calibrationWindow = new CalibrationWindow();
    //calibrationWindow->setCalibrator(&calibrator);
    QRect primary = QApplication::desktop()->screenGeometry(1);
    calibrationWindow->move(primary.center());
    calibrationWindow->showFullScreen();

    calibrationPoints.clear();

    // enable calibrating
    state = Calibrating;
}

void Calibrator::dismissCalibration()
{
    qDebug("dismiss calibration");
    state = None;
}

Calibrator::CalibratorState Calibrator::getState()
{
    return state;
}

void Calibrator::setPosition(Point2f p_position)
{
    position = p_position;
}

Point Calibrator::calculatePosition(Point p_position, int screen_width, int screen_height, int calibPadding, double* relativePercentX, double* relativePercentY)
{
    // save position
    setPosition(p_position);

    Point a = calibrationPoints[0];
    Point b = calibrationPoints[1];
    Point c = calibrationPoints[2];
    Point d = calibrationPoints[3];

    Point p = position;

    double C = (double)(a.y - p.y) * (d.x - p.x) - (double)(a.x - p.x) * (d.y - p.y);
    double B = (double)(a.y - p.y) * (c.x - d.x) + (double)(b.y - a.y) * (d.x - p.x) - (double)(a.x - p.x) * (c.y - d.y) - (double)(b.x - a.x) * (d.y - p.y);
    double A = (double)(b.y - a.y) * (c.x - d.x) - (double)(b.x - a.x) * (c.y - d.y);

    double D = B * B - 4 * A * C;

    double u = (-B - sqrt(D)) / (2 * A);

    double p1x = a.x + (b.x - a.x) * u;
    double p2x = d.x + (c.x - d.x) * u;

    double px = p.x;

    double v = (px - p1x) / (p2x - p1x);

    // in display bounds
    /*
    if (u > 1)
        u = 1;
    else if (u < 0)
        u = 0;

    if (v > 1)
        v = 1;
    else if (v < 0)
        v = 0;
    */

    Point ret;
    // calculate screen coordinates
    ret.x = u*(screen_width-2*calibPadding)+calibPadding;
    ret.y = v*(screen_height-2*calibPadding)+calibPadding;

    // save relative coordinates
    *relativePercentX = u;
    *relativePercentY = v;

    return ret;
}

Mat& Calibrator::drawCalibration(Mat &frame)
{
    return frame;
}


