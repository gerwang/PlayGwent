//
// Created by Gerwa on 2017/9/9.
//

#ifndef PLAYGWENT_EARTH_ELEMENTAL_H
#define PLAYGWENT_EARTH_ELEMENTAL_H


#include "../GameController.h"
#include "CardMonitorBase.h"

class Earth_Elemental : public CardMonitorBase {
Q_OBJECT
public slots:

    void slotOnHandSeleted(CardInfo *seletedCard, bool allowCancel) override;

    void slotOnDeploy(CardInfo *mover, int fromR, int toR) override;

    void slotOnDeathWish(CardInfo *mover, int fromR, int toR) override;
};


#endif //PLAYGWENT_EARTH_ELEMENTAL_H
