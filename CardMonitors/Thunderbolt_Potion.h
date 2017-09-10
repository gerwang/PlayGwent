//
// Created by Gerwa on 2017/9/9.
//

#ifndef PLAYGWENT_THUNDERBOLT_POTION_H
#define PLAYGWENT_THUNDERBOLT_POTION_H

#include "../GameController.h"
#include "CardMonitorBase.h"

class Thunderbolt_Potion : public CardMonitorBase {
Q_OBJECT
public slots:

    void slotOnHandSeleted(CardInfo *seletedCard, bool allowCancel) override;
};

#endif //PLAYGWENT_THUNDERBOLT_POTION_H
