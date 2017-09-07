//
// Created by Gerwa on 2017/9/5.
//

#ifndef PLAYGWENT_CONSOLEVIEW_H
#define PLAYGWENT_CONSOLEVIEW_H


#include <QtCore/QString>
#include "AbstractUI.h"
#include "ConsoleWindow.h"
#include "ui_consoleui.h"
#include "AbstractIOBuffer.h"

class ConsoleView : public QObject, public AbstractUI {

public:

    explicit ConsoleView(QObject *parent = 0);

    ~ConsoleView() override;

    void setSource(int row, int column) override;

    void releaseSource() override;//when user click right mouse

//    void addTarget(int row, int column) override;// there can be MULTIPLE targets!

//    void removeTarget(int row, int column) override;

//    void setUIState(State state) override;

    void setValidRows(const QList<int> &validRows) override;

    void resetValidRows() override;

    void setValidPositions(const QList<QPoint> &validPositions) override;

    void resetValidPositions() override;

    void setLabelText(int index, QString text) override;

    void moveCard(int fromR, int fromC, int toR, int toC) override;
    //the dead card will be moved to graveyard by logic, because unseen elder's swallow will move the cards together

    void moveCard(QList<CardInfo *> &fromSlot, int fromIndex, QList<CardInfo *> &toSlot, int toIndex);

//    void removeCard(int fromR, int fromC) override;//remove from the game

//    void spawnCard(int toR, int toC, CardInfo *info) override;

    void getUserInput(QString &clicktype, int &row, int &column, int player) override;

//    void showSelfDamage(const QList<QPoint> &targets) override;

//    void showDamage(QPoint source,
//                    const QList<QPoint> &targets) override;//show damage visual effects(animations) from source to MULTIPLE targets

//    void showSelfBoost(const QList<QPoint> &targets) override;

//    void showBoost(QPoint source,
//                   const QList<QPoint> &targets) override;

//    void showSwallow(QPoint source,
//                     const QList<QPoint> &targets) override;

    void setCurrentPlayer(int player) override;//switched by logic every turn
    void setLocalPlayer(int player) override;//set local player at the beginning of the game

//    void showTimer(double timeleft) override;

//    void clearTimer() override;

    QString getCardInfoString(CardInfo *info);

    void displayCardSlotInfo(int row);

    void display();

//    void applyWeatherOnRow(Weather weathertype, int row);


    void setWholeRowValidPositions(int row) override;

    void setButtonEnabled(int button, bool enabled) override;

    void switchToScene(Scene nextScene) override;

    Scene getCurrentScene() override;

    int getLocalPlayer() override;

    void playRandomCoinAnimation() override;

    void execMessageBox(const QString &title, const QString &message, int duration) override;

    void setPlayerInputState(int player, InputState state) override;

    void loadCardFromAssets(GameAssets *assets) override;

private:
    InputState playerInputState[2];// for each player
    ConsoleWindow *mainwindow;

    AbstractIOBuffer *inputBuffer[2];

    QPoint source;
    QList<QPoint> targets;

    Scene currentScene;

    QList<CardInfo *> cardslots[ROW_NUM];
    Weather rowWeather[ROW_NUM];

    QList<bool> posBoolValid[ROW_NUM];
    bool validRow[ROW_NUM];

    QLabel *pQLabel[20];
    QPushButton *pQPushButton[4];
    QLineEdit *pLineEdit;

    QTextBrowser *textBrowser;

    int currentPlayer;
    int localPlayer;
};


#endif //PLAYGWENT_CONSOLEVIEW_H
