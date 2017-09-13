//
// Created by Gerwa on 2017/9/6.
//

#ifndef PLAYGWENT_GAMEASSETS_H
#define PLAYGWENT_GAMEASSETS_H


#include <QtCore/QObject>
#include "CardInfo.h"
#include "GameConstant.h"
#include "Deck.h"
#include <algorithm>

class GameAssets : public QObject {//store all the info's that are on the game logic
Q_OBJECT

public:
    bool isGameEnd();
    //if the game ends

    bool getPlayerPass(int player);
    //if player has passed this round

    void setPlayerPass(int player, bool pass);

    QList<CardInfo *> &getCardArray(int row);

    int getPlayerWinRound(int player);

    void setPlayerWinRound(int player, int winRound);

    Weather getRowWeather(int row);

    void setRowWeather(int row, Weather weather);

    const QString &getPlayerName(int player);

    void setPlayerName(int player, const QString &name);

    //judge if game is end according to the round

    int getPlayerScore(int player, int round);

    void setPlayerScore(int player, int round, int score);

    void resetRandomSeed();//flush the random seed

    bool isRoundStart() const;

    void setRoundStart(bool start);

    int getPreviousWinner() const;

    void setPreviousWinner(int previousWinner);

    int getCurrentPlayer() const;

    void setCurrentPlayer(int currentPlayer);

    static bool isBattleField(int row);

    static bool isGraveyard(int row);

    static int
    findTopmostANameNotInB(const QList<CardInfo *> &a, const QList<CardInfo *> &b);//return -1 if all a's names are in b

    static bool listContainsName(const QList<CardInfo *> &list, const QString &name);

    bool isPlayerHandEmpty(int player);

    bool isAllPlayerPass();

    bool isHandled() const;

    void setHandled(bool handled);

    int getCurrentRound() const;

    void setCurrentRound(int currentRound);

    unsigned int getRandomSeed() const;

    void setRandomSeed(unsigned int randomSeed);

    //some operation on cards
    static QList<CardInfo *> getTopNCards(const QList<CardInfo *> &deck, int n);//index larger,topper
    QList<CardInfo *> getPlayerDeckTopNCards(int player, int n);

    static int getDeckIndex(int player);

    static int getHandIndex(int player);

    void getCardPosition(CardInfo *card, int &row, int &column);

    void moveCardPosToPos(int fromR, int fromC, int toR, int toC);

    int getRowCombatValueSum(int row);

    int getPlayerCombatValueSum(int player);

    void addPlayerWinRound(int player);

    void addCurrentRound();

    void clearAllGameRows();

    void loadPlayerDeck(const QList<QString> &zeroNameList,
                        const QList<QString> &oneNameList);

    void createCardsRandomlyOnNameListToRow(QList<QString> namelist, int row);

    int getCardIndexByName(int row, const QString &name);

    static int getEnemySameRowNumber(int rowNumber);

    static int getPlayerMelee(int player);

    static int getPlayerRanged(int player);

    static int getPlayerSiege(int player);

    static int whosePlayerRow(int row);

    void updateRowStrongest(int row, QList<CardInfo *> &result, CardInfo *exclude);

    void updateRowWeakest(int row, QList<CardInfo *> &result, CardInfo *exclude);

    void removeCardFromGame(int row, int column);

    bool cardAlreadyInGraveyard(CardInfo *card);

    int playerBattlefieldBegin(int player);

    int playerBattlefieldEnd(int player);

    bool isCardOnBattlefield(CardInfo *card);

    void setDeckTypeCount(int type, int cnt);

    int getDeckTypeCount(int type);

    void updateDeckTypeCount();

    int getDeckTotalCount() const;

    void setDeckTotalCount(int deckTotalCount);

    void fromJson(const QJsonObject &json);

    void toJson(QJsonObject &json);

    bool isDeckValidMove(CardInfo::Type cardType);

    bool isDeckValidSave();

    QList<Deck> &getDecks();

    void setDecks(const QList<Deck> &decks);

    const QString &getLeaderName() const;

    void setLeaderName(const QString &leaderName);

    int getDeckBuilderTargetRow(CardInfo *card);

private:
    //some psedu-local variable for game looping
    bool roundStart;
    int previousWinner;
    int currentPlayer;
    int currentRound;
    bool handled;

    QList<CardInfo *> cardarray[ROW_NUM];
    Weather rowWeather[ROW_NUM];
    int playerWinRound[2];//the number of round that player has won
    bool playerPass[2];
    QString playerName[2];
    int playerScore[2][3];
    int deckTypeCount[3];
    int deckTotalCount;
    QList<Deck> decks;
    unsigned randomSeed;
    QString leaderName;
};


#endif //PLAYGWENT_GAMEASSETS_H
