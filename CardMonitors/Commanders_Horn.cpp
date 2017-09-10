//
// Created by Gerwa on 2017/9/9.
//

#include "Commanders_Horn.h"

void Commanders_Horn::slotOnHandSeleted(CardInfo *seletedCard, bool allowCancel) {
    //this operation is different from defaultvoid Thunderbolt_Potion::slotOnHandSeleted(CardInfo *seletedCard, bool allowCancel) {
    //this operation is different from default
    if (seletedCard->getCardName() == "Commanders_Horn") {
        GameAssets *assets = GameController::controller()->getAssets();

        QList<QPoint> validPositions;
        for (int row = Player1_Siege; row <= Player0_Siege; row++) {//iterate all battlefields
            int columnSize = assets->getCardArray(row).size();
            //all units on battlefields are valid
            for (int column = 0; column < columnSize; column++) {
                validPositions.append(QPoint(row, column));
            }
        }

        Command command;
        int toR, toC;
        GameController::controller()->handleHandValidPosition(seletedCard, allowCancel, validPositions, command, toR,
                                                              toC);

        if (command == Command::LeftClick) {
            //boost begin
            const QList<CardInfo *> &targetBattlefield = assets->getCardArray(toR);
            QList<CardInfo *> dests;
            dests.append(targetBattlefield[toC]);//neighboring 5 unit
            for (int offset = 1; offset <= 2; offset++) {
                if (toC - offset >= 0) {
                    dests.append(targetBattlefield[toC - offset]);
                }
                if (toC + offset < targetBattlefield.size()) {
                    dests.append(targetBattlefield[toC + offset]);
                }
            }
            GameController::controller()->boostFromSrcToDests(seletedCard, dests, 4, 0);//0 armor, 4 boost
            GameController::controller()->DestroyCard(seletedCard);//used, destroy it
            //no need to use DestroySpecialCard
            //boost end
        }
    }
}
