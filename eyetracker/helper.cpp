#include "helper.h"

Helper::Helper()
{
}

string Helper::timeToTimestamp(time_t t)
{
    stringstream strm;
    strm << t;
    return strm.str();
}

string Helper::timestampToFormatted(string t)
{
    time_t raw_time = atoi(t.c_str());

    struct tm* timeinfo;
    char buffer[80];

    timeinfo = localtime(&raw_time);

    strftime(buffer, 80, "%y/%m/%d %H:%M:%S", timeinfo);

    return string(buffer);
}
