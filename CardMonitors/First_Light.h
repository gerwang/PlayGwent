//
// Created by Gerwa on 2017/9/10.
//

#ifndef PLAYGWENT_FIRST_LIGHT_H
#define PLAYGWENT_FIRST_LIGHT_H

#include "../GameController.h"
#include "CardMonitorBase.h"

class First_Light : public CardMonitorBase {
Q_OBJECT
public slots:

    void slotOnHandSeleted(CardInfo *seletedCard, bool allowCancel) override;

    void slotOnDeploy(CardInfo *mover, int fromR, int toR) override;
};


#endif //PLAYGWENT_FIRST_LIGHT_H
