//
// Created by Gerwa on 2017/10/5.
//

#ifndef PLAYGWENT_TIMEWIDGET_H
#define PLAYGWENT_TIMEWIDGET_H

#include <QGraphicsWidget>
#include <QtCore/QTimer>
#include <QPropertyAnimation>
#include "AnimatedRect.h"

class TimeWidget : public QGraphicsWidget {
Q_OBJECT
private:
    QGraphicsPixmapItem timerIcon, unnameIcon;
    AnimatedRect timerRect;
    QPropertyAnimation rectAnimation;
    QTimer timer;
    QGraphicsTextItem textItem;
    int timeLeft;
public:
    TimeWidget();

    void resetTimer(int timeLeft);

    void startTimer();

    void stopTimer();

signals:

    void timeup();

public slots:

    void timerUpdate();
};


#endif //PLAYGWENT_TIMEWIDGET_H
