//
// Created by Gerwa on 2017/9/10.
//

#include "Torrential_Rain.h"

void Torrential_Rain::slotOnHandSeleted(CardInfo *seletedCard, bool allowCancel) {
    if (seletedCard->getCardName() == "Torrential_Rain") {
        GameController::controller()->defaultHandleHand(seletedCard, allowCancel);
    }
}

void Torrential_Rain::slotOnDeploy(CardInfo *mover, int fromR, int toR) {
    if (mover->getCardName() == "Torrential_Rain") {
        GameController::controller()->changeRowWeather(toR, Weather::Rainy);
        GameController::controller()->DestroySpecialCard(mover, fromR);//effect card, destroy
    }
}

void Torrential_Rain::slotOnTurnStart() {
    GameAssets *assets = GameController::controller()->getAssets();
    int currentPlayer = assets->getCurrentPlayer();
    int Begin = assets->playerBattlefieldBegin(currentPlayer);
    int End = assets->playerBattlefieldEnd(currentPlayer);
    for (int row = Begin; row <= End; row++) {
        if (assets->getRowWeather(row) == Weather::Rainy) {
            //handle code here
            const QList<CardInfo *> &battlefield = assets->getCardArray(row);
            QList<CardInfo *> dests;
            if (!battlefield.empty()) {
                int firstRandomIndex = qrand() % battlefield.size();
                dests.append(battlefield[firstRandomIndex]);

                if (battlefield.size() > 1) {//has two
                    int secondRandomIndex = qrand() % (battlefield.size() - 1);

                    if (secondRandomIndex >= firstRandomIndex) {
                        secondRandomIndex++;// a math trick
                    }

                    dests.append(battlefield[secondRandomIndex]);
                }
            }
            if (!dests.empty()) {
                GameController::controller()->causeWeatherDamage(row, dests, 1, true);
            }
            //handle code end
        }
    }
}