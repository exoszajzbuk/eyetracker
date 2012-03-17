#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <ctype.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <iostream>
#include <sstream>
#include <fstream>

using namespace cv;
using namespace std;


bool sortByCircularityPredicate(vector<double> p_1, vector<double> p_2)
{
    return p_1[2] < p_2[2];
}


bool pupilFound(RotatedRect p_rect)
{
    return p_rect.center.x != -1 && p_rect.center.y != -1;
}


void drawMeasurePoint(Mat& p_image, Point p_point)
{
    Scalar green = Scalar(0,255,0);
    Scalar red = Scalar(0,0,255);
    int cross_size = 20;

    circle(p_image, p_point, 10, red, 2, -1);
    line(p_image, Point(p_point.x-cross_size, p_point.y), Point(p_point.x+cross_size, p_point.y), green, 2);
    line(p_image, Point(p_point.x, p_point.y-cross_size), Point(p_point.x, p_point.y+cross_size), green, 2);
}


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
    Mat frame, gray, equalized, thresholded, inverted;
    int thresh = 5;

    // contours
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;

    // object properties
    vector<vector<double> > properties;

    // previous objects
    vector<RotatedRect> positions;
    bool notFound = false;
    int errors = 0;
    RotatedRect init = RotatedRect(Point(320,240), Size(100,100), 0);

    // measurement vector
    vector<Point> measurePoints;
    vector<RotatedRect> measureValues;

    int screen_width = 1366;
    int screen_height = 768;
    int grid_size = 100;

    int start_x = (screen_width%grid_size)/2;
    int start_y = (screen_height%grid_size)/2;

    for (int i=0; i<(screen_height/grid_size)+1; i++)
    {
        for (int j=0; j<(screen_width/grid_size)+1; j++)
        {
            measurePoints.push_back(Point(start_y+j*grid_size, start_x+i*grid_size));
        }
    }

    Mat measureImage = Mat(screen_height, screen_width, CV_8UC3);
    int measureIndex = -1;

    // init GUI
    namedWindow("Original", WINDOW_AUTOSIZE);
    cvMoveWindow("Original", 20, 20);
    namedWindow("Objects", WINDOW_AUTOSIZE);
    cvMoveWindow("Objects", 420, 20);

    createTrackbar("Threshold", "Objects", &thresh, 255);

    // start measuring fps
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

        // inverse for contour finding
        inverted=255-thresholded ;

        // image for objects
        Mat objects = Mat::zeros(inverted.rows, inverted.cols, CV_8UC3);

        // reset found counter
        notFound = false;

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
                    drawContours(objects, contours, idx, green, CV_FILLED, 8, hierarchy, 0);
                }
            }

            if (properties.size() > 0)
            {
                // sort by fitness value
                sort(properties.begin(), properties.end(), sortByCircularityPredicate);

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
                        drawContours(objects, contours, pupilIndex, red, CV_FILLED, 8, hierarchy, 0);
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
                positions.push_back(init);
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


        // ellipse of the eye
        RotatedRect eyePosition = positions.back();


        // if eye is opened
        if (pupilFound(eyePosition))
        {
            // ----------
            // DRAW PUPIL
            Scalar red(0, 0, 255);

            ellipse(frame, eyePosition, red, 2);
            circle(frame, eyePosition.center, 2, red, 2);
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
        s.precision(2);
        s << fixed << "FPS: " << fps << " | ";

        if (pupilFound(eyePosition))
        {
            s << "Eye position: (" << eyePosition.center.x << "," << eyePosition.center.y << ")";
        }
        else
        {
            s << "Eye closed";
        }

        displayStatusBar("Original", s.str(), 1000);

        // render images
        imshow("Original", frame);
        imshow("Objects", objects);

        // quit
        int c = waitKey(1);
        if (c == 'q' || c == 'Q' || (c & 255) == 27)
        {
           break;
        }
        // save data
        else if (c == '\r')
        {
            if (measureIndex < (int)measurePoints.size())
            {
                if (measureIndex == -1)
                {
                    // open measurement window
                    namedWindow("Measurement", CV_WINDOW_AUTOSIZE | CV_GUI_NORMAL);
                    cvSetWindowProperty("Measurement", CV_WND_PROP_FULLSCREEN, CV_WINDOW_FULLSCREEN);
                }
                else
                {
                    // save stuff
                    measureValues.push_back(eyePosition);
                }

                // next point
                measureIndex++;

                // render next point
                measureImage.setTo(Scalar(0,0,0));
                drawMeasurePoint(measureImage, measurePoints[measureIndex]);
                imshow("Measurement", measureImage);

                // destroy window if necessary
                if (measureIndex == measurePoints.size())
                {
                    destroyWindow("Measurement");
                }
            }
        }
    }


    // save if measurement if complete
    if (measureIndex == measurePoints.size())
    {
        // set filename to current date
        time_t t = time(0);
        struct tm *now = localtime(&t);

        stringstream filename;
        filename << (now->tm_year + 1900) << "-" << (now->tm_mon + 1) << "-" << now->tm_mday << "_" << now->tm_hour << "-" << now->tm_min << "-" << now->tm_sec << ".txt";

        ofstream outfile;
        outfile.open(filename.str().c_str());

        for (unsigned int i = 0; i<measurePoints.size(); i++)
        {
            outfile << measurePoints[i].x << ";"
                    << measurePoints[i].y << ";"
                    << measureValues[i].center.x << ";"
                    << measureValues[i].center.y << ";"
                    << measureValues[i].size.width << ";"
                    << measureValues[i].size.height << ";"
                    << measureValues[i].angle << endl;
        }

        outfile.close();

    }

    return 0;
}
