//
// Created by Gerwa on 2017/9/10.
//

#ifndef PLAYGWENT_CLEAR_SKIES_H
#define PLAYGWENT_CLEAR_SKIES_H


#include "../GameController.h"
#include "CardMonitorBase.h"

class Clear_Skies : public CardMonitorBase {
Q_OBJECT
public slots:

    void slotOnHandSeleted(CardInfo *seletedCard, bool allowCancel) override;

    void slotOnDeploy(CardInfo *mover, int fromR, int toR) override;
};


#endif //PLAYGWENT_CLEAR_SKIES_H
