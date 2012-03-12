#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <ctype.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <iostream>
#include <sstream>

using namespace cv;
using namespace std;

int main(int argc, char** argv)
{
    VideoCapture cap;

    cap.open(1);
    if( cap.isOpened() )
            cout << "Video " << argv[1] <<
                ": width=" << cap.get(CV_CAP_PROP_FRAME_WIDTH) <<
                ", height=" << cap.get(CV_CAP_PROP_FRAME_HEIGHT) <<
                ", nframes=" << cap.get(CV_CAP_PROP_FRAME_COUNT) << endl;

    if( !cap.isOpened() )
    {
        cout << "Could not initialize capturing...\n";
        return -1;
    }

    // set width and height
    cap.set(CV_CAP_PROP_FRAME_WIDTH, 640);
    cap.set(CV_CAP_PROP_FRAME_HEIGHT, 480);

    // for benchmarking
    time_t start, end;
    double fps;
    int counter = 0;
    double sec;

    // init frames
    Mat frame, gray, equalized, thresholded, blurred, inverted, objects;
    int thresh = 8;
    //Mat structElement = getStructuringElement(MORPH_RECT, Size(3,3));

    // object propertes
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;

    vector<vector<double> > properties;

    // init GUI
    namedWindow("Original", WINDOW_AUTOSIZE);
    cvMoveWindow("Original", 20, 20);
    namedWindow("Objects", WINDOW_AUTOSIZE);
    cvMoveWindow("Objects", 420, 20);

    createTrackbar("Threshold", "Objects", &thresh, 255);

    // start measuring
    time(&start);

    // main loop
    for(;;)
    {
        // get frame and flip it
        cap >> frame;
        if( frame.empty() )
            break;
        flip(frame, frame, 0);

        // greyscale
        cvtColor(frame, gray, CV_BGR2GRAY);
        equalizeHist(gray, equalized);
        threshold(equalized, thresholded, thresh, 255, THRESH_BINARY);

        // blur
        //morphologyEx(out, out, MORPH_CLOSE, structElement);
        //morphologyEx(out, out, MORPH_OPEN, structElement);
        medianBlur(thresholded, blurred, 3);

        // inverse for contour finding
        inverted=255-blurred;

        // find contours
        findContours(inverted, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);

        Mat objects = Mat::zeros(inverted.rows, inverted.cols, CV_8UC3);

        // if contours found
        if (contours.size() > 0)
        {
            // init
            double maxArea = 0;
            int maxIndex = 0;
            properties.clear();

            // iterate through first-level contours
            for(int idx=0; idx >= 0; idx = hierarchy[idx][0] )
            {
                // set contour's convex hull
                convexHull(contours[idx], contours[idx], true);

                // calculate properties
                double area = contourArea(contours[idx]);
                double perimeter = arcLength(contours[idx], true);
                double circularity = perimeter/(2*sqrt(M_PI*area));

                // save properties
                vector<double> props;
                props.push_back(idx);
                props.push_back(area);
                props.push_back(perimeter);
                props.push_back(circularity);

                properties.push_back(props);

                if(area > maxArea)
                {
                    maxArea = area;
                    maxIndex = idx;
                }

                /*
                // draw object with green
                Scalar green(0, 255, 0);
                drawContours(objects, contours, idx, green, CV_FILLED, 8, hierarchy, 0);

                // draw ellipses to original
                if(contours[idx].size() > 5)
                {
                    RotatedRect ell = fitEllipse(contours[idx]);
                    ellipse(frame, ell, green, 2);
                    circle(frame, ell.center, 2, green, 2);
                }
                */
            }

            /*
            // MAX AREA OBJECT
            // draw maximum-area object with red
            Scalar red(0, 0, 255);
            drawContours(objects, contours, maxIndex, red, CV_FILLED, 8, hierarchy, 0);

            if(contours[maxIndex].size() > 5)
            {
                RotatedRect ell = fitEllipse(contours[maxIndex]);
                ellipse(frame, ell, red, 2);
                circle(frame, ell.center, 2, red, 2);
            }
            */
        }

        /*
        // draw Eye region
        Scalar yellow(0,255,255);
        Point upperleft = Point(289,264);
        Point upperright = Point(475,217);
        Point lowerright = Point(459,281);
        Point lowerleft = Point(302,307);

        line(frame, upperleft, upperright, yellow, 2, 8);
        line(frame, upperright, lowerright, yellow, 2, 8);
        line(frame, lowerright, lowerleft, yellow, 2, 8);
        line(frame, lowerleft, upperleft, yellow, 2, 8);
        */

        // calculate current fps
        time(&end);
        ++counter;
        sec = difftime(end, start);
        fps = counter/sec;

        stringstream s;
        s << fps;

        displayStatusBar("Original", s.str(), 1000);

        // render images
        imshow("Original", frame);
        //imshow("Objects", objects);

        // quit
        int c = waitKey(1);
        if (c == 'q' || c == 'Q' || (c & 255) == 27)
        {
           break;
        }
    }

    return 0;
}
