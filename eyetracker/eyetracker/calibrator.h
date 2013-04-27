#ifndef CALIBRATOR_H
#define CALIBRATOR_H

#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

class Calibrator
{
public:
    Calibrator();

private:
    Point calculatePosition(Point a, Point b, Point c, Point d, Point p, int screen_width, int screen_height, int calibPadding, double* relativePercentX, double* relativePercentY);

};

#endif // CALIBRATOR_H
