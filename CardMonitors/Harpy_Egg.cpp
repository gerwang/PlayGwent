//
// Created by Gerwa on 2017/9/9.
//

#include "Harpy_Egg.h"

void Harpy_Egg::slotOnHandSeleted(CardInfo *seletedCard, bool allowCancel) {
    if (seletedCard->getCardName() == "Harpy_Egg") {
        //in fact, the card will not appear in deck
        GameController::controller()->defaultHandleHand(seletedCard, allowCancel);
    }
}

void Harpy_Egg::slotOnConsume(CardInfo *consumer, CardInfo *food) {
    if (food->getCardName() == "Harpy_Egg") {
        consumer->setCurrentStrength(consumer->getCurrentStrength() + 4);//extra 4 boost
    }
}

void Harpy_Egg::slotOnDeathWish(CardInfo *mover, int fromR, int toR) {
    if (mover->getCardName() == "Harpy_Egg") {
        GameAssets *assets = GameController::controller()->getAssets();
        int belongPlayer = assets->whosePlayerRow(fromR);//fromR is the battlefield row
        int Begin = assets->playerBattlefieldBegin(belongPlayer);
        int End = assets->playerBattlefieldEnd(belongPlayer);
        int randomIndex = Begin + qrand() % (End - Begin + 1);
        int endColumn = assets->getCardArray(randomIndex).size();
        GameController::controller()->spawnCardToPosByPlayer(
                CardInfo::createByName("Harpy_Hatchling"), randomIndex, endColumn, belongPlayer);
    }
}
