//
// Created by Gerwa on 2017/9/9.
//

#ifndef PLAYGWENT_COMMANDERS_HORN_H
#define PLAYGWENT_COMMANDERS_HORN_H

#include "../GameController.h"
#include "CardMonitorBase.h"

class Commanders_Horn : public CardMonitorBase {
Q_OBJECT
public slots:

    void slotOnHandSeleted(CardInfo *seletedCard, bool allowCancel) override;
};


#endif //PLAYGWENT_COMMANDERS_HORN_H
