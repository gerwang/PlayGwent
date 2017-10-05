//
// Created by Gerwa on 2017/9/12.
//

#include "GraphicsUI.h"
#include "CardInfo.h"
#include "CardWidget.h"
#include "CardArrayWidget.h"
#include "AnimatedEllipse.h"
#include "AnimatedRect.h"
#include <QDebug>
#include <QPushButton>
#include <cstring>
#include <QOpenGLWidget>
#include <QtOpenGL>
#include <QPalette>

const QPointF GraphicsUI::ButtonPos[BUTTON_NUM] = {
        SceneOriginPoint[AbstractUI::PlayerChooserScene] + QPointF(757, 830),
        SceneOriginPoint[AbstractUI::GameScene] + QPointF(129, 708),
        SceneOriginPoint[AbstractUI::DeckBuilderScene] + QPointF(852, 833),
        SceneOriginPoint[AbstractUI::DeckBuilderScene] + QPointF(40, 426),
        SceneOriginPoint[AbstractUI::DeckBuilderScene] + QPointF(40, 494)
};

const QString GraphicsUI::ButtonText[BUTTON_NUM] = {
        tr("Stop Choosing"),
        tr("Pass"),
        tr("Save Deck and Exit"),
        tr("Import from Clipboard"),
        tr("Export to Clipboard")
};


const QPointF GraphicsUI::LabelPos[LABEL_NUM] = {
        SceneOriginPoint[AbstractUI::GameScene] + QPointF(1549, 31),
        SceneOriginPoint[AbstractUI::GameScene] + QPointF(1262, 30),
        SceneOriginPoint[AbstractUI::GameScene] + QPointF(371, 33),
        SceneOriginPoint[AbstractUI::GameScene] + QPointF(366, 174),
        SceneOriginPoint[AbstractUI::GameScene] + QPointF(365, 272),
        SceneOriginPoint[AbstractUI::GameScene] + QPointF(365, 369),

        SceneOriginPoint[AbstractUI::GameScene] + QPointF(365, 489),
        SceneOriginPoint[AbstractUI::GameScene] + QPointF(365, 586),
        SceneOriginPoint[AbstractUI::GameScene] + QPointF(365, 685),
        SceneOriginPoint[AbstractUI::GameScene] + QPointF(371, 850),
        SceneOriginPoint[AbstractUI::GameScene] + QPointF(1264, 848),
        SceneOriginPoint[AbstractUI::GameScene] + QPointF(1551, 847),

        SceneOriginPoint[AbstractUI::PlayerChooserScene] + QPointF(682, 117),

        SceneOriginPoint[AbstractUI::GameScene] + QPointF(122, 38),
        SceneOriginPoint[AbstractUI::GameScene] + QPointF(118, 822),

        SceneOriginPoint[AbstractUI::DeckBuilderScene] + QPointF(260, 198),
        SceneOriginPoint[AbstractUI::DeckBuilderScene] + QPointF(233, 276),
        SceneOriginPoint[AbstractUI::DeckBuilderScene] + QPointF(234, 354),
        SceneOriginPoint[AbstractUI::DeckBuilderScene] + QPointF(193, 97),

        SceneOriginPoint[AbstractUI::GameScene] + QPointF(232, 265),
        SceneOriginPoint[AbstractUI::GameScene] + QPointF(236, 582)
};

GraphicsUI::GraphicsUI() {
    view.setScene(this);
    view.setSceneRect(QRectF(SceneOriginPoint[MainMenuScene], QSizeF(1600, 900)));
    view.show();
    currentScene = MainMenuScene;

    background[0] = QPixmap("assets/background/mainmenu.png");
    background[1] = QPixmap("assets/background/game.png");
    background[2] = QPixmap("assets/background/deckbuilder.png");
    background[3] = QPixmap("assets/background/mainmenu.png");

    focusWidget = nullptr;
    spiritWidget = nullptr;

    for (int i = 0; i < LABEL_NUM; i++) {
        textItem[i].setFont(QFont("Arial", 16, 80, false));
        textItem[i].setPos(LabelPos[i]);
        textItem[i].setDefaultTextColor(QColor(201, 199, 197));
        addItem(&textItem[i]);
    }
    for (int row = Player1_Siege; row <= Player0_Siege; row++) {
        textItem[row].setFont(QFont("Arial", 40, 300, false));
        textItem[row].setPos(textItem[row].pos() - QPointF(7, 7));
    }
    for (int row = Player1_CombatValue; row <= Player0_CombatValue; row++) {
        textItem[row].setFont(QFont("Arial", 40, 300, false));
    }
    textItem[Label_ALL_DeckBuilder].setFont(QFont("Arial", 24, 300, false));
    textItem[Player_CardChooser_Title].setFont(QFont("Arial", 32, 300, false));

    QPalette palette;
    palette.setColor(QPalette::ButtonText, QColor(201, 199, 197));
    for (int i = 0; i < BUTTON_NUM; i++) {
        pushButton[i].setStyleSheet("QPushButton {border-image: url(assets/ui/menu_inactive2.tex.png)}"
                                            "QPushButton:hover{border-image:url(assets/ui/menu_active2.tex.png)}"
                                            "QPushButton:pressed{border-image:url(assets/ui/menu_hit2.tex.png)}");
        pushButton[i].setFont(QFont("Arial", 16, 80, false));
        pushButton[i].setPalette(palette);
        pushButton[i].setText(ButtonText[i]);
        buttonProxy[i] = addWidget(&pushButton[i]);
        buttonProxy[i]->setPos(ButtonPos[i]);
        pushButton[i].setVisible(false);
    }

    lineEdit.setFont(QFont("Arial", 16, 80, false));
    lineEditProxy = addWidget(&lineEdit);
    lineEditProxy->setPos(SceneOriginPoint[AbstractUI::DeckBuilderScene] + QPointF(647, 25));

    for (int row = 0; row < ROW_NUM; row++) {
        cardArrays[row].setDefaultFace(DefaultFace[row]);
        QPointF origin{};
        if (row <= Player0_Graveyard) {
            origin = SceneOriginPoint[AbstractUI::GameScene];
        } else if (Player_Candidate <= row && row <= Player_Seleted) {
            origin = SceneOriginPoint[AbstractUI::PlayerChooserScene];
        } else if (DeckBuilder_Candidate <= row && row <= DeckBuilder_Melee_Event) {
            origin = SceneOriginPoint[AbstractUI::DeckBuilderScene];
        } else {
            origin = SceneOriginPoint[AbstractUI::MainMenuScene];
        }
        if (ModeArea[row] == 0) {
            cardArrays[row].setFixedWidthMode(ArgOne[row], ArgTwo[row], CardWidth[row]);
        } else {
            cardArrays[row].setRollableMode(ArgOne[row], ArgTwo[row] + origin.x(), CardWidth[row]);
        }
        addItem(&cardArrays[row]);
        cardArrays[row].setPos(origin + QPointF(PosX[row], PosY[row]));
    }
    cardArrays[Player_Candidate].setRenderInfo(true);
    cardArrays[Main_Menu].setRenderInfo(true);

    coinWidget = new CardWidget;
    coinWidget->setFront(QPixmap("assets/ui/timer-icon_player.tex.png"));
    coinWidget->setBack(QPixmap("assets/ui/timer-icon_enemy.tex.png"));
    coinWidget->setRenderFlag(0);
    coinWidget->setPos(SceneOriginPoint[AbstractUI::GameScene] + QPointF(115, 392));
    coinWidget->setWidth(110);
    addItem(coinWidget);

    selectionLine.setPen(QPen(QColor(0, 0, 255, 128), 30, Qt::DashDotDotLine, Qt::RoundCap));

    ghostCoordinate = QPoint(-1, -1);
    sourceGhost = nullptr;

    resetValidRows();
    setLocalPlayer(0);
}

void GraphicsUI::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent) {
    const QList<QGraphicsItem *> &itemList = items(mouseEvent->scenePos());
    bool handled = false;
    for (const auto &item:itemList) {
        auto widget = dynamic_cast<QGraphicsWidget *>(item);
        if (widget != nullptr) {
            QString name = widget->metaObject()->className();
            if (inputState == AbstractUI::RejectAll) {

                if (name == "CardWidget") {
                    if (widget == coinWidget) {
                        continue;
                    }
                    handled = true;
                    auto cardwidget = dynamic_cast<CardWidget *>(widget);
                    if (focusWidget != cardwidget && !cardwidget->isAnimated()) {
                        setFocusWidget(cardwidget);
                    }
                    break;
                }

            } else if (inputState == AbstractUI::MustValidRow) {

                if (name == "CardArrayWidget") {
                    auto arraywidget = dynamic_cast<CardArrayWidget *>(widget);
                    if (arraywidget->isClickValid()) {
                        handled = true;
                        QPoint coordinate = locateMousePosition(arraywidget, mouseEvent->scenePos());
                        if (ghostCoordinate != coordinate) {
                            if (ghostCoordinate == QPoint(-1, -1)) {
                                addItem(sourceGhost);
                                if (sourceGhost != nullptr) {
                                    sourceGhost->setPos(
                                            arraywidget->mapToScene(
                                                    arraywidget->makeSpaceForCardAt(coordinate.y(), animationFlag)));
                                }
                            } else {
                                cardArrays[ghostCoordinate.x()].makeSpaceForCardAt(-1, animationFlag);
                                if (sourceGhost != nullptr) {
                                    QPropertyAnimation *animation = new QPropertyAnimation(sourceGhost, "pos");
                                    animation->setEndValue(arraywidget->mapToScene(
                                            arraywidget->makeSpaceForCardAt(coordinate.y(), animationFlag)));
                                    animation->setDuration(CardArrayMakePlaceDuration);
                                    animation->start(QPropertyAnimation::DeleteWhenStopped);
                                }
                            }
                            ghostCoordinate = coordinate;
                        }
                        break;
                    }
                }

            } else if (inputState == AbstractUI::MustValidTarget) {

                if (name == "CardWidget") {
                    auto cardwidget = dynamic_cast<CardWidget *>(widget);
                    if (cardwidget->isValidTarget()) {
                        handled = true;
                        if (focusWidget != cardwidget) {
                            setFocusWidget(cardwidget);
                        }

                        if (source != nullptr) {
                            selectionLine.setLine(
                                    QLineF(source->mapToScene(source->mapFromParent(source->geometry().center())),
                                           cardwidget->mapToScene(
                                                   cardwidget->mapFromParent(cardwidget->geometry().center()))));
                        }
                        break;
                    }
                }

            }
        }
    }
    if (!handled) {
        if (inputState == AbstractUI::RejectAll) {
            releaseFocusWidget();
            createSpiritWidget(nullptr);
        } else if (inputState == AbstractUI::MustValidTarget) {
            releaseFocusWidget();
            createSpiritWidget(nullptr);
            if (source != nullptr) {
                selectionLine.setLine(
                        QLineF(source->mapToScene(source->mapFromParent(source->geometry().center())),
                               mouseEvent->scenePos()));
            }
        } else if (inputState == AbstractUI::MustValidRow) {
            if (ghostCoordinate != QPoint(-1, -1)) {
                cardArrays[ghostCoordinate.x()].makeSpaceForCardAt(-1, animationFlag);
                ghostCoordinate = QPoint(-1, -1);
                if (sourceGhost != nullptr && sourceGhost->scene() == this) {
                    removeItem(sourceGhost);
                }
            }
        }
    }
}

void GraphicsUI::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent) {
    if (inputState == AbstractUI::RejectAll) {
        return;
    }


    const QList<QGraphicsItem *> &itemList = items(mouseEvent->scenePos());
    Command command{};
    int row = -1, column = -1;
    bool handled = false;
    if (mouseEvent->button() == Qt::RightButton) {
        command = Command::RightClick;
        handled = true;
    } else {
        for (const auto &item:itemList) {
            auto widget = dynamic_cast<QGraphicsWidget *>(item);
            if (widget != nullptr) {
                QString name = widget->metaObject()->className();

                if (name == "QGraphicsProxyWidget") {
                    if (widget == buttonProxy[PushButtonEnum::ESCAPE]) {
                        command = Command::EscapeChoose;
                        handled = true;
                        break;
                    } else if (widget == buttonProxy[PushButtonEnum::BUTTON_PASS]) {
                        command = Command::Pass;
                        handled = true;
                        break;
                    } else if (widget == buttonProxy[PushButtonEnum::SAVEDECK]) {
                        command = Command::SaveDeck;
                        handled = true;
                        break;
                    } else if (widget == buttonProxy[PushButtonEnum::CLIPBOARD_IMPORT]) {
                        command = Command::ClipboardImport;
                        handled = true;
                        break;
                    } else if (widget == buttonProxy[PushButtonEnum::CLIPBOARD_EXPORT]) {
                        command = Command::ClipboardExport;
                        handled = true;
                        break;
                    }
                } else {

                    if (inputState == AbstractUI::MustValidTarget) {
                        if (name == "CardWidget") {
                            auto cardwidget = dynamic_cast<CardWidget *>(widget);
                            if (cardwidget->isValidTarget()) {

                                command = Command::LeftClick;
                                for (int i = 0; i < ROW_NUM; i++) {
                                    for (int j = 0; j < cardArrays[i].getCards().size(); j++) {
                                        if (cardArrays[i][j] == cardwidget) {
                                            row = i;
                                            column = j;
                                            break;
                                        }
                                    }
                                }
                                handled = true;
                                break;

                            }
                        }
                    }

                }

            }
        }


        if (inputState == AbstractUI::MustValidRow) {
            if (ghostCoordinate != QPoint(-1, -1)) {
                command = Command::LeftClick;
                row = ghostCoordinate.x();
                column = ghostCoordinate.y();
                handled = true;
            }
        }


    }
    if (handled) {
        for (auto target:outputBuffers) {
            target->writeUserOutput(command, row, column);
        }
    }
}

void GraphicsUI::drawBackground(QPainter *painter, const QRectF &rect) {
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setRenderHint(QPainter::SmoothPixmapTransform);
    for (int i = 0; i < 4; i++) {
        painter->drawPixmap(SceneOriginPoint[i], background[i]);
    }
}

void GraphicsUI::switchToScene(AbstractUI::Scene scene) {
    QPropertyAnimation animation(&view, "sceneRect");
    animation.setEndValue(QRectF(SceneOriginPoint[scene], QSizeF(1600, 900)));
    animation.setDuration(SwitchSceneDuration);
    QEventLoop loop;
    connect(&animation, &QPropertyAnimation::finished, &loop, &QEventLoop::quit);
    animation.start();
    loop.exec();
    currentScene = scene;
}

void GraphicsUI::setButtonEnabled(int button, bool enabled) {
    pushButton[button].setVisible(enabled);
}

AbstractUI::Scene GraphicsUI::getCurrentScene() {
    return currentScene;
}

int GraphicsUI::getLocalPlayer() {
    return localPlayer;
}

void GraphicsUI::playRandomCoinAnimation() {
    coinWidget->flip();
}

void GraphicsUI::execMessageBox(const QString &title, const QString &message, int duration) {
    QEventLoop loop;
    QMessageBox messageBox;
    messageBox.setWindowTitle(title);
    messageBox.setText(message);
    messageBox.setStandardButtons(QMessageBox::NoButton);
    messageBox.setFont(QFont("Arial", 24, 300, false));
    QTimer::singleShot(duration, &loop, &QEventLoop::quit);
    messageBox.show();
    loop.exec();
    messageBox.hide();
}

void GraphicsUI::setWholeRowValidPositions(int row) {
    for (auto card:cardArrays[row].getCards()) {
        card->setValidTarget(true);
    }
}

void GraphicsUI::setLocalPlayer(int player) {
    if ((localPlayer != player) && (localPlayer == 1 || player == 1)) {//opponent, change game pos
        swapGameFacePos();
    }
    if ((localPlayer != player) && (localPlayer == -1 || player == -1)) {
        changePlayerOneVisible();
    }
    localPlayer = player;
}

void GraphicsUI::setCurrentPlayer(int player) {
    currentPlayer = player;

    int targetFace = currentPlayer;
    if (localPlayer == 1) {
        targetFace ^= 1;
    }

    while (coinWidget->getFace() != targetFace) {
        coinWidget->flip();
    }
}

void GraphicsUI::clearAllWeatherOnBattlefield() {
    for (int row = Player1_Siege; row <= Player0_Siege; row++) {
        cardArrays[row].clearWeather();
    }
}

void GraphicsUI::setRowWeather(int row, Weather weatherType) {
    cardArrays[row].changeWeatherTo(weatherType);
}


void GraphicsUI::getUserInput(Command &clicktype, int &row, int &column, int player) {
    if (useTimerFlag) {
        startRound();
    }
    inputBuffer[player]->getUserInput(clicktype, row, column);
    if (useTimerFlag) {
        stopRound();
    }
}

void GraphicsUI::moveCard(int fromR, int fromC, int toR, int toC) {//write a bloking function
    auto mover = cardArrays[fromR].getCards().at(fromC);
    cardArrays[fromR].removeCardAt(fromC, animationFlag);
    cardArrays[toR].addCardAt(mover, toC, animationFlag, true);//blocking
}

void GraphicsUI::setLabelText(int index, QString text) {
    textItem[index].setPlainText(text);
}

void GraphicsUI::resetValidPositions() {
    for (auto &cardArray:cardArrays) {
        for (auto card:cardArray.getCards()) {
            card->setValidTarget(false);
        }
    }
}

void GraphicsUI::setValidPositions(const QList<QPoint> &validPositions) {
    for (auto &point:validPositions) {
        cardArrays[point.x()].getCards().at(point.y())->setValidTarget(true);
    }
}

void GraphicsUI::resetValidRows() {
    for (auto &cardArray:cardArrays) {
        cardArray.setClickValid(false);
    }
}

void GraphicsUI::setValidRow(int row) {
    cardArrays[row].setClickValid(true);
}

void GraphicsUI::setPlayerInputState(int player, AbstractUI::InputState state) {
    if (player == localPlayer) {
        inputState = state;
    }
}

void GraphicsUI::releaseSource() {
    if (source != nullptr) {
        source->shrink();
        if (selectionLine.scene() == this) {
            removeItem(&selectionLine);
        }
        source = nullptr;
    }
    if (sourceGhost != nullptr) {
        if (sourceGhost->scene() == this) {
            removeItem(sourceGhost);
        }
        sourceGhost->deleteLater();
        sourceGhost = nullptr;
    }
}

void GraphicsUI::setSource(int row, int column) {
    if (source != nullptr) {
        releaseSource();
    }
    source = cardArrays[row].getCards().at(column);
    source->expand();
    selectionLine.setLine(QLineF(source->geometry().center(), source->geometry().center()));
    addItem(&selectionLine);
    sourceGhost = new CardWidget(source->getCardinfo());
    sourceGhost->setHeight(source->getDefaultHeight());
    sourceGhost->setRenderFlag(sourceGhost->getRenderFlag() | CardWidget::DrawTransparent);
}

QString GraphicsUI::getLineEditText() {
    return lineEdit.text();
}

void GraphicsUI::setLineEditText(const QString &text) {
    lineEdit.setText(text);
}

void GraphicsUI::loadCardFromAssets(GameAssets *assets) {//no one wants the function to block
    for (int row = 0; row < ROW_NUM; row++) {
        loadARowFromAssets(row, assets);
    }
}

void GraphicsUI::removeCardFromGame(int row, int column) {
    validateBeforeErase(cardArrays[row][column]);
    cardArrays[row].eraseCardFromGameAt(column);
}

void GraphicsUI::showConsume(int fromR, int fromC, int toR, int toC) {
    showUpwardingRectangles(QColor(255, 0, 0, 128), {QPoint(toR, toC)});
    showFlyingEllipseFromSrcToDests(Qt::yellow, QPoint(toR, toC), {QPoint(fromR, fromC)});
    showUpwardingRectangles(QColor(0, 255, 0, 128), {QPoint(fromR, fromC)});
}

void GraphicsUI::showSubtractCD(int row, int column) {
    cardArrays[row][column]->rotateCDAnimation(1, 1);
}

void GraphicsUI::showResumeCD(int row, int column) {
    cardArrays[row][column]->rotateCDAnimation(-1, 2);
}

void GraphicsUI::showBoost(QPoint src, const QList<QPoint> &dests) {
    if (src != QPoint(-1, -1)) {
        showFlyingEllipseFromSrcToDests(Qt::green, src, dests);
    }
    showUpwardingRectangles(QColor(0, 255, 0, 128), dests);
}

void GraphicsUI::showDamage(QPoint src, const QList<QPoint> &dests) {
    if (src != QPoint(-1, -1)) {
        showFlyingEllipseFromSrcToDests(Qt::red, src, dests);
    }
    showUpwardingRectangles(QColor(255, 0, 0, 128), dests);
}

void GraphicsUI::showFlyingEllipseFromSrcToDests(const QColor &color, QPoint src, const QList<QPoint> &dests) {
    auto *group = new QParallelAnimationGroup();
    static const int radius = 15, duration = 400;
    for (const auto &dest:dests) {
        auto item = new AnimatedEllipse;
        item->setRect(-radius, -radius, radius * 2, radius * 2);
        item->setPen(QColor(0, 0, 0, 0));
        item->setBrush(color);
        item->setZValue(100);
        item->setParent(group);
        addItem(item);
        QPropertyAnimation *animation = new QPropertyAnimation(item, "pos", group);
        animation->setStartValue(cardArrays[src.x()].mapToScene(cardArrays[src.x()][src.y()]->geometry().center()));
        animation->setEndValue(cardArrays[dest.x()].mapToScene(cardArrays[dest.x()][dest.y()]->geometry().center()));
        animation->setEasingCurve(QEasingCurve::InOutQuad);
        animation->setDuration(duration);
        group->addAnimation(animation);
    }
    QEventLoop loop;
    connect(group, &QParallelAnimationGroup::finished, &loop, &QEventLoop::quit);
    group->start();
    loop.exec();
    delete group;
}

void GraphicsUI::showUpwardingRectangles(const QColor &color, const QList<QPoint> &dests) {
    auto group = new QParallelAnimationGroup();
    static const int duration = 400;
    for (const auto &dest:dests) {
        auto seq = new QSequentialAnimationGroup(group);
        auto destwidget = cardArrays[dest.x()][dest.y()];
        QPointF position = destwidget->mapToScene(destwidget->mapFromParent(destwidget->pos()));
        auto rectItem = new AnimatedRect;
        rectItem->setPen(QColor(0, 0, 0, 0));
        rectItem->setBrush(color);
        addItem(rectItem);
        rectItem->setParent(seq);

        auto *inAnim = new QPropertyAnimation(rectItem, "rect", seq);
        inAnim->setStartValue(QRectF(position + QPointF(0, destwidget->geometry().height()),
                                     QSizeF(destwidget->geometry().width(), 0)));
        inAnim->setEndValue(QRectF(position, destwidget->size()));
        inAnim->setDuration(duration);
        inAnim->setEasingCurve(QEasingCurve::OutQuad);
        seq->addAnimation(inAnim);

        auto *outAnim = new QPropertyAnimation(rectItem, "rect", seq);
        outAnim->setEndValue(QRectF(position, QSizeF(destwidget->geometry().width(), 0)));
        outAnim->setDuration(duration);
        outAnim->setEasingCurve(QEasingCurve::InQuad);
        seq->addAnimation(outAnim);
        group->addAnimation(seq);
    }
    QEventLoop loop;
    connect(group, &QParallelAnimationGroup::finished, &loop, &QEventLoop::quit);
    group->start();
    loop.exec();
    group->deleteLater();
}

void GraphicsUI::spawnNewCard(CardInfo *card, int row, int column) {
    cardArrays[row].spawnCardAt(column, card, animationFlag);
}

void GraphicsUI::createSpiritWidget(CardWidget *fromWidget) {
    static const QPoint SpiritPosition[4] = {QPoint(-1, -1), QPoint(1313, 178), QPoint(1309, 120), QPoint(-1, -1)};
    if (spiritWidget != nullptr) {
        spiritWidget->spiritExitScene();
        spiritWidget = nullptr;
    }
    if (fromWidget != nullptr) {
        if (SpiritPosition[currentScene] != QPoint(-1, -1)) {
            spiritWidget = new CardWidget(fromWidget->getCardinfo());
            spiritWidget->setRenderFlag(fromWidget->getRenderFlag() | CardWidget::DrawInfo);
            spiritWidget->setFace(fromWidget->getFace());
            addItem(spiritWidget);
            spiritWidget->setPos(SceneOriginPoint[currentScene] + SpiritPosition[currentScene]);
            spiritWidget->spiritEnterScene();
        }
    }
}

void GraphicsUI::releaseFocusWidget() {
    if (focusWidget != nullptr) {
        focusWidget->shrink();
        focusWidget = nullptr;
    }
}

QPoint
GraphicsUI::locateMousePosition(CardArrayWidget *arraywidget, const QPointF &mousePos) {
    int row = -1, column = -1;
    for (int index = 0; index < ROW_NUM; index++) {
        if (arraywidget == &cardArrays[index]) {
            row = index;
            break;
        }
    }
    if (row != -1) {
        column = 0;
        for (auto &cardwidget:arraywidget->getCards()) {
            QPointF centerPos = cardwidget->mapToScene(cardwidget->mapFromParent(cardwidget->geometry().center()));
            if (mousePos.x() > centerPos.x()) {
                column++;
            } else {
                break;
            }
        }
    }
    return {row, column};
}

void GraphicsUI::setFocusWidget(CardWidget *cardwidget) {
    releaseFocusWidget();
    focusWidget = cardwidget;
    focusWidget->setZValue(100);
    focusWidget->expand();
    createSpiritWidget(cardwidget);
}

void GraphicsUI::validateBeforeErase(CardWidget *cardwidget) {
    if (cardwidget == nullptr) {
        return;
    }
    if (sourceGhost != nullptr && sourceGhost->getCardinfo() == cardwidget->getCardinfo()) {
        releaseSource();
    }
    if (spiritWidget != nullptr && spiritWidget->getCardinfo() == cardwidget->getCardinfo()) {
        createSpiritWidget(nullptr);
    }
    if (focusWidget == cardwidget) {
        focusWidget = nullptr;
    }
    if (spiritWidget == cardwidget) {
        spiritWidget = nullptr;
    }
    if (sourceGhost == cardwidget) {
        sourceGhost = nullptr;
    }
    if (source == cardwidget) {
        source = nullptr;
    }
}

void GraphicsUI::swapGameFacePos() {
    for (int row = Player1_Graveyard; row <= Player1_Melee; row++) {
        int enemyRow = Player0_Graveyard - row;
        auto tempPos = cardArrays[row].pos();
        cardArrays[row].setPos(cardArrays[enemyRow].pos());
        cardArrays[enemyRow].setPos(tempPos);

        auto tempFace = cardArrays[row].getDefaultFace();
        cardArrays[row].setDefaultFace(cardArrays[enemyRow].getDefaultFace());
        cardArrays[enemyRow].setDefaultFace(tempFace);

        auto tempLabelPos = textItem[row].pos();
        textItem[row].setPos(textItem[enemyRow].pos());
        textItem[enemyRow].setPos(tempLabelPos);
    }

    auto combatvaluePos = textItem[Player0_CombatValue].pos();
    textItem[Player0_CombatValue].setPos(textItem[Player1_CombatValue].pos());
    textItem[Player1_CombatValue].setPos(combatvaluePos);

    auto tempNamePos = textItem[Player0_Name].pos();
    textItem[Player0_Name].setPos(textItem[Player1_Name].pos());
    textItem[Player1_Name].setPos(tempNamePos);
}

void GraphicsUI::setPlayerInputBuffer(int player, AbstractInputBuffer *input) {
    inputBuffer[player] = input;
}

QList<AbstractOutputBuffer *> &GraphicsUI::getOutputBuffers() {
    return outputBuffers;
}

void GraphicsUI::loadARowFromAssets(int row, GameAssets *assets) {
    cardArrays[row].performSetWeather(assets->getRowWeather(row));
    while (!cardArrays[row].getCards().empty()) {
        validateBeforeErase(cardArrays[row][0]);
        cardArrays[row].eraseCardFromGameAt(0);
    }
    const QList<CardInfo *> &array = assets->getCardArray(row);
    for (int column = 0; column < array.size(); column++) {
        cardArrays[row].spawnCardAt(column, array[column], false);
    }
}

void GraphicsUI::setLeader(CardInfo *leaderInfo) {
    leaderWidget = new CardWidget(leaderInfo);
    leaderWidget->setWidth(95);
    addItem(leaderWidget);
    leaderWidget->setPos(SceneOriginPoint[DeckBuilderScene] + LeaderPosition);
}

void GraphicsUI::clearLeader() {
    if (leaderWidget->scene() == this) {
        removeItem(leaderWidget);
    }
    leaderWidget->deleteLater();
    leaderWidget = nullptr;
}

CardArrayWidget *GraphicsUI::getRowCardArrayWidget(int row) {
    return &cardArrays[row];
}

bool GraphicsUI::isAnimationFlag() const {
    return animationFlag;
}

void GraphicsUI::setAnimationFlag(bool animationFlag) {
    GraphicsUI::animationFlag = animationFlag;
}

void GraphicsUI::changePlayerOneVisible() {
    int facemode = cardArrays[Player1_Hand].getDefaultFace();
    cardArrays[Player1_Hand].setDefaultFace(facemode ^ 1);
}

CardWidget *GraphicsUI::getFocusWidget() const {
    return focusWidget;
}

void GraphicsUI::startRound() {
    if (currentPlayer != localPlayer) {
        return;
    }
    timeWidget.setPos(SceneOriginPoint[currentScene] + TimerPosition[currentScene]);
    timeWidget.startTimer();
    addItem(&timeWidget);
    connect(&timeWidget, &TimeWidget::timeup, this, &GraphicsUI::handleTimeUp);
}

void GraphicsUI::stopRound() {
    if (currentPlayer != localPlayer) {
        return;
    }
    timeWidget.stopTimer();
    removeItem(&timeWidget);
    disconnect(&timeWidget, &TimeWidget::timeup, this, &GraphicsUI::handleTimeUp);
}

void GraphicsUI::handleTimeUp() {
    for (auto target:outputBuffers) {
        target->writeUserOutput(Command::Offline, -1, -1);
    }
}

void GraphicsUI::resetRound() {
    timeWidget.resetTimer(60);
}

void GraphicsUI::setUseTimerFlag(bool useTimerFlag) {
    GraphicsUI::useTimerFlag = useTimerFlag;
}

