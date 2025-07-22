#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "ui_widget.h"
#include "before.h"
#include "play01.h"
#include "player.h"
#include "over.h"
#include "levelcomplete.h"
#include "gameover.h"


class Widget : public QWidget

{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    enum class Page { Before, Play01, Over, LevelComplete, GameOver };
    Page currentPage;

    void changeBackground(int level); // 添加关卡背景更换方法
    void paintEvent(QPaintEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);// alt +回车
    void timerEvent(QTimerEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

    static int getRandomInt(int a,int b);
    void gameStart();
    void showLevelComplete(int currentLevel, long long score);
    ~Widget();

    Before *before = nullptr;
    Play01 *play01 = nullptr;
    Player *player = nullptr;
    Over *over = nullptr;
    GameOver *gameOver = nullptr;
    LevelComplete *levelComplete = nullptr;
    QRect pauseButtonRect; // 暂停按钮区域
    bool isGamePaused; // 游戏暂停状态
private slots:
    void onNextLevelClicked();
    void onRestartLevelClicked();
    void onBackToHomeClicked();
    void resetGameState(bool resetStats, int level = 1, long long targetScore = 5000);
    void onStartGameClicked();
    void onRestartGameClicked();
    void onPauseGameClicked();
    void onResumeGameClicked();
    long long getScore() const;

private:
    Ui::widget *ui;
    int timerId;
    QPixmap backgroundImage; // 背景图片
    QPixmap backgroundImage2; // 第二张背景图片
    int bgY1; // 第一张背景Y坐标
    int bgY2; // 第二张背景Y坐标
    const int bgSpeed = 5; // 背景滚动速度
    const int bgHeight = 768; // 背景图片高度
};
#endif // WIDGET_H
