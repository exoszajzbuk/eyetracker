#ifndef SESSIONITEMWIDGET_H
#define SESSIONITEMWIDGET_H

#include "session.h"
#include <QWidget>

namespace Ui {
class SessionItemWidget;
}

class SessionItemWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit SessionItemWidget(const Session& session, QWidget *parent = 0);
    ~SessionItemWidget();
    
private:
    Ui::SessionItemWidget *ui;
};

#endif // SESSIONITEMWIDGET_H
