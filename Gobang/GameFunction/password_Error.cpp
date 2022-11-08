#include "password_Error.h"
#include <QGraphicsDropShadowEffect>

password_Error::password_Error(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
    // wipe off frame
    setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint | windowFlags());
    setAttribute(Qt::WA_TranslucentBackground, true);
    QGraphicsDropShadowEffect* shadow = new QGraphicsDropShadowEffect(this);
    shadow->setOffset(0, 0);  // shadow distance
    shadow->setColor(Qt::black); // shadow color
    shadow->setBlurRadius(20);  // radius
    QWidget* widget = new QWidget(this);
    widget->setObjectName("widgetShadow");
    widget->setGraphicsEffect(shadow);  // shadow
    widget->setStyleSheet("#widgetShadow{background:#f5f5f5;border-radius:5px;}");
    QSize s = size();
    widget->resize(s);
    resize(s.width() + 20, s.height() + 20);
    widget->move(10, 10);
    for (QWidget* w : findChildren<QWidget*>())
    {
        if (w != widget)
        {
            if (w->parent() == this)
            {
                QRect r = w->geometry();
                w->setParent(widget);
                w->setGeometry(r);
            }
        }
    }
}

password_Error::~password_Error()
{
}

void password_Error::on_confirm_clicked(void)
{
    close();
}