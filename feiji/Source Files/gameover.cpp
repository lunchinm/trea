#include "gameover.h"
#include "widget.h"
#include <QPainter>
#include <QKeyEvent>
#include <QFont>

GameOver::GameOver(Widget *widget, QObject *parent) : QObject(parent), widget(widget), currentScore(0), windowWidth(0), windowHeight(0), isPaused(false)
{
}

void GameOver::setCurrentScore(long long score)
{
    currentScore = score;
}

void GameOver::init(int windowWidth, int windowHeight, bool isPaused)
{
    this->windowWidth = windowWidth;
    this->windowHeight = windowHeight;
    this->isPaused = isPaused;

    // 按钮区域初始化
    int buttonWidth = 200;
    int buttonHeight = 60;
    int buttonY = windowHeight / 2 + (isPaused ? 20 : 50);

    restartButtonRect = QRect(windowWidth/2 - buttonWidth - 20, buttonY, buttonWidth, buttonHeight);
    homeButtonRect = QRect(windowWidth/2 + 20, buttonY, buttonWidth, buttonHeight);

    if (isPaused) {
        resumeButtonRect = QRect(windowWidth/2 - buttonWidth/2, windowHeight/2 - 60, buttonWidth, buttonHeight);
    }
}

void GameOver::draw(QPainter &painter)
{
    // 绘制背景半透明遮罩
    painter.fillRect(0, 0, windowWidth, windowHeight, QColor(0, 0, 0, 180));

    // 设置字体和绘制标题
    painter.setFont(QFont(":/new/prefix1/Doto.ttf", 36, QFont::Bold));
    painter.setPen(isPaused ? Qt::yellow : Qt::red);
    painter.drawText(QRect(0, windowHeight/4, windowWidth, 60), Qt::AlignCenter, isPaused ? "Game Paused" : "Game Over");

    // 绘制当前分数
    painter.setFont(QFont(":/new/prefix1/Doto.ttf", 24));
    painter.setPen(Qt::white);
    painter.drawText(QRect(0, windowHeight/4 + 80, windowWidth, 40), Qt::AlignCenter,
                     QString("Current Score: %1").arg(currentScore));

    // 绘制按钮
    painter.setBrush(Qt::green);
    painter.drawRect(restartButtonRect);
    painter.drawRect(homeButtonRect);

    if (isPaused) {
        painter.setBrush(Qt::blue);
        painter.drawRect(resumeButtonRect);
    }

    // 绘制按钮文字
    painter.setPen(Qt::white);
    painter.setFont(QFont(":/new/prefix1/Doto.ttf", 16, QFont::Bold));
    painter.drawText(restartButtonRect, Qt::AlignCenter, "Restart Level");
    painter.drawText(homeButtonRect, Qt::AlignCenter, "Back to Home");

    if (isPaused) {
        painter.drawText(resumeButtonRect, Qt::AlignCenter, "Resume Game");
    }

    // 绘制操作提示
    painter.setPen(Qt::gray);
    painter.setFont(QFont(":/new/prefix1/Doto.ttf", 14));
    painter.drawText(QRect(0, windowHeight/2 + 150, windowWidth, 30), Qt::AlignCenter,
                     isPaused ? "Press ESC to Resume" : "Press ESC to Return Home");
}

void GameOver::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape) {
        if (isPaused) {
            emit resumeGameClicked();
        } else {
            emit backToHomeClicked();
        }
    }
}

void GameOver::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton) {
        if (isRestartClicked(event->pos())) {
            emit restartLevelClicked();
        } else if (isHomeClicked(event->pos())) {
            emit backToHomeClicked();
        } else if (isPaused && resumeButtonRect.contains(event->pos())) {
            emit resumeGameClicked();
        }
    }
}

bool GameOver::isRestartClicked(const QPoint &pos)
{
    return restartButtonRect.contains(pos);
}

bool GameOver::isHomeClicked(const QPoint &pos)
{
    return homeButtonRect.contains(pos);
}

// 其他事件处理方法留空实现
void GameOver::keyReleaseEvent(QKeyEvent *event) {}
void GameOver::mouseReleaseEvent(QMouseEvent *event) {}