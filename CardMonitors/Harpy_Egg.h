//
// Created by Gerwa on 2017/9/9.
//

#ifndef PLAYGWENT_HARPY_EGG_H
#define PLAYGWENT_HARPY_EGG_H

#include "../GameController.h"
#include "CardMonitorBase.h"

class Harpy_Egg : public CardMonitorBase {
Q_OBJECT
public slots:

    void slotOnHandSeleted(CardInfo *seletedCard, bool allowCancel) override;

    void slotOnConsume(CardInfo *consumer, CardInfo *food) override;

    void slotOnDeathWish(CardInfo *mover, int fromR, int toR)override ;
};


#endif //PLAYGWENT_HARPY_EGG_H
