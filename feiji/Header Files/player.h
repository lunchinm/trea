#ifndef PLAYER_H
#define PLAYER_H
#include "plane.h"
#include <QPainter>
#include <QKeyEvent>
#include <QPixmap>
#include <QList>
#include "bullet.h"

class Widget;
class Play01;



class Player
{
public:
    Player(Widget *widget);
    ~Player();

    //   处理画笔的函数
    void draw(QPainter &painter);
    //    处理键盘事件的函数
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    //    处理鼠标事件的函数
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void update();

    Plane *playerPlane = nullptr;
    Widget *widget;

    void initGame(int windowWidth, int windowHeight, bool resetStats = true);
    Plane* getPlayerPlane() const { return playerPlane; }

public:
    QList<Bullet*>& getBullets() { return bullets; }

private:
    int windowWidth;
    int windowHeight;
    QList<Bullet*> bullets;
    QPixmap bulletImage;
    int bulletSpawnTimer;
    int bulletSpawnInterval;
    int originalSpawnInterval;
    bool isInvincible;
    int invincibleTimer;
    QList<int> boostTimers; // 多个boost道具计时器
    int originalBulletSpeed;
    int originalBulletDamage;
    int additionalBulletDamage;
    bool bulletDamageBoostActive;
    int bulletDamageBoostTimer;
    int permanentDamageBoost; // 永久伤害提升
    int permanentSpawnReduction; // 永久攻击间隔减少
    int bulletCount; // 弹道数量
    int boostPowerUpCount; // boost道具收集数量
private:
    QPixmap shieldImage; // 护盾图片
    int maxHealthDamagePercent; // 敌机生命上限伤害百分比加成(0-10%)
    int lostHealthDamagePercent; // 敌机已损失生命值伤害百分比加成
    int pendingSpawnReduction; // 待应用的射击间隔减少量
    int pendingDamageBoost; // 待应用的伤害提升量

    void shoot();

public:
    // 获取伤害百分比加成
    int getMaxHealthDamagePercent() const { return maxHealthDamagePercent; }
    int getLostHealthDamagePercent() const { return lostHealthDamagePercent; }
    int getPermanentDamageBoost() const { return permanentDamageBoost; }
    bool isBoostActive() const { return !boostTimers.isEmpty(); }

    void setInvincible(bool invincible, int duration);
    void boostBullets(int duration);
    void restoreHealth(int amount);
    void increaseMaxHealth(int amount);
    void boostBulletDamage(int duration);

};

#endif // PLAYER_H
