//
// Created by Gerwa on 2017/9/8.
//

#include "Arachas_Hatchling.h"

void Arachas_Hatching::slotOnHandSeleted(CardInfo *seletedCard, bool allowCancel) {
    //in fact,this card will not appear in hand
    if (seletedCard->getCardName() == "Arachas_Hatchling") {
        GameController::controller()->defaultHandleHand(seletedCard, allowCancel);
    }
}

void Arachas_Hatching::slotOnDeploy(CardInfo *mover, int fromR, int toR) {
    if (mover->getCardName() == "Arachas_Hatchling") {
        //note: the fromR maybe not from DECK or HAND, but whosePlayerRow should still work update: will cause strange animation
        auto assets = GameController::controller()->getAssets();
        int fromPlayer = assets->whosePlayerRow(fromR);
        int deckIndex = assets->getDeckIndex(fromPlayer);
        QList<CardInfo *> arachasInDeck;
        for (auto card:assets->getCardArray(deckIndex)) {
            if (card->getCardName() == "Arachas") {
                //because arachas can only be placed in ranged
                int rangedIndex = assets->getPlayerRanged(fromPlayer);
                GameController::controller()->moveCardToRightTop(card, rangedIndex);
                break;//one each,let the others deploy
            }
        }
    }
}


