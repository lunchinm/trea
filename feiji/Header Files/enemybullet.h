#ifndef ENEMYBULLET_H
#define ENEMYBULLET_H
#include <QObject>
#include <QPixmap>
#include <QPoint>

class EnemyBullet : public QObject
{
    Q_OBJECT
public:
    explicit EnemyBullet(QPoint position, int speed, const QPixmap& image, int damage, QObject *parent = nullptr);
    ~EnemyBullet();

    void update();
    void draw(QPainter &painter);
    bool isOutOfBounds(int windowHeight) const;
    QRect getCollisionRect() const;

    QPoint getPosition() const { return m_position; }
    const QPixmap& getImage() const { return m_image; }
    int getDamage() const { return m_damage; }

private:
    QPoint m_position;    // 子弹位置
    int m_speed;          // 子弹速度
    QPixmap m_image;      // 子弹图片
    int m_damage;         // 子弹伤害值
};

#endif // ENEMYBULLET_H