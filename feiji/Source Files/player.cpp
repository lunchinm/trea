#include "player.h"
#include <QDebug>


Player::Player(Widget *widget) : widget(widget), bulletSpawnInterval(500), originalSpawnInterval(500), bulletSpawnTimer(0),
    isInvincible(false), invincibleTimer(0),
    originalBulletSpeed(6), originalBulletDamage(50), permanentDamageBoost(0), permanentSpawnReduction(0), bulletCount(1), bulletDamageBoostActive(false), additionalBulletDamage(0), boostPowerUpCount(0), maxHealthDamagePercent(0), lostHealthDamagePercent(0) {
    this->playerPlane = new Plane();
    // 加载子弹图片
    if (!bulletImage.load(":/new/prefix1/res/hero_bullet_7.png")) {
        qDebug() << "Failed to load bullet image!";
    }
    // 加载护盾图片
    if (!shieldImage.load(":/new/prefix1/res/shiled_circle.png")) {
        qDebug() << "Failed to load shield image!";
    }
}

Player::~Player()
{
    delete playerPlane;
}

void Player::draw(QPainter &painter)
{
    // 绘制无敌状态护盾
    if (isInvincible && playerPlane) {
        painter.save();
        QPoint center(playerPlane->x + playerPlane->width/2, playerPlane->y + playerPlane->height/2);
        int shieldWidth = shieldImage.width();
        int shieldHeight = shieldImage.height();
        // 绘制护盾图片，居中显示
        painter.drawPixmap(center.x() - shieldWidth/2, center.y() - shieldHeight/2, shieldImage);
        painter.restore();
    }

    // 无敌状态下闪烁效果
    if (isInvincible && (invincibleTimer / 5) % 2 == 0) {
        painter.setOpacity(0.5);
    }
    //    画战机
    playerPlane->draw(painter);
    painter.setOpacity(1.0);

    // 绘制所有子弹
    for (Bullet *bullet : bullets) {
        bullet->draw(painter);
    }

    // 绘制玩家血条和数字
    if (playerPlane) {
        // 血条背景（红色）
        painter.setBrush(Qt::red);
        painter.drawRect(20, windowHeight - 60, 100, 20);
        // 血条前景（绿色）
        painter.setBrush(Qt::green);
        // 使用飞机实际宽度计算血条，与碰撞区域匹配
        int healthWidth = (playerPlane->health > 0) ? (playerPlane->health * playerPlane->width / playerPlane->maxHealth) : 0;
        painter.drawRect(20, windowHeight - 60, healthWidth, 20);
        // 血条边框（黑色）
        painter.setPen(Qt::black);
        painter.setBrush(Qt::NoBrush);
        painter.drawRect(20, windowHeight - 60, 100, 20);
        // 生命值文字
        painter.setPen(Qt::green);
        painter.setFont(QFont("Arial", 14, QFont::Bold));
        painter.drawText(130, windowHeight - 45, QString("%1/%2").arg(playerPlane->health).arg(playerPlane->maxHealth));
    }
}

void Player::keyPressEvent(QKeyEvent *event)
{
    playerPlane->keyPressEvent(event);
}



void Player::keyReleaseEvent(QKeyEvent *event)
{
    playerPlane->keyReleaseEvent(event);
}
void Player::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        shoot(); // 鼠标点击发射子弹
    }
}
void Player::initGame(int windowWidth, int windowHeight, bool resetStats)
{
    this->windowWidth = windowWidth;
    this->windowHeight = windowHeight;
    // 初始化玩家飞机位置到屏幕底部中央
    // 调整初始位置确保可见
    playerPlane->x = windowWidth / 2 - 25; // 减去矩形宽度的一半以居中
    playerPlane->y = windowHeight - 150;    // 向上调整避免超出底部边界
    // 仅在新游戏开始时重置生命值
    if (resetStats) {
        playerPlane->health = 150;
        playerPlane->maxHealth = 150;
    }
    // 重置飞机移动状态
    playerPlane->resetMovementState();
    // 清除所有子弹
    qDeleteAll(bullets);
    bullets.clear();
}

void Player::setInvincible(bool invincible, int duration)
{
    isInvincible = invincible;
    if (invincible)
    {
        invincibleTimer = duration / 20; // 转换为20ms为单位的计时器
        playerPlane->isInvincible = true;
    }
}

void Player::restoreHealth(int amount)
{
    if (playerPlane) {
        playerPlane->health = qMin(playerPlane->health + amount, playerPlane->maxHealth);
    }
}

void Player::increaseMaxHealth(int amount)
{
    if (playerPlane) {
        playerPlane->maxHealth += amount;
        playerPlane->health = qMin(playerPlane->health + amount, playerPlane->maxHealth);
    }
}

void Player::boostBulletDamage(int duration)
{
    this->additionalBulletDamage = originalBulletDamage + permanentDamageBoost;
    bulletDamageBoostActive = true;
    bulletDamageBoostTimer = duration / 20; // 转换为20ms为单位的计时器
}

void Player::boostBullets(int duration)
{
    // 弹道数量控制（最大15）
    // 无论弹道数量多少，始终应用3秒临时效果
    boostTimers.append(duration); // 添加3秒临时效果计时器
    pendingSpawnReduction += 50; // 存储待应用的永久属性提升
    pendingDamageBoost += 10;

    // 临时效果：3秒内攻速翻倍、伤害翻倍
    boostTimers.append(duration / 20);

    // 待应用的永久效果：3秒后减少50ms攻击间隔
    pendingSpawnReduction += 50;

    // 待应用的永久效果：3秒后增加10点伤害
    pendingDamageBoost += 10;

    if (bulletCount < 15) {
        // 检查是否达到250ms阈值（基于当前已应用的永久减少量）
        if (originalSpawnInterval - permanentSpawnReduction <= 250) {
            bulletCount++;
            originalSpawnInterval = 500; // 重置攻击间隔
            permanentSpawnReduction = 0; // 重置减少量
        }
    } else {
        // 弹道达到15后应用新效果
        boostPowerUpCount++;

        // 计算伤害百分比加成
        if (maxHealthDamagePercent < 10) {
            maxHealthDamagePercent = qMin(10, boostPowerUpCount / 100);
        } else {
            lostHealthDamagePercent = boostPowerUpCount / 100 - 10;
        }

        // 减少射击间隔（最低50ms）
        bulletSpawnInterval = qMax(50, bulletSpawnInterval - 10);

        // 添加临时提升计时器（3秒效果）
        boostTimers.append(duration / 20);

        // 立即应用永久伤害提升
        permanentDamageBoost += 10;
    }
}

void Player::update()
{
    if (playerPlane)
    {
        playerPlane->move();
    }

    // 更新无敌状态计时器
    if (isInvincible)
    {
        invincibleTimer--;
        if (invincibleTimer <= 0)
        {
            isInvincible = false;
            playerPlane->isInvincible = false;
        }
    }

    // 更新子弹伤害提升计时器
    if (bulletDamageBoostActive) {
        bulletDamageBoostTimer--;
        if (bulletDamageBoostTimer <= 0) {
            bulletDamageBoostActive = false;
            additionalBulletDamage = 0;
        }
    }

    // 处理所有boost计时器
    for (int i = boostTimers.size() - 1; i >= 0; --i) {
        boostTimers[i]--;
        if (boostTimers[i] <= 0) {
            // 临时效果结束，应用待处理的永久属性提升
            permanentSpawnReduction += pendingSpawnReduction;
            permanentDamageBoost += pendingDamageBoost;
            pendingSpawnReduction = 0;
            pendingDamageBoost = 0;

            // 更新原始发射间隔
            originalSpawnInterval = 500 - permanentSpawnReduction;

            // 检查是否需要增加弹道
            if (originalSpawnInterval <= 250) {
                bulletCount++;
                originalSpawnInterval = 500; // 重置攻击间隔
                permanentSpawnReduction = 0; // 重置减少量
            }

            boostTimers.removeAt(i);
        }
    }

    // 设置临时攻速提升
    bulletSpawnInterval = originalSpawnInterval;
    if (!boostTimers.isEmpty()) {
        bulletSpawnInterval /= 2;
    }

    // 更新子弹生成计时器
    bulletSpawnTimer += 20; // 每次更新间隔20ms
    if (bulletSpawnTimer >= bulletSpawnInterval) {
        shoot();
        bulletSpawnTimer = 0;
    }

    // 更新子弹位置并移除超出边界的子弹
    for (int i = 0; i < bullets.size(); ) {
        Bullet *bullet = bullets[i];
        bullet->update();
        if (bullet->isOutOfBounds(windowHeight)) {
            bullets.removeAt(i);
            delete bullet;
        } else {
            i++;
        }
    }
}

void Player::mouseReleaseEvent(QMouseEvent *event)
{
    // 鼠标释放事件处理
}

void Player::shoot()
{
    if (bulletImage.isNull()) return;

    int bulletWidth = bulletImage.width();
    int bulletHeight = bulletImage.height();

    // 计算基准子弹位置（玩家飞机中心偏上）
    int baseX = playerPlane->x + (playerPlane->width - bulletWidth) / 2;
    int y = playerPlane->y - bulletHeight;

    // 根据子弹强化状态设置速度和伤害
    int bulletSpeed = !boostTimers.isEmpty() ? originalBulletSpeed * 2 : originalBulletSpeed;
    int baseDamage = originalBulletDamage + permanentDamageBoost;
    int bulletDamage = !boostTimers.isEmpty() ? baseDamage        * 2 : baseDamage;
    if (bulletDamageBoostActive) {
        bulletDamage += additionalBulletDamage;
    }

    // 多弹道发射
    for (int i = 0; i < bulletCount; i++) {
        // 计算每个弹道相对最左边弹道的相对水平偏移（间隔0像素）
        int offset =  bulletWidth * i ;
        // 计算最左边弹道的x加上偏移（先在右边0像素处生成子弹，再整体左移bulletWidth/2像素）
        int x = baseX + offset - bulletWidth / 2 * (bulletCount - 1) ;
        Bullet *bullet = new Bullet(QPoint(x, y), bulletSpeed, bulletImage, bulletDamage);
        bullets.append(bullet);
    }
}
