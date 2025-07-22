#ifndef POWERUP_H
#define POWERUP_H

#include <QObject>
#include <QPixmap>
#include <QPoint>

enum PowerUpType {
    HEALTH_RESTORE,
    HEALTH_INCREASE,
    INVINCIBILITY,
    BULLET_BOOST
};

class PowerUp : public QObject {
    Q_OBJECT
public:
    explicit PowerUp(PowerUpType type, QPoint position, QObject *parent = nullptr);
    void update();
    void draw(QPainter &painter) const;
    bool isOutOfBounds(int windowHeight) const;
    QRect getCollisionRect() const;

    PowerUpType type;
    QPoint position;
    bool isActive;

private:
    QPixmap image;
    int speed;
};

#endif // POWERUP_H