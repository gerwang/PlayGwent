//
// Created by Gerwa on 2017/9/10.
//

#include "Caranthir.h"

void Caranthir::slotOnHandSeleted(CardInfo *seletedCard, bool allowCancel) {
    if (seletedCard->getCardName() == "Caranthir") {
        GameController::controller()->defaultHandleHand(seletedCard, allowCancel);
    }
}

void Caranthir::slotOnDeploy(CardInfo *mover, int fromR, int toR) {
    if (mover->getCardName() == "Caranthir") {
        GameAssets *assets = GameController::controller()->getAssets();
        int targetPlayer = assets->whosePlayerRow(fromR) ^1;
        int sameRow = assets->getEnemySameRowNumber(toR);
        int Begin = assets->playerBattlefieldBegin(targetPlayer);
        int End = assets->playerBattlefieldEnd(targetPlayer);
        QList<QPoint> validPositions;
        for (int row = Begin; row <= End; row++) {
            if (row == sameRow) {
                continue;
            } else {
                int rowSize = assets->getCardArray(row).size();
                for (int column = 0; column < rowSize; column++) {
                    validPositions.append(QPoint(row, column));
                }
            }
        }
        if (!validPositions.empty()) {
            Command command;
            int targetRow, targetColumn;
            GameController::controller()->handleHandValidPosition(mover, false, validPositions, command, targetRow,
                                                                  targetColumn);
            CardInfo *seletedCard = assets->getCardArray(targetRow).at(targetColumn);
            GameController::controller()->moveCardToRightTop(seletedCard, sameRow);
        }
        GameController::controller()->changeRowWeather(sameRow, Weather::Frozen);//apply frost
    }
}
