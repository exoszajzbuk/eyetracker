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
    Point calculatePosition(Point position, int screen_width, int screen_height, int calibPadding, double* relativePercentX, double* relativePercentY);
    Mat& drawCalibration(Mat &);

private:
    CalibratorState state;
    CalibrationWindow* calibrationWindow;
    Point2f position;
    vector<Point> calibrationPoints;
};

#endif // CALIBRATOR_H
