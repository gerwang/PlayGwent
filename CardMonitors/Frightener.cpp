//
// Created by Gerwa on 2017/9/9.
//

#include "Frightener.h"

void Frightener::slotOnHandSeleted(CardInfo *seletedCard, bool allowCancel) {
    if (seletedCard->getCardName() == "Frightener") {
        GameController::controller()->defaultHandleHand(seletedCard, allowCancel);
    }
}

void Frightener::slotOnDeploy(CardInfo *mover, int fromR, int toR) {
    if (mover->getCardName() == "Frightener") {
        GameAssets *assets = GameController::controller()->getAssets();
        int toPlayer = assets->whosePlayerRow(toR);
        QList<QPoint> validPositions;
        int Begin = assets->playerBattlefieldBegin(toPlayer);
        int End = assets->playerBattlefieldEnd(toPlayer);
        for (int row = Begin; row <= End; row++) {
            if (row == toR) {//same row as frightener
                continue;
            }
            const QList<CardInfo *> &battlefield = assets->getCardArray(row);
            for (int column = 0; column < battlefield.size(); column++) {
                validPositions.append(QPoint(row, column));
            }
        }
        if (!validPositions.empty()) {//has a unit to move
            Command command;
            int targetRow, targetColumn;
            GameController::controller()->handleHandValidPosition(mover, false, validPositions, command, targetRow,
                                                                  targetColumn);
            CardInfo *otherCard = assets->getCardArray(targetRow).at(targetColumn);
            GameController::controller()->moveCardToRightTop(otherCard, toR);//the same row as frightener
        }
        //move 1 card from deck to hand
        int fromPlayer = assets->whosePlayerRow(fromR);
        if (!assets->getCardArray(assets->getDeckIndex(fromPlayer)).empty()) {//still have card in deck
            GameController::controller()->moveNCardsFromDeckToHand(fromPlayer, 1);
        }
    }
}
