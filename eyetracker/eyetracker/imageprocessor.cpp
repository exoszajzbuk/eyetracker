#include "imageprocessor.h"
#include <QDebug>

ImageProcessor::ImageProcessor()
{
    flipped = Mat();
    grey = Mat();
    equalized = Mat();
    thresholded = Mat();
    inverted = Mat();
    blobs = Mat();

    errors = 0;

    initRect = RotatedRect(Point(320,240), Size(100,100), 0);
}

ImageProcessor::~ImageProcessor()
{

}

Point2f ImageProcessor::process(const Mat& frame)
{
    qDebug("processing");

    // process frame
    flip(frame, flipped, 0);
    cvtColor(flipped, grey, CV_BGR2GRAY);
    equalizeHist(grey, equalized);
    threshold(equalized, thresholded, 5, 255, THRESH_BINARY);
    inverted=255-thresholded;

    blobs = Mat::zeros(inverted.rows, inverted.cols, CV_8UC3);

    // reset found counter
    bool notFound = false;

    // find contours
    findContours(inverted, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

    // if contours found
    if (contours.size() > 0)
    {
        properties.clear();

        // gather contour information
        for(int idx=0; idx >= 0; idx = hierarchy[idx][0] )
        {
            // set contour's convex hull
            convexHull(contours[idx], contours[idx], true);

            // calculate properties
            double area = contourArea(contours[idx]);
            double perimeter = arcLength(contours[idx], true);
            double circularity = perimeter/(2*sqrt(M_PI*area));

            // only for significant contours
            if (area > 2500)
            {
                // save properties
                vector<double> props;
                props.push_back(idx);           // 0
                props.push_back(area);          // 1
                props.push_back(circularity);   // 2

                properties.push_back(props);

                // draw object with green
                Scalar green(0, 255, 0);
                drawContours(blobs, contours, idx, green, CV_FILLED, 8, hierarchy, 0);
            }
        }

        if (properties.size() > 0)
        {
            // sort by fitness value
            sort(properties.begin(), properties.end(), ImageProcessor::sortByCircularityPredicate);

            // select pupil center
            int pupilIndex = properties[0][0];

            // if circular enough
            if(properties[0][2] < 1.07)
            {
                // must have enough points
                if(contours[pupilIndex].size() > 5)
                {
                    // -----------
                    // PUPIL FOUND
                    RotatedRect ell = fitEllipse(contours[pupilIndex]);
                    positions.push_back(ell);

                    // draw object with red
                    Scalar red(0, 0, 255);
                    drawContours(blobs, contours, pupilIndex, red, CV_FILLED, 8, hierarchy, 0);
                }
                else
                {
                    notFound = true;
                }
            }
            else
            {
                notFound = true;
            }
        }
        else
        {
            notFound = true;
        }
    }
    else
    {
        notFound = true;
    }

    // if no pupil found use the last one
    if (notFound == true)
    {
        errors++;

        if (positions.size() == 0)
        {
            positions.push_back(initRect);
        }
        else
        {
            if (errors < 5)
            {
                // just use last value
                positions.push_back(positions.back());
            }
            else
            {
                positions.push_back(RotatedRect(Point(-1,-1),Size(0,0),0));
            }
        }
    }
    else
    {
        errors = 0;
    }

    // delete previous positions if any
    if (positions.size() > 5)
    {
        positions.erase(positions.begin());
    }

    // ellipse of the eye
    RotatedRect eyePosition = positions.back();

    // if eye is opened
    if (pupilFound(eyePosition))
    {
        // ----------
        // DRAW PUPIL
        Scalar red(0, 0, 255);

        ellipse(flipped, eyePosition, red, 2);
        circle(flipped, eyePosition.center, 2, red, 2);
    }

    return eyePosition.center;
}

Mat& ImageProcessor::getEqualized()
{
    return equalized;
}

Mat& ImageProcessor::getThresholded()
{
    return thresholded;
}

Mat& ImageProcessor::getBlobs()
{
    return blobs;
}

Mat& ImageProcessor::getPupil()
{
    return flipped;
}

bool ImageProcessor::pupilFound(RotatedRect p_rect)
{
    return p_rect.center.x != -1 && p_rect.center.y != -1;
}


bool ImageProcessor::sortByCircularityPredicate(vector<double> p_1, vector<double> p_2)
{
    return p_1[2] < p_2[2];
}
