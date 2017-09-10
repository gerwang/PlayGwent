//
// Created by Gerwa on 2017/9/10.
//

#ifndef PLAYGWENT_TORRENTIAL_RAIN_H
#define PLAYGWENT_TORRENTIAL_RAIN_H


#include "../GameController.h"
#include "CardMonitorBase.h"

class Torrential_Rain : public CardMonitorBase {
Q_OBJECT
public slots:

    void slotOnHandSeleted(CardInfo *seletedCard, bool allowCancel) override;

    void slotOnDeploy(CardInfo *mover, int fromR, int toR) override;

    void slotOnTurnStart() override;
};

#endif //PLAYGWENT_TORRENTIAL_RAIN_H
