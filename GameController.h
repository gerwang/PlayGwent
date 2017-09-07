//
// Created by Gerwa on 2017/9/5.
//

#ifndef PLAYGWENT_GAMECONTROLLER_H
#define PLAYGWENT_GAMECONTROLLER_H

#include <QObject>
#include <QFutureSynchronizer>
#include <QFuture>
#include <QtConcurrent/QtConcurrent>
#include "AbstractUI.h"
#include "GameAssets.h"

class GameController : public QObject {
public:
    static GameController *getController();

    GameAssets *getAssets();//only GameController Base can own the assets
    AbstractUI *getUI();//only GameController Base can own the UI

    void loadUIfromAssets();

    template<typename T>
    void registerProcessor(T *childProcessor);//add inherited processor

    void startGame();

    bool gameLoop();//return true if not end

    void moveCardToRightTop(CardInfo *mover, int targetRow);//move ui and asset simutanouesly

    void performMoveCardToRightTop(int row, int column, int targetRow);

    void moveCardToPos(CardInfo *info, int toR, int toC);

private:
    static GameController *mainController;

    AbstractUI *gameUI;
    GameAssets *assets;

    void controllerHandleBeforeMove(CardInfo *mover, int fromR, int toR);

    void controllerHandleAfterMove(CardInfo *mover, int fromR, int toR);

    void controllerHandleDeploy(CardInfo *mover, int fromR, int toR);

    void performMovePosToPos(int fromR, int fromC, int toR, int toC);

    void performChooseCard(int candidateIndex, int seletedIndex, int supplementIndex,
                           int chooseNum, int player, bool supply, bool allowEscape, const QString &title);

    void performRedrawCard(int player);//just a wrapper of performChooseCard

    void dispatchChooseCard();
    //this function would REALLY BLOCK, so use it in QtConcurrent

    void handleRedrawCard();

    void moveNCardsFromDeckToHand(int player, int n);

    void performRandomlyMoveAllCardsFromAToB(int fromR, int toR);

    void performMoveAllCardsFromAToB(int fromR, int toR);

    void switchUIToChooserScene(int player);

    void setCurrentPlayerForAssetsAndUI(int player);

protected:
    GameController() = default;

public slots:

signals:

    void afterMoveForUser(CardInfo *mover, int fromR, int toR);

    void beforeMoveForUser(CardInfo *mover, int fromR, int toR);

    void beforeDeploy(CardInfo *mover, int fromR, int toR);

    void onDeploy(CardInfo *mover, int fromR, int toR);

    void onDeathWish(CardInfo *mover, int fromR, int toR);

    void onTurnStart();//you can get the current player from assets
};


#endif //PLAYGWENT_GAMECONTROLLER_H
