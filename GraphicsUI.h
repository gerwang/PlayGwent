//
// Created by Gerwa on 2017/9/12.
//

#ifndef PLAYGWENT_GRAPHICSUI_H
#define PLAYGWENT_GRAPHICSUI_H

#include "ZoomGraphicsView.h"
#include "NetworkManager.h"
#include "AbstractUI.h"
#include "AbstractIOBuffer.h"
#include "CardWidget.h"
#include "CardArrayWidget.h"
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

protected:
    void drawBackground(QPainter *painter, const QRectF &rect) override;

private:
    ZoomGraphicsView view;
    NetworkManager networkManager;
    int localPlayer;
    int currentPlayer;
    AbstractUI::InputState playerInputState[2];
    AbstractUI::Scene currentScene;
    AbstractIOBuffer *inputBuffer[2];
    bool rowValidation[ROW_NUM];

    QPushButton pushButton[BUTTON_NUM];
    QLineEdit lineEdit;
    QGraphicsTextItem textItem[LABEL_NUM];

    QGraphicsProxyWidget *buttonProxy[BUTTON_NUM];
    QGraphicsProxyWidget *lineEditProxy;

    QPixmap background[4];
    CardWidget *focusWidget{};
    CardWidget *spritWidget{};
    CardArrayWidget cardArrays[ROW_NUM];

    CardWidget *coinWidget;

    CardWidget *source{};

    QGraphicsLineItem seletionLine;
public:
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

};


#endif //PLAYGWENT_GRAPHICSUI_H
