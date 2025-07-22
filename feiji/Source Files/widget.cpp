#include "widget.h"
#include "ui_widget.h"
#include <QPainter>
#include <QKeyEvent>
#include <QDebug>

#include <ctime>
#include <cstdlib>

#include "before.h"
#include "play01.h"
#include "player.h"
#include "over.h"


Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::widget),
    currentPage(Page::Before)
{
    ui->setupUi(this);
    this->setWindowTitle("雷霆战机");
    this->resize(600,800);
    //    把最大宽和 高设置成相同
    this->setMinimumWidth(600);
    this->setMaximumWidth(600);
    this->setMinimumHeight(800);
    this->setMaximumHeight(800);
    srand(time(0));

    this->before = new Before(this, this);
    this->play01 = new Play01(this);
    this->player = new Player(this);
    this->over = new Over(this, this);
    this->gameOver = new GameOver(this, this);
    this->levelComplete = new LevelComplete(this, nullptr);
    this->isGamePaused = false;
    this->pauseButtonRect = QRect(width() - 60, 10, 50, 50);

    // 连接LevelComplete信号与槽
    connect(levelComplete, &LevelComplete::nextLevelClicked, this, &Widget::onNextLevelClicked);
    connect(levelComplete, &LevelComplete::restartLevelClicked, this, &Widget::onRestartLevelClicked);
    connect(levelComplete, &LevelComplete::backToHomeClicked, this, &Widget::onBackToHomeClicked);
    connect(before, &Before::startGameClicked, this, &Widget::onStartGameClicked);
    connect(over, &Over::restartGameClicked, this, &Widget::onRestartGameClicked);
    connect(gameOver, &GameOver::restartLevelClicked, this, &Widget::onRestartLevelClicked);
    connect(gameOver, &GameOver::backToHomeClicked, this, &Widget::onBackToHomeClicked);
    connect(gameOver, &GameOver::resumeGameClicked, this, &Widget::onResumeGameClicked);

    // 主界面不加载背景图片
    backgroundImage = QPixmap();
    backgroundImage2 = QPixmap();
    // 设置主界面背景色为黑色
    this->setStyleSheet("background-color: black;");
    // 初始化背景位置
    bgY1 = 0;
    bgY2 = -bgHeight;
    this->startTimer(20);//1秒后触发 某个函数

    // 初始化暂停按钮区域
    pauseButtonRect = QRect(width() - 60, 10, 50, 50);
}



int Widget::getRandomInt(int a, int b)
{
    int ranInt = (rand() % (b-a+1))+ a;
    return ranInt;
}
long long Widget::getScore() const {
    return play01 ? play01->getScore() : 0;
}

void Widget::onPauseGameClicked() {
    if (currentPage != Page::Play01 || isGamePaused) return;

    isGamePaused = true;
    killTimer(timerId); // 停止游戏计时器
    gameOver->setCurrentScore(getScore());
    gameOver->init(width(), height(), true); // true表示暂停状态
    currentPage = Page::GameOver;
    update();
}

void Widget::onResumeGameClicked()
{
    if (currentPage != Page::GameOver || !isGamePaused) return;

    isGamePaused = false;
    timerId = startTimer(20); // 重启游戏计时器
    currentPage = Page::Play01;
    isGamePaused = false;
    // 重置飞机移动状态
    player->getPlayerPlane()->resetMovementState();
    update();

}

void Widget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    // 绘制背景图片
    if (!backgroundImage.isNull()) {
        // 绘制第一张背景图片
        painter.drawPixmap(0, bgY1, width(), height(), backgroundImage.scaled(width(), height(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
        // 绘制第二张背景图片
        painter.drawPixmap(0, bgY2, width(), height(), backgroundImage2.scaled(width(), height(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    }
    switch(currentPage) {
    case Page::Before:
        before->draw(painter);
        break;
    case Page::Play01:
        play01->draw(painter);
        player->draw(painter);
        // 绘制暂停按钮
        painter.drawPixmap(pauseButtonRect, QPixmap(":/new/prefix1/res/pause.png").scaled(pauseButtonRect.width(), pauseButtonRect.height(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
        break;
    case Page::GameOver:
        gameOver->draw(painter);
        break;
    case Page::Over:
        over->draw(painter);
        break;
    case Page::LevelComplete:
        levelComplete->draw(painter);
        break;
    }

}

void Widget::keyPressEvent(QKeyEvent *event)
{
    switch(currentPage) {
    case Page::Before:
        before->keyPressEvent(event);
        break;
    case Page::Play01:
        if (event->key() == Qt::Key_Escape) {
            onPauseGameClicked();
        } else if (!isGamePaused) {
            play01->keyPressEvent(event);
            player->keyPressEvent(event);
        }
        break;
    case Page::Over:
        over->keyPressEvent(event);
        break;
    case Page::LevelComplete:
        levelComplete->keyPressEvent(event);
        break;
    case Page::GameOver:
        gameOver->keyPressEvent(event);
        break;
    }
}



void Widget::keyReleaseEvent(QKeyEvent *event)
{
    switch(currentPage) {
    case Page::Before:
        before->keyReleaseEvent(event);
        break;
    case Page::Play01:
        play01->keyReleaseEvent(event);
        player->keyReleaseEvent(event);
        break;
    case Page::Over:
        over->keyReleaseEvent(event);
        break;
    case Page::GameOver:
        gameOver->keyReleaseEvent(event);
        break;
    default:
        break;
    }
}

void Widget::mousePressEvent(QMouseEvent *event)
{
    switch(currentPage) {
    case Page::Before:
        before->mousePressEvent(event);
        break;
    case Page::Play01:
        if (pauseButtonRect.contains(event->pos())) {
            onPauseGameClicked();
        } else {
            player->mousePressEvent(event);
        }
        break;
    case Page::Over:
        over->mousePressEvent(event);
        break;
    case Page::LevelComplete:
        levelComplete->mousePressEvent(event);
        break;
    case Page::GameOver:
        gameOver->mousePressEvent(event);
        break;
    }
}

void Widget::mouseReleaseEvent(QMouseEvent *event)
{
    switch(currentPage) {
    case Page::Before:
        before->mouseReleaseEvent(event);
        break;
    case Page::Play01:
        player->mouseReleaseEvent(event);
        break;
    case Page::Over:
        over->mouseReleaseEvent(event);
        break;
    case Page::GameOver:
        gameOver->mouseReleaseEvent(event);
        break;
    default:
        break;
    }
}

void Widget::gameStart()
{
    // 安全停止现有计时器
    if (timerId != -1) {
        killTimer(timerId);
        timerId = -1;
    }

    if(currentPage == Page::Before)
    {
        // 返回首页，重置游戏实例
        delete play01;
        play01 = new Play01(this);
    }
    resetGameState(true);
}

void Widget::showLevelComplete(int currentLevel, long long score)
{
    levelComplete->setLevelInfo(currentLevel, score);
    levelComplete->init(this->width(), this->height());
    currentPage = Page::LevelComplete;
    update();
}

void Widget::onNextLevelClicked()
{
    int nextLevel = play01->getCurrentLevel();
    long long nextTarget = play01->getTargetScore();
    resetGameState(false, nextLevel, nextTarget);
    changeBackground(nextLevel);
}

void Widget::onRestartLevelClicked()
{
    resetGameState(false, play01->getCurrentLevel(), play01->getTargetScore());
}

void Widget::onBackToHomeClicked()
{
    currentPage = Page::Before;
    delete play01;
    play01 = new Play01(this);
    isGamePaused = false;
    // 重置背景为空白
    backgroundImage = QPixmap();
    backgroundImage2 = QPixmap();
    update();
}

void Widget::resetGameState(bool resetStats, int level, long long targetScore)
{
    currentPage = Page::Play01;
    isGamePaused = false;
    play01->setCurrentLevel(level);
    play01->setTargetScore(targetScore);
    play01->initGame(width(), height(), resetStats);
    // 强制重置生命值，解决负生命值问题
    player->initGame(width(), height(), true);
    // 加载对应关卡背景
    changeBackground(level);
    if (timerId != -1) killTimer(timerId);
    timerId = startTimer(20);
    update();
}

void Widget::onStartGameClicked()
{
    resetGameState(true, 1);
}

void Widget::onRestartGameClicked()
{
    resetGameState(true);
}

void Widget::timerEvent(QTimerEvent *event)
{
    // 只处理当前有效的计时器事件
    if (event->timerId() != timerId) {
        return;
    }
    if (currentPage == Page::Play01) {
        play01->update();
        player->update();
        // 更新背景位置
        bgY1 += bgSpeed;
        bgY2 += bgSpeed;
        // 检查背景是否需要重置
        if (bgY1 >= height()) {
            bgY1 = bgY2 - height();
        }
        if (bgY2 >= height()) {
            bgY2 = bgY1 - height();
        }
    }
    this->update();
}


Widget::~Widget()
{
    if (timerId != -1) {
        killTimer(timerId);
        timerId = -1;
    }
    delete ui;
    delete before;
    delete play01;
    delete player;
    delete over;
    delete levelComplete;
}

void Widget::changeBackground(int level)
{
    QString imagePath;
    switch(level) {
    case 1:
        imagePath = ":/new/prefix1/res/img_bg_level_1.jpg";
        break;
    case 2:
        imagePath = ":/new/prefix1/res/img_bg_level_2.jpg";
        break;
    case 3:
        imagePath = ":/new/prefix1/res/img_bg_level_3.jpg";
        break;
    case 4:
        imagePath = ":/new/prefix1/res/img_bg_level_4.jpg";
        break;
    case 5:
        imagePath = ":/new/prefix1/res/img_bg_level_5.jpg";
        break;
    default:
        imagePath = ":/new/prefix1/res/img_bg_level_1.jpg";
        break;
    }

    if (!backgroundImage.load(imagePath)) {
        qDebug() << "Failed to load background image for level" << level << ":" << imagePath;
        // 尝试加载默认背景图
        if (!backgroundImage.load(":/new/prefix1/res/img_bg_level_1.jpg")) {
            qDebug() << "Failed to load default background image!";
        }
    } else {
        qDebug() << "Successfully loaded background image for level" << level << ":" << imagePath;
    }
    // 同时更新第二张背景图
    if (!backgroundImage2.load(imagePath)) {
        qDebug() << "Failed to load second background image for level" << level << ":" << imagePath;
        // 尝试加载默认背景图
        if (!backgroundImage2.load(":/new/prefix1/res/img_bg_level_1.jpg")) {
            qDebug() << "Failed to load default second background image!";
        }
    } else {
        qDebug() << "Successfully loaded second background image for level" << level << ":" << imagePath;
    }
    this->update(); // 强制重绘以显示新背景
}

