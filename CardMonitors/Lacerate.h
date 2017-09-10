//
// Created by Gerwa on 2017/9/9.
//

#ifndef PLAYGWENT_LACERATE_H
#define PLAYGWENT_LACERATE_H

#include "../GameController.h"
#include "CardMonitorBase.h"

class Lacerate : public CardMonitorBase {
Q_OBJECT
public slots:

    void slotOnHandSeleted(CardInfo *seletedCard, bool allowCancel) override;

    void slotOnDeploy(CardInfo *mover, int fromR, int toR) override;
};


#endif //PLAYGWENT_LACERATE_H
