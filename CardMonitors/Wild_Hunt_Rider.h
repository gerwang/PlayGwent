//
// Created by Gerwa on 2017/9/10.
//

#ifndef PLAYGWENT_WILD_HUNT_RIDER_H
#define PLAYGWENT_WILD_HUNT_RIDER_H

#include "../GameController.h"
#include "CardMonitorBase.h"

class Wild_Hunt_Rider : public CardMonitorBase {
Q_OBJECT
public slots:

    void slotOnHandSeleted(CardInfo *seletedCard, bool allowCancel) override;

    void slotOnWeatherDamage(int row, const QList<CardInfo *> &dests) override;
};


#endif //PLAYGWENT_WILD_HUNT_RIDER_H
