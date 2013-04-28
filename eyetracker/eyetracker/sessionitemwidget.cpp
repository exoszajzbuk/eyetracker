#include "sessionitemwidget.h"
#include "ui_sessionitemwidget.h"

SessionItemWidget::SessionItemWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SessionItemWidget)
{
    ui->setupUi(this);
}

SessionItemWidget::~SessionItemWidget()
{
    delete ui;
}
