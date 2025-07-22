#include "play01.h"
#include "widget.h"
#include <ctime>
#include <cstdlib>

Play01::~Play01()
{
    // 释放所有爆炸效果资源
    qDeleteAll(bombs);
    bombs.clear();
}

Play01::Play01(Widget *widget)
    : widget(widget), enemySpeed(1), spawnInterval(2000), spawnTimer(0), score(0), isPaused(false), playerPos(QPoint(0, 0)), timerId(-1)
{
    srand(time(0)); // 初始化随机数种子
}

void Play01::initGame(int windowWidth, int windowHeight, bool resetLevel)
{
    this->windowWidth = windowWidth;
    this->windowHeight = windowHeight;
    enemies.clear(); // 清空敌机列表
    score = 0;       // 重置分数
    if (resetLevel) {
        currentLevel = 1; // 初始化为第一关
        targetScore = 5000; // 第一关目标分数5000
    }
    // 初始化分数动画
    scoreAnimation = {0, 0, 0, 0, 30}; // start, target, current, timer, duration
    spawnTimer = 0;  // 重置生成计时器
}

void Play01::update()
{
    spawnTimer += 20; // 累加定时器间隔(20ms)
    static bool leveledUpThisFrame = false;
    leveledUpThisFrame = false; // 重置每帧升级状态

    // 生成新敌机
    if (spawnTimer >= spawnInterval)
    {
        int enemyType = getRandomEnemyType();
        Enemy *enemy = new Enemy(enemyType, currentLevel, nullptr);
        // 随机X坐标(确保敌机在窗口内)
        int x = getRandomInt(enemy->size/2, windowWidth - enemy->size/2);
        enemy->pos = QPoint(x, 0);
        enemies.append(enemy);
        spawnTimer = 0;
    }

    // 获取玩家子弹列表
    QList<Bullet*>& bullets = widget->player->getBullets();

    // 碰撞检测 A: 玩家子弹与敌机
    for (int i = bullets.size() - 1; i >= 0; --i)
    {
        Bullet* bullet = bullets[i];
        QRect bulletRect(bullet->getPosition(), QSize(10, 20));

        for (int j = 0; j < enemies.size(); ++j)
        {
            Enemy* enemy = enemies[j];
            if (!enemy->isAlive) continue;

            if (bulletRect.intersects(enemy->getCollisionRect()))
            {
                // 子弹对敌机造成伤害（含百分比加成）
                int bulletDamage = bullet->getDamage();
                int percentMax = (enemy->originalHealth * widget->player->getMaxHealthDamagePercent()) / 100;
    int percentLost = ((enemy->originalHealth - enemy->currentHealth) * widget->player->getLostHealthDamagePercent()) / 100;
    int permanentDamage = widget->player->getPermanentDamageBoost();
    int totalDamage = bulletDamage + percentMax + percentLost + permanentDamage;
if (widget->player->isBoostActive()) {
    totalDamage *= 2; // 临时效果期间伤害翻倍（含永久加成）
}
enemy->hit(totalDamage);

                // 移除子弹
                bullets.removeAt(i);
                delete bullet;

                // 检查敌机是否被摧毁
                if (enemy->currentHealth <= 0)
                {
                    // 添加爆炸效果
                    bombs.append(new Bomb(enemy->pos, enemy->size));
                    enemy->isAlive = false;
                    // 转移敌机子弹到全局列表
                    QList<EnemyBullet*>& enemyBulletsList = enemy->getBullets();
                    enemyBullets.append(enemyBulletsList);
                    enemyBulletsList.clear();

                    // 基于关卡的动态道具掉落概率
                    int currentLevel = this->currentLevel; // 获取当前关卡
                    int baseProbability = currentLevel * 20; // 每关增加20%
                    int extraBoostProbability = qMax(0, baseProbability - 100); // 超出100%的部分
                    baseProbability = qMin(100, baseProbability); // 基础概率上限100%

                    // 基础概率生成普通道具
                    if (rand() % 100 < baseProbability) {
                        // 确保PowerUpType枚举包含MAX_HEALTH_BOOST
                        PowerUpType type = static_cast<PowerUpType>(rand() % 4);
                        PowerUp* powerup = new PowerUp(type, enemy->pos);
                        powerups.append(powerup);
                    }

                    // 超出100%的部分生成boost道具
                    if (extraBoostProbability > 0 && rand() % 100 < extraBoostProbability) {
                        PowerUp* boostPowerup = new PowerUp(BULLET_BOOST, enemy->pos); // 使用正确的枚举值BULLET_BOOST
                        powerups.append(boostPowerup);
                    }

                    // 加分
                    // 使用基础生命值×2倍分数乘数计算得分
                    int baseHealth = enemy->originalHealth / enemy->getMultiplier();
                    int addScore = baseHealth * enemy->getDamageScoreMultiplier();
                    score += addScore;
                    scorePopups.append({QString("+%1").arg(addScore), enemy->pos, 0, 30});
                    updateScoreAnimation();
                }

                break; // 一个子弹只能击中一个敌机
            }
        }
    }

    // 碰撞检测 B: 敌机子弹与玩家
    Plane* playerPlane = widget->player->getPlayerPlane();
    if (playerPlane && !playerPlane->isDestroyed)
    {
        QRect playerRect(
            playerPlane->x,
            playerPlane->y,
            playerPlane->width,
            playerPlane->height
            );

        for (Enemy* enemy : enemies)
        {
            if (!enemy->isAlive) continue;

            QList<EnemyBullet*>& enemyBullets = enemy->getBullets();
            for (int i = enemyBullets.size() - 1; i >= 0; --i)
            {
                EnemyBullet* bullet = enemyBullets[i];
                QRect bulletRect(bullet->getPosition(), bullet->getImage().size());

                if (bulletRect.intersects(playerRect))
                {
                    // 添加子弹碰撞爆炸效果（无论是否无敌都显示）
                    bombs.append(new Bomb(bullet->getPosition(), 40));

                    // 玩家受伤
                    bool wasInvincible = playerPlane->isInvincible;
                    if (!wasInvincible) {
                        playerPlane->health -= bullet->getDamage();
                        if (playerPlane->health <= 0)
                        {
                            playerPlane->isDestroyed = true;
                            widget->gameOver->setCurrentScore(score);
                        widget->gameOver->init(widget->width(), widget->height(), false);
                        widget->currentPage = Widget::Page::GameOver;
                            bombs.append(new Bomb(QPoint(playerPlane->x + playerPlane->width/2, playerPlane->y + playerPlane->height/2), playerPlane->width));
                        }
                    }
                    // 每次被击中都重置2秒无敌时间
                    widget->player->setInvincible(true, 2000);

                    // 移除子弹
                    enemyBullets.removeAt(i);
                    delete bullet;
                }
                else if (bullet->isOutOfBounds(windowHeight))
                {
                    // 子弹超出边界
                    enemyBullets.removeAt(i);
                    delete bullet;
                }
            }
        }
    }

    // 更新并检测全局敌机子弹碰撞
    for (int i = enemyBullets.size() - 1; i >= 0; --i) {
        EnemyBullet* bullet = enemyBullets[i];
        bullet->update();

        // 碰撞检测
        if (playerPlane && !playerPlane->isDestroyed) {
            QRect playerRect(playerPlane->x, playerPlane->y, playerPlane->width, playerPlane->height);
            if (playerRect.intersects(bullet->getCollisionRect())) {
                // 添加爆炸效果
                bombs.append(new Bomb(bullet->getPosition(), 40));

                // 处理伤害
                bool wasInvincible = playerPlane->isInvincible;
                if (!wasInvincible) {
                    playerPlane->health -= bullet->getDamage();
                    if (playerPlane->health <= 0) {
                        playerPlane->isDestroyed = true;
                        widget->gameOver->setCurrentScore(score);
                    widget->gameOver->init(widget->width(), widget->height(), false);
                    widget->currentPage = Widget::Page::GameOver;
                        bombs.append(new Bomb(QPoint(playerPlane->x + playerPlane->width/2, playerPlane->y + playerPlane->height/2), playerPlane->width));
                    }
                }
                // 设置无敌状态
                widget->player->setInvincible(true, 2000);

                // 移除子弹
                enemyBullets.removeAt(i);
                delete bullet;
                continue;
            }
        }

        // 边界检测
        if (bullet->isOutOfBounds(windowHeight)) {
            enemyBullets.removeAt(i);
            delete bullet;
        }
    }

    // 更新敌机和子弹
    for (Enemy* enemy : enemies)
    {
        if (enemy->isAlive)
        {
            enemy->update();
            // 检查敌机是否超出边界
            if (enemy->isOutOfBounds(windowHeight))
            {
                // 敌机飞出屏幕底部，扣分
                int deductScore = enemy->currentHealth;
                // 启动分数变化动画
                scoreAnimation.startValue = score;
                score -= deductScore;
                scoreAnimation.targetValue = score;
                scoreAnimation.timer = 0;
                scorePopups.append({QString("-%1").arg(deductScore), enemy->pos, 0, 30});

                // 创建爆炸效果
                bombs.append(new Bomb(enemy->pos, enemy->size));
                enemy->isAlive = false;
            }
        }
    }

    // 敌机血量动画已在Enemy::update()中处理

    // 更新爆炸效果

    // 更新爆炸效果
    for (int bombIndex = 0; bombIndex < bombs.size();) {
        bombs[bombIndex]->update();
        if (bombs[bombIndex]->isFinished()) {
            delete bombs[bombIndex];
            bombs.removeAt(bombIndex);
        } else {
            bombIndex++;
        }
    }

    // 更新道具
    for (int i = 0; i < powerups.size(); ) {
        PowerUp* powerup = powerups[i];
        powerup->update();
        if (powerup->isOutOfBounds(windowHeight) || !powerup->isActive) {
            powerups.removeAt(i);
            delete powerup;
        } else {
            i++;
        }
    }

    // 检测玩家与道具碰撞
    if (playerPlane && !playerPlane->isDestroyed) {
        QRect playerRect(playerPlane->x, playerPlane->y, playerPlane->width, playerPlane->height);

        for (int i = 0; i < powerups.size(); ) {
            PowerUp* powerup = powerups[i];
            if (powerup->isActive && playerRect.intersects(powerup->getCollisionRect())) {
                // 应用道具效果
                switch(powerup->type) {
                case 0: // HEALTH_RESTORE
                    widget->player->restoreHealth(50);
                    break;
                case 1: // MAX_HEALTH_BOOST
                    widget->player->increaseMaxHealth(20);
                    break;
                case 2: // INVINCIBILITY
                    widget->player->setInvincible(true, 3000);
                    break;
                case 3: // BULLET_BOOST
                    widget->player->boostBullets(3000);
                    widget->player->boostBulletDamage(3000);
                    break;
                }
                powerup->isActive = false;
                powerups.removeAt(i);
                delete powerup;
            } else {
                i++;
            }
        }
    }

    // 更新爆炸效果

    for (int i = 0; i < bombs.size();) {
        bombs[i]->update();
        if (bombs[i]->isFinished()) {
            delete bombs[i];
            bombs.removeAt(i);
        } else {
            i++;
        }
    }

    // 更新分数动画
    for (int i = 0; i < scorePopups.size(); )
    {
        ScorePopup& popup = scorePopups[i];
        popup.timer++;
        if (popup.timer >= popup.duration)
        {
            scorePopups.removeAt(i);
        }
        else
        {
            i++;
        }
    }

    // 更新分数动画显示
    if (scoreAnimation.timer < scoreAnimation.duration) {
        float progress = static_cast<float>(scoreAnimation.timer) / scoreAnimation.duration;
        scoreAnimation.currentValue = scoreAnimation.startValue +
                                      static_cast<int>((scoreAnimation.targetValue - scoreAnimation.startValue) * progress);
        scoreAnimation.timer++;
    } else {
        scoreAnimation.currentValue = scoreAnimation.targetValue;
    }

    // 清理死亡敌机
    for (int i = 0; i < enemies.size(); ) {
        if (!enemies[i]->isAlive) {
            delete enemies[i];
            enemies.removeAt(i);
        } else {
            i++;
        }
    }

    // 检查关卡升级，确保每次更新仅提升一级
    if (!leveledUpThisFrame && score >= targetScore && currentLevel < 5) {
        leveledUpThisFrame = true;
        // 提升关卡难度
        currentLevel++;
        targetScore *= 10; // 每关目标分数10倍增幅
        enemySpeed = 1 + (currentLevel - 1) * 0.5; // 每关增加0.5速度
        spawnInterval = qMax(500, 2000 - (currentLevel - 1) * 300); // 每关减少300ms生成间隔
        // 清除所有敌机、子弹和爆炸效果
        enemies.clear();
        qDeleteAll(bombs);
        bombs.clear();
        widget->player->getBullets().clear();
        // 隐藏玩家飞机
        widget->player->playerPlane->x = -1000;
        widget->player->playerPlane->y = -1000;
        widget->currentPage = Widget::Page::LevelComplete;
        widget->showLevelComplete(currentLevel - 1, score);
        return; // 升级后立即退出更新函数，防止重复升级
    }
}

void Play01::draw(QPainter &painter)
{
    // 绘制分数
    painter.setPen(Qt::white);
    painter.setFont(QFont(":/new/prefix1/Doto.ttf", 16));
    painter.drawText(20, 30, QString("Score: %1").arg(scoreAnimation.currentValue));

    // 绘制敌机
    for (const Enemy *enemy : enemies)
    {
        if (enemy->isAlive)
        {
            enemy->draw(painter);
        }
    }

    // 绘制敌机子弹
    for (EnemyBullet* bullet : enemyBullets) {
        bullet->draw(painter);
    }

    // 绘制所有爆炸效果
    for (Bomb *bomb : bombs) {
        bomb->draw(painter);
    }

    // 绘制道具
    for (const PowerUp* powerup : powerups) {
        powerup->draw(painter);
    }

    // 绘制分数变化动画
    painter.setFont(QFont(":/new/prefix1/Doto.ttf", 14, QFont::Bold));
    for (const ScorePopup& popup : scorePopups)
    {
        // 计算透明度 (随时间淡出)
        int alpha = 255 * (1 - (float)popup.timer / popup.duration);
        // 计算Y轴偏移 (随时间上升)
        int yOffset = -20 * (float)popup.timer / popup.duration;

        painter.setPen(QColor(255, 215, 0, alpha)); // 金色文字
        painter.drawText(
            popup.position.x() - 20,
            popup.position.y() + yOffset - 30,
            popup.text
            );
    }
}

int Play01::getRandomInt(int min, int max)
{
    return rand() % (max - min + 1) + min;
}

int Play01::getRandomEnemyType()
{
    // 根据关卡调整敌机类型概率，关卡越高高级敌机概率越大
    int randValue = getRandomInt(1, 100);
    int typeProbability = qMin(currentLevel * 20 - 20 , 70); // 最高70%概率出现高级敌机

    if (randValue <= typeProbability) {
        // 高级敌机 (4-7)
        return getRandomInt(5, 7);
    } else if (randValue <= typeProbability + 20) {
        // 中级敌机 (2-3)
        return getRandomInt(2, 4);
    } else {
        // 初级敌机 (1)
        return 1;
    }
}

void Play01::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_O) {

        qDebug()<<"启动破解版隐藏功能，已为您生成所有道具一份";

        // 生成所有类型道具
        for (int i = 0; i < 4; ++i) {
            PowerUpType type = static_cast<PowerUpType>(i);
            int x = getRandomInt(40, windowWidth - 40);
            PowerUp* powerup = new PowerUp(type, QPoint(x, 0));
            powerups.append(powerup);
        }
    }


    if (event->key() == Qt::Key_P) {

        qDebug()<<"启动破解版隐藏功能，已自动为您开启下一关";

        // 设置分数为当前关卡目标分数
        qDebug()<<"p";
        int addScore = targetScore - score;
        score = targetScore;
        // 更新分数动画
        scoreAnimation.startValue = score - addScore;
        scoreAnimation.targetValue = score;
        scoreAnimation.timer = 0;
        // 添加分数变化动画效果
        if (addScore > 0) {
            scorePopups.append({QString("+%1").arg(addScore), QPoint(windowWidth/2, windowHeight/2), 0, 30});
        }
    }
}

void Play01::keyReleaseEvent(QKeyEvent *event)
{
    // 暂停功能已移至Widget类处理
}

int Play01::getEnemySpeed() const
{
    return enemySpeed;
}

void Play01::updateScoreAnimation()
{
    scoreAnimation.startValue = scoreAnimation.currentValue;
    scoreAnimation.targetValue = score;
    scoreAnimation.timer = 0;
}

