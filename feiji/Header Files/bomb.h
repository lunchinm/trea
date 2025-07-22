#ifndef BOMB_H
#define BOMB_H

#include <QObject>
#include <QPixmap>
#include <QPoint>

class Bomb : public QObject
{
    Q_OBJECT
public:
    explicit Bomb(QPoint position, int size, QObject *parent = nullptr);
    void update();
    void draw(QPainter &painter);
    bool isFinished() const { return m_isFinished; }

private:
    QList<QPixmap> m_frames;
    int m_currentFrame;
    int m_frameDelay; // 每帧显示的游戏循环次数
    int m_frameTimer;
    QPoint m_position;
    int m_size; // 爆炸动画尺寸
    bool m_isFinished;
};

#endif // BOMB_H
