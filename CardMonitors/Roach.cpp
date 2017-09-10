//
// Created by Gerwa on 2017/9/8.
//

#include "Roach.h"

void Roach::slotOnHandSeleted(CardInfo *seletedCard, bool allowCancel) {
    if (seletedCard->getCardName() == "Roach") {
        GameController::controller()->defaultHandleHand(seletedCard, allowCancel);
    }
}

void Roach::slotBeforeDeploy(CardInfo *mover, int fromR, int toR) {
    //warning: must be played from HAND
    if (mover->getType() == CardInfo::Gold) {//excluded leader already
        auto assets = GameController::controller()->getAssets();
        int player = assets->whosePlayerRow(fromR);
        if (fromR == assets->getHandIndex(player)) {//played from hand
            //find all roaches from DECK
            int deckIndex = assets->getDeckIndex(player);
            QList<CardInfo *> allRoaches;
            for (auto card:assets->getCardArray(deckIndex)) {
                if (card->getCardName() == "Roach") {
                    allRoaches.append(card);
                }
            }
            int Begin = assets->playerBattlefieldBegin(player),
                    End = assets->playerBattlefieldEnd(player);
            for (auto roach:allRoaches) {
                //generate random index in [begin,end]
                int randomIndex = Begin + qrand() % (End - Begin + 1);
                GameController::controller()->moveCardToRightTop(roach, randomIndex);
            }
        }
    }
}
