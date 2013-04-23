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

    Mat& getEqualized();
    Mat& getThresholded();
    Mat& getBlobs();
    Mat& getPupil();

private:
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
