//
// Created by Gerwa on 2017/9/8.
//

#include "Vran_Warrior.h"

void Vran_Warrior::slotOnHandSeleted(CardInfo *seletedCard, bool allowCancel) {
    if (seletedCard->getCardName() == "Vran_Warrior") {
        GameController::controller()->defaultHandleHand(seletedCard, allowCancel);
    }
}

void Vran_Warrior::slotOnDeploy(CardInfo *mover, int fromR, int toR) {
    if (mover->getCardName() == "Vran_Warrior") {
        //consume the righter ally
        auto assets = GameController::controller()->getAssets();
        const QList<CardInfo *> &rowDestination = assets->getCardArray(toR);
        int toC = rowDestination.indexOf(mover);
        if (toC + 1 < rowDestination.size()) {//really has a unit it consume
            CardInfo *rightFood = rowDestination[toC + 1];//righter one
            GameController::controller()->Consume(mover, rightFood);
            if (assets->isCardOnBattlefield(rightFood)) {//CHECK the food is still on battlefield
                GameController::controller()->DestroyCard(rightFood);
            }
        }
    }
}

void Vran_Warrior::slotOnTurnStart() {
    auto assets = GameController::controller()->getAssets();
    int currentPlayer = assets->getCurrentPlayer();

    int Begin = assets->playerBattlefieldBegin(currentPlayer);
    int End = assets->playerBattlefieldEnd(currentPlayer);

    for (int row = Begin; row <= End; row++) {//iterate ONLY CURRENTPLAYER 's battlefield
        //remeber the ORDER: from RIGHT to LEFT, case vran_warrior may consume another vran_warrior on the right of him
        const QList<CardInfo *> &battlefield = assets->getCardArray(row);
        for (int column = battlefield.size() - 1; column >= 0; column--) {
            CardInfo *unit = battlefield[column];
            //in this game, all spawn operation will spawn on the rightmost of a row
            //so the solution is currently okay
            if (unit->getCardName() == "Vran_Warrior") {
                GameController::controller()->subtractCD(unit);

                if (unit->getCD() == 0) {
                    if (column + 1 < battlefield.size()) {//has something to consume
                        CardInfo *rightFood = battlefield[column + 1];
                        GameController::controller()->Consume(unit, rightFood);
                        if (assets->isCardOnBattlefield(rightFood)) {
                            GameController::controller()->DestroyCard(rightFood);
                        }
                    }
                    GameController::controller()->resumeCD(unit, 2);
                }

            }

        }
    }
}
