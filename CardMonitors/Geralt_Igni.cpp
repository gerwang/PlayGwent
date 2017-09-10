//
// Created by Gerwa on 2017/9/8.
//

#include "Geralt_Igni.h"

void Geralt_Igni::slotOnHandSeleted(CardInfo *seletedCard, bool allowCancel) {
    if (seletedCard->getCardName() == "Geralt_Igni") {
        GameController::controller()->defaultHandleHand(seletedCard, allowCancel);
    }
}

void Geralt_Igni::slotOnDeploy(CardInfo *mover, int fromR, int toR) {
    if (mover->getCardName() == "Geralt_Igni") {
        auto assets = GameController::controller()->getAssets();
        int enemyRow = assets->getEnemySameRowNumber(toR);
        if (assets->getRowCombatValueSum(enemyRow) >= 25) {
            QList<CardInfo *> strongList;
            assets->updateRowStrongest(enemyRow, strongList, nullptr);
            for (auto strongCard:strongList) {
                GameController::controller()->DestroyCard(strongCard);
            }
        }
    }
}
