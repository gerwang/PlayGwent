//
// Created by Gerwa on 2017/9/9.
//

#include "Bekkers_Twisted_Mirror.h"

void Bekkers_Twisted_Mirror::slotOnHandSeleted(CardInfo *seletedCard, bool allowCancel) {
    if (seletedCard->getCardName() == "Bekkers_Twisted_Mirror") {
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
                allowCancel = true;
                command = Command::RightClick;
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

void Bekkers_Twisted_Mirror::slotOnDeploy(CardInfo *mover, int fromR, int toR) {
    if (mover->getCardName() == "Bekkers_Twisted_Mirror") {
        GameAssets *assets = GameController::controller()->getAssets();
        QList<CardInfo *> strongList, weakList;
        for (int row = Player1_Siege; row <= Player0_Siege; row++) {//iterate all battlefield
            assets->updateRowStrongest(row, strongList, mover);
            assets->updateRowWeakest(row, weakList, mover);
        }
        if (!strongList.empty()) {
            int strongRandomIndex = qrand() % strongList.size();
            CardInfo *strongCard = strongList[strongRandomIndex];
            int nutrition = std::min(10, strongCard->getCurrentStrength());
            GameController::controller()->damageFromSrcToDests(mover, {strongCard}, nutrition, false);

            for (int index = weakList.size() - 1; index >= 0; index--) {//iterate reversely because of delete
                if (weakList[index]->getCardName() == strongCard->getCardName()) {
                    weakList.removeAt(index);
                }
            }
            if (!weakList.empty()) {
                int weakRandomIndex = qrand() % weakList.size();
                CardInfo *weakCard = weakList[weakRandomIndex];
                GameController::controller()->boostFromSrcToDests(mover, {weakCard}, nutrition, 0);//0 armor
            }
        }
        GameController::controller()->DestroySpecialCard(mover, fromR);
        //effect card, delete
    }
}
