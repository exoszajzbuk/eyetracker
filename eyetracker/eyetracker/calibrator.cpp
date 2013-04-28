#include "calibrator.h"

#include <QApplication>
#include <QDesktopWidget>
#include <QDebug>

Calibrator::Calibrator()
{
    state = None;
    padding = 50;
    buttonSize = 40;

    // get primary screen
    screen = QApplication::desktop()->screenGeometry(1);

    // calibration points
    points.clear();
    int deltaNeg = padding-buttonSize/2;
    int deltaPos = padding+buttonSize/2;
    points.push_back(Point(deltaNeg, deltaNeg));
    points.push_back(Point(screen.width()-deltaPos, deltaNeg));
    points.push_back(Point(screen.width()-deltaPos, screen.height()-deltaPos));
    points.push_back(Point(deltaNeg, screen.height()-deltaPos));
}

void Calibrator::startCalibrating()
{
    qDebug("start calibrating");

    // create window
    window = new CalibrationWindow();
    window->setCalibrator(this);
    window->move(screen.center());

    // clear calibration points
    values.clear();
    index = 0;

    // show next point
    showCalibrationPoint(index);

    // show fullscreen window
    window->showFullScreen();

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

Point Calibrator::calculatePosition(Point p_position, double* relativePercentX, double* relativePercentY)
{
    // save position
    setPosition(p_position);

    // values
    Point a = values[0];
    Point b = values[1];
    Point c = values[2];
    Point d = values[3];
    Point p = position;
    int screen_width = screen.width();
    int screen_height = screen.height();
    int calibPadding = padding;

    // calculate
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

Mat& Calibrator::drawCalibrationPoly(Mat &frame)
{
    qDebug("draw poly");
    return frame;
}

void Calibrator::foundCalibrationPoint()
{
    qDebug("found point");
    if (index < points.size()-1)
    {
        index++;
        showCalibrationPoint(index);
    }
    else
    {
        qDebug("calibration finished");
        window->close();
        delete window;

        // set state
        state = Calibrated;
    }
}

void Calibrator::showCalibrationPoint(int p_index)
{
    window->setTargetPosition(points[p_index]);
}
