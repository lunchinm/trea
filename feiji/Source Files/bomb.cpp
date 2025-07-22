#include "bomb.h"
#include <QPixmap>
#include <QPainter>
#include <QDebug>

Bomb::Bomb(QPoint position, int size, QObject *parent) : QObject(parent),
    m_position(position), m_size(size), m_currentFrame(0), m_frameTimer(0), m_isFinished(false)
{
    // 计算每帧显示的游戏循环次数（总时长1秒，游戏循环间隔20ms，共50帧）
    // 7张图片分配50帧：前6张各7帧，最后1张8帧
    m_frameDelay = (m_currentFrame < 6) ? 7 : 8;

    // 加载爆炸动画帧
    for (int i = 1; i <= 7; ++i) {
        QString path = QString(":/new/prefix1/res/bomb-%1.png").arg(i);
        QPixmap frame;
        if (frame.load(path)) {
            m_frames.append(frame.scaled(m_size, m_size, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        } else {
            qDebug() << "Failed to load bomb frame: " << path;
            m_frames.append(QPixmap(m_size, m_size)); // 创建空 pixmap 作为占位符
        }
    }
}

void Bomb::update()
{
    if (m_isFinished || m_frames.isEmpty()) return;

    m_frameTimer++;
    if (m_frameTimer >= m_frameDelay) {
        m_frameTimer = 0;
        m_currentFrame++;

        // 更新当前帧的延迟（最后一帧显示8帧）
        if (m_currentFrame < m_frames.size()) {
            m_frameDelay = (m_currentFrame < 6) ? 7 : 8;
        }

        // 检查动画是否完成
        if (m_currentFrame >= m_frames.size()) {
            m_isFinished = true;
        }
    }
}

void Bomb::draw(QPainter &painter)
{
    if (m_isFinished || m_currentFrame >= m_frames.size()) return;

    // 绘制当前帧，居中显示
    QPoint drawPos = m_position - QPoint(m_size / 2, m_size / 2);
    painter.drawPixmap(drawPos, m_frames[m_currentFrame]);
}
