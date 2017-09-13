//
// Created by Gerwa on 2017/9/14.
//

#ifndef PLAYGWENT_ANIMATEDRECT_H
#define PLAYGWENT_ANIMATEDRECT_H

#include <QObject>
#include <QGraphicsRectItem>

class AnimatedRect : public QObject, public QGraphicsRectItem {
Q_OBJECT
    Q_PROPERTY(QRectF rect
                       READ
                               rect
                       WRITE
                       setRect)
};


#endif //PLAYGWENT_ANIMATEDRECT_H
