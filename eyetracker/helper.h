#ifndef HELPER_H
#define HELPER_H

#include <ctime>
#include <opencv2/opencv.hpp>

using namespace std;

class Helper
{
public:
    Helper();

    static string timeToTimestamp(time_t t);
    static string timestampToFormatted(string t);

};

#endif // HELPER_H
