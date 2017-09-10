//
// Created by Gerwa on 2017/9/9.
//

#include "Earth_Elemental.h"

void Earth_Elemental::slotOnHandSeleted(CardInfo *seletedCard, bool allowCancel) {
    if (seletedCard->getCardName() == "Earth_Elemental") {
        GameController::controller()->defaultHandleHand(seletedCard, allowCancel);
    }
}

void Earth_Elemental::slotOnDeploy(CardInfo *mover, int fromR, int toR) {
    if (mover->getCardName() == "Earth_Elemental") {
        //gain a shield
        GameAssets *assets = GameController::controller()->getAssets();
        int toC = assets->getCardArray(toR).indexOf(mover);//faster index
        AbstractUI *gameUI = GameController::controller()->getGameUI();
        mover->setShield(true);
        gameUI->showBoost(QPoint(-1, -1), {QPoint(toR, toC)});
    }
}

void Earth_Elemental::slotOnDeathWish(CardInfo *mover, int fromR, int toR) {
    if (mover->getCardName() == "Earth_Elemental") {
        GameAssets *assets = GameController::controller()->getAssets();
        int fromPlayer = assets->whosePlayerRow(fromR);
        for (int cnt = 0; cnt < 2; cnt++) {//spawn 2 lesser earth elemental on rightmost of this row
            GameController::controller()->spawnCardToPosByPlayer(CardInfo::createByName("Lesser_Earth_Elemental"),
                                                                 fromR, assets->getCardArray(fromR).size(), fromPlayer);
        }
    }
}
