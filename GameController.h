//
// Created by Gerwa on 2017/9/5.
//

#ifndef PLAYGWENT_GAMECONTROLLER_H
#define PLAYGWENT_GAMECONTROLLER_H

#include "AbstractUI.h"
#include "GameAssets.h"
#include "SubclassRegister.h"
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

    static GameController *getController();

    template<typename T>
    void registerProcessor(T *childProcessor);//add inherited processor

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

    void performMoveCardToRightTop(int row, int column, int targetRow);

    void performRandomlyMoveAllCardsFromAToB(int fromR, int toR);

    void performMoveAllCardsFromAToB(int fromR, int toR);

    //choose card when round starts
    void performChooseCard(int candidateIndex, int seletedIndex, int supplementIndex,
                           int chooseNum, int player, bool supply, bool allowEscape,
                           bool allowSwitchScene, const QString &title);

    void handleRedrawCard();

    void moveAllFromBattlefieldToGraveyard();

    void moveToGraveyard(CardInfo *mover);


    void switchUIToChooserScene(int player);

    void setCurrentPlayerForAssetsAndUI(int player);

    void setPlayerPassAndJudgeRoundEnd(int currentPlayer);

protected:
    GameController() = default;

    void moveCardToRightTop(CardInfo *mover, int targetRow);//move ui and asset simutanouesly

    void moveCardToPos(CardInfo *info, int toR, int toC);

    //move cards and trigger abilities
    void moveNCardsFromDeckToHand(int player, int n);

public slots:

    virtual void slotAfterMoveForUser(CardInfo *mover, int fromR, int toR) {
        //do nothing
    }

    virtual void slotBeforeMoveForUser(CardInfo *mover, int fromR, int toR) {
        //do nothing
    }

    virtual void slotBeforeDeploy(CardInfo *mover, int fromR, int toR) {
        //do nothing
    }

    virtual void slotOnDeploy(CardInfo *mover, int fromR, int toR) {
        //do nothing
    }

    virtual void slotOnDeathWish(CardInfo *mover, int fromR, int toR) {
        //do nothing
    }

    virtual void slotOnTurnStart() {
        //do nothing
    }//you can get the current player from assets

    virtual void slotOnHandSeleted(CardInfo *seletedCard) {
        //do nothing
    }

    virtual void slotAfterDeploy(CardInfo *mover, int fromR, int toR) {
        //do nothing
    }

signals:

    void afterMoveForUser(CardInfo *mover, int fromR, int toR);

    void beforeMoveForUser(CardInfo *mover, int fromR, int toR);

    void beforeDeploy(CardInfo *mover, int fromR, int toR);

    void onDeploy(CardInfo *mover, int fromR, int toR);

    void onDeathWish(CardInfo *mover, int fromR, int toR);

    void onTurnStart();//you can get the current player from assets

    void onHandSeleted(CardInfo *seletedCard);

    void afterDeploy(CardInfo *mover, int fromR, int toR);
};


#endif //PLAYGWENT_GAMECONTROLLER_H
