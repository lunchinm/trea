#ifndef BEFORE_H
#define BEFORE_H
#include <QPainter>
#include <QKeyEvent>
#include <QMouseEvent>
class Widget;

class Before : public QObject
{
    Q_OBJECT

public:
    explicit Before(Widget *widget, QObject *parent = nullptr);

    Widget *widget;

    //   处理画笔的函数
    void draw(QPainter &painter);
    //    处理键盘事件的函数
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

signals:
    void startGameClicked();


};

#endif // BEFORE_H
