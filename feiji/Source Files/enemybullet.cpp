#include "enemybullet.h"
#include <QPainter>

EnemyBullet::EnemyBullet(QPoint position, int speed, const QPixmap& image, int damage, QObject *parent)
    : QObject(parent), m_position(position), m_speed(speed), m_image(image), m_damage(damage)
{}

EnemyBullet::~EnemyBullet()
{}

void EnemyBullet::update()
{
    // 敌机子弹向下移动
    m_position.setY(m_position.y() + m_speed);
}

void EnemyBullet::draw(QPainter &painter)
{
    if (!m_image.isNull()) {
        painter.drawPixmap(m_position, m_image);
    }
}

bool EnemyBullet::isOutOfBounds(int windowHeight) const
{
    // 子弹超出屏幕底部
    return m_position.y() > windowHeight;
}

QRect EnemyBullet::getCollisionRect() const
{
    // 使用固定尺寸10x20作为碰撞矩形，匹配视觉大小
    return QRect(m_position, QSize(10, 20));
}