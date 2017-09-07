//
// Created by Gerwa on 2017/9/5.
//

#include "GameController.h"
#include <QFutureWatcher>

template<typename T>
void GameController::registerProcessor(T *childProcessor) {
    connect(this, &GameController::afterMoveForUser, childProcessor, &T::slotAfterMoveForUser);
    connect(this, &GameController::beforeMoveForUser, childProcessor, &T::slotBeforeMoveForUser);
    connect(this, &GameController::beforeDeploy, childProcessor, &T::slotBeforeDeploy);
    connect(this, &GameController::onDeploy, childProcessor, &T::slotOnDeploy);
    connect(this, &GameController::onDeathWish, childProcessor, &T::slotOnDeathWish);
    connect(this, &GameController::onTurnStart, childProcessor, &T::slotOnTurnStart);
    connect(this, &GameController::onHandSeleted, childProcessor, &T::slotOnHandSeleted);
    connect(this, &GameController::afterDeploy, childProcessor, &T::slotAfterDeploy);
}

void GameController::startGame() {//player use which deck is already choosen
    //for cut line relink, no local variable is permitted
    assets->setPreviousWinner(-1);//-1 means start or draw
    assets->setCurrentRound(0);
    assets->setRoundStart(true);
    assets->clearWeatherOnAllRows();//TODO make GUI perform the same
    GameController::getController()->getGameUI()->
            loadCardFromAssets(GameController::getController()->getAssets());
    //TODO modify the test code
    for (int player = 0; player < 2; player++) {
        assets->setPlayerWinRound(player, 0);
        assets->setPlayerPass(player, false);
        for (int Round = 0; Round < 3; Round++) {
            assets->setPlayerScore(player, Round, 0);
        }
    }

    while (gameLoop()) {
        //TODO upload the current state to server
    }
    //game end, enter reporing interface
    QString message = assets->getPlayerName(0) + tr(" VS ") + assets->getPlayerName(1) + tr("\n");
    for (int roundCount = 0; roundCount < assets->getCurrentRound(); roundCount++) {
        //the current new round will not be processed, so use < instead of <=
        message += QString::number(assets->getPlayerScore(0, roundCount)) +
                   tr(" : ") +
                   QString::number(assets->getPlayerScore(1, roundCount)) +
                   tr("\n");
    }
    QString title = tr("Game Draw!");
    if (assets->getPlayerWinRound(0) < assets->getPlayerWinRound(1)) {
        title = assets->getPlayerName(1) + tr(" wins the game!");
    } else if (assets->getPlayerWinRound(0) > assets->getPlayerWinRound(1)) {
        title = assets->getPlayerName(0) + tr(" wins the game!");
    }
    //CAUTION: compare win rounds when game end, not total score!
    gameUI->execMessageBox(title, message, GameEndDuration);
    //game end
}

bool GameController::gameLoop() {
    if (assets->isGameEnd()) {
        return false;
    }
    if (assets->isRoundStart()) {//round start not long before
        assets->setRoundStart(false);
        moveAllFromBattlefieldToGraveyard();//clear the battlefield
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
                setPlayerPassAndJudgeRoundEnd(currentPlayer);
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
                        assets->setHandled(true);// no need to request another user input
                        setPlayerPassAndJudgeRoundEnd(currentPlayer);
                    } else if (command == "leftclick") {
                        //not handle yet, maybe cancelled by right click
                        CardInfo *seletedCard = assets->getCardArray(row).at(column);
                        emit onHandSeleted(seletedCard);
                    } else {
                        qWarning() << "unhandled user input:" << command;
                    }
                }
            }
        }
        assets->setCurrentPlayer(currentPlayer ^ 1);
    }
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
    emit afterDeploy(mover, fromR, toR);//for foglet
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
                                  bool supply, bool allowEscape, bool allowSwitchScene, const QString &title) {
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
            if (allowSwitchScene) {
                if (gameUI->getCurrentScene() != AbstractUI::GameScene) {//in local player chooser scene
                    gameUI->switchToScene(AbstractUI::GameScene);
                } else if (gameUI->getCurrentScene() == AbstractUI::GameScene) {
                    switchUIToChooserScene(player);//player==localplayer
                }
            }
        } else if (command == "escape" + QString::number(player)) {//two buttons: escape0 and escape1
            //already use a message box to ensure the opearation via UI
            break;
        }
    }
}


void GameController::dispatchChooseCard() {
    QFutureSynchronizer<void> synchronizer;
    for (int player = 0; player < 2; player++) {
        synchronizer.addFuture(QtConcurrent::run(this, &GameController::performRedrawCard, player));
    }
}


void GameController::handleRedrawCard() {//handle two players' redraw card
    for (int player = 0; player < 2; player++) {
        //add card from deck to hand first
        moveNCardsFromDeckToHand(player, NumberToChoose[assets->getCurrentRound()]);
        //perform move card from hand to candidate
        int playerHand = assets->getHandIndex(player);
        int playerCandidate = assets->getCandidateIndex(player);
        performMoveAllCardsFromAToB(playerHand, playerCandidate);
        int candidateIndex = assets->getCandidateIndex(player);
        gameUI->resetValidPositions();
        gameUI->setWholeRowValidPositions(candidateIndex);
        int seletedIndex = assets->getSeletedIndex(player);
        int deckIndex = assets->getDeckIndex(player);
        //insert the seleted randomly to deck
        performRandomlyMoveAllCardsFromAToB(seletedIndex, deckIndex);
        //perform move candidate BACK to hand
        int candidateIndex = assets->getCandidateIndex(player);
        int handIndex = assets->getHandIndex(player);
        performMoveAllCardsFromAToB(candidateIndex, handIndex);
//
//    switchUIToChooserScene(gameUI->getLocalPlayer());
//    gameUI->resetValidPositions();//must be set here because multithread can ruin the validposition
//    gameUI->setWholeRowValidPositions(Player0_Candidate);
//    gameUI->setWholeRowValidPositions(Player1_Candidate);
//
//    QFutureWatcher<void> watcher;
//    QEventLoop eventLoop;
//    connect(&watcher, &QFutureWatcher<void>::finished, &eventLoop, &QEventLoop::quit);
//    QFuture<void> future = QtConcurrent::run(this, &GameController::dispatchChooseCard);
//    watcher.setFuture(future);
//    eventLoop.exec();

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

void GameController::setPlayerPassAndJudgeRoundEnd(int currentPlayer) {
    assets->setPlayerPass(currentPlayer, true);
    if (assets->isAllPlayerPass()) {
        //end a round

        //calculate
        int combatValue[2] = {assets->getPlayerCombatValueSum(0),
                              assets->getPlayerCombatValueSum(1)};
        int currentRound = assets->getCurrentRound();
        for (int player = 0; player < 2; player++) {
            assets->setPlayerScore(player, currentRound, combatValue[player]);
        }

        QString message = tr("Round") + QString::number(currentRound + 1);

        int localPlayer = gameUI->getLocalPlayer();
        if (combatValue[localPlayer] < combatValue[localPlayer ^ 1]) {
            message += tr(" Lose!");
            assets->addPlayerWinRound(localPlayer ^ 1);
            assets->setPreviousWinner(localPlayer ^ 1);
        } else if (combatValue[localPlayer] == combatValue[localPlayer ^ 1]) {
            message += tr(" Draw!");
            assets->setPreviousWinner(-1);//dummy value, draw
        } else if (combatValue[localPlayer] > combatValue[localPlayer ^ 1]) {
            message += tr(" Win!");
            assets->addPlayerWinRound(localPlayer);
            assets->setPreviousWinner(localPlayer);
        }

        gameUI->execMessageBox(tr("Round Ended"), message, RoundEndDuration);
        assets->addCurrentRound();
        assets->setRoundStart(true);//start another round
    }
}

void GameController::moveAllFromBattlefieldToGraveyard() {
    for (int row = Player1_Siege; row <= Player0_Siege; row++) {//all battlefield
        while (!assets->getCardArray(row).empty()) {
            CardInfo *mover = assets->getCardArray(row).last();
            moveToGraveyard(mover);
        }
    }
}

void GameController::moveToGraveyard(CardInfo *mover) {
    int row, column;
    assets->getCardPosition(mover, row, column);
    if (row <= Player1_Melee) {//player1's card
        moveCardToRightTop(mover, Player1_Graveyard);
    } else {
        moveCardToRightTop(mover, Player0_Graveyard);
    }
}

AbstractUI *GameController::getGameUI() const {
    return gameUI;
}

void GameController::setGameUI(AbstractUI *gameUI) {
    GameController::gameUI = gameUI;
}

void GameController::setAssets(GameAssets *assets) {
    GameController::assets = assets;
}

GameAssets *GameController::getAssets() const {
    return assets;
}

GameController *GameController::mainController = nullptr;

GameController *GameController::getController() {
    if (mainController == nullptr) {
        mainController = new GameController();
        registerSubclasses();
    }
    return mainController;
}
