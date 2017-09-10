//
// Created by Gerwa on 2017/9/10.
//

#include "Clear_Skies.h"

void Clear_Skies::slotOnHandSeleted(CardInfo *seletedCard, bool allowCancel) {
    if (seletedCard->getCardName() == "Clear_Skies") {
        // in fact, clear skies won't appear in hand
        GameController::controller()->defaultHandleHand(seletedCard, allowCancel);
    }
}

void Clear_Skies::slotOnDeploy(CardInfo *mover, int fromR, int toR) {
    if (mover->getCardName() == "Clear_Skies") {
        GameController::controller()->DestroySpecialCard(mover, fromR);//effect card, destroy
        GameAssets *assets = GameController::controller()->getAssets();
        int fromPlayer = assets->whosePlayerRow(fromR);
        int Begin = assets->playerBattlefieldBegin(fromPlayer);
        int End = assets->playerBattlefieldEnd(fromPlayer);
        for (int row = Begin; row <= End; row++) {
            if (assets->getRowWeather(row) != Weather::Sunny) {
                GameController::controller()->changeRowWeather(row, Weather::Sunny);
            }
        }
    }
}
