#ifndef ENEMY_H
#define ENEMY_H
#include <QObject>
#include <QPixmap>
#include <QPoint>
#include "valueanimation.h"
#include "enemybullet.h"

class Enemy : public QObject
{
    Q_OBJECT
public:
    explicit Enemy(int type, int level, QObject *parent = nullptr);
    ~Enemy();

    void update();
    void draw(QPainter &painter) const;
    void shoot();
    void hit(int damage);
    QList<EnemyBullet*>& getBullets();
    bool isOutOfBounds(int windowHeight) const;
    QRect getCollisionRect() const;
    int getDamageScoreMultiplier() const { return damageScoreMultiplier; }
    int getMultiplier() const { return multiplier; }

    QPoint pos;               // 位置
    int originalHealth;       // 原始血量
    int currentHealth;        // 当前血量
    bool isAlive;             // 是否存活
    int size;                 // 敌机大小
    int type;                 // 敌机类型(1-7)
    ValueAnimation healthAnimation; // 血量动画
    QList<EnemyBullet*> bullets; // 敌机子弹列表

private:
    int multiplier;           // 关卡乘数（5倍/关）
    int damageScoreMultiplier;// 伤害和分数乘数（2倍/关）
    QPixmap planeImage;       // 敌机图片
    QPixmap bulletImage;      // 子弹图片
    int bulletSpeed;          // 子弹速度
    int shootInterval;        // 射击间隔(ms)
    int shootTimer;           // 射击计时器

    void loadImages();
};

#endif // ENEMY_H