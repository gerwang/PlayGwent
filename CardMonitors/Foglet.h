//
// Created by Gerwa on 2017/9/10.
//

#ifndef PLAYGWENT_FOGLET_H
#define PLAYGWENT_FOGLET_H


#include "../GameController.h"
#include "CardMonitorBase.h"

class Foglet : public CardMonitorBase {
Q_OBJECT
public slots:

    void slotOnHandSeleted(CardInfo *seletedCard, bool allowCancel) override;

    void slotAfterWeatherChanged(int row, Weather oldWeather) override;
};

#endif //PLAYGWENT_FOGLET_H
