#include "heatmapperwindow.h"
#include "ui_heatmapperwindow.h"

HeatmapperWindow::HeatmapperWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HeatmapperWindow)
{
    ui->setupUi(this);
}

HeatmapperWindow::~HeatmapperWindow()
{
    delete ui;
}
