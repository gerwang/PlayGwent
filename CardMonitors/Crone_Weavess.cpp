//
// Created by Gerwa on 2017/9/9.
//

#include "Crone_Weavess.h"

void Crone_Weavess::slotOnHandSeleted(CardInfo *seletedCard, bool allowCancel) {
    if (seletedCard->getCardName() == "Crone_Weavess") {
        GameController::controller()->defaultHandleHand(seletedCard, allowCancel);
    }
}

void Crone_Weavess::slotOnDeploy(CardInfo *mover, int fromR, int toR) {
    if (mover->getCardName() == "Crone_Weavess") {
        GameAssets *assets = GameController::controller()->getAssets();
        int fromPlayer = assets->whosePlayerRow(fromR);
        const QList<CardInfo *> &deck = assets->getCardArray(assets->getDeckIndex(fromPlayer));
        QList<QString> croneNames = {"Crone_Brewess", "Crone_Weavess", "Crone_Whispess"};
        //listen to all because there will be only one silver card in actual game
        for (auto card:deck) {
            if (croneNames.contains(card->getCardName())) {
                GameController::controller()->moveCardToRightTop(card, assets->getPlayerSiege(fromPlayer));
                break;//dfs algorithm let others deploy
                //they are all on siege
            }
        }
    }
}