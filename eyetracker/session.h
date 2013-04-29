#ifndef SESSION_H
#define SESSION_H

#include <QPixmap>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

class Session
{
public:
    Session();
    ~Session();

    void setImage(QPixmap i);
    void addPoint(Point p);

    void save(string name);
    void load(string name);

    void getStats();

    // static for sessio names
    static vector<string> getNames();

private:
    QPixmap image;
    vector<Point> points;
};

#endif // SESSION_H
