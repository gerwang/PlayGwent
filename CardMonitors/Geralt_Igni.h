//
// Created by Gerwa on 2017/9/8.
//

#ifndef PLAYGWENT_GERALT_IGNI_H
#define PLAYGWENT_GERALT_IGNI_H


#include "../GameController.h"
#include "CardMonitorBase.h"

class Geralt_Igni : public CardMonitorBase {
Q_OBJECT
public slots:

    void slotOnHandSeleted(CardInfo *seletedCard, bool allowCancel) override;

    void slotOnDeploy(CardInfo *mover, int fromR, int toR) override;
};


#endif //PLAYGWENT_GERALT_IGNI_H
