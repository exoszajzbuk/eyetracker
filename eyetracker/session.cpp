#include "session.h"

#include <iostream>
#include <QDebug>
#include <QDir>
#include <QFile>

Session::Session()
{
    points.clear();
}

Session::~Session()
{
}

void Session::setImage(QPixmap i)
{
    image = i;
}

void Session::addPoint(Point p)
{
    points.push_back(p);
}

void Session::save(string p_name)
{
    name = p_name;

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

    for (unsigned int i=0; i<points.size(); ++i)
    {
        out << points.at(i).x << "," << points.at(i).y << endl;
    }
}

void Session::load(string p_name)
{
    name = p_name;

    string folderName = string("session_")+name;
    string imageName = folderName+"/image.png";
    string pointsName = folderName+"/points.txt";

    // load image
    image.load(imageName.c_str());

    // load points
    QFile pointsFile(pointsName.c_str());
    pointsFile.open(QIODevice::ReadOnly);

    QTextStream in(&pointsFile);
    QString line = in.readLine();
    while (!line.isNull()) {
        string lineStr = line.toLocal8Bit().constData();
        //cout << lineStr << endl;

        std::vector<std::string> coords;
        istringstream f(lineStr);
        string s;
        while (getline(f,s,','))
        {
            //std::cout << s << std::endl;
            coords.push_back(s);
        }

        Point2f p(atoi(coords.at(0).c_str()), atoi(coords.at(1).c_str()));
        addPoint(p);

        line = in.readLine();
    }
}

// read session names
vector<string> Session::getNames()
{
    vector<string> names;
    string prefix = "session_";

    QDir base = QDir();
    base.setFilter(QDir::Dirs | QDir::NoDotAndDotDot);
    base.setSorting(QDir::Name);

    QStringList dirs = base.entryList();

    for (int i = 0; i < dirs.size(); ++i)
    {
        string dir = dirs.at(i).toLocal8Bit().constData();

        // if dire has the right prefix
        if (dir.compare(0,8,prefix) == 0)
        {
            string name = dir.substr(8, string::npos);
            names.push_back(name);
        }
    }

    // reverse order
    reverse(names.begin(), names.end());
    return names;
}

string Session::getName() const
{
    return name;
}

vector<Point> Session::getPoints() const
{
    return points;
}

QPixmap Session::getImage() const
{
    return image;
}
