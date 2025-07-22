#ifndef BULLET_H
#define BULLET_H

#include <QObject>
#include <QPixmap>
#include <QPoint>

class Bullet : public QObject
{
    Q_OBJECT
public:
    explicit Bullet(QPoint position, int speed, const QPixmap& image, int damage = 50, QObject *parent = nullptr);
    void update();
    void draw(QPainter &painter);
    bool isOutOfBounds(int windowHeight) const;
    QPoint getPosition() const { return m_position; }
    const QPixmap& getImage() const { return m_image; }
    int getDamage() const { return m_damage; }

private:
    QPoint m_position;
    int m_speed;
    QPixmap m_image;
    int m_damage;
};

#endif // BULLET_H
