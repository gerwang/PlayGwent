//
// Created by Gerwa on 2017/9/6.
//

#include "GameAssets.h"
#include <QDebug>

bool GameAssets::getPlayerPass(int player) {
    return playerPass[player];
}

void GameAssets::setPlayerPass(int player, bool pass) {
    playerPass[player] = pass;
}

QList<CardInfo *> &GameAssets::getCardArray(int row) {
    return cardarray[row];
}

int GameAssets::getPlayerWinRound(int player) {
    return playerWinRound[player];
}

void GameAssets::setPlayerWinRound(int player, int winRound) {
    playerWinRound[player] = winRound;
}

Weather GameAssets::getRowWeather(int row) {
    return rowWeather[row];
}

void GameAssets::setRowWeather(int row, Weather weather) {
    rowWeather[row] = weather;
}

const QString &GameAssets::getPlayerName(int player) {
    return playerName[player];
}

void GameAssets::setPlayerName(int player, const QString &name) {
    playerName[player] = name;
}

bool GameAssets::isGameEnd() {
    return playerWinRound[0] >= 2 ||//player0  win
           playerWinRound[1] >= 2 ||//player1  win
           currentRound >= 3;
    //           playerWinRound[0] + playerWinRound[1] < 3;//not draw WRONG!
}

int GameAssets::getPlayerScore(int player, int round) {
    return playerScore[player][round];
}

void GameAssets::setPlayerScore(int player, int round, int score) {
    playerScore[player][round] = score;
}

unsigned int GameAssets::getRandomSeed() const {
    return randomSeed;
}

void GameAssets::setRandomSeed(unsigned int randomSeed) {
    GameAssets::randomSeed = randomSeed;
}

void GameAssets::resetRandomSeed() {
    qsrand(randomSeed);
}

bool GameAssets::isRoundStart() const {
    return roundStart;
}

void GameAssets::setRoundStart(bool start) {
    GameAssets::roundStart = start;
}

int GameAssets::getPreviousWinner() const {
    return previousWinner;
}

void GameAssets::setPreviousWinner(int previousWinner) {
    GameAssets::previousWinner = previousWinner;
}

int GameAssets::getCurrentPlayer() const {
    return currentPlayer;
}

void GameAssets::setCurrentPlayer(int currentPlayer) {
    GameAssets::currentPlayer = currentPlayer;
}

int GameAssets::getCurrentRound() const {
    return currentRound;
}

void GameAssets::setCurrentRound(int currentRound) {
    GameAssets::currentRound = currentRound;
}

QList<CardInfo *> GameAssets::getTopNCards(const QList<CardInfo *> &deck, int n) {
    if (n > deck.size()) {
        qDebug() << "deck size not enough! 12345647";
        n = deck.size();
    }
    return deck.mid(deck.size() - n, n);
}

QList<CardInfo *> GameAssets::getPlayerDeckTopNCards(int player, int n) {
    return getTopNCards(cardarray[getDeckIndex(player)], n);
}

int GameAssets::getDeckIndex(int player) {
    if (player == 0) {
        return Player0_Deck;
    } else if (player == 1) {
        return Player1_Deck;
    } else {
        qWarning() << "Invalid Player number!123475867";
        return -1;
    }
}

void GameAssets::getCardPosition(CardInfo *card, int &row, int &column) {
    for (int i = 0; i < ROW_NUM; i++) {
        for (int j = 0; j < cardarray[i].size(); j++) {
            if (cardarray[i][j] == card) {//compare pointer,really
                row = i;
                column = j;
                return;
            }
        }
    }
    qWarning() << "card not in assets!";
}

void GameAssets::moveCardPosToPos(int fromR, int fromC, int toR, int toC) {
    CardInfo *card = cardarray[fromR].takeAt(fromC);
    cardarray[toR].insert(toC, card);
}

bool GameAssets::isBattleField(int row) {
    return Player1_Siege <= row && row <= Player0_Siege;
}

bool GameAssets::isGraveyard(int row) {
    return row == Player0_Graveyard || row == Player1_Graveyard;
}

int GameAssets::getHandIndex(int player) {
    if (player == 0) {
        return Player0_Hand;
    } else if (player == 1) {
        return Player1_Hand;
    } else {
        qWarning() << "invalid player number 2545274";
        return -1;
    }
}


int GameAssets::findTopmostANameNotInB(const QList<CardInfo *> &a, const QList<CardInfo *> &b) {
    //larger,topper
    for (int top = a.size() - 1; top >= 0; top--) {
        CardInfo *topCard = a[top];
        if (!listContainsName(b, topCard->getCardName())) {
            return top;
        }
    }
    return -1;
}

bool GameAssets::listContainsName(const QList<CardInfo *> &list, const QString &name) {
    for (auto info:list) {
        if (info->getCardName() == name) {
            return true;
        }
    }
    return false;
}

bool GameAssets::isPlayerHandEmpty(int player) {
    int handIndex = getHandIndex(player);
    return cardarray[handIndex].empty();
}

bool GameAssets::isAllPlayerPass() {
    return playerPass[0] && playerPass[1];
}

bool GameAssets::isHandled() const {
    return handled;
}

void GameAssets::setHandled(bool handled) {
    GameAssets::handled = handled;
}

int GameAssets::getRowCombatValueSum(int row) {
    int sum = 0;
    for (auto card:cardarray[row]) {
        sum += card->getCurrentStrength();
    }
    return sum;
}

int GameAssets::getPlayerCombatValueSum(int player) {
    int sum = 0;
    if (player == 0) {
        for (int row = Player0_Melee; row <= Player0_Siege; row++) {
            sum += getRowCombatValueSum(row);
        }
    } else if (player == 1) {
        for (int row = Player1_Siege; row <= Player1_Melee; row++) {
            sum += getRowCombatValueSum(row);
        }
    }
    return sum;
}

void GameAssets::addPlayerWinRound(int player) {
    playerWinRound[player]++;
}

void GameAssets::addCurrentRound() {
    currentRound++;
}

void GameAssets::clearAllGameRows() {
    for (int row = Player1_Graveyard; row <= Player0_Graveyard; row++) {
        cardarray[row].clear();
        rowWeather[row] = Weather::Sunny;
    }
}

void GameAssets::loadPlayerDeck(const QList<QString> &zeroNameList,
                                const QList<QString> &oneNameList) {
    clearAllGameRows();
    resetRandomSeed();//set random seed, no need to synchosize random bahavior
    createCardsRandomlyOnNameListToRow(zeroNameList, Player0_Deck);
    createCardsRandomlyOnNameListToRow(oneNameList, Player1_Deck);
}

void GameAssets::createCardsRandomlyOnNameListToRow(QList<QString> namelist, int row) {
    //randomize
    int n = namelist.size();
    for (int index = 0; index < n; index++) {
        int otherIndex = index + qrand() % (n - index);//range [index,n-1]
        std::swap(namelist[index], namelist[otherIndex]);//correct random algorithm
        cardarray[row].append(CardInfo::createByName(namelist[index]));//allocate NEW memory!
        //WARNING: FIND BUG: cardarray means &cardarray[0]!!
    }
}

int GameAssets::getEnemySameRowNumber(int rowNumber) {
    return Player0_Graveyard - rowNumber;
}

int GameAssets::getPlayerMelee(int player) {
    return player == 0 ? Player0_Melee : Player1_Melee;
}

int GameAssets::getPlayerRanged(int player) {
    return player == 0 ? Player0_Ranged : Player1_Ranged;
}

int GameAssets::getPlayerSiege(int player) {
    return player == 0 ? Player0_Siege : Player1_Siege;
}

void GameAssets::updateRowStrongest(int row, QList<CardInfo *> &result, CardInfo *exclude) {
    for (auto info:cardarray[row]) {
        if (info == exclude) {
            continue;
        } else if (result.empty()) {
            result.append(info);
        } else if (info->getCurrentStrength() > result.first()->getCurrentStrength()) {
            result.clear();
            result.append(info);
        } else if (info->getCurrentStrength() == result.first()->getCurrentStrength()) {
            result.append(info);
        }
    }
}

void GameAssets::removeCardFromGame(int row, int column) {
    CardInfo *temp = cardarray[row].takeAt(column);
    temp->deleteLater();//use deletelater to provide a safer way
}

bool GameAssets::cardAlreadyInGraveyard(CardInfo *card) {
    return cardarray[Player0_Graveyard].contains(card) ||
           cardarray[Player1_Graveyard].contains(card);
}

int GameAssets::whosePlayerRow(int row) {
    if (Player0_Melee <= row && row <= Player0_Graveyard) {
        return 0;
    } else if (Player1_Graveyard <= row && row <= Player1_Melee) {
        return 1;
    } else {
        return -1;
    }
}

int GameAssets::playerBattlefieldBegin(int player) {
    return player == 0 ? Player0_Melee : Player1_Siege;
}

int GameAssets::playerBattlefieldEnd(int player) {
    return player == 0 ? Player0_Siege : Player1_Melee;
}

bool GameAssets::isCardOnBattlefield(CardInfo *card) {
    for (int row = Player1_Siege; row <= Player0_Siege; row++) {
        if (cardarray[row].contains(card)) {
            return true;
        }
    }
    return false;
}

void GameAssets::updateRowWeakest(int row, QList<CardInfo *> &result, CardInfo *exclude) {
    for (auto info:cardarray[row]) {
        if (info == exclude) {
            continue;
        } else if (result.empty()) {
            result.append(info);
        } else if (info->getCurrentStrength() < result.first()->getCurrentStrength()) {
            result.clear();
            result.append(info);
        } else if (info->getCurrentStrength() == result.first()->getCurrentStrength()) {
            result.append(info);
        }
    }
}




//you should always perform MOVE instead of REMOVE, case REMOVE is a really serios thing