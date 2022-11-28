//
// Created by Gerwa on 2017/10/5.
//

#include "TimeWidget.h"
#include "CardWidget.h"

TimeWidget::TimeWidget() {
    timerRect.setParentItem(this);
    timerRect.setBrush(Qt::cyan);

    timerIcon.setPixmap(
            CardWidget::getPixmap("assets/ui/timer-icon_player.tex.png").scaledToWidth(65, Qt::SmoothTransformation));
    timerIcon.setParentItem(this);
    timerIcon.setPos(0, 0);

    unnameIcon.setPixmap(
            CardWidget::getPixmap("assets/ui/unnamed.tex.png").scaledToWidth(80, Qt::SmoothTransformation));
    unnameIcon.setParentItem(this);
    unnameIcon.setPos(762, -8);

    textItem.setParentItem(this);
    textItem.setPos(774 + 10, 4 + 7);
    textItem.setFont(QFont("Arial", 16, 80, false));
    textItem.setDefaultTextColor(Qt::lightGray);

    rectAnimation.setTargetObject(&timerRect);
    rectAnimation.setPropertyName("rect");

    timer.setInterval(1000);
    connect(&timer, &QTimer::timeout, this, &TimeWidget::timerUpdate);
}

void TimeWidget::startTimer() {
    rectAnimation.start();
    timer.start();
}

void TimeWidget::timerUpdate() {
    timeLeft--;
    textItem.setPlainText(QString::number(timeLeft));

    if (timeLeft == 0) {
        emit timeup();
    }
}

void TimeWidget::stopTimer() {
    timer.stop();
    rectAnimation.stop();
}

void TimeWidget::resetTimer(int timeLeft) {
    this->timeLeft = timeLeft;
    textItem.setPlainText(QString::number(timeLeft));

    timerRect.setRect(63, 24, 695, 15);
    rectAnimation.setEndValue(QRectF(758, 24, 0, 15));
    rectAnimation.setDuration(timeLeft * 1000);
}
