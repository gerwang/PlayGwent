//
// Created by Gerwa on 2017/9/6.
//

#ifndef PLAYGWENT_GAMEASSETS_H
#define PLAYGWENT_GAMEASSETS_H


#include <QtCore/QObject>
#include "cardinfo.h"
#include "AbstractUI.h"
#include "GameConstant.h"

class GameAssets : public QObject {//store all the info's that are on the game logic
Q_OBJECT
public:
    bool isGameEnd();

    bool getPlayerPass(int player);

    void setPlayerPass(int player, bool pass);

    QList<CardInfo *> &getCardArray(int row);

    int getPlayerWinRound(int player);

    void setPlayerWinRound(int player, int winRound);

    AbstractUI::Weather getRowWeather(int row);

    void setRowWeather(int row, AbstractUI::Weather weather);

    const QString &getPlayerName(int player);

    void setPlayerName(int player, const QString &name);

    //judge if game is end according to the round

    int getPlayerScore(int player, int round);

    void setPlayerScore(int player, int round, int score);

    int getWinner(int round);

    void setWinner(int round, int player);

    void resetRandomSeed();//flush the random seed

private:
    QList<CardInfo *> cardarray[ROW_NUM];
    AbstractUI::Weather rowWeather[ROW_NUM];
    int playerWinRound[2];//the number of round that player has won
    bool playerPass[2];
    QString playerName[2];
    int playerScore[2][3];
    int winner[3];//-1 if draw
    unsigned randomSeed;

public:
    bool isNeedChooseCard() const;

    void setNeedChooseCard(bool needChooseCard);

    int getPreviousWinner() const;

    void setPreviousWinner(int previousWinner);

    int getCurrentPlayer() const;

    void setCurrentPlayer(int currentPlayer);

    bool isBattleField(int row);

    bool isGraveyard(int row);

    static int
    findTopmostANameNotInB(const QList<CardInfo *> &a, const QList<CardInfo *> &b);//return -1 if all a's names are in b

    static bool listContainsName(const QList<CardInfo *> &list, const QString &name);

    bool isPlayerHandEmpty(int player);

    bool isAllPlayerPass();

private:
    //some psedu-local variable for game looping
    bool needChooseCard;
    int previousWinner;
    int currentPlayer;
    int currentRound;
    bool handled;
public:
    bool isHandled() const;

    void setHandled(bool handled);

public:
    int getCurrentRound() const;

    void setCurrentRound(int currentRound);

public:
    unsigned int getRandomSeed() const;

    void setRandomSeed(unsigned int randomSeed);

    //some operation on cards
    static QList<CardInfo *> getTopNCards(const QList<CardInfo *> &deck, int n);//index larger,topper
    QList<CardInfo *> getPlayerDeckTopNCards(int player, int n);

    static int getDeckIndex(int player);

    static int getHandIndex(int player);

    static int getCandidateIndex(int player);

    static int getSeletedIndex(int player);

    void getCardPosition(CardInfo *card, int &row, int &column);

    void moveCardPosToPos(int fromR, int fromC, int toR, int toC);
};


#endif //PLAYGWENT_GAMEASSETS_H
