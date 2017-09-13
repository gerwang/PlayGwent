//
// Created by Gerwa on 2017/9/5.
//

#ifndef PLAYGWENT_ABSTRACTUI_H
#define PLAYGWENT_ABSTRACTUI_H

#include <QObject>
#include <QList>
#include <QPoint>
#include "CardInfo.h"
#include "GameConstant.h"
#include "GameAssets.h"

class AbstractUI {
public:
    enum InputState {
        RejectAll, MustValidTarget, MustValidRow
    };

    enum Scene {
        MainMenuScene, GameScene, DeckBuilderScene, PlayerChooserScene
    };

    virtual void setLineEditText(const QString &text)=0;

    virtual QString getLineEditText()=0;

    virtual void setSource(int row, int column)=0;

    virtual void releaseSource()=0;//when user click right mouse

    virtual void setPlayerInputState(int player, InputState state)=0;

    virtual void setValidRow(int row)=0;

    virtual void resetValidRows()=0;

    virtual void setValidPositions(const QList<QPoint> &validPositions)=0;

    virtual void resetValidPositions()=0;

    virtual void setLabelText(int index, QString text)=0;

    virtual void moveCard(int fromR, int fromC, int toR, int toC)=0;
    //the dead card will be moved to graveyard by logic, because unseen elder's swallow will move the cards together

    virtual void getUserInput(Command &clicktype, int &row, int &column, int player)=0;

    virtual void setRowWeather(int row, Weather weatherType)=0;

    virtual void clearAllWeatherOnBattlefield()=0;

    virtual void setCurrentPlayer(int player)=0;//switched by logic every turn

    virtual void setLocalPlayer(int player)=0;//set local player at the beginning of the game

    virtual void setWholeRowValidPositions(int row)=0;

    virtual void setButtonEnabled(int button, bool enabled)=0;

    virtual void switchToScene(Scene scene)=0;

    virtual Scene getCurrentScene()=0;

    virtual int getLocalPlayer()=0;

    virtual void playRandomCoinAnimation()=0;

    virtual void execMessageBox(const QString &title, const QString &message, int duration)=0;

    virtual void loadCardFromAssets(GameAssets *assets)=0;

    virtual void removeCardFromGame(int row, int column)=0;

    virtual void showConsume(int fromR, int fromC, int toR, int toC)=0;

    virtual void showSubtractCD(int row, int column)=0;

    virtual void showResumeCD(int row, int column)=0;

    virtual void showBoost(QPoint src, const QList<QPoint> &dests)=0;

    virtual void showDamage(QPoint src, const QList<QPoint> &dests)=0;

    virtual void spawnNewCard(CardInfo *card, int row, int column)=0;
};


#endif //PLAYGWENT_ABSTRACTUI_H
