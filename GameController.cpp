//
// Created by Gerwa on 2017/9/5.
//

#include "GameController.h"
#include "SubclassRegister.h"

void GameController::startGame() {//player use which deck is already choosen
    //for cut line relink, no local variable is permitted
    assets->setPreviousWinner(-1);//-1 means start or draw
    assets->setCurrentRound(0);
    assets->setRoundStart(true);
    GameController::controller()->getGameUI()->
            loadCardFromAssets(GameController::controller()->getAssets());
    //TODO modify the test code
    gameUI->switchToScene(AbstractUI::GameScene);
    for (int player = 0; player < 2; player++) {
        assets->setPlayerWinRound(player, 0);
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
    if (assets->isRoundStart()) {//round start not long before
        assets->setRoundStart(false);
        DestroyAllCardsOnBattlefield();//clear the battlefield
        clearWeatherOnAllBattlefield();
        for (int player = 0; player < 2; player++) {
            assets->setPlayerPass(player, false);//always forget to initialize a new round
        }
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
                    gameUI->setWholeRowValidPositions(assets->getHandIndex(currentPlayer));
                    gameUI->setButtonEnabled(BUTTON_PASS, true);
                    Command command;
                    int row, column;
                    gameUI->getUserInput(command, row, column, currentPlayer);
                    gameUI->setButtonEnabled(BUTTON_PASS, false);
                    gameUI->resetValidPositions();
                    gameUI->setPlayerInputState(currentPlayer, AbstractUI::RejectAll);

                    if (command == Command::Pass) {
                        //user choose to pass
                        assets->setHandled(true);// no need to request another user input
                        setPlayerPassAndJudgeRoundEnd(currentPlayer);
                    } else if (command == Command::LeftClick) {
                        //not handle yet, maybe cancelled by right click
                        CardInfo *seletedCard = assets->getCardArray(row).at(column);
                        requestPlayerToPlayFromHand(seletedCard, true);
                    } else {
                        qWarning() << "unhandled user input:" << static_cast<int>(command);
                    }
                }
            }
        }
        if (assets->isGameEnd()) {
            return false;
        } else {
            setCurrentPlayerForAssetsAndUI(currentPlayer ^ 1);//play animation, may block
        }
    }
    return true;
}

void GameController::moveCardToRightTop(CardInfo *mover, int targetRow) {
    int row, column;
    assets->getCardPosition(mover, row, column);

    controllerHandleBeforeMove(mover, row, targetRow);

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

    gameUI->setLabelText(Player_CardChooser_Title, title);//player0 14 player1 15 card chooser title
    gameUI->setButtonEnabled(ESCAPE, allowEscape);

    int cardChoosed = 0;
    while (cardChoosed < chooseNum) {

        Command command;
        int row, column;
        gameUI->setWholeRowValidPositions(candidateIndex);
        gameUI->setPlayerInputState(player, AbstractUI::MustValidTarget);
        gameUI->getUserInput(command, row, column, player);
        gameUI->setPlayerInputState(player, AbstractUI::RejectAll);
        gameUI->resetValidPositions();

        if (command == Command::LeftClick) {//must be a valid input (filtered by UI)
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
        } else if (command == Command::RightClick) {
            if (allowSwitchScene) {
                if (gameUI->getCurrentScene() != AbstractUI::GameScene) {//in local player chooser scene
                    gameUI->switchToScene(AbstractUI::GameScene);
                } else if (gameUI->getCurrentScene() == AbstractUI::GameScene) {
                    gameUI->switchToScene(AbstractUI::PlayerChooserScene);
                }
            }
        } else if (command == Command::Escape) {//map two buttons to a same command
            //already use a message box to ensure the opearation via UI
            break;
        }
    }
}

void GameController::handleRedrawCard() {//handle two players' redraw card
    for (int player = 0; player < 2; player++) {
        //add card from deck to hand first
        moveNCardsFromDeckToHand(player, NumberToChoose[assets->getCurrentRound()]);

        gameUI->switchToScene(AbstractUI::PlayerChooserScene);

        //perform move card from hand to candidate
        int handIndex = assets->getHandIndex(player);
        performMoveAllCardsFromAToB(handIndex, Player_Candidate);

        int deckIndex = assets->getDeckIndex(player);
        performChooseCard(Player_Candidate, Player_Seleted,
                          deckIndex, NumberToRedraw[assets->getCurrentRound()], player,
                          true, true, true, tr("%1 is choosing %2 card(s)").arg(
                        assets->getPlayerName(player)).arg(NumberToRedraw[assets->getCurrentRound()]));

        //insert the seleted randomly to deck
        performRandomlyMoveAllCardsFromAToB(Player_Seleted, deckIndex);
        //perform move candidate BACK to hand
        performMoveAllCardsFromAToB(Player_Candidate, handIndex);

        gameUI->switchToScene(AbstractUI::GameScene);
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
        message += tr("\n%1 : %2\n%3 %4").arg(assets->getPlayerName(0)).arg(assets->getPlayerName(1)).arg(
                combatValue[0]).arg(combatValue[1]);

        gameUI->execMessageBox(tr("Round Ended"), message, RoundEndDuration);
        assets->addCurrentRound();
        assets->setRoundStart(true);//start another round
    }
}

void GameController::DestroyAllCardsOnBattlefield() {//some Doomed card should be removed from game
    for (int row = Player1_Siege; row <= Player0_Siege; row++) {//all battlefield
        while (!assets->getCardArray(row).empty()) {
            CardInfo *mover = assets->getCardArray(row).last();
            DestroyCard(mover);
        }
    }
}

void GameController::moveToGraveyard(CardInfo *mover) {
    int row, column;
    assets->getCardPosition(mover, row, column);
    mover->resetAttributes();
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

GameController *GameController::controller() {
    if (mainController == nullptr) {
        mainController = new GameController();
        registerSubclasses();
    }
    return mainController;
}

void GameController::defaultHandleHand(CardInfo *seletedCard, bool allowCancel) {
    //default behavior: choose the row and pick it
    int currentPlayer = assets->getCurrentPlayer();
    int fromR, fromC;
    assets->getCardPosition(seletedCard, fromR, fromC);
    gameUI->setSource(fromR, fromC);


    //set valid rows
    int targetPlayer = currentPlayer;
    if (seletedCard->getLoyalty() == CardInfo::Disloyal) {
        targetPlayer = currentPlayer ^ 1;
    }

    while (true) {
        switch (seletedCard->getRow()) {
            case CardInfo::Event:
                gameUI->setValidRow(assets->getPlayerMelee(targetPlayer));
                gameUI->setValidRow(assets->getPlayerRanged(targetPlayer));
                gameUI->setValidRow(assets->getPlayerSiege(targetPlayer));
                break;
            case CardInfo::Melee:
                gameUI->setValidRow(assets->getPlayerMelee(targetPlayer));
                break;
            case CardInfo::Ranged:
                gameUI->setValidRow(assets->getPlayerRanged(targetPlayer));
                break;
            case CardInfo::Siege:
                gameUI->setValidRow(assets->getPlayerSiege(targetPlayer));
                break;
            default:
                qWarning() << "invalid Row Card info!1238764";
        }
        gameUI->setPlayerInputState(currentPlayer, AbstractUI::MustValidRow);
        Command command;
        int toR, toC;
        gameUI->getUserInput(command, toR, toC, currentPlayer);
        gameUI->setPlayerInputState(currentPlayer, AbstractUI::RejectAll);
        gameUI->resetValidRows();

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
            moveCardToPos(seletedCard, toR, toC);
            break;
        }
    }
}

void GameController::clearWeatherOnAllBattlefield() {
    for (int row = Player1_Siege; row <= Player0_Siege; row++) {
        assets->setRowWeather(row, Weather::Sunny);
    }
    gameUI->clearAllWeatherOnBattlefield();
}

void GameController::DestroyCard(CardInfo *card) {
    if (assets->cardAlreadyInGraveyard(card)) {
        removeCardFromGame(card);
    } else {
        moveToGraveyard(card);
        if (card->hasAttribute("Doomed")) {
            removeCardFromGame(card);
        }
    }
}

void GameController::removeCardFromGame(CardInfo *card) {
    int row, column;
    assets->getCardPosition(card, row, column);
    gameUI->removeCardFromGame(row, column);//must perform before
    assets->removeCardFromGame(row, column);
}

void GameController::Consume(CardInfo *consumer, CardInfo *food) {
    //do not destroy the food here, let processor to destroy it later
    int nutrition = food->getCurrentStrength();
    //user the pointer only, no need to access the asset
    consumer->setCurrentStrength(consumer->getCurrentStrength() + nutrition);
    food->setCurrentStrength(0);
    emit onConsume(consumer, food);//the gameUI hasn't update the screen, no one can see or know time difference

    int fromR, fromC, toR, toC;
    assets->getCardPosition(consumer, fromR, fromC);
    assets->getCardPosition(food, toR, toC);
    gameUI->showConsume(fromR, fromC, toR, toC);
    emit afterConsume(consumer, food);
}

void GameController::subtractCD(CardInfo *unit) {
    unit->setCD(unit->getCD() - 1);
    int row, column;
    assets->getCardPosition(unit, row, column);
    gameUI->showSubtractCD(row, column);
}

void GameController::resumeCD(CardInfo *unit, int initialCD) {
    unit->setCD(initialCD);
    int row, column;
    assets->getCardPosition(unit, row, column);
    gameUI->showResumeCD(row, column);
}

void GameController::boostFromSrcToDests(CardInfo *src, const QList<CardInfo *> &dests, int boost, int armor) {
    for (auto dest:dests) {
        dest->setArmor(dest->getArmor() + armor);
        dest->setCurrentStrength(dest->getCurrentStrength() + boost);
    }
    //no card listens to boost
    int row, column;
    QPoint srcPoint;
    if (src != nullptr) {
        assets->getCardPosition(src, row, column);
        srcPoint = QPoint(row, column);
    } else {
        srcPoint = QPoint(-1, -1);
    }
    QList<QPoint> destPoints;
    for (auto dest:dests) {
        assets->getCardPosition(dest, row, column);
        destPoints.append(QPoint(row, column));
    }
    gameUI->showBoost(srcPoint, destPoints);
}

void GameController::damageFromSrcToDests(CardInfo *src, const QList<CardInfo *> &dests, int damage, bool armorUseful) {
    for (auto dest:dests) {
        int tempDamage = damage;
        if (dest->isShield()) {//has a shield
            dest->setShield(false);
        } else {
            if (armorUseful) {
                int resist = std::min(tempDamage, dest->getArmor());
                tempDamage -= resist;
                dest->setArmor(dest->getArmor() - resist);
            }
            tempDamage = std::min(tempDamage, dest->getCurrentStrength());
            dest->setCurrentStrength(dest->getCurrentStrength() - tempDamage);
        }
    }

    int row, column;
    QPoint srcPoint;
    if (src != nullptr) {
        assets->getCardPosition(src, row, column);
        srcPoint = QPoint(row, column);
    } else {
        srcPoint = QPoint(-1, -1);
    }
    QList<QPoint> destPoints;
    for (auto dest:dests) {
        assets->getCardPosition(dest, row, column);
        destPoints.append(QPoint(row, column));
    }
    gameUI->showDamage(srcPoint, destPoints);

    for (auto dest:dests) {//check if someone should be destroyed
        if (dest->getCurrentStrength() == 0) {
            DestroyCard(dest);
        }
    }
}

void
GameController::spawnCardToPosByPlayer(CardInfo *card, int row, int column,//the card must be a new constructed instance
                                       int player) {//the player who triggered this operation
    assets->getCardArray(row).insert(column, card);
    gameUI->spawnNewCard(card, row, column);
    if (assets->isBattleField(row)) {
        controllerHandleDeploy(card, assets->getDeckIndex(player), row);//trigger deploy
    }
}

void
GameController::handleHandValidPosition(CardInfo *seletedCard, bool allowCancel, const QList<QPoint> &validPositions,
                                        Command &command, int &toR, int &toC) {
    int currentPlayer = assets->getCurrentPlayer();
    int fromR, fromC;
    assets->getCardPosition(seletedCard, fromR, fromC);
    gameUI->setSource(fromR, fromC);

    while (true) {
        gameUI->setValidPositions(validPositions);
        gameUI->setPlayerInputState(currentPlayer, AbstractUI::MustValidTarget);

        gameUI->getUserInput(command, toR, toC, currentPlayer);
        gameUI->setPlayerInputState(currentPlayer, AbstractUI::RejectAll);
        gameUI->resetValidPositions();

        if (command == Command::RightClick) {
            if (allowCancel) {
                gameUI->releaseSource();
                break;
            }
        } else if (command == Command::LeftClick) {
            assets->setHandled(true);
            gameUI->releaseSource();
            break;
        }
    }
}

void GameController::requestPlayerToPlayFromHand(CardInfo *seletedCard, bool allowCancel) {
    emit onHandSeleted(seletedCard, allowCancel);
}

void GameController::changeRowWeather(int row, Weather weatherType) {
    Weather oldWeather = assets->getRowWeather(row);
    assets->setRowWeather(row, weatherType);
    gameUI->setRowWeather(row, weatherType);
    emit afterWeatherChanged(row, oldWeather);
}

void GameController::causeWeatherDamage(int row, const QList<CardInfo *> &dests, int damage, bool armorUseful) {
    emit onWeatherDamage(row, dests);
    damageFromSrcToDests(nullptr, dests, damage, armorUseful);
}

void GameController::DestroySpecialCard(CardInfo *specialCard, int fromR) {
    if (assets->cardAlreadyInGraveyard(specialCard)) {
        removeCardFromGame(specialCard);
    } else {
        int fromPlayer = assets->whosePlayerRow(fromR);
        int graveyardIndex = (fromPlayer == 0 ? Player0_Graveyard : Player1_Graveyard);
        moveCardToRightTop(specialCard, graveyardIndex);
        if (specialCard->hasAttribute("Doomed")) {
            removeCardFromGame(specialCard);
        }
    }
}


