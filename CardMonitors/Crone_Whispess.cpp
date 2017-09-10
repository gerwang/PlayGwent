//
// Created by Gerwa on 2017/9/9.
//

#include "Crone_Whispess.h"

void Crone_Whispess::slotOnHandSeleted(CardInfo *seletedCard, bool allowCancel) {
    if (seletedCard->getCardName() == "Crone_Whispess") {
        GameController::controller()->defaultHandleHand(seletedCard, allowCancel);
    }
}

void Crone_Whispess::slotOnDeploy(CardInfo *mover, int fromR, int toR) {
    if (mover->getCardName() == "Crone_Whispess") {
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
