//
// Created by Gerwa on 2017/9/8.
//

#ifndef PLAYGWENT_VRAN_WARRIOR_H
#define PLAYGWENT_VRAN_WARRIOR_H

#include "../GameController.h"
#include "CardMonitorBase.h"

class Vran_Warrior : public CardMonitorBase {
Q_OBJECT
public slots:

    void slotOnHandSeleted(CardInfo *seletedCard, bool allowCancel) override;

    void slotOnDeploy(CardInfo *mover, int fromR, int toR) override;

    void slotOnTurnStart() override;
};

#endif //PLAYGWENT_VRAN_WARRIOR_H
