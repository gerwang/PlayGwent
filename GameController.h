//
// Created by Gerwa on 2017/9/5.
//

#ifndef PLAYGWENT_GAMECONTROLLER_H
#define PLAYGWENT_GAMECONTROLLER_H

#include "AbstractUI.h"
#include "GameAssets.h"
#include <QObject>
#include <QFutureSynchronizer>
#include <QFuture>
#include <QtConcurrent/QtConcurrent>

class GameController : public QObject {
Q_OBJECT
public:
    AbstractUI *getGameUI() const;//only GameController Base can own the UI

    GameAssets *getAssets() const;//only GameController Base can own the assets

    void setGameUI(AbstractUI *gameUI);

    void setAssets(GameAssets *assets);

    static GameController *controller();

    template<typename T>
    void registerProcessor(T *processor);//add inherited processor

    void startGame();

private:
    static GameController *mainController;

    AbstractUI *gameUI{};
    GameAssets *assets{};


    bool gameLoop();//return true if not end

    void controllerHandleBeforeMove(CardInfo *mover, int fromR, int toR);

    void controllerHandleAfterMove(CardInfo *mover, int fromR, int toR);

    void controllerHandleDeploy(CardInfo *mover, int fromR, int toR);

    //with prefix perform, the operation will not trigger abilities
    void performMovePosToPos(int fromR, int fromC, int toR, int toC);

    void performRandomlyMoveAllCardsFromAToB(int fromR, int toR);

    void handleRedrawCard();

    void DestroyAllCardsOnBattlefield();

    void moveToGraveyard(CardInfo *mover);

    void setCurrentPlayerForAssetsAndUI(int player);

    void setPlayerPassAndJudgeRoundEnd(int currentPlayer);

    void clearWeatherOnAllBattlefield();

protected:
    GameController() = default;

public://functions declared here can only be called by monitors' controller()-> etc
    void defaultHandleHand(CardInfo *seletedCard, bool allowCancel);

    void handleHandValidPosition(CardInfo *seletedCard, bool allowCancel, const QList<QPoint> &validPositions,
                                 Command &command, int &toR, int &toC);

    void moveCardToPos(CardInfo *info, int toR, int toC);

    void moveCardToRightTop(CardInfo *mover, int targetRow);//move ui and asset simutanouesly

    void DestroyCard(CardInfo *card);

    void Consume(CardInfo *consumer, CardInfo *food);

    void subtractCD(CardInfo *unit);

    void resumeCD(CardInfo *unit, int initialCD);

    void boostFromSrcToDests(CardInfo *src, const QList<CardInfo *> &dests, int boost,
                             int armor);//nullptr if there is not src

    void damageFromSrcToDests(CardInfo *src, const QList<CardInfo *> &dests, int damage, bool armorUseful);

    void spawnCardToPosByPlayer(CardInfo *card, int row, int column, int player);

    //move cards and trigger abilities
    void moveNCardsFromDeckToHand(int player, int n);

    void performMoveCardToRightTop(int row, int column, int targetRow);

    //choose card when round starts
    void performChooseCard(int candidateIndex, int seletedIndex, int supplementIndex,
                           int chooseNum, int player, bool supply, bool allowEscape,
                           bool allowSwitchScene, const QString &title);

    void performMoveAllCardsFromAToB(int fromR, int toR);

    void requestPlayerToPlayFromHand(CardInfo *seletedCard, bool allowCancel);

    void changeRowWeather(int row, Weather weatherType);

    void causeWeatherDamage(int row, const QList<CardInfo *> &dests, int damage, bool armorUseful);

    void DestroySpecialCard(CardInfo *specialCard, int fromR);

    void removeCardFromGame(CardInfo *card);

signals:

    void afterMoveForUser(CardInfo *mover, int fromR, int toR);

    void beforeMoveForUser(CardInfo *mover, int fromR, int toR);

    void beforeDeploy(CardInfo *mover, int fromR, int toR);

    void onDeploy(CardInfo *mover, int fromR, int toR);

    void onDeathWish(CardInfo *mover, int fromR, int toR);

    void onTurnStart();//you can get the current player from assets

    void onHandSeleted(CardInfo *seletedCard, bool allowCancel);

    void afterDeploy(CardInfo *mover, int fromR, int toR);

    void afterConsume(CardInfo *consumer, CardInfo *food);

    void onConsume(CardInfo *consumer, CardInfo *food);

    void afterWeatherChanged(int row, Weather oldWeather);

    void onWeatherDamage(int row, const QList<CardInfo *> &dests);
};

template<typename T>
void GameController::registerProcessor(T *processor) {
    connect(this, &GameController::afterMoveForUser, processor, &T::slotAfterMoveForUser);
    connect(this, &GameController::beforeMoveForUser, processor, &T::slotBeforeMoveForUser);
    connect(this, &GameController::beforeDeploy, processor, &T::slotBeforeDeploy);
    connect(this, &GameController::onDeploy, processor, &T::slotOnDeploy);
    connect(this, &GameController::onDeathWish, processor, &T::slotOnDeathWish);
    connect(this, &GameController::onTurnStart, processor, &T::slotOnTurnStart);
    connect(this, &GameController::onHandSeleted, processor, &T::slotOnHandSeleted);
    connect(this, &GameController::afterDeploy, processor, &T::slotAfterDeploy);
    connect(this, &GameController::afterConsume, processor, &T::slotAfterConsume);
    connect(this, &GameController::onConsume, processor, &T::slotOnConsume);
    connect(this, &GameController::afterWeatherChanged, processor, &T::slotAfterWeatherChanged);
    connect(this, &GameController::onWeatherDamage, processor, &T::slotOnWeatherDamage);
}

#endif //PLAYGWENT_GAMECONTROLLER_H
