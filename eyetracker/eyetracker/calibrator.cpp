#include "calibrator.h"

Calibrator::Calibrator()
{
}

Point Calibrator::calculatePosition(Point a, Point b, Point c, Point d, Point p, int screen_width, int screen_height, int calibPadding, double* relativePercentX, double* relativePercentY)
{
    double C = (double)(a.y - p.y) * (d.x - p.x) - (double)(a.x - p.x) * (d.y - p.y);
    double B = (double)(a.y - p.y) * (c.x - d.x) + (double)(b.y - a.y) * (d.x - p.x) - (double)(a.x - p.x) * (c.y - d.y) - (double)(b.x - a.x) * (d.y - p.y);
    double A = (double)(b.y - a.y) * (c.x - d.x) - (double)(b.x - a.x) * (c.y - d.y);

    double D = B * B - 4 * A * C;

    double u = (-B - sqrt(D)) / (2 * A);

    double p1x = a.x + (b.x - a.x) * u;
    double p2x = d.x + (c.x - d.x) * u;

    double px = p.x;

    double v = (px - p1x) / (p2x - p1x);

    // in display bounds
    /*
    if (u > 1)
        u = 1;
    else if (u < 0)
        u = 0;

    if (v > 1)
        v = 1;
    else if (v < 0)
        v = 0;
    */

    Point ret;
    // calculate screen coordinates
    ret.x = u*(screen_width-2*calibPadding)+calibPadding;
    ret.y = v*(screen_height-2*calibPadding)+calibPadding;

    // save relative coordinates
    *relativePercentX = u;
    *relativePercentY = v;

    return ret;
}
