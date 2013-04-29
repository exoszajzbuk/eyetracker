#include "session.h"

#include <QDebug>

Session::Session()
{
    qDebug("session initialized");
    points.clear();
}

Session::~Session()
{
    qDebug("session destroyed");
}

void Session::setImage(QPixmap i)
{
    image = i;
}

void Session::addPoint(Point p)
{
    points.push_back(p);
}

void Session::save(string name)
{
    qDebug(name.c_str());
    qDebug() << points.size();
}

void Session::load(string name)
{

}
