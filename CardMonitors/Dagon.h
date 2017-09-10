//
// Created by Gerwa on 2017/9/10.
//

#ifndef PLAYGWENT_DAGON_H
#define PLAYGWENT_DAGON_H

#include "../GameController.h"
#include "CardMonitorBase.h"

class Dagon : public CardMonitorBase {
Q_OBJECT
public slots:

    void slotOnHandSeleted(CardInfo *seletedCard, bool allowCancel) override;

    void slotOnDeploy(CardInfo *mover, int fromR, int toR) override;
};

#endif //PLAYGWENT_DAGON_H
