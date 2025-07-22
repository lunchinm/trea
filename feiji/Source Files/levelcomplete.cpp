#include "levelcomplete.h"
#include "widget.h"
#include <QPainter>
#include <QKeyEvent>
#include <QDebug>

LevelComplete::LevelComplete(Widget *widget, QObject *parent) : QObject(parent), widget(widget), currentLevel(0), score(0), windowWidth(0), windowHeight(0)
{
}

void LevelComplete::setLevelInfo(int currentLevel, long long score)
{
    this->currentLevel = currentLevel;
    this->score = score;
}

void LevelComplete::init(int windowWidth, int windowHeight)
{
    this->windowWidth = windowWidth;
    this->windowHeight = windowHeight;

    // 按钮尺寸和位置
    int buttonWidth = 200;
    int buttonHeight = 60;
    int spacing = 30;
    int startY = windowHeight / 2 + 40;

    nextLevelButton = QRect(windowWidth/2 - buttonWidth/2, startY, buttonWidth, buttonHeight);
    restartLevelButton = QRect(windowWidth/2 - buttonWidth/2, startY + buttonHeight + spacing, buttonWidth, buttonHeight);
    backToHomeButton = QRect(windowWidth/2 - buttonWidth/2, startY + (buttonHeight + spacing)*2, buttonWidth, buttonHeight);
}

void LevelComplete::draw(QPainter &painter)
{
    // 绘制背景半透明遮罩
    painter.fillRect(0, 0, windowWidth, windowHeight, QColor(0, 0, 0, 180));

    // 绘制标题
    painter.setPen(Qt::yellow);
    painter.setFont(QFont(":/new/prefix1/Doto.ttf", 36, QFont::Bold));
    painter.drawText(QRect(0, windowHeight/4, windowWidth, 60), Qt::AlignCenter, QString("Level %1 Complete!").arg(currentLevel));

    // 绘制分数
    painter.setPen(Qt::white);
    painter.setFont(QFont(":/new/prefix1/Doto.ttf", 24));
    painter.drawText(QRect(0, windowHeight/4 + 80, windowWidth, 40), Qt::AlignCenter, QString("Current Score: %1").arg(score));

    // 绘制按钮
    painter.setFont(QFont(":/new/prefix1/Doto.ttf", 18, QFont::Bold));

    // 下一关按钮
    painter.setBrush(Qt::green);
    painter.drawRect(nextLevelButton);
    painter.setPen(Qt::white);
    painter.drawText(nextLevelButton, Qt::AlignCenter, "Next Level");

    // 重新开始按钮
    painter.setBrush(Qt::blue);
    painter.drawRect(restartLevelButton);
    painter.drawText(restartLevelButton, Qt::AlignCenter, "Restart Level");

    // 返回首页按钮
    painter.setBrush(Qt::red);
    painter.drawRect(backToHomeButton);
    painter.drawText(backToHomeButton, Qt::AlignCenter, "Back to Home");
}

void LevelComplete::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter)
    {
        // 默认按回车进入下一关
        emit nextLevelClicked();
    }
}

void LevelComplete::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        if (isNextLevelClicked(event->pos()))
        {
            emit nextLevelClicked();
        }
        else if (isRestartLevelClicked(event->pos()))
        {
            emit restartLevelClicked();
        }
        else if (isBackToHomeClicked(event->pos()))
        {
            emit backToHomeClicked();
        }
    }
}

bool LevelComplete::isNextLevelClicked(const QPoint &pos)
{
    return nextLevelButton.contains(pos);
}

bool LevelComplete::isRestartLevelClicked(const QPoint &pos)
{
    return restartLevelButton.contains(pos);
}

bool LevelComplete::isBackToHomeClicked(const QPoint &pos)
{
    return backToHomeButton.contains(pos);
}
