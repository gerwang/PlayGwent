//
// Created by Gerwa on 2017/9/5.
//

#ifndef PLAYGWENT_ABSTRACTUI_H
#define PLAYGWENT_ABSTRACTUI_H

#include <QObject>
#include <QList>
#include <QPoint>
#include "cardinfo.h"
#include "GameConstant.h"
#include "GameAssets.h"

class AbstractUI {
public:
    enum InputState {
        RejectAll, MustValidTarget, MustValidRow
    };

    enum Scene {
        MainMenuScene, GameScene, DeckBuilderScene, Player0ChooserScene, Player1ChooserScene
    };

    virtual void setSource(int row, int column)=0;

    virtual void releaseSource()=0;//when user click right mouse

//    virtual void addTarget(int row, int column)=0;// there can be MULTIPLE targets!

//    virtual void removeTarget(int row, int column)=0;

    virtual void setPlayerInputState(int player, InputState state)=0;

    virtual void setValidRows(const QList<int> &validRows)=0;

    virtual void resetValidRows()=0;

    virtual void setValidPositions(const QList<QPoint> &validPositions)=0;

    virtual void resetValidPositions()=0;

    virtual void setLabelText(int index, QString text)=0;

    virtual void moveCard(int fromR, int fromC, int toR, int toC)=0;
    //the dead card will be moved to graveyard by logic, because unseen elder's swallow will move the cards together

//    virtual void removeCard(int fromR, int fromC)=0;//remove from the game

//    virtual void spawnCard(int toR, int toC, CardInfo *info)=0;

    virtual void getUserInput(QString &clicktype, int &row, int &column, int player)=0;

//    virtual void applyWeatherOnRow(Weather weatherType, int row)=0;

//    virtual void showSelfDamage(const QList<QPoint> &targets)=0;

//    virtual void showDamage(QPoint source,
//                            const QList<QPoint> &targets)=0;//show damage visual effects(animations) from source to MULTIPLE targets

//    virtual void showSelfBoost(const QList<QPoint> &targets)=0;

//    virtual void showBoost(QPoint source,
//                           const QList<QPoint> &targets)=0;

//    virtual void showSwallow(QPoint source,
//                             const QList<QPoint> &targets)=0;

    virtual void setCurrentPlayer(int player)=0;//switched by logic every turn
    virtual void setLocalPlayer(int player)=0;//set local player at the beginning of the game

//    virtual void showTimer(double timeleft)=0;

//    virtual void clearTimer()=0;

    // not implemented yet
//    virtual void setEnableAnimation(bool enabled)=0; obselete

    virtual void setWholeRowValidPositions(int row)=0;

    virtual void setButtonEnabled(int button, bool enabled)=0;

    virtual void switchToScene(Scene scene)=0;

    virtual Scene getCurrentScene()=0;

    virtual int getLocalPlayer()=0;

    virtual void playRandomCoinAnimation()=0;

    virtual void execMessageBox(const QString &title, const QString &message, int duration)=0;

    virtual void loadCardFromAssets(GameAssets *assets)=0;

signals:

    void clicked();
    //notify the logic that the interface has received a VALID user input,
    // but leave rightclick and stopfollow to be judged by logic
};


#endif //PLAYGWENT_ABSTRACTUI_H
