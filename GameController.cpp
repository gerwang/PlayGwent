//
// Created by Gerwa on 2017/9/5.
//

#include "GameController.h"
#include "SubclassRegister.h"
#include "Deck.h"
#include "GraphicsUI.h"
#include "RoomDialog.h"
#include "LoginDialog.h"
#include <QGuiApplication>
#include <QClipboard>
#include <QApplication>
#include <QtWidgets/QMessageBox>

void GameController::startGame() {//player use which deck is already choosen
    //for cut line relink, no local variable is permitted

    loadFromDeckAndJson();
    assets->setPreviousWinner(-1);//-1 means start or draw
    assets->setCurrentRound(0);
    assets->setRoundStatus(0);
    assets->setCurrentPlayer(0);
    QJsonObject object;
    assets->toJson(object);
    assets->fromJson(object);//TODO
    gameUI->loadCardFromAssets(assets);
    gameUI->switchToScene(AbstractUI::GameScene);
    for (int player = 0; player < 2; player++) {
        assets->setPlayerWinRound(player, 0);
        for (int Round = 0; Round < 3; Round++) {
            assets->setPlayerScore(player, Round, 0);
        }
    }
    enterGameLoop(false);
}

void GameController::gameLoop() {
    gameUI->resetRound();
    gameUI->setUseTimerFlag(true);
    if (assets->getRoundStatus() < 2) {//round start not long before
        if (assets->getRoundStatus() == 0) {
            DestroyAllCardsOnBattlefield();//clear the battlefield
            clearWeatherOnAllBattlefield();
            for (int player = 0; player < 2; player++) {
                assets->setPlayerPass(player, false);//always forget to initialize a new round
            }
            handleRedrawCard(0);
            assets->setCurrentPlayer(1);//for player1 to choose card
            if (gameUI->getLocalPlayer() == 0) {
                transmitEndMessage();
            }
        } else if (assets->getRoundStatus() == 1) {
            handleRedrawCard(1);
            if (assets->getPreviousWinner() == -1) {
                assets->setCurrentPlayer(-1);
            } else {
                assets->setCurrentPlayer(assets->getPreviousWinner());
            }
            if (gameUI->getLocalPlayer() == 1) {
                transmitEndMessage();
            }
        }
        assets->setRoundStatus(assets->getRoundStatus() + 1);
    } else {//users' turn start
        gameUI->setAnimationFlag(true);
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

                    if (gameUI->getLocalPlayer() == assets->getCurrentPlayer()) {
                        gameUI->setButtonEnabled(BUTTON_PASS, true);
                    }
                    Command command;
                    int row, column;
                    gameUI->getUserInput(command, row, column, currentPlayer);
                    if (gameUI->getLocalPlayer() == assets->getCurrentPlayer()) {
                        gameUI->setButtonEnabled(BUTTON_PASS, false);
                    }
                    gameUI->resetValidPositions();
                    gameUI->setPlayerInputState(currentPlayer, AbstractUI::RejectAll);

                    if (command == Command::Offline) {
                        gameUI->execMessageBox(tr("Player timeout"),
                                               tr("%1 is timeout").arg(assets->getPlayerName(currentPlayer)),
                                               PlayerTimeoutDuration);
                        randomlyMoveOneCardFromHandToGraveyard(currentPlayer);
                        assets->setHandled(true);
                    } else if (command == Command::Pass) {
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
        gameUI->setAnimationFlag(false);
        if (gameUI->getLocalPlayer() == currentPlayer) {
            transmitEndMessage();
        }
        if (assets->isGameEnd()) {//some hint for server to judge the player next round
            assets->setCurrentPlayer(assets->getWinner());
        } else if (assets->isAllPlayerPass()) {//round end, but game not end
            assets->setCurrentPlayer(0);//for player 0 to choose card
        } else {
            assets->setCurrentPlayer(currentPlayer ^ 1);
        }
    }
    gameUI->setUseTimerFlag(false);
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
    updateLabel();
}

void
GameController::performChooseCard(int candidateIndex, int seletedIndex, int supplementIndex, int chooseNum, int player,
                                  bool supply, bool allowEscape, bool allowSwitchScene, const QString &title) {
    QList<CardInfo *> &candidate = assets->getCardArray(candidateIndex),
            &seleted = assets->getCardArray(seletedIndex),
            &supplement = assets->getCardArray(supplementIndex);

    gameUI->getRowCardArrayWidget(Player_Candidate)->setPos(PosX[Player_Candidate], PosY[Player_Candidate]);
    gameUI->setLabelText(Player_CardChooser_Title, title);

    int cardChoosed = 0;
    while (cardChoosed < chooseNum) {

        Command command;
        int row, column;
        if (gameUI->getLocalPlayer() == player) {
            gameUI->setButtonEnabled(ESCAPE, allowEscape);
        }
        gameUI->setWholeRowValidPositions(candidateIndex);
        gameUI->setPlayerInputState(player, AbstractUI::MustValidTarget);
        gameUI->getUserInput(command, row, column, player);
        gameUI->setPlayerInputState(player, AbstractUI::RejectAll);
        gameUI->resetValidPositions();
        if (gameUI->getLocalPlayer() == player) {
            gameUI->setButtonEnabled(ESCAPE, false);
        }

        if (command == Command::Offline) {
            gameUI->execMessageBox(tr("Player timeout"), tr("%1 is timeout").arg(assets->getPlayerName(player)),
                                   PlayerTimeoutDuration);
            if (allowEscape) {
                command = Command::EscapeChoose;
                row = -1;
                column = -1;
            } else {//WARNING: if some operation needs more than ONE choose and the player is offline, the game will STUCK!
                command = Command::LeftClick;
                row = candidateIndex;
                column = qrand() % assets->getCardArray(candidateIndex).size();
            }
        }

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
                    gameUI->setAnimationFlag(true);
                    performMovePosToPos(supplementIndex, nextColumn, candidateIndex, column);
                    gameUI->setAnimationFlag(false);
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
        } else if (command == Command::EscapeChoose) {//map two buttons to a same command
            //already use a message box to ensure the opearation via UI
            break;
        }
    }


    gameUI->setLabelText(Player_CardChooser_Title, "");//clean up
}

void GameController::handleRedrawCard(int player) {//handle two players' redraw card
    //add card from deck to hand first
    moveNCardsFromDeckToHand(player, NumberToChoose[assets->getCurrentRound()]);

    if (gameUI->getLocalPlayer() != -1 && player != gameUI->getLocalPlayer()) {
        gameUI->getRowCardArrayWidget(Player_Candidate)->setDefaultFace(1);
    }

    gameUI->switchToScene(AbstractUI::PlayerChooserScene);

    //perform move card from hand to candidate
    int handIndex = assets->getHandIndex(player);
    performMoveAllCardsFromAToB(handIndex, Player_Candidate);

    int deckIndex = assets->getDeckIndex(player);
    performChooseCard(Player_Candidate, Player_Seleted,
                      deckIndex, NumberToRedraw[assets->getCurrentRound()], player,
                      true, true, true, tr("%1 is choosing %2 card(s) to replace").arg(
                    assets->getPlayerName(player)).arg(NumberToRedraw[assets->getCurrentRound()]));

    //insert the seleted randomly to deck
    performRandomlyMoveAllCardsFromAToB(Player_Seleted, deckIndex);
    //perform move candidate BACK to hand
    performMoveAllCardsFromAToB(Player_Candidate, handIndex);

    gameUI->switchToScene(AbstractUI::GameScene);

    if (gameUI->getLocalPlayer() != -1 && player != gameUI->getLocalPlayer()) {
        gameUI->getRowCardArrayWidget(Player_Candidate)->setDefaultFace(0);
    }
}

void GameController::performRandomlyMoveAllCardsFromAToB(int fromR, int toR) {
    for (int fromC = assets->getCardArray(fromR).size() - 1; fromC >= 0; fromC--) {
        int toC = qrand() % assets->getCardArray(toR).size();//must generate random number in order!
        performMovePosToPos(fromR, fromC, toR, toC);
    }
}

void GameController::performMoveAllCardsFromAToB(int fromR, int toR) {//must follow the order
    bool prevFlag = gameUI->isAnimationFlag();
    gameUI->setAnimationFlag(false);
    while (!assets->getCardArray(fromR).empty()) {//from left to right or from buttom to top
        performMoveCardToRightTop(fromR, 0, toR);
    }
    gameUI->setAnimationFlag(prevFlag);
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

        if (combatValue[0] < combatValue[1]) {
            message += assets->getPlayerName(1) + tr(" Win!");
            assets->addPlayerWinRound(1);
            assets->setPreviousWinner(1);
        } else if (combatValue[0] == combatValue[1]) {
            message += tr(" Draw!");
            assets->setPreviousWinner(-1);//dummy value, draw
        } else if (combatValue[0] > combatValue[1]) {
            message += assets->getPlayerName(0) + tr(" Win!");
            assets->addPlayerWinRound(0);
            assets->setPreviousWinner(0);
        }
        message += tr("\n%1 : %2\n%3 %4").arg(assets->getPlayerName(0)).arg(assets->getPlayerName(1)).arg(
                combatValue[0]).arg(combatValue[1]);

        gameUI->execMessageBox(tr("Round Ended"), message, RoundEndDuration);
        assets->addCurrentRound();
        assets->setRoundStatus(0);//start another round
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

        if (command == Command::Offline) {
            gameUI->execMessageBox(tr("Player timeout"), tr("%1 is timeout").arg(assets->getPlayerName(currentPlayer)),
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
    updateLabel();
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
    updateLabel();
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
    for (auto dest: dests) {
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
    for (auto dest: dests) {
        assets->getCardPosition(dest, row, column);
        destPoints.append(QPoint(row, column));
    }
    gameUI->showBoost(srcPoint, destPoints);
    updateLabel();
}

void GameController::damageFromSrcToDests(CardInfo *src, const QList<CardInfo *> &dests, int damage, bool armorUseful) {
    for (auto dest: dests) {
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
    for (auto dest: dests) {
        assets->getCardPosition(dest, row, column);
        destPoints.append(QPoint(row, column));
    }
    gameUI->showDamage(srcPoint, destPoints);
    updateLabel();

    for (auto dest: dests) {//check if someone should be destroyed
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
    updateLabel();
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

        if (command == Command::Offline) {
            gameUI->execMessageBox(tr("Player timeout"), tr("%1 is timeout").arg(assets->getPlayerName(currentPlayer)),
                                   PlayerTimeoutDuration);
            command = Command::LeftClick;
            int randomIndex = qrand() % validPositions.size();
            toR = validPositions[randomIndex].x();
            toC = validPositions[randomIndex].y();
        }

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

void GameController::cleanUpRow(int row) {
    const QList<CardInfo *> &array = assets->getCardArray(row);
    while (!array.empty()) {
        removeCardFromGame(array.first());
    }
}

void GameController::cleanUpGame() {
    clearWeatherOnAllBattlefield();
    for (int row = Player1_Graveyard; row <= Player0_Graveyard; row++) {
        cleanUpRow(row);
    }
    gameUI->getOutputBuffers().clear();
    for (int player = 0; player < 2; player++) {
        gameUI->setPlayerInputBuffer(player, nullptr);
        setPlayerName(player, "");
    }
    gameUI->setLocalPlayer(0);
    gameUI->switchToScene(AbstractUI::PlayerChooserScene);
}

void GameController::updateLabel() {
    for (int row = Player1_Graveyard; row <= Player1_Hand; row++) {
        gameUI->setLabelText(row, QString::number(assets->getCardArray(row).size()));
    }
    for (int row = Player1_Siege; row <= Player0_Siege; row++) {//battlefield
        gameUI->setLabelText(row, QString::number(assets->getRowCombatValueSum(row)));
    }
    for (int row = Player0_Hand; row <= Player0_Graveyard; row++) {
        gameUI->setLabelText(row, QString::number(assets->getCardArray(row).size()));
    }
    assets->updateDeckTypeCount();
    gameUI->setLabelText(Label_Bronze_DeckBuilder, QString::number(assets->getDeckTypeCount(CardInfo::Bronze)));
    gameUI->setLabelText(Label_Silver_DeckBuilder, QString::number(assets->getDeckTypeCount(CardInfo::Silver)));
    gameUI->setLabelText(Label_Gold_DeckBuilder, QString::number(assets->getDeckTypeCount(CardInfo::Gold)));
    gameUI->setLabelText(Label_ALL_DeckBuilder, QString::number(assets->getDeckTotalCount()));
    gameUI->setLabelText(Player0_CombatValue, QString::number(assets->getPlayerCombatValueSum(0)));
    gameUI->setLabelText(Player1_CombatValue, QString::number(assets->getPlayerCombatValueSum(1)));
}

void GameController::setPlayerName(int player, const QString &name) {
    assets->setPlayerName(player, name);
    if (player == 0) {
        gameUI->setLabelText(Player0_Name, name);
    } else if (player == 1) {
        gameUI->setLabelText(Player1_Name, name);
    }
}

void GameController::startDeckBuilder() {
    deckBuilderInit();
    for (int row = DeckBuilder_Candidate; row <= DeckBuilder_Melee_Event; row++) {
        gameUI->setWholeRowValidPositions(row);
    }
    gameUI->setButtonEnabled(CLIPBOARD_IMPORT, true);
    gameUI->setButtonEnabled(CLIPBOARD_EXPORT, true);
    gameUI->setButtonEnabled(SAVEDECK, true);
    gameUI->setPlayerInputState(0, AbstractUI::MustValidTarget);
    gameUI->setAnimationFlag(true);
    while (true) {
        Command command;
        int row, column;
        gameUI->getUserInput(command, row, column, 0);
        if (command == Command::LeftClick) {
            CardInfo *seletedCard = assets->getCardArray(row).at(column);
            if (row == DeckBuilder_Candidate) {
                if (assets->isDeckValidMove(seletedCard->getType())) {
                    int targetRow = assets->getDeckBuilderTargetRow(seletedCard);
                    performMoveToNeighbor(targetRow, seletedCard);
                } else {
                    gameUI->execMessageBox(tr("invalid move"), tr("do not meet the card standard"),
                                           RoundEndDuration);
                }
            } else {
                performMoveToNeighbor(DeckBuilder_Candidate, seletedCard);
            }
        } else if (command == Command::ClipboardExport) {
            Deck currentDeck = currentStateToDeck();
            QClipboard *clipboard = QGuiApplication::clipboard();
            clipboard->setText(currentDeck.toJsonStr());
        } else if (command == Command::ClipboardImport) {
            Deck outerDeck;
            QClipboard *clipboard = QGuiApplication::clipboard();
            int error = outerDeck.parseDeckFromString(clipboard->text());
            if (error == 0) {
                gameUI->setAnimationFlag(false);
                performMoveAccordingToDeck(outerDeck);
                gameUI->setAnimationFlag(true);
            } else if (error == 1) {
                gameUI->execMessageBox(tr("syntax error"), tr("cannot parse clipboard"),
                                       RoundEndDuration);
            } else if (error == 2) {
                gameUI->execMessageBox(tr("invalid deck"), tr("Your deck is invalid"),
                                       RoundEndDuration);
            }
        } else if (command == Command::SaveDeck) {
            if (!assets->isDeckValidSave()) {
                gameUI->execMessageBox(tr("cannot save deck"), tr("card numbers do not meet the requirement"),
                                       RoundEndDuration);
            } else {
                QFile file("assets/card_decks/" + gameUI->getLineEditText() + ".json");
                if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
                    gameUI->execMessageBox(tr("cannot save deck"), tr("open file failed!"),
                                           RoundEndDuration);
                } else {
                    Deck currentDeck = currentStateToDeck();
                    file.write(currentDeck.toJsonStr());
                    file.close();
                    break;//successful, return to mainmenu
                }
            }
        }
    }
    gameUI->setAnimationFlag(false);
    gameUI->setPlayerInputState(0, AbstractUI::RejectAll);
    gameUI->setButtonEnabled(SAVEDECK, false);
    gameUI->setButtonEnabled(CLIPBOARD_IMPORT, false);
    gameUI->setButtonEnabled(CLIPBOARD_EXPORT, false);
    gameUI->resetValidPositions();
    deckBuilderCleanUp();
}


Deck GameController::performChooseDeck(bool allowNewDeck) {
    gameUI->switchToScene(AbstractUI::PlayerChooserScene);
    //leader choose scene
    prepareChooseDecks(allowNewDeck);

    performChooseCard(Player_Candidate, Player_Seleted, Player_Seleted, 1, 0, false, false, false,
                      tr("Please choose a deck"));
    QString seletedName = assets->getCardArray(Player_Seleted).first()->getCardName();

    cleanUpRow(Player_Candidate);
    cleanUpRow(Player_Seleted);

    Deck nowDeck{};

    if (seletedName == "New_Deck") {
        for (const auto &leaderName: LeaderNameList) {
            performSpawnCardToPos(CardInfo::createByName(leaderName), Player_Candidate,
                                  assets->getCardArray(Player_Candidate).size());
        }
        performChooseCard(Player_Candidate, Player_Seleted, Player_Seleted, 1, 0, false, false, false,
                          tr("Choose your leader"));
        QString seletedLeader = assets->getCardArray(Player_Seleted).first()->getCardName();
        cleanUpRow(Player_Candidate);
        cleanUpRow(Player_Seleted);

        nowDeck.setLeader(seletedLeader);
        nowDeck.getCards().clear();
    } else {
        for (const auto &deck: assets->getDecks()) {
            if (deck.getName() == seletedName) {
                nowDeck = deck;
                break;
            }
        }
    }
    assets->getDecks().clear();
    return nowDeck;
}

void GameController::deckBuilderInit() {
    gameUI->setLocalPlayer(0);
    gameUI->setPlayerInputBuffer(0, ioBuffer);
    gameUI->getOutputBuffers().append(ioBuffer);
    network->logout();

    Deck nowDeck = performChooseDeck(true);

    gameUI->switchToScene(AbstractUI::DeckBuilderScene);
    initializeDeckCards();
    performMoveAccordingToDeck(nowDeck);
}

void GameController::deckBuilderCleanUp() {
    gameUI->setLocalPlayer(-1);
    gameUI->setPlayerInputBuffer(0, nullptr);
    gameUI->getOutputBuffers().clear();
    if (network->login(username, password) != 0) {
        std::exit(0);//terminate the game
    }
    for (int row = DeckBuilder_Candidate; row <= DeckBuilder_Melee_Event; row++) {
        cleanUpRow(row);
    }

    assets->setLeaderName("");
    gameUI->clearLeader();
    assets->getLeaderInfo()->deleteLater();
    assets->setLeaderInfo(nullptr);

    gameUI->setLineEditText("");
    QTimer::singleShot(1, this, &GameController::startMainMenu);
}

void GameController::prepareChooseDecks(bool allowNewDeck) {
    QDir deckDir("assets/card_decks");
    deckDir.setFilter(QDir::Files | QDir::NoSymLinks);
    QFileInfoList fileInfoList = deckDir.entryInfoList();
    assets->getDecks().clear();
    for (const auto &fileInfo: fileInfoList) {
        if (QString::compare(fileInfo.suffix(), "json", Qt::CaseInsensitive) == 0) {
            QFile file;
            file.setFileName(fileInfo.absoluteFilePath());
            file.open(QIODevice::ReadOnly | QIODevice::Text);
            QString jsonStr = file.readAll();
            file.close();
            Deck deck;
            if (deck.parseDeckFromString(jsonStr) == 0) {//no error
                assets->getDecks().append(deck);
                CardInfo *card = CardInfo::createByName(deck.getLeader());
                performSpawnCardToPos(card, Player_Candidate, assets->getCardArray(Player_Candidate).size());
                card->setCardName(deck.getName());
            }
        }
    }
    if (allowNewDeck) {
        CardInfo *newDeckCard = CardInfo::createByName("New_Deck");
        performSpawnCardToPos(newDeckCard, Player_Candidate, assets->getCardArray(Player_Candidate).size());
    }
}

void GameController::performSpawnCardToPos(CardInfo *card, int row, int column) {
    assets->getCardArray(row).insert(column, card);
    gameUI->spawnNewCard(card, row, column);
    updateLabel();
}

void GameController::initializeDeckCards() {
    for (const auto &card: CardNameList) {
        CardInfo::Type type = Deck::getTypeByName(card);
        QList<CardInfo *> &candidateRow = assets->getCardArray(DeckBuilder_Candidate);
        for (int cnt = 0; cnt < MaxSameTypeCount[type]; cnt++) {
            candidateRow.append(CardInfo::createByName(card));
        }
    }
    gameUI->loadARowFromAssets(DeckBuilder_Candidate, assets);
}

bool GameController::performMoveAccordingToDeck(Deck &deck) {
    for (int row = DeckBuilder_NoHP; row <= DeckBuilder_Melee_Event; row++) {
        performMoveAllCardsFromAToB(row, DeckBuilder_Candidate);
    }

    assets->setLeaderName(deck.getLeader());

    if (assets->getLeaderInfo() != nullptr) {
        gameUI->clearLeader();
        assets->getLeaderInfo()->deleteLater();
        assets->setLeaderInfo(nullptr);
    }

    assets->setLeaderInfo(CardInfo::createByName(assets->getLeaderName()));
    gameUI->setLeader(assets->getLeaderInfo());

    gameUI->setLineEditText(deck.getName());
    for (const auto &cardName: deck.getCards()) {
        int index = assets->getCardIndexByName(DeckBuilder_Candidate, cardName);
        if (index == -1) {
            return false;
        }
        CardInfo *card = assets->getCardArray(DeckBuilder_Candidate).at(index);
        if (!assets->isDeckValidMove(card->getType())) {
            return false;
        }
        int targetRow = assets->getDeckBuilderTargetRow(card);
        performMoveToNeighbor(targetRow, card);
    }
    return true;
}

void GameController::performMoveToNeighbor(int targetRow, CardInfo *card) {
    int index = assets->getCardIndexByName(targetRow, card->getCardName());
    if (index == -1) {
        index = 0;
    }
    int row, column;
    assets->getCardPosition(card, row, column);
    performMovePosToPos(row, column, targetRow, index);
}

Deck GameController::currentStateToDeck() {
    Deck result;
    result.setLeader(assets->getLeaderName());
    result.getCards().clear();
    result.setName(gameUI->getLineEditText());
    for (int row = DeckBuilder_NoHP; row <= DeckBuilder_Melee_Event; row++) {
        for (auto card: assets->getCardArray(row)) {
            result.getCards().append(card->getCardName());
        }
    }
    return result;
}

void GameController::startMainMenu() {
    prepareMainMenu();
    while (true) {
//        inviteTimer.start(); TODO implement invite
        gameUI->setPlayerInputState(0, AbstractUI::MustValidTarget);
        Command command;
        int row, column;
        gameUI->getUserInput(command, row, column, 0);
        gameUI->setPlayerInputState(0, AbstractUI::RejectAll);
//        inviteTimer.stop();
        if (command == Command::LeftClick) {
            QString cardName = assets->getCardArray(row).at(column)->getCardName();
            if (cardName == "Match") {

                gameDeck = performChooseDeck(false);

                QMessageBox messageBox;
                messageBox.setWindowTitle("Matching");
                messageBox.setText("Finding appropriate player to compete with...");
                messageBox.setStandardButtons(QMessageBox::No);
                messageBox.setButtonText(QMessageBox::No, "Cancel");
                connect(&messageBox, &QMessageBox::rejected, []() { exit(0); });
                messageBox.show();
                QJsonObject request;

                request.insert("command", "start_match");
                QJsonObject jsonDeckObject;
                gameDeck.toJson(jsonDeckObject);
                request.insert("deck", jsonDeckObject);
                network->writeJsonObject(request);

                while (true) {
                    startFortune = network->readJsonObject();
                    QString command = startFortune["command"].toString();
                    if (command == "heartbeat") {
                        QJsonObject heartbeat;
                        heartbeat.insert("command", "heartbeat_response");
                        network->writeJsonObject(heartbeat);
                    } else if (command == "init") {
                        break;
                    }
                }

                messageBox.hide();

                QTimer::singleShot(1, this, &GameController::startGame);
                cleanUpMainMenu();
                return;

            } else if (cardName == "Server_Status") {

                QJsonObject request;
                request.insert("command", "get_list");
                network->writeJsonObject(request);
                request = network->readJsonObject();
                RoomDialog dialog(request);
                int result = dialog.exec();
                if (result == QDialog::Accepted) {
                    if (!dialog.getSelectedPlayer().isEmpty() &&
                        dialog.getSelectedPlayer() != username) {//a player cannot invite himself

                        /*
                        gameDeck = performChooseDeck(false);
                        QJsonObject inviteRequest;
                        inviteRequest.insert("command", "invite");
                        inviteRequest.insert("target", dialog.getSelectedPlayer());
                        QJsonObject jsonGameDeck;
                        gameDeck.toJson(jsonGameDeck);
                        inviteRequest.insert("deck", jsonGameDeck);
                        network->writeJsonObject(inviteRequest);

                        QMessageBox messageBox1;
                        messageBox1.setWindowTitle(tr("inviting"));
                        messageBox1.setText(tr("waiting for player to respnose"));
                        messageBox1.setStandardButtons(0);
                        messageBox1.show();

                        QJsonObject inviteResponse = network->readJsonObject();

                        messageBox1.hide();

                        if (inviteResponse["command"].toString() != "respond_invite") {
                            qWarning() << "not proper response! expected respond_invite, find "
                                       << inviteResponse["command"].toString();
                        }
                        if (inviteResponse["validation"].toBool()) {
                            startFortune = network->readJsonObject();
                            cleanUpMainMenu();
                            QTimer::singleShot(1, this, &GameController::startGame);
                            return;
                        } else {
                            QMessageBox::information(nullptr, tr("cannot invite"), inviteResponse["reason"].toString());
                            cleanUpMainMenu();
                            prepareMainMenu();
                        }
                         *///TODO implement invite

                    } else if (!dialog.getSelectedGame().isEmpty()) {
                        QJsonObject gamePost;
                        gamePost.insert("command", "watch");
                        gamePost.insert("gameName", dialog.getSelectedGame());
                        network->writeJsonObject(gamePost);

                        QJsonObject response = network->readJsonObject();

                        if (response["command"].toString() != "respond_watch") {
                            qWarning() << "invalid response";
                        }
                        if (response["validation"].toBool()) {
                            QMessageBox messageBox;
                            messageBox.setWindowTitle("Pending");
                            messageBox.setText("Waiting for the current turn to end.");
                            messageBox.setStandardButtons(0);
                            messageBox.show();
                            resumeFortune = network->readJsonObject();
                            messageBox.hide();
                            QString cmd = resumeFortune["command"].toString();
                            if (cmd == "end") {
                                QMessageBox::information(nullptr, tr("cannot watch game"),
                                                         tr("game has already ended"));
                            } else {
                                cleanUpMainMenu();
                                QTimer::singleShot(1, this, &GameController::resumeGame);
                                return;
                            }
                        } else {
                            QMessageBox::information(nullptr, tr("cannot watch game"), response["reason"].toString());
                        }
                    }
                }

            } else if (cardName == "Deck_Builder") {
                QTimer::singleShot(0, this, &GameController::startDeckBuilder);
                cleanUpMainMenu();
                return;
            }
        } else if (command == Command::QueryInvite) {
            QJsonObject request;
            request.insert("command", "isinvited");
            network->writeJsonObject(request);
            request = network->readJsonObject();
            if (request["command"].toString() != "respond_isinvited") {
                qWarning() << "not proper command, expected respond_isinvited, find " << request["command"].toString();
            }
            if (request["isinvited"].toBool()) {
                QString invitorName = request["inviter"].toString();
                int result = QMessageBox::question(nullptr, tr("Incoming invitation"),
                                                   tr("Do you want to play with %1?").arg(invitorName));
                QJsonObject invite_response;
                invite_response.insert("command", "invite_response");
                invite_response.insert("inviterName", invitorName);
                if (result == QMessageBox::Yes) {
                    invite_response.insert("accept", true);
                    gameDeck = performChooseDeck(false);
                    QJsonObject jsonDeck;
                    gameDeck.toJson(jsonDeck);
                    invite_response.insert("deck", jsonDeck);
                    network->writeJsonObject(invite_response);
                    startFortune = network->readJsonObject();
                    cleanUpMainMenu();
                    QTimer::singleShot(1, this, &GameController::startGame);
                    return;
                } else {
                    invite_response.insert("accept", false);
                    network->writeJsonObject(invite_response);
                }
            }
        }
    }
}

void GameController::prepareMainMenu() {
    if (gameUI->getCurrentScene() != AbstractUI::MainMenuScene) {
        gameUI->switchToScene(AbstractUI::MainMenuScene);
    }
    gameUI->setAnimationFlag(true);
    static const QString MainMenuNameList[3] = {"Match", "Server_Status", "Deck_Builder"};
    for (const auto &name: MainMenuNameList) {
        performSpawnCardToPos(CardInfo::createByName(name), Main_Menu, assets->getCardArray(Main_Menu).size());
    }
    gameUI->setAnimationFlag(false);
    gameUI->setLocalPlayer(0);
    gameUI->setPlayerInputBuffer(0, ioBuffer);
    gameUI->getOutputBuffers().append(ioBuffer);
    gameUI->setWholeRowValidPositions(Main_Menu);
    inviteTimer.setInterval(4000);
    connect(&inviteTimer, &QTimer::timeout, ioBuffer, &ScreenIOBuffer::addInviteMessage);
}

void GameController::cleanUpMainMenu() {
    gameUI->setLocalPlayer(0);
    gameUI->setPlayerInputBuffer(0, nullptr);
    gameUI->getOutputBuffers().clear();
    gameUI->resetValidPositions();
    disconnect(&inviteTimer, &QTimer::timeout, ioBuffer, &ScreenIOBuffer::addInviteMessage);
    cleanUpRow(Main_Menu);
}

void GameController::start() {
    gameUI = new GraphicsUI();
    assets = new GameAssets();
    network = new NetworkManager();
    ioBuffer = new ScreenIOBuffer();

    bool resume;
    while (true) {
        getUserNameAndPassWord();
        int result = network->login(username, password);
        if (result != -1) {
            resume = (result == 1);
            break;
        }
    }

    if (resume) {
        QMessageBox messageBox;
        messageBox.setWindowTitle(tr("Resuming game"));
        messageBox.setText("resuming game");
        messageBox.setStandardButtons(0);
        messageBox.show();
        resumeFortune = network->readJsonObject();
        messageBox.hide();
        QString cmd = resumeFortune["command"].toString();
        if (cmd == "end") {
            QMessageBox::information(nullptr, tr("cannot watch game"),
                                     tr("game has already ended"));
        } else {
            QTimer::singleShot(1, this, &GameController::resumeGame);
            return;
        }
    } else {
        QTimer::singleShot(1, this, &GameController::startMainMenu);
    }
}

void GameController::getUserNameAndPassWord() {
    LoginDialog loginDialog;
    int result = loginDialog.exec();
    if (result == QDialog::Accepted) {
        username = loginDialog.getUserName();
        password = loginDialog.getPassword();
    } else {
        std::exit(0);
    }
}

void GameController::loadFromDeckAndJson() {
    int localPlayer = startFortune["localplayer"].toInt();
    gameUI->setLocalPlayer(localPlayer);

    gameUI->setPlayerInputBuffer(localPlayer, ioBuffer);
    gameUI->setPlayerInputBuffer(localPlayer ^ 1, network);

    gameUI->getOutputBuffers().clear();
    gameUI->getOutputBuffers().append(ioBuffer);
    gameUI->getOutputBuffers().append(network);

    setPlayerName(localPlayer, username);
    setPlayerName(localPlayer ^ 1, startFortune["name"].toString());

    assets->setRandomSeed(static_cast<unsigned int>(startFortune["seed"].toInt()));
    assets->resetRandomSeed();
    //must set randomseed before spawning deck

    QJsonObject otherDeckObject = startFortune["deck"].toObject();
    Deck otherDeck;
    otherDeck.fromJson(otherDeckObject);
    assets->clearAllGameRows();
    if (localPlayer == 0) {
        assets->loadPlayerFromDeck(0, gameDeck);
        assets->loadPlayerFromDeck(1, otherDeck);
    } else {
        assets->loadPlayerFromDeck(0, otherDeck);
        assets->loadPlayerFromDeck(1, gameDeck);
    }
}

void GameController::uploadReadyState() {
    QJsonObject readyInfo;
    readyInfo.insert("command", "ready");
    QJsonObject jsonGameState;
    assets->toJson(jsonGameState);
    readyInfo.insert("gameState", jsonGameState);
    readyInfo.insert("gameEndFlag", assets->isGameEnd());
    if (assets->isGameEnd()) {
        readyInfo.insert("currentPlayer", assets->getWinner());
    } else {
        readyInfo.insert("currentPlayer", assets->getCurrentPlayer());
    }
    network->writeJsonObject(readyInfo);
}

bool GameController::getStartRoundInfo() {
    QJsonObject startRoundInfo = network->readJsonObject();
    if (startRoundInfo["command"].toString() == "startround") {
        bool shouldRest = startRoundInfo["shouldreset"].toBool();
        if (shouldRest) {
            assets->resetRandomSeed();
        }
        int currentPlayerFromServer = startRoundInfo["currentplayer"].toInt();
        if (assets->getCurrentPlayer() == -1) {
            gameUI->playRandomCoinAnimation();
        }
        setCurrentPlayerForAssetsAndUI(currentPlayerFromServer);
        return false;
    } else if (startRoundInfo["command"] == "end") {
        return true;
    }
}

void GameController::randomlyMoveOneCardFromHandToGraveyard(int player) {
    const QList<CardInfo *> &hand = assets->getCardArray(assets->getHandIndex(player));
    int randomIndex = qrand() % hand.size();
    int graveyardRow = (player == 0 ? Player0_Graveyard : Player1_Graveyard);
    moveCardToRightTop(hand.at(randomIndex), graveyardRow);
}

void GameController::transmitEndMessage() {
    QJsonObject endInfo;
    endInfo.insert("command", "end_transmit");
    network->writeJsonObject(endInfo);
}

void GameController::resumeGame() {
    if (resumeFortune["command"] != "resumegame") {
        qWarning() << "not proper command resume game" + resumeFortune["command"].toString();
    }
    assets->fromJson(resumeFortune["gameState"].toObject());

    int localPlayer = -1;
    for (int player = 0; player < 2; player++) {
        if (assets->getPlayerName(player) == username) {
            localPlayer = player;
            break;
        }
    }
    gameUI->setLocalPlayer(localPlayer);

    gameUI->loadCardFromAssets(assets);
    updateLabel();
    gameUI->setLabelText(Player0_Name, assets->getPlayerName(0));
    gameUI->setLabelText(Player1_Name, assets->getPlayerName(1));

    gameUI->switchToScene(AbstractUI::PlayerChooserScene);
    gameUI->switchToScene(AbstractUI::GameScene);

    gameUI->getOutputBuffers().clear();
    if (localPlayer == -1) {
        for (int player = 0; player < 2; player++) {
            gameUI->setPlayerInputBuffer(player, network);
        }
    } else {
        gameUI->getOutputBuffers().append(ioBuffer);
        gameUI->getOutputBuffers().append(network);
        gameUI->setPlayerInputBuffer(localPlayer, ioBuffer);
        gameUI->setPlayerInputBuffer(localPlayer ^ 1, network);
    }
    enterGameLoop(true);
}

void GameController::enterGameLoop(bool skipFirst) {
    do {
        if (skipFirst) {
            skipFirst = false;
        } else {
            uploadReadyState();
        }
        if (getStartRoundInfo()) {//the game ends
            break;
        }
        gameLoop();
    } while (true);

    //game end, enter reporting interface
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
    cleanUpGame();
    QTimer::singleShot(1, this, &GameController::startMainMenu);
}

void GameController::setAnimation(bool flag) {
    gameUI->setAnimationFlag(flag);
}


