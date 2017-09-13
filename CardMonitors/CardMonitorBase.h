//
// Created by Gerwa on 2017/9/8.
//

#ifndef PLAYGWENT_CARDMONITORBASE_H
#define PLAYGWENT_CARDMONITORBASE_H

#include <QObject>
#include "../CardInfo.h"
#include "../GameConstant.h"

class CardMonitorBase : public QObject {
Q_OBJECT
public slots:

    virtual void slotAfterMoveForUser(CardInfo *mover, int fromR, int toR) {
        //do nothing
    }

    virtual void slotBeforeMoveForUser(CardInfo *mover, int fromR, int toR) {
        //do nothing
    }

    virtual void slotBeforeDeploy(CardInfo *mover, int fromR, int toR) {
        //do nothing
    }

    virtual void slotOnDeploy(CardInfo *mover, int fromR, int toR) {
        //do nothing
    }

    virtual void slotOnDeathWish(CardInfo *mover, int fromR, int toR) {
        //do nothing
    }

    virtual void slotOnTurnStart() {
        //do nothing
    }//you can get the current player from assets

    virtual void slotOnHandSeleted(CardInfo *seletedCard, bool allowCancel) {
        //default behavior: choose a row to deploy, but must be called in that subclass
    }

    virtual void slotAfterDeploy(CardInfo *mover, int fromR, int toR) {
        //do nothing
    }

    virtual void slotAfterConsume(CardInfo *consumer, CardInfo *food) {

    }

    virtual void slotOnConsume(CardInfo *consumer, CardInfo *food) {

    }

    virtual void slotAfterWeatherChanged(int row, Weather oldWeather) {
        //do nothing
    }

    virtual void slotOnWeatherDamage(int row, const QList<CardInfo *> &dests) {

    }
};


#endif //PLAYGWENT_CARDMONITORBASE_H
