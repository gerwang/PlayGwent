//
// Created by Gerwa on 2017/9/10.
//

#include "Wild_Hunt_Rider.h"

void Wild_Hunt_Rider::slotOnHandSeleted(CardInfo *seletedCard, bool allowCancel) {
    if (seletedCard->getCardName() == "Wild_Hunt_Rider") {
        GameController::controller()->defaultHandleHand(seletedCard, allowCancel);
    }
}

void Wild_Hunt_Rider::slotOnWeatherDamage(int row, const QList<CardInfo *> &dests) {
    GameAssets *assets = GameController::controller()->getAssets();
    if (assets->getRowWeather(row) == Weather::Frozen) {
        int oppositeRow = assets->getEnemySameRowNumber(row);
        const QList<CardInfo *> &oppositeBattlefield = assets->getCardArray(oppositeRow);
        int riderCount = 0;
        for (auto unit:oppositeBattlefield) {
            if (unit->getCardName() == "Wild_Hunt_Rider") {
                riderCount++;
            }
        }
        if (riderCount > 0) {
            GameController::controller()->damageFromSrcToDests(nullptr, dests, riderCount, true);
        }
    }
}
