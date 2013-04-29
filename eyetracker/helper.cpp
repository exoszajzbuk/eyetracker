#include "helper.h"

Helper::Helper()
{
}

string Helper::timestr(time_t t)
{
    stringstream strm;
    strm << t;
    return strm.str();
}
