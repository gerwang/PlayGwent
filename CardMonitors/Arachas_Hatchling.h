//
// Created by Gerwa on 2017/9/8.
//

#ifndef PLAYGWENT_ARACHAS_HATCHLING_H
#define PLAYGWENT_ARACHAS_HATCHLING_H

#include "../GameController.h"
#include "CardMonitorBase.h"

class Arachas_Hatching : public CardMonitorBase {
Q_OBJECT
public slots:

    void slotOnHandSeleted(CardInfo *seletedCard, bool allowCancel) override;

    void slotOnDeploy(CardInfo *mover, int fromR, int toR) override;
};

#endif //PLAYGWENT_ARACHAS_HATCHLING_H
