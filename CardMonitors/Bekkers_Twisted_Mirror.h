//
// Created by Gerwa on 2017/9/9.
//

#ifndef PLAYGWENT_BEKKERS_TWISTED_MIRROR_H
#define PLAYGWENT_BEKKERS_TWISTED_MIRROR_H


#include "../GameController.h"
#include "CardMonitorBase.h"

class Bekkers_Twisted_Mirror : public CardMonitorBase {
Q_OBJECT
public slots:

    void slotOnHandSeleted(CardInfo *seletedCard, bool allowCancel) override;

    void slotOnDeploy(CardInfo *mover, int fromR, int toR) override;
};


#endif //PLAYGWENT_BEKKERS_TWISTED_MIRROR_H
