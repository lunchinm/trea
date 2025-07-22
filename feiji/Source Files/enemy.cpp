#include "enemy.h"
#include <QPainter>
#include <QDebug>

Enemy::Enemy(int type, int level, QObject *parent) : QObject(parent), type(type), isAlive(true), shootTimer(0)
{
    loadImages();
    // 根据敌机类型设置属性
    multiplier = 1;
    for (int i = 1; i < level; ++i) multiplier *= 5;
    damageScoreMultiplier = 1;
    for (int i = 1; i < level; ++i) damageScoreMultiplier *= 2;
    switch(type) {
    case 1: size=50; originalHealth=50*multiplier; bulletSpeed=2; shootInterval=2400 - multiplier / 5; break;
    case 2: size=55; originalHealth=70*multiplier; bulletSpeed=2; shootInterval=2200 - multiplier / 5; break;
    case 3: size=60; originalHealth=100*multiplier; bulletSpeed=2; shootInterval=2000 - multiplier / 5; break;
    case 4: size=65; originalHealth=120*multiplier; bulletSpeed=2; shootInterval=1600 - multiplier / 5; break;
    case 5: size=70; originalHealth=150*multiplier; bulletSpeed=2; shootInterval=1400 - multiplier / 5; break;
    case 6: size=75; originalHealth=180*multiplier; bulletSpeed=3; shootInterval=1200 - multiplier / 5; break;
    case 7: size=80; originalHealth=200*multiplier; bulletSpeed=3; shootInterval=1000 - multiplier / 5; break;
    default: size=50; originalHealth=50; bulletSpeed=2; shootInterval=2000; break;
    }
    currentHealth = originalHealth;
    healthAnimation = {originalHealth, originalHealth, originalHealth, 0, 15};
}

Enemy::~Enemy()
{
    // 不删除子弹，由游戏管理器处理
    // qDeleteAll(bullets);
    // bullets.clear();
}

void Enemy::loadImages()
{
    // 加载敌机图片
    if (!planeImage.load(QString(":/new/prefix1/res/img-plane_%1.png").arg(type))) {
        qDebug() << "Failed to load enemy plane image for type" << type;
        planeImage = QPixmap(size, size);
        planeImage.fill(QColor(255, 0, 0, 128)); // 红色半透明占位符
    }
    // 加载子弹图片
    if (!bulletImage.load(QString(":/new/prefix1/res/bullet_%1.png").arg(type))) {
        qDebug() << "Failed to load bullet image for type" << type;
        bulletImage = QPixmap(10, 20);
        bulletImage.fill(QColor(0, 255, 0, 128)); // 绿色半透明占位符
    }
}

void Enemy::update()
{
    if (!isAlive) return;

    // 移动敌机（向下移动）
    pos.setY(pos.y() + 1);

    // 更新射击计时器
    shootTimer += 20; // 假设每帧20ms
    if (shootTimer >= shootInterval) {
        shoot();
        shootTimer = 0;
    }

    // 更新子弹
    for (int i = 0; i < bullets.size(); ) {
        EnemyBullet *bullet = bullets[i];
        bullet->update();
        if (bullet->isOutOfBounds(800)) { // 假设窗口高度800
            bullets.removeAt(i);
            delete bullet;
        } else {
            i++;
        }
    }

    // 更新血量动画
    if (healthAnimation.timer < healthAnimation.duration) {
        float progress = (float)healthAnimation.timer / healthAnimation.duration;
        healthAnimation.currentValue = healthAnimation.startValue + 
            (healthAnimation.targetValue - healthAnimation.startValue) * progress;
        healthAnimation.timer++;
    } else {
        healthAnimation.currentValue = healthAnimation.targetValue;
    }
}

void Enemy::draw(QPainter &painter) const
{
    if (!isAlive) return;

    // 绘制敌机
    QRect rect(pos.x() - size/2, pos.y() - size/2, size, size);
    painter.drawPixmap(rect, planeImage.scaled(size, size, Qt::KeepAspectRatio, Qt::SmoothTransformation));

    // 绘制血量文本
    painter.setPen(Qt::white);
    painter.setFont(QFont("Arial", 12, QFont::Bold));
    painter.drawText(rect.x(), rect.y() - 10, QString("%1/%2").arg(currentHealth).arg(originalHealth));

    // 绘制子弹
    for (EnemyBullet *bullet : bullets) {
        bullet->draw(painter);
    }
}

void Enemy::hit(int damage)
{
    currentHealth -= damage;
    healthAnimation.startValue = healthAnimation.currentValue;
    healthAnimation.targetValue = currentHealth;
    healthAnimation.timer = 0;
}

QList<EnemyBullet*>& Enemy::getBullets()
{
    return bullets;
}

void Enemy::shoot()
{
    if (bulletImage.isNull()) return;

    int bulletWidth = bulletImage.width();
    int bulletHeight = bulletImage.height();

    // 计算子弹位置（敌机中心偏下）
    int x = pos.x() - bulletWidth/2 ;
    int y = pos.y() + size/2;
    // 根据敌机类型设置不同伤害值
    int damage = ((type == 1) ? 10 : (type <= 3 ? 20 : 30)) * damageScoreMultiplier;
    EnemyBullet *bullet = new EnemyBullet(QPoint(x, y), bulletSpeed, bulletImage, damage);
    bullets.append(bullet);
}

bool Enemy::isOutOfBounds(int windowHeight) const
{
    return pos.y() + size/2 >= windowHeight;
}

QRect Enemy::getCollisionRect() const
{
    return QRect(pos.x() - size/2, pos.y() - size/2, size, size);
}