//
// Created by Gerwa on 2017/9/14.
//

#ifndef PLAYGWENT_ANIMATEDELLIPSE_H
#define PLAYGWENT_ANIMATEDELLIPSE_H

#include <QObject>
#include <QGraphicsEllipseItem>

class AnimatedEllipse : public QObject, public QGraphicsEllipseItem {
Q_OBJECT
    Q_PROPERTY(QRectF rect
                       READ
                               rect
                       WRITE
                       setRect)
    Q_PROPERTY(QPointF pos
                       READ
                               pos
                       WRITE
                       setPos)
};


#endif //PLAYGWENT_ANIMATEDELLIPSE_H
