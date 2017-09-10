//
// Created by Gerwa on 2017/9/10.
//

#include "Woodland_Spirit.h"

void Woodland_Spirit::slotOnHandSeleted(CardInfo *seletedCard, bool allowCancel) {
    if (seletedCard->getCardName() == "Woodland_Spirit") {
        GameController::controller()->defaultHandleHand(seletedCard, allowCancel);
    }
}

void Woodland_Spirit::slotOnDeploy(CardInfo *mover, int fromR, int toR) {
    if (mover->getCardName() == "Woodland_Spirit") {
        GameAssets *assets = GameController::controller()->getAssets();
        int fromPlayer = assets->whosePlayerRow(fromR);
        int meleeIndex = assets->getPlayerMelee(fromPlayer);//rabid wolf is on melee
        const QList<CardInfo *> &meleeBattlefield = assets->getCardArray(meleeIndex);
        for (int cnt = 0; cnt < 3; cnt++) {//3 wolves
            GameController::controller()->spawnCardToPosByPlayer(CardInfo::createByName("Rabid_Wolf"), meleeIndex,
                                                                 meleeBattlefield.size(), fromPlayer);
        }
        //activate fog
        GameController::controller()->changeRowWeather(assets->getEnemySameRowNumber(toR), Weather::Foggy);
    }
}
