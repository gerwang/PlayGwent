//
// Created by Gerwa on 2017/9/9.
//

#include "Thunderbolt_Potion.h"

void Thunderbolt_Potion::slotOnHandSeleted(CardInfo *seletedCard, bool allowCancel) {
    //this operation is different from default
    if (seletedCard->getCardName() == "Thunderbolt_Potion") {
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
            dests.append(targetBattlefield[toC]);//neighboring 3 unit
            if (toC - 1 >= 0) {
                dests.append(targetBattlefield[toC - 1]);
            }
            if (toC + 1 < targetBattlefield.size()) {
                dests.append(targetBattlefield[toC + 1]);
            }
            GameController::controller()->boostFromSrcToDests(seletedCard, dests, 3, 2);//2 armor, 3 boost
            GameController::controller()->DestroyCard(seletedCard);//used, destroy it
            //no need to use DestroySpecialCard
            //boost end
        }
    }
}
