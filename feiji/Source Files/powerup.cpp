#include "powerup.h"
#include <QPainter>
#include <QDebug>

PowerUp::PowerUp(PowerUpType type, QPoint position, QObject *parent) :
    QObject(parent), type(type), position(position), isActive(true), speed(3)
{
    // 根据道具类型加载对应的图像
    switch(type) {
    case HEALTH_RESTORE:
        if (!image.load(":/new/prefix1/res/blood_returning.png")) {
            qDebug() << "Failed to load health restore image!";
        }
        break;
    case HEALTH_INCREASE:
        if (!image.load(":/new/prefix1/res/blood_plus.png")) {
            qDebug() << "Failed to load health increase image!";
        }
        break;
    case INVINCIBILITY:
        if (!image.load(":/new/prefix1/res/shield.png")) {
            qDebug() << "Failed to load invincibility image!";
        }
        break;
    case BULLET_BOOST:
        if (!image.load(":/new/prefix1/res/boost.png")) {
            qDebug() << "Failed to load bullet boost image!";
        }
        break;
    }
}

void PowerUp::update()
{
    // 道具向下移动
    position.setY(position.y() + speed);
}

void PowerUp::draw(QPainter &painter) const
{
    if (isActive && !image.isNull()) {
        painter.drawPixmap(position, image.scaled(40，40 , Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    }
}

bool PowerUp::isOutOfBounds(int windowHeight) const
{
    return position.y() > windowHeight;
}

QRect PowerUp::getCollisionRect() const
{
    // 使用缩放后的尺寸进行碰撞检测
    return QRect(position, QSize(40，40));
}
