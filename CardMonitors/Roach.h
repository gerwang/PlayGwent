//
// Created by Gerwa on 2017/9/8.
//

#ifndef PLAYGWENT_ROACH_H
#define PLAYGWENT_ROACH_H

#include "../GameController.h"
#include "CardMonitorBase.h"

class Roach : public CardMonitorBase {
Q_OBJECT
public slots:

    void slotOnHandSeleted(CardInfo *seletedCard, bool allowCancel) override;

    void slotBeforeDeploy(CardInfo *mover, int fromR, int toR) override;
};

#endif //PLAYGWENT_ROACH_H
