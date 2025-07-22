#ifndef LEVELCOMPLETE_H
#define LEVELCOMPLETE_H
#include <QPainter>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QRect>
#include <QObject>

class Widget;

class LevelComplete : public QObject
{
    Q_OBJECT

public:
    explicit LevelComplete(Widget *widget, QObject *parent = nullptr);

    Widget *widget;

    // 设置当前关卡和分数
    void setLevelInfo(int currentLevel, long long score);
    // 初始化界面
    void init(int windowWidth, int windowHeight);
    // 绘制界面
    void draw(QPainter &painter);
    // 事件处理
    void keyPressEvent(QKeyEvent *event);
    void mousePressEvent(QMouseEvent *event);

    // 检查按钮点击
    bool isNextLevelClicked(const QPoint &pos);
    bool isRestartLevelClicked(const QPoint &pos);
    bool isBackToHomeClicked(const QPoint &pos);

signals:
    void nextLevelClicked();
    void restartLevelClicked();
    void backToHomeClicked();

private:
    int currentLevel;          // 当前关卡
    long long score;           // 当前分数
    QRect nextLevelButton;     // 下一关按钮区域
    QRect restartLevelButton;  // 重新开始按钮区域
    QRect backToHomeButton;    // 返回首页按钮区域
    int windowWidth;           // 窗口宽度
    int windowHeight;          // 窗口高度
};

#endif // LEVELCOMPLETE_H
