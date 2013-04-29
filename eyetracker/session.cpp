#include "session.h"

#include <iostream>
#include <QDebug>
#include <QDir>
#include <QFile>

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
    string folderName = string("session_")+name;
    string imageName = folderName+"/image.png";
    string pointsName = folderName+"/points.txt";

    // create dir
    QDir().mkdir(folderName.c_str());

    // save image
    QFile imageFile(imageName.c_str());
    imageFile.open(QIODevice::WriteOnly);
    image.save(&imageFile, "PNG");
    imageFile.close();

    // save points
    QFile pointsFile(pointsName.c_str());
    pointsFile.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&pointsFile);

    for (int i=0; i<points.size(); ++i)
    {
        out << points.at(i).x << "," << points.at(i).y << endl;
    }
}

void Session::load(string name)
{

}
