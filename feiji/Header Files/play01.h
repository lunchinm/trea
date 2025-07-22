#ifndef Play01_H
#define Play01_H
#include <QPainter>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QList>
#include <QPoint>
#include "bomb.h"
#include "enemy.h"
#include "valueanimation.h"
#include "powerup.h"

class Widget;

class Play01
{
public:

    Play01(Widget *widget);
    ~Play01();

    Widget *widget;
    bool isPaused;
    QPoint playerPos;
    int timerId;

    // 初始化游戏数据
    void initGame(int windowWidth, int windowHeight, bool resetLevel = true);
    // 更新游戏状态
    void update();
    // 处理画笔的函数
    void draw(QPainter &painter);
    // 处理键盘事件的函数
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

    int getEnemySpeed() const;
    int getCurrentLevel() const { return currentLevel; }
    void setCurrentLevel(int level) { currentLevel = level; }
    long long getTargetScore() const { return targetScore; }
    void setTargetScore(long long score) { targetScore = score; }
    long long getScore() const { return score; }
    void setEnemySpeed(int newEnemySpeed) { enemySpeed = newEnemySpeed; }

private:
    QList<PowerUp*> powerups; // 道具列表
    QList<EnemyBullet*> enemyBullets; // 敌机子弹列表
    // 分数变化动画结构体
    struct ScorePopup {
        QString text;
        QPoint position;
        int timer;
        int duration;
    };

    QList<ScorePopup> scorePopups; // 分数变化动画列表
    ValueAnimation scoreAnimation; // 总分变化动画

    QList<Enemy*> enemies;    // 敌机列表
    QList<Bomb*> bombs;      // 爆炸效果列表
    int enemySpeed;          // 敌机速度

    int spawnInterval;       // 生成间隔(毫秒)
    int spawnTimer;          // 生成计时器
    int windowWidth;         // 窗口宽度
    int windowHeight;        // 窗口高度
    long long score;         // 分数
    int currentLevel;         // 当前关卡
    long long targetScore;    // 目标分数

    // 生成随机数
    int getRandomInt(int min, int max);
    // 生成随机敌机类型
    int getRandomEnemyType();
    // 更新分数动画
    void updateScoreAnimation();

};

#endif // Play01_H
