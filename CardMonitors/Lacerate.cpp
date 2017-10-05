//
// Created by Gerwa on 2017/9/9.
//

#include "Lacerate.h"

void Lacerate::slotOnHandSeleted(CardInfo *seletedCard, bool allowCancel) {
    if (seletedCard->getCardName() == "Lacerate") {
        //default behavior: choose the row and pick it
        GameAssets *assets = GameController::controller()->getAssets();
        AbstractUI *gameUI = GameController::controller()->getGameUI();
        int currentPlayer = assets->getCurrentPlayer();
        int fromR, fromC;
        assets->getCardPosition(seletedCard, fromR, fromC);
        gameUI->setSource(fromR, fromC);

        while (true) {
            //set all battlefields valid
            for (int row = Player1_Siege; row <= Player0_Siege; row++) {//iterate all battlefields
                gameUI->setValidRow(row);
            }

            gameUI->setPlayerInputState(currentPlayer, AbstractUI::MustValidRow);
            Command command;
            int toR, toC;
            gameUI->getUserInput(command, toR, toC, currentPlayer);
            gameUI->setPlayerInputState(currentPlayer, AbstractUI::RejectAll);
            gameUI->resetValidRows();

            if (command == Command::Offline) {
                gameUI->execMessageBox(tr("Player timeout"),
                                       tr("%1 is timeout").arg(assets->getPlayerName(currentPlayer)),
                                       PlayerTimeoutDuration);
                command = Command::RightClick;
                allowCancel = true;
            }

            if (command == Command::RightClick) {
                //user try to cancel
                if (allowCancel) {//allow cancel,user will choose another card
                    gameUI->releaseSource();
                    break;
                }
            } else if (command == Command::LeftClick) {
                assets->setHandled(true);
                //meaningful input
                gameUI->releaseSource();//when choose position,before move we should release the SOURCE visual effect
                GameController::controller()->moveCardToPos(seletedCard, toR, toC);
                break;
            }
        }
    }
}

void Lacerate::slotOnDeploy(CardInfo *mover, int fromR, int toR) {
    if (mover->getCardName() == "Lacerate") {
        GameAssets *assets = GameController::controller()->getAssets();
        QList<CardInfo *> dests;
        const QList<CardInfo *> &targetBattlefield = assets->getCardArray(toR);
        for (auto unit:targetBattlefield) {
            if (unit != mover) {//do not damage the lacerate itself
                dests.append(unit);
            }
        }
        GameController::controller()->damageFromSrcToDests(mover, dests, 3, true);
        GameController::controller()->DestroySpecialCard(mover, fromR);//effect card, destroy
    }
}
