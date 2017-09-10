//
// Created by Gerwa on 2017/9/9.
//

#ifndef PLAYGWENT_BITING_FROST_H
#define PLAYGWENT_BITING_FROST_H


#include "../GameController.h"
#include "CardMonitorBase.h"

class Biting_Frost : public CardMonitorBase {
Q_OBJECT
public slots:

    void slotOnHandSeleted(CardInfo *seletedCard, bool allowCancel) override;

    void slotOnDeploy(CardInfo *mover, int fromR, int toR) override;

    void slotOnTurnStart() override;
};


#endif //PLAYGWENT_BITING_FROST_H
