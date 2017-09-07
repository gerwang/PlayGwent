//
// Created by Gerwa on 2017/9/6.
//

#include "GameAssets.h"

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

AbstractUI::Weather GameAssets::getRowWeather(int row) {
    return rowWeather[row];
}

void GameAssets::setRowWeather(int row, AbstractUI::Weather weather) {
    rowWeather[row] = weather;
}

const QString &GameAssets::getPlayerName(int player) {
    return playerName[player];
}

void GameAssets::setPlayerName(int player, const QString &name) {
    playerName[player] = name;
}

bool GameAssets::isGameEnd() {
    return playerWinRound[0] < 2 &&//player0 don't win
           playerWinRound[1] < 2 &&//player1 don't win
           playerWinRound[0] + playerWinRound[1] < 3;//not draw
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

int GameAssets::getWinner(int round) {
    return winner[round];
}

void GameAssets::setWinner(int round, int player) {//player can be -1
    winner[round] = player;
}

void GameAssets::resetRandomSeed() {
    qsrand(randomSeed);
}

bool GameAssets::isNeedChooseCard() const {
    return needChooseCard;
}

void GameAssets::setNeedChooseCard(bool needChooseCard) {
    GameAssets::needChooseCard = needChooseCard;
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

int GameAssets::getCandidateIndex(int player) {
    if (player == 0) {
        return Player0_Candidate;
    } else if (player == 1) {
        return Player1_Candidate;
    } else {
        qWarning() << "invalid player!2q894371309845";
    }
}

int GameAssets::getSeletedIndex(int player) {
    if (player == 0) {
        return Player0_Seleted;
    } else if (player == 1) {
        return Player1_Seleted;
    } else {
        qWarning() << "invalid player!2q894371309845";
    }
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

//you should always perform MOVE instead of REMOVE, case REMOVE is a really serios thing