//
// Created by Gerwa on 2017/9/9.
//

#ifndef PLAYGWENT_GEELS_H
#define PLAYGWENT_GEELS_H


#include "../GameController.h"
#include "CardMonitorBase.h"

class Geels : public CardMonitorBase {
Q_OBJECT
public slots:

    void slotOnHandSeleted(CardInfo *seletedCard, bool allowCancel) override;

    void slotOnDeploy(CardInfo *mover, int fromR, int toR) override;
};


#endif //PLAYGWENT_GEELS_H
