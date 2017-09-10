//
// Created by Gerwa on 2017/9/9.
//

#include "Biting_Frost.h"

void Biting_Frost::slotOnHandSeleted(CardInfo *seletedCard, bool allowCancel) {
    if (seletedCard->getCardName() == "Biting_Frost") {
        GameController::controller()->defaultHandleHand(seletedCard, allowCancel);
    }
}

void Biting_Frost::slotOnDeploy(CardInfo *mover, int fromR, int toR) {
    if (mover->getCardName() == "Biting_Frost") {
        GameController::controller()->changeRowWeather(toR, Weather::Frozen);
        GameController::controller()->DestroySpecialCard(mover, fromR);//effect card, destroy
    }
}

void Biting_Frost::slotOnTurnStart() {
    GameAssets *assets = GameController::controller()->getAssets();
    int currentPlayer = assets->getCurrentPlayer();
    int Begin = assets->playerBattlefieldBegin(currentPlayer);
    int End = assets->playerBattlefieldEnd(currentPlayer);
    for (int row = Begin; row <= End; row++) {
        if (assets->getRowWeather(row) == Weather::Frozen) {
            //handle code here
            QList<CardInfo *> weakList;
            assets->updateRowWeakest(row, weakList, nullptr);
            if (!weakList.empty()) {
                int randomIndex = qrand() % weakList.size();
                GameController::controller()->causeWeatherDamage(row, {weakList[randomIndex]}, 2, true);
            }
            //handle code end
        }
    }
}
