//
// Created by Gerwa on 2017/9/8.
//

#include "Arachas.h"

void Arachas::slotOnHandSeleted(CardInfo *seletedCard, bool allowCancel) {
    if (seletedCard->getCardName() == "Arachas") {
        GameController::controller()->defaultHandleHand(seletedCard, allowCancel);
    }
}

void Arachas::slotOnDeploy(CardInfo *mover, int fromR, int toR) {
    //WARNING:I implemented the dfs algorithm, so we should play one card each and return
    if (mover->getCardName() == "Arachas") {
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
