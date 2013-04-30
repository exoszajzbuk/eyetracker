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

    void save(string p_name);
    void load(string p_name);

    string getName() const;
    QPixmap getImage() const;
    vector<Point> getPoints() const;

    // static for sessio names
    static vector<string> getNames();

private:
    string name;
    QPixmap image;
    vector<Point> points;
};

#endif // SESSION_H
