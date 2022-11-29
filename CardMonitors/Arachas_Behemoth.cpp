//
// Created by Gerwa on 2017/9/8.
//

#include "Arachas_Behemoth.h"

void Arachas_Behemoth::slotOnHandSeleted(CardInfo *seletedCard, bool allowCancel) {
    if (seletedCard->getCardName() == "Arachas_Behemoth") {
        GameController::controller()->defaultHandleHand(seletedCard, allowCancel);
    }
}

void Arachas_Behemoth::slotOnDeploy(CardInfo *mover, int fromR, int toR) {
    if (mover->getCardName() == "Arachas_Behemoth") {
        GameController::controller()->boostFromSrcToDests(nullptr, {mover}, 0,
                                                          2);//gain 2 armors TODO:test if {} is okay
    }
}

void Arachas_Behemoth::slotAfterConsume(CardInfo *consumer, CardInfo *food) {//one consume per call
    auto assets = GameController::controller()->getAssets();
    int fromR, fromC;
    assets->getCardPosition(consumer, fromR, fromC);
    int player = assets->whosePlayerRow(fromR);
    int Begin = assets->playerBattlefieldBegin(player);
    int End = assets->playerBattlefieldEnd(player);
    QList<CardInfo *> arachasBehemoths;
    for (int row = Begin; row <= End; row++) {//only find arachas_behemoth in ally
        for (auto card:assets->getCardArray(row)) {
            if (card->getCardName() == "Arachas_Behemoth") {
                arachasBehemoths.append(card);
            }
        }
    }
    int consumerPlayer = assets->whosePlayerRow(fromR);
    for (auto card:arachasBehemoths) {
        //spawn an arachas_hatchling on the rightmost of random ally row
        int randomIndex = Begin + assets->randint() % (End - Begin + 1);
        GameController::controller()->spawnCardToPosByPlayer(CardInfo::createByName("Arachas_Hatchling"), randomIndex,
                                                             assets->getCardArray(randomIndex).size(), consumerPlayer);
        GameController::controller()->damageFromSrcToDests(nullptr, {card}, 1, false);//ignore armor
    }
}
