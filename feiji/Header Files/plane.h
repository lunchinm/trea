#ifndef PLANE_H
#define PLANE_H
#include <QPainter>
#include <QKeyEvent>
#include <QPixmap>
#include <QDebug>


class Plane
{
public:
    Plane();
    
    Plane(const Plane &) = default;
    Plane(Plane &&) = default;
    Plane &operator=(const Plane &) = default;
    Plane &operator=(Plane &&) = default;
    void draw(QPainter &painter);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    QPixmap image;
    int x =200;
    int y =650;
    // 玩家图片尺寸固定为100x100像素
    int width = 100;
    int height = 100;
    int health;
    int maxHealth;
    bool isInvincible = false;
    bool isDestroyed = false;
    
    // 移动键是否被按下
    bool isUp = false;
    bool isDown = false;
    bool isLeft = false;
    bool isRight = false;
    
    void move();
    void resetMovementState();
    QRect getCollisionRect() const {
        // 使用成员变量作为碰撞尺寸，与视觉表现匹配
        return QRect(x, y, width, height);
    }
    
};

#endif // PLANE_H
