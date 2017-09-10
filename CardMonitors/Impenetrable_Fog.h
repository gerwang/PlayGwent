//
// Created by Gerwa on 2017/9/9.
//

#ifndef PLAYGWENT_IMPENETRABLE_FOG_H
#define PLAYGWENT_IMPENETRABLE_FOG_H


#include "../GameController.h"
#include "CardMonitorBase.h"

class Impenetrable_Fog : public CardMonitorBase {
Q_OBJECT
public slots:

    void slotOnHandSeleted(CardInfo *seletedCard, bool allowCancel) override;

    void slotOnDeploy(CardInfo *mover, int fromR, int toR) override;

    void slotOnTurnStart() override;
};

#endif //PLAYGWENT_IMPENETRABLE_FOG_H
