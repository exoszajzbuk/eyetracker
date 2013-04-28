#ifndef CALIBRATOR_H
#define CALIBRATOR_H

#include "calibrationwindow.h"

#include <QDesktopWidget>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

class Calibrator
{
public:
    Calibrator();

    enum CalibratorState
    {
        None,
        Calibrating,
        Calibrated
    };

    void startCalibrating();
    void dismissCalibration();
    CalibratorState getState();
    void setPosition(Point2f);

    // public interface
    void foundCalibrationPoint();

    // calculations
    Point calculatePosition(Point position, double* relativePercentX, double* relativePercentY);
    Mat& drawCalibrationPoly(Mat &);

private:
    CalibratorState state;
    CalibrationWindow* window;

    void showCalibrationPoint(int);

    QRect screen;

    Point2f position;
    int padding;
    int buttonSize;

    vector<Point> points;
    vector<Point> values;

    int index;
};

#endif // CALIBRATOR_H
