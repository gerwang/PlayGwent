//
// Created by Gerwa on 2017/9/9.
//

#include "Impenetrable_Fog.h"

void Impenetrable_Fog::slotOnHandSeleted(CardInfo *seletedCard, bool allowCancel) {
    if (seletedCard->getCardName() == "Impenetrable_Fog") {
        GameController::controller()->defaultHandleHand(seletedCard, allowCancel);
    }
}

void Impenetrable_Fog::slotOnDeploy(CardInfo *mover, int fromR, int toR) {
    if (mover->getCardName() == "Impenetrable_Fog") {
        GameController::controller()->changeRowWeather(toR, Weather::Foggy);
        GameController::controller()->DestroySpecialCard(mover, fromR);//effect card, destroy
    }
}

void Impenetrable_Fog::slotOnTurnStart() {
    GameAssets *assets = GameController::controller()->getAssets();
    int currentPlayer = assets->getCurrentPlayer();
    int Begin = assets->playerBattlefieldBegin(currentPlayer);
    int End = assets->playerBattlefieldEnd(currentPlayer);
    for (int row = Begin; row <= End; row++) {
        if (assets->getRowWeather(row) == Weather::Foggy) {
            //handle code here
            QList<CardInfo *> strongList;
            assets->updateRowStrongest(row, strongList, nullptr);
            if (!strongList.empty()) {
                int randomIndex = assets->randint() % strongList.size();
                GameController::controller()->causeWeatherDamage(row, {strongList[randomIndex]}, 2, true);
            }
            //handle code end
        }
    }
}