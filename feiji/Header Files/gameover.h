#ifndef GAMEOVER_H
#define GAMEOVER_H
#include <QPainter>
#include <QKeyEvent>
#include <QRect>
#include <QObject>
#include <QPoint>

class Widget;

class GameOver : public QObject
{
    Q_OBJECT

public:
    explicit GameOver(Widget *widget, QObject *parent = nullptr);

    Widget *widget;

    // 设置当前分数
    void setCurrentScore(long long score);
    // 初始化界面
    void init(int windowWidth, int windowHeight, bool isPaused = false);
    // 绘制界面
    void draw(QPainter &painter);
    // 事件处理
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    // 检查按钮点击
    bool isRestartClicked(const QPoint &pos);
    bool isHomeClicked(const QPoint &pos);

signals:
    void restartLevelClicked();
    void backToHomeClicked();
    void resumeGameClicked();

private:
    long long currentScore;       // 当前分数
    QRect restartButtonRect;      // 重新开始按钮区域
    QRect homeButtonRect;         // 返回首页按钮区域
    QRect resumeButtonRect;       // 继续游戏按钮区域(仅暂停时显示)
    int windowWidth;              // 窗口宽度
    int windowHeight;             // 窗口高度
    bool isPaused;                // 是否为暂停状态
};

#endif // GAMEOVER_H