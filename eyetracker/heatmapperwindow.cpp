#include "heatmapperwindow.h"
#include "ui_heatmapperwindow.h"

#include <QApplication>
#include <QDesktopWidget>

HeatmapperWindow::HeatmapperWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HeatmapperWindow)
{
    ui->setupUi(this);

    // fullscreen bg
    ui->bgImage->setGeometry(QApplication::desktop()->screenGeometry(1));
}

HeatmapperWindow::~HeatmapperWindow()
{
    delete ui;
}

void HeatmapperWindow::setImage(const QImage& image)
{
    ui->bgImage->setPixmap(QPixmap::fromImage(image));
}
