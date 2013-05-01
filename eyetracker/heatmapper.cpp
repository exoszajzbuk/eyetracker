#include "heatmapper.h"
#include "mainwindow.h"
#include "helper.h"

#include <QApplication>
#include <QDesktopWidget>

Heatmapper::Heatmapper()
{
}

Heatmapper::~Heatmapper()
{
}

void Heatmapper::setMainWindow(MainWindow *p_mainWindow)
{
    mainWindow = p_mainWindow;
}

void Heatmapper::generate(const Session &p_session)
{
    session = p_session;

    QRect screen = QApplication::desktop()->screenGeometry(1);

    window = new HeatmapperWindow();
    window->move(screen.center());
    window->showFullScreen();

    background = Helper::qimage2mat(session.getImage().toImage());
}
