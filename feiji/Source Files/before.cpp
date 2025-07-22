#include "before.h"
#include "widget.h"
#include <QDebug>

Before::Before(Widget *widget, QObject *parent)
    : QObject(parent), widget(widget)
{
}

void Before::draw(QPainter &painter)
{
    painter.fillRect(widget->rect(), Qt::white);
    painter.setPen(Qt::black);
    // 绘制开始提示
    painter.setFont(QFont(":/new/prefix1/Doto.ttf", 24));
    painter.drawText(150, 350, "点击或按Enter键开始游戏");

}

void Before::keyPressEvent(QKeyEvent *event)
{
    //
}

void Before::keyReleaseEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Return)
    {
        emit startGameClicked();
    }
}

void Before::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        emit startGameClicked();
    }
}

void Before::mouseReleaseEvent(QMouseEvent *event)
{
    //
}
