#include "sessionitemwidget.h"
#include "ui_sessionitemwidget.h"
#include "helper.h"

SessionItemWidget::SessionItemWidget(const Session& session, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SessionItemWidget)
{
    ui->setupUi(this);

    string name = Helper::timestampToFormatted(session.getName());
    stringstream points;
    points << session.getPoints().size() << " points";
    QPixmap image = session.getImage().scaled(50, 30);

    ui->name->setText(QString(name.c_str()));
    ui->points->setText(QString(points.str().c_str()));
    ui->image->setPixmap(image);
}

SessionItemWidget::~SessionItemWidget()
{
    delete ui;
}
