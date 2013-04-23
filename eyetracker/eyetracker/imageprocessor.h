#ifndef IMAGEPROCESSOR_H
#define IMAGEPROCESSOR_H

#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

class ImageProcessor
{
public:
    ImageProcessor();
    ~ImageProcessor();
    Point2f process(const Mat &);

    enum DisplayMode
    {
        Equalized,
        Thresholded,
        Blobs,
        Pupil
    };

    void setDisplayMode(DisplayMode);
    Mat& getDisplayImage();

private:
    DisplayMode displayMode;

    static bool sortByCircularityPredicate(vector<double>, vector<double>);
    bool pupilFound(RotatedRect);

    Mat flipped;
    Mat grey;
    Mat equalized;
    Mat thresholded;
    Mat inverted;
    Mat blobs;

    RotatedRect initRect;

    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    vector<vector<double> > properties;
    vector<RotatedRect> positions;

    int errors;
};

#endif // IMAGEPROCESSOR_H
