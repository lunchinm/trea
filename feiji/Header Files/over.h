#ifndef Over_H
#define Over_H
#include <QPainter>
#include <QKeyEvent>
#include <QRect>
#include <QObject>

class Widget;

class Over : public QObject
{
    Q_OBJECT

public:
    explicit Over(Widget *widget, QObject *parent = nullptr);

    Widget *widget;

    // 设置最终分数
    void setFinalScore(int score);
    // 初始化结束界面
    void init(int windowWidth, int windowHeight);
    // 处理画笔的函数
    void draw(QPainter &painter);
    // 处理键盘事件的函数
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    // 检查鼠标点击是否在按钮上
    bool isRestartClicked(const QPoint &pos);

signals:
    void restartGameClicked();

private:
    int finalScore;               // 最终分数
    QRect restartButtonRect;      // 重新开始按钮区域
    int windowWidth;              // 窗口宽度
    int windowHeight;             // 窗口高度
};

#endif // Over_H
