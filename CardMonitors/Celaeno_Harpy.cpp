//
// Created by Gerwa on 2017/9/9.
//

#include "Celaeno_Harpy.h"

void Celaeno_Harpy::slotOnHandSeleted(CardInfo *seletedCard, bool allowCancel) {
    if (seletedCard->getCardName() == "Celaeno_Harpy") {
        GameController::controller()->defaultHandleHand(seletedCard, allowCancel);
    }
}

void Celaeno_Harpy::slotOnDeploy(CardInfo *mover, int fromR, int toR) {
    if (mover->getCardName() == "Celaeno_Harpy") {
        //spawn 2 eggs on the LEFT of it
        GameAssets *assets = GameController::controller()->getAssets();
        int fromPlayer = assets->whosePlayerRow(fromR);
        for (int cnt = 0; cnt < 2; cnt++) {
            int toC = assets->getCardArray(toR).indexOf(mover);//faster index
            GameController::controller()->spawnCardToPosByPlayer(CardInfo::createByName("Harpy_Egg"), toR, toC,
                                                                 fromPlayer);
        }
    }
}
