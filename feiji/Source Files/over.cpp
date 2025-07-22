#include "over.h"
#include "widget.h"
#include <QPainter>
#include <QKeyEvent>

Over::Over(Widget *widget, QObject *parent) : QObject(parent), widget(widget), finalScore(0), windowWidth(0), windowHeight(0)
{
}

void Over::setFinalScore(int score)
{
    finalScore = score;
}

void Over::init(int windowWidth, int windowHeight)
{
    this->windowWidth = windowWidth;
    this->windowHeight = windowHeight;
    // 设置重新开始按钮位置(居中)
    restartButtonRect = QRect(windowWidth/2 - 100, windowHeight/2 + 50, 200, 60);
}

void Over::draw(QPainter &painter)
{
    // 绘制背景半透明遮罩
    painter.fillRect(0, 0, windowWidth, windowHeight, QColor(0, 0, 0, 180));

    // 设置字体和绘制游戏结束标题
    painter.setFont(QFont("宋体", 40, QFont::Bold));
    painter.setPen(Qt::red);
    painter.drawText(QRect(0, windowHeight/4, windowWidth, 60), Qt::AlignCenter, "Game Over");

    // 绘制最终分数
    painter.setFont(QFont(":/new/prefix1/Doto.ttf", 24));
    painter.setPen(Qt::white);
    painter.drawText(QRect(0, windowHeight/2 - 30, windowWidth, 40), Qt::AlignCenter,
                     QString("最终分数: %1").arg(finalScore));

    //绘制重新开始按钮
    painter.setBrush(Qt::green);
    painter.drawRect(restartButtonRect);
    painter.setPen(Qt::white);
    painter.setFont(QFont(":/new/prefix1/Doto.ttf", 18, QFont::Bold));
    painter.drawText(restartButtonRect, Qt::AlignCenter, "Restart");

    //绘制操作提示
    painter.setPen(Qt::gray);
    painter.setFont(QFont(":/new/prefix1/Doto.ttf", 14));
    painter.drawText(QRect(0, windowHeight*3/4, windowWidth, 30), Qt::AlignCenter,
                     "按Enter键或点击按钮重新开始");
}

void Over::keyPressEvent(QKeyEvent *event)
{
    // 已通过信号处理页面切换，无需直接修改statuts
    if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter)
    {
        // 处理逻辑已在keyReleaseEvent中通过信号实现
    }
}

void Over::keyReleaseEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Return)
    {
        emit restartGameClicked();
    }
}

void Over::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton && isRestartClicked(event->pos()))
    {
        emit restartGameClicked();
    }
}

void Over::mouseReleaseEvent(QMouseEvent *event)
{
    // 鼠标释放事件处理逻辑
}

bool Over::isRestartClicked(const QPoint &pos)
{
    return restartButtonRect.contains(pos);
}
