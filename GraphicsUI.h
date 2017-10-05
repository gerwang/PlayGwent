//
// Created by Gerwa on 2017/9/12.
//

#ifndef PLAYGWENT_GRAPHICSUI_H
#define PLAYGWENT_GRAPHICSUI_H

#include "ZoomGraphicsView.h"
#include "NetworkManager.h"
#include "AbstractUI.h"
#include "AbstractInputBuffer.h"
#include "CardWidget.h"
#include "CardArrayWidget.h"
#include "AbstractOutputBuffer.h"
#include "TimeWidget.h"
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsProxyWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QGraphicsTextItem>
#include <QGraphicsLineItem>

class GraphicsUI : public QGraphicsScene, public AbstractUI {
Q_OBJECT
public:

    const static QPointF ButtonPos[BUTTON_NUM];
    const static QString ButtonText[BUTTON_NUM];
    const static QPointF LabelPos[LABEL_NUM];

    GraphicsUI();

    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent) override;

    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent) override;

    void showFlyingEllipseFromSrcToDests(const QColor &color, QPoint src, const QList<QPoint> &dests);

    void showUpwardingRectangles(const QColor &color, const QList<QPoint> &dests);

    void createSpiritWidget(CardWidget *fromWidget);

    void releaseFocusWidget();

    void setFocusWidget(CardWidget *cardwidget);

    QPoint locateMousePosition(CardArrayWidget *arrayWidget, const QPointF &mousePos);

    CardArrayWidget *getRowCardArrayWidget(int row);

protected:
    void drawBackground(QPainter *painter, const QRectF &rect) override;

private:
    ZoomGraphicsView view;

private:
    int localPlayer;
    int currentPlayer;
    AbstractUI::InputState inputState;
    AbstractUI::Scene currentScene;
    QList<AbstractOutputBuffer *> outputBuffers;
public:
    QList<AbstractOutputBuffer *> &getOutputBuffers();

private:

    QPushButton pushButton[BUTTON_NUM];
    QLineEdit lineEdit;
    QGraphicsTextItem textItem[LABEL_NUM];

    QGraphicsProxyWidget *buttonProxy[BUTTON_NUM];
    QGraphicsProxyWidget *lineEditProxy;

    QPixmap background[4];
    CardArrayWidget cardArrays[ROW_NUM];

    CardWidget *focusWidget{};
    CardWidget *spiritWidget{};
    CardWidget *sourceGhost;
    CardWidget *coinWidget;
    CardWidget *source{};
    CardWidget *leaderWidget;

    QGraphicsLineItem selectionLine;

    QPoint ghostCoordinate{-1, -1};

    AbstractInputBuffer *inputBuffer[2];

    TimeWidget timeWidget{};

    bool animationFlag{};
    bool useTimerFlag{};
public:

    void setUseTimerFlag(bool useTimerFlag);

    CardWidget *getFocusWidget() const;

    bool isAnimationFlag() const;

    void setAnimationFlag(bool animationFlag);

    void setPlayerInputBuffer(int player, AbstractInputBuffer *input);

    void validateBeforeErase(CardWidget *cardwidget);

    void setLineEditText(const QString &text) override;

    QString getLineEditText() override;

    void setSource(int row, int column) override;

    void releaseSource() override;//when user click right mouse

    void setPlayerInputState(int player, AbstractUI::InputState state) override;

    void setValidRow(int row) override;

    void resetValidRows() override;

    void setValidPositions(const QList<QPoint> &validPositions) override;

    void resetValidPositions() override;

    void setLabelText(int index, QString text) override;

    void moveCard(int fromR, int fromC, int toR, int toC) override;
    //the dead card will be moved to graveyard by logic, because unseen elder's swallow will move the cards together

    void getUserInput(Command &clicktype, int &row, int &column, int player) override;

    void setRowWeather(int row, Weather weatherType) override;

    void clearAllWeatherOnBattlefield() override;

    void setCurrentPlayer(int player) override;//switched by logic every turn

    void setLocalPlayer(int player) override;//set local player at the beginning of the game

    void setWholeRowValidPositions(int row) override;

    void setButtonEnabled(int button, bool enabled) override;

    void switchToScene(AbstractUI::Scene scene) override;

    AbstractUI::Scene getCurrentScene() override;

    int getLocalPlayer() override;

    void playRandomCoinAnimation() override;

    void execMessageBox(const QString &title, const QString &message, int duration) override;

    void loadCardFromAssets(GameAssets *assets) override;

    void removeCardFromGame(int row, int column) override;

    void showConsume(int fromR, int fromC, int toR, int toC) override;

    void showSubtractCD(int row, int column) override;

    void showResumeCD(int row, int column) override;

    void showBoost(QPoint src, const QList<QPoint> &dests) override;

    void showDamage(QPoint src, const QList<QPoint> &dests) override;

    void spawnNewCard(CardInfo *card, int row, int column) override;

    void swapGameFacePos();

    void setLeader(CardInfo *leaderInfo);

    void clearLeader();

    void loadARowFromAssets(int row, GameAssets *assets);

    void changePlayerOneVisible();

    void startRound();

    void stopRound();

    void resetRound();

    void handleTimeUp();
};


#endif //PLAYGWENT_GRAPHICSUI_H
