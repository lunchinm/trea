#include "plane.h"
#include "config.h"
#include <QDebug>

void Plane::draw(QPainter &painter)
{
    if (image.isNull()) {
        painter.fillRect(x, y, width, height, Qt::blue); // 图像加载失败时绘制蓝色矩形作为占位符
    } else {
        QPixmap scaledImage = image.scaled(width, height, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        int scaledWidth = scaledImage.width();
        int scaledHeight = scaledImage.height();
        int drawX = x + (width - scaledWidth) / 2;
        int drawY = y + (height - scaledHeight) / 2;
        painter.drawPixmap(drawX, drawY, scaledImage);
    }
    this->move();
}

void Plane::keyPressEvent(QKeyEvent *event)
{
    int code = event->key();
    if(code == Qt::Key_W ){
        isUp = true;
        qDebug()<<"w1";
    }else if(code == Qt::Key_S ){
        isDown = true;
        qDebug()<<"s1";
    }else if(code == Qt::Key_A ){
        isLeft = true;
        qDebug()<<"a1";
    }else if(code == Qt::Key_D ){
        isRight = true;
        qDebug()<<"d1";
    }
}

void Plane::keyReleaseEvent(QKeyEvent *event)
{
    int code = event->key();
    if(code == Qt::Key_W ){
        isUp = false;
        qDebug()<<"w2";
    }else if(code == Qt::Key_S ){
        isDown = false;
        qDebug()<<"s2";
    }else if(code == Qt::Key_A ){
        isLeft = false;
        qDebug()<<"a2";
    }else if(code == Qt::Key_D ){
        isRight = false;
        qDebug()<<"d2";
    }

}

void Plane::resetMovementState()
{
    isUp = false;
    isDown = false;
    isLeft = false;
    isRight = false;
}

void Plane::move()
{

    if(isUp && y > 0)
        y -= PlANE_MOVE_SPEED;
    if(isDown && y < 800 - height)
        y += PlANE_MOVE_SPEED;
    if(isLeft && x > 0)
        x -= PlANE_MOVE_SPEED;
    if(isRight && x < 600 - width)
        x += PlANE_MOVE_SPEED;
}

Plane::Plane() : x(200), y(650),health(150), maxHealth(150), isInvincible(false), isDestroyed(false),
    isUp(false), isDown(false), isLeft(false), isRight(false)
{
    if (!image.load(":/new/prefix1/res/hero.png")) {
        qDebug() << "Failed to load player plane image!";
        // 如果图片加载失败，创建一个红色占位符
        image = QPixmap(width, height);
        image.fill(Qt::red);
    } else {
        // 确保图片缩放到指定尺寸
        image = image.scaled(width, height, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    }
}
