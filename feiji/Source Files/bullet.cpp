#include "bullet.h"
#include <QPainter>

Bullet::Bullet(QPoint position, int speed, const QPixmap& image, int damage, QObject *parent) :
    QObject(parent), m_position(position), m_speed(speed), m_image(image), m_damage(damage)
{
}

void Bullet::update()
{
    m_position.setY(m_position.y() - m_speed);
}

void Bullet::draw(QPainter &painter)
{
    if (!m_image.isNull()) {
        painter.drawPixmap(m_position, m_image);
    }
}

bool Bullet::isOutOfBounds(int windowHeight) const
{
    return m_position.y() < -m_image.height();
}
