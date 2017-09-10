//
// Created by Gerwa on 2017/9/9.
//

#include "Unseen_Elder.h"

void Unseen_Elder::slotOnHandSeleted(CardInfo *seletedCard, bool allowCancel) {
    if (seletedCard->getCardName() == "Unseen_Elder") {
        GameController::controller()->defaultHandleHand(seletedCard, allowCancel);
    }
}

void Unseen_Elder::slotOnDeploy(CardInfo *mover, int fromR, int toR) {
    if (mover->getCardName() == "Unseen_Elder") {
        GameAssets *assets = GameController::controller()->getAssets();

        int fromPlayer = assets->whosePlayerRow(fromR);
        QList<CardInfo *> consumed;
        for (int cnt = 0; cnt < 3; cnt++) {//3 times
            //first inspect if there is an ally
            QList<QPoint> validPositions;
            int Begin = assets->playerBattlefieldBegin(fromPlayer);
            int End = assets->playerBattlefieldEnd(fromPlayer);
            for (int row = Begin; row <= End; row++) {//iterate ally battlefield
                const QList<CardInfo *> &battlefield = assets->getCardArray(row);
                for (int column = 0; column < battlefield.size(); column++) {
                    if (battlefield[column] != mover && battlefield[column]->getCurrentStrength() > 0) {
                        //not unseen_elder himself or a unit which priviously consumed
                        validPositions.append(QPoint(row, column));
                    }
                }
            }
            if (validPositions.empty()) {//no ally to consume
                break;
            }
            int targetRow, targetColumn;
            Command command;
            GameController::controller()->handleHandValidPosition(mover, false, validPositions, command, targetRow,
                                                                  targetColumn);
            CardInfo *food = assets->getCardArray(targetRow).at(targetColumn);
            mover->setInitialStrength(mover->getInitialStrength() + food->getCurrentStrength());
            //strengthen instead of boosting
            GameController::controller()->Consume(mover, food);
            consumed.append(food);
        }
        for (auto food:consumed) {
            if (assets->isCardOnBattlefield(food)) {
                GameController::controller()->DestroyCard(food);
            }
        }
    }
}
