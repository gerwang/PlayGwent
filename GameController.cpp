//
// Created by Gerwa on 2017/9/5.
//

#include "GameController.h"
#include <QFutureWatcher>

GameAssets *GameController::getAssets() {
    return assets;
}

AbstractUI *GameController::getUI() {
    return gameUI;
}

GameController *GameController::getController() {
    if (mainController == nullptr) {
        mainController = new GameController();
    }
    return mainController;
}

void GameController::loadUIfromAssets() {
    //TODO
}

template<typename T>
void GameController::registerProcessor(T *childProcessor) {
    //TODO

}

void GameController::startGame() {//player use which deck is already choosen
    //for cut line relink, no local variable is permitted
    assets->resetRandomSeed();//set random seed, no need to synchosize random bahaviors
    assets->setPreviousWinner(-1);//-1 means start or draw
    assets->setCurrentRound(0);
    while (gameLoop()) {
        //pass
    }
    //TODO: game end, enter reporing interface

}

bool GameController::gameLoop() {
    if (assets->isGameEnd()) {
        return false;
    }
    if (assets->isNeedChooseCard()) {//round start not long before
        assets->setNeedChooseCard(false);
        handleRedrawCard();
        if (assets->getPreviousWinner() == -1) {//start or draw
            gameUI->playRandomCoinAnimation();//let ui play the animation
            setCurrentPlayerForAssetsAndUI(qrand() % 2);
        } else {
            setCurrentPlayerForAssetsAndUI(assets->getPreviousWinner());
        }
    } else {//users' turn start
        emit onTurnStart();//calculate weather and abilities here
        const int currentPlayer = assets->getCurrentPlayer();
        if (!assets->getPlayerPass(currentPlayer)) {//player not pass
            if (assets->isPlayerHandEmpty(currentPlayer)) {
                //force to pass
                assets->setPlayerPass(currentPlayer, true);
                if (assets->isAllPlayerPass()) {
                    //TODO all player has end round finished
                } else {
                    //do nothing, switch to another player
                }
            } else {
                //Player must input
                assets->setHandled(false);// not lock a player
                while (!assets->isHandled()) {
                    //user choose a hand card

                    gameUI->setPlayerInputState(currentPlayer, AbstractUI::MustValidTarget);
                    gameUI->resetValidPositions();//set all to invalid
                    gameUI->setWholeRowValidPositions(assets->getHandIndex(currentPlayer));
                    gameUI->setButtonEnabled(BUTTON_PASS, true);
                    QString command;
                    int row, column;
                    gameUI->getUserInput(command, row, column, currentPlayer);
                    gameUI->setButtonEnabled(BUTTON_PASS, false);
                    gameUI->setPlayerInputState(currentPlayer, AbstractUI::RejectAll);

                    if (command == "pass") {
                        //user choose to pass

                    } else if (command == "leftclick") {

                    } else {
                        qWarning() << "unhandled user input:" << command;
                    }
                }
            }
        }
        assets->setCurrentPlayer(currentPlayer ^ 1);
    }
    //TODO upload the current state to server
    return true;
}

void GameController::moveCardToRightTop(CardInfo *mover, int targetRow) {
    int row, column;
    assets->getCardPosition(mover, row, column);
    controllerHandleBeforeMove(mover, row, column);
    assets->getCardPosition(mover, row, column);

    performMoveCardToRightTop(row, column, targetRow);

    controllerHandleAfterMove(mover, row, targetRow);
}

void GameController::moveCardToPos(CardInfo *mover, int toR, int toC) {
    //WARNING: always ASSUME that the target row index would not change by abilities
    int row, column;
    assets->getCardPosition(mover, row, column);
    controllerHandleBeforeMove(mover, row, toR);
    assets->getCardPosition(mover, row, column);//must update the pos!

    performMovePosToPos(row, column, toR, toC);//take care that the target column index may change because of ability,
    //but QList can handle the out of index insertion well

    controllerHandleAfterMove(mover, row, toR);
}

void GameController::controllerHandleAfterMove(CardInfo *mover, int fromR, int toR) {
    if (!assets->isBattleField(fromR) && assets->isBattleField(toR)) {//deploy
        controllerHandleDeploy(mover, fromR, toR);
    } else {
        emit afterMoveForUser(mover, fromR, toR);
    }
}

void GameController::controllerHandleDeploy(CardInfo *mover, int fromR, int toR) {
    emit beforeDeploy(mover, fromR, toR);
    emit onDeploy(mover, fromR, toR);
}

void GameController::controllerHandleBeforeMove(CardInfo *mover, int fromR, int toR) {
    if (assets->isBattleField(fromR) && assets->isGraveyard(toR)) {
        emit onDeathWish(mover, fromR, toR);
    } else {
        emit beforeMoveForUser(mover, fromR, toR);
    }
}

void GameController::performMovePosToPos(int fromR, int fromC, int toR, int toC) {
    //always move assets before ui because animation would block
    assets->moveCardPosToPos(fromR, fromC, toR, toC);
    gameUI->moveCard(fromR, fromC, toR, toC);
}

void
GameController::performChooseCard(int candidateIndex, int seletedIndex, int supplementIndex, int chooseNum, int player,
                                  bool supply, bool allowEscape, const QString &title) {
    QList<CardInfo *> &candidate = assets->getCardArray(candidateIndex),
            &seleted = assets->getCardArray(seletedIndex),
            &supplement = assets->getCardArray(supplementIndex);

    int playerChooserTitle = (player == 0 ? Player0_CardChooser_Title : Player1_CardChooser_Title);
    gameUI->setLabelText(playerChooserTitle, title);//player0 14 player1 15 card chooser title
    PushButtonEnum playerButton = (player == 0 ? ESCAPE0 : ESCAPE1);
    gameUI->setButtonEnabled(playerButton, allowEscape);

    int cardChoosed = 0;
    while (cardChoosed < chooseNum) {

        QString command;
        int row, column;
        gameUI->setPlayerInputState(player, AbstractUI::MustValidTarget);
        gameUI->getUserInput(command, row, column, player);
        gameUI->setPlayerInputState(player, AbstractUI::RejectAll);

        if (command == "leftclick") {//must be a valid input (filtered by UI)
            if (row != candidateIndex) {
                qWarning() << "Invalid input!351468786";
            }

            performMoveCardToRightTop(row, column, seletedIndex);
            cardChoosed++;

            if (supply) {//need to supply from supplyment
                //find the topmost item that are not in the blacklist according to NAME
                int nextColumn = assets->findTopmostANameNotInB(supplement, seleted);
                if (nextColumn != -1) {//really has one to supply
                    performMovePosToPos(supplementIndex, nextColumn, candidateIndex, column);
                }
            }
        } else if (command == "rightclick") {
            if (player == gameUI->getLocalPlayer()) {//must be a local player to switch scene
                if (gameUI->getCurrentScene() != AbstractUI::GameScene) {//in local player chooser scene
                    gameUI->switchToScene(AbstractUI::GameScene);
                } else if (gameUI->getCurrentScene() == AbstractUI::GameScene) {
                    switchUIToChooserScene(player);//player==localplayer
                } else {
                    qWarning() << "invalid scene switch!1237896";
                }
            }
        } else if (command == "escape" + QString::number(player)) {//two buttons: escape0 and escape1
            //already use a message box to ensure the opearation via UI
            break;
        }
    }

    if (player == gameUI->getLocalPlayer()) {
        //switch back to scene, do not wait for the other player
        gameUI->switchToScene(AbstractUI::GameScene);
    }
}


void GameController::dispatchChooseCard() {
    QFutureSynchronizer synchronizer;
    for (int player = 0; player < 2; player++) {
        synchronizer.addFuture(QtConcurrent::run(this, &GameController::performRedrawCard, player));
    }
}


void GameController::handleRedrawCard() {//handle two players' redraw card
    for (int player = 0; player < 2; player++) {
        //add card from deck to hand first
        moveNCardsFromDeckToHand(player, NumberToChoose[assets->getCurrentRound()]);
        //perform move card from hand to candidate
        int playerDeck = assets->getDeckIndex(player);
        int playerCandidate = assets->getCandidateIndex(player);
        performMoveAllCardsFromAToB(playerDeck, playerCandidate);
    }

    switchUIToChooserScene(gameUI->getLocalPlayer());

    QFutureWatcher<void> watcher;
    QEventLoop eventLoop;
    connect(&watcher, &QFutureWatcher<void>::finished, &eventLoop, &QEventLoop::quit);
    QFuture future = QtConcurrent::run(this, &GameController::dispatchChooseCard);
    watcher.setFuture(future);
    eventLoop.exec();

    for (int player = 0; player < 2; player++) {
        int seletedIndex = assets->getSeletedIndex(player);
        int deckIndex = assets->getDeckIndex(player);
        //insert the seleted randomly to deck
        performRandomlyMoveAllCardsFromAToB(seletedIndex, deckIndex);
        //perform move candidate BACK to hand
        int candidateIndex = assets->getCandidateIndex(player);
        int handIndex = assets->getHandIndex(player);
        performMoveAllCardsFromAToB(candidateIndex, handIndex);
    }
}

void GameController::performRedrawCard(int player) {
    if (player == 0) {
        performChooseCard(Player0_Candidate, Player0_Seleted,
                          Player0_Deck, NumberToRedraw[assets->getCurrentRound()],
                          player, true, true, tr("Select card(s) to redraw:"));
        performMoveAllCardsFromAToB(Player0_Candidate, Player0_Hand);
    } else if (player == 1) {
        performChooseCard(Player1_Candidate, Player1_Seleted,
                          Player1_Deck, NumberToRedraw[assets->getCurrentRound()],
                          player, true, true, tr("Selete card(s) to redraw:"));
        performMoveAllCardsFromAToB(Player1_Candidate, Player1_Hand);
    }
}

void GameController::performRandomlyMoveAllCardsFromAToB(int fromR, int toR) {
    for (int fromC = assets->getCardArray(fromR).size() - 1; fromC >= 0; fromC--) {
        int toC = qrand() % assets->getCardArray(toR).size();//must generate random number in order!
        performMovePosToPos(fromR, fromC, toR, toC);
    }
}

void GameController::performMoveAllCardsFromAToB(int fromR, int toR) {//must follow the order
    while (!assets->getCardArray(fromR).empty()) {//from left to right or from buttom to top
        performMoveCardToRightTop(fromR, 0, toR);
    }
}

void GameController::performMoveCardToRightTop(int row, int column, int targetRow) {
    int targetColumn = assets->getCardArray(targetRow).size();//to right top
    performMovePosToPos(row, column, targetRow, targetColumn);
}

void GameController::switchUIToChooserScene(int player) {
    if (player == 0) {
        gameUI->switchToScene(AbstractUI::Player0ChooserScene);
    } else if (player == 1) {
        gameUI->switchToScene(AbstractUI::Player1ChooserScene);
    } else {
        qWarning() << "Error!235832549";
    }
}

void GameController::moveNCardsFromDeckToHand(int player, int n) {
    int playerDeck = assets->getDeckIndex(player);
    int playerHand = assets->getHandIndex(player);
    for (int cnt = 0; cnt < n; cnt++) {
        if (!assets->getCardArray(playerDeck).empty()) {
            CardInfo *topCard = assets->getCardArray(playerDeck).last();//the topmost card column number in deck
            moveCardToRightTop(topCard, playerHand);
            //this can trigger abilities cause some factions really listen to the operation (not monsters uh)
        } else {
            qWarning() << "no enough card to choose from deck!213798";
        }
    }
}

void GameController::setCurrentPlayerForAssetsAndUI(int player) {
    assets->setCurrentPlayer(player);
    gameUI->setCurrentPlayer(player);//this function may block
}
