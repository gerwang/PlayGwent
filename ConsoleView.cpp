//
// Created by Gerwa on 2017/9/5.
//

#include <QtCore/QMetaEnum>
#include "ConsoleView.h"
#include <QDebug>
#include <QtWidgets/QMessageBox>
#include <QTimer>
#include "ConsoleIOBuffer.h"

ConsoleView::ConsoleView(QObject *parent) {
    mainwindow = new ConsoleWindow();
    QLabel *tempLabel[LABEL_NUM] = {mainwindow->ui->label,
                                    mainwindow->ui->label_2,
                                    mainwindow->ui->label_3,
                                    mainwindow->ui->label_4,
                                    mainwindow->ui->label_5,
                                    mainwindow->ui->label_6,
                                    mainwindow->ui->label_7,
                                    mainwindow->ui->label_8,
                                    mainwindow->ui->label_9,
                                    mainwindow->ui->label_10,
                                    mainwindow->ui->label_11,
                                    mainwindow->ui->label_12,
                                    mainwindow->ui->label_13,
                                    mainwindow->ui->label_14,
                                    mainwindow->ui->label_15,
                                    mainwindow->ui->label_16,
                                    mainwindow->ui->label_17,
                                    mainwindow->ui->label_18,
                                    mainwindow->ui->label_19};
    for (int i = 0; i < LABEL_NUM; i++) {
        pQLabel[i] = tempLabel[i];
    }
    pLineEdit = mainwindow->ui->lineEdit;
    QPushButton *tempButton[BUTTON_NUM] = {mainwindow->ui->pushButton,
                                           mainwindow->ui->pushButton_2,
                                           mainwindow->ui->pushButton_3};
    for (int i = 0; i < BUTTON_NUM; i++) {
        pQPushButton[i] = tempButton[i];
    }
    inputBuffer[0] = new ConsoleIOBuffer(mainwindow->ui->pushButton_5, mainwindow->ui->lineEdit_2);
    inputBuffer[1] = new ConsoleIOBuffer(mainwindow->ui->pushButton_6, mainwindow->ui->lineEdit_3);

    playerInputState[0] = playerInputState[1] = AbstractUI::RejectAll;
    textBrowser = mainwindow->ui->textBrowser;
    mainwindow->show();

    source = QPoint(-1, -1);
    currentScene = AbstractUI::MainMenuScene;

    for (int row = 0; row < ROW_NUM; row++) {
        rowWeather[row] = Weather::Sunny;
        validRow[row] = false;
    }
    currentPlayer = localPlayer = -1;
}

ConsoleView::~ConsoleView() {
    delete mainwindow;
}

void ConsoleView::setSource(int row, int column) {
    source = QPoint(row, column);
    textBrowser->append(
            tr("set source (%1, %2, %3)").arg(row).arg(column).arg(cardslots[row][column]->getCardName()));
}

void ConsoleView::releaseSource() {
    source = QPoint(-1, -1);//the dummy value -1
    textBrowser->append(tr("source removed"));
}

/*
void ConsoleView::addTarget(int row, int column) {
    QPoint target(row, column);
    if (!targets.contains(target)) {
        targets.append(target);
        textBrowser->append(
                tr("add target (%1, %2, %3)\n").arg(row).arg(column).arg(cardslots[row][column]->getCardName()));
    }
}

void ConsoleView::removeTarget(int row, int column) {
    QPoint target(row, column);
    targets.removeAll(target);
    textBrowser->append(
            tr("remove target (%1, %2, %3)\n").arg(row).arg(column).arg(cardslots[row][column]->getCardName()));
}
 */

/*
void ConsoleView::setUIState(AbstractUI::State state) {
    playerInputState = state;
    textBrowser->append(
            tr("current state switched: %1").arg(QMetaEnum::fromType<State>().valueToKey(state)));
}
 */

void ConsoleView::resetValidRows() {
    for (bool &i : validRow) {
        i = false;
    }
}

void ConsoleView::setValidPositions(const QList<QPoint> &validPositions) {
    for (const auto &pos:validPositions) {
        if (pos.x() < 0 || pos.x() >= ROW_NUM) {
            qDebug() << "invalid x!135476864";
        }
        if (pos.y() < 0 || pos.y() >= posBoolValid[pos.x()].size()) {
            qDebug() << "invalid y!687669463";
        }
        posBoolValid[pos.x()][pos.y()] = true;
    }
}

void ConsoleView::resetValidPositions() {
    for (auto &pos :posBoolValid) {
        for (bool &value:pos) {
            value = false;
        }
    }
}

void ConsoleView::setLabelText(int index, QString text) {
    pQLabel[index]->setText(text);
}

QString ConsoleView::getCardInfoString(CardInfo *info) {
    const char *nameColor[] = {"chocolate", "silver", "orange", "gold"};
    QString nameField =
            tr("<font color=\"") + tr(nameColor[info->getType()]) + tr("\">") + info->getCardName() + tr("</font>");
    const char *strenghColor[] = {"red", "black", "green"};
    int index = 1;
    if (info->getCurrentStrength() < info->getInitialStrength()) {
        index = 0;
    } else if (info->getCurrentStrength() > info->getInitialStrength()) {
        index = 2;
    }
    QString strengthField =
            tr("<font color=\"") + tr(strenghColor[index]) + tr("\">") + QString::number(info->getCurrentStrength()) +
            tr("</font>");
    QString result = tr("(") + nameField + tr(", ") + strengthField;
    if (info->getCD() >= 0) {
        result += tr(", CD: ") + QString::number(info->getCD());
    }
    if (info->getArmor() > 0) {
        result += tr(", armor: ") + QString::number(info->getArmor());
    }
    if (info->isShield()) {
        result += tr(", shield");
    }
    result += tr(")");
    return result;
}

void ConsoleView::displayCardSlotInfo(int row) {
    QString RowIndexColor[2] = {"green", "red"};
    QString result = tr("<font color=\"%1\">[").arg(RowIndexColor[validRow[row]]) + QString::number(row) + tr("] ") +
                     CardSlotMSG[row] +
                     tr("</font>: ");
    if (rowWeather[row] != Weather::Sunny) {
        result += WeatherMSG[rowWeather[row]];
    }
    for (int column = 0; column < cardslots[row].size(); column++) {
        QString validColor[2] = {"green", "red"};
        QString columnStr = tr("<font color=\"%1\">[%2]</font>").arg(validColor[posBoolValid[row][column]]).arg(column);
        result += columnStr;
        if (source == QPoint(row, column)) {//been set source
            result += tr("src:");
        }
        if (targets.contains(QPoint(row, column))) {
            result += tr("target:");
        }
        result += getCardInfoString(cardslots[row][column]);
    }
    textBrowser->append(result);
}

void ConsoleView::display() {
    textBrowser->append("begin-------------------------------------------------------------------");
    textBrowser->append(tr("Player %1's turn in scene %2").arg(currentPlayer).arg(SceneMSG[currentScene]));
    for (int row = 0; row < ROW_NUM; row++) {
        displayCardSlotInfo(row);
    }
    textBrowser->append("end-------------------------------------------------------------------");
}

void ConsoleView::moveCard(int fromR, int fromC, int toR, int toC) {
//    qDebug() << cardslots[fromR].size() << fromC << "237486";//TODO delete test
//    qDebug() << cardslots[fromR][fromC]->getCardName() << "546846876";//TODO delete test
    textBrowser->append(
            tr("moving %1(%2,%3)->(%4,%5)").arg(cardslots[fromR][fromC]->getCardName()).arg(fromR).arg(fromC).arg(
                    toR).arg(toC));
    bool valid = posBoolValid[fromR][fromC];
    posBoolValid[fromR].removeAt(fromC);
    posBoolValid[toR].insert(toC, valid);
    moveCard(cardslots[fromR], fromC, cardslots[toR], toC);
}

/*
void ConsoleView::removeCard(int fromR, int fromC) {
    cardslots[fromR].removeAt(fromC);
    posBoolValid[fromR].removeAt(fromC);

    textBrowser->append(tr("removing (%1,%2,%3)\n").arg(fromR).arg(fromC).arg(cardslots[fromR][fromC]->getCardName()));
}

void ConsoleView::spawnCard(int toR, int toC, CardInfo *info) {
    cardslots[toR].insert(toC, info);
    posBoolValid[toR].insert(toC, false);//TODO: will the update of valid array on time?

    textBrowser->append(tr("spawning (%1,%2,%3)\n").arg(toR).arg(toC).arg(info->getCardName()));
}
*/
void ConsoleView::moveCard(QList<CardInfo *> &fromSlot, int fromIndex, QList<CardInfo *> &toSlot, int toIndex) {
    if (&fromSlot == &toSlot) {
        qDebug() << "same slot! 65468435" << fromIndex << toIndex;//consider gell, he is okay now
    }
    CardInfo *mover = fromSlot[fromIndex];
    fromSlot.removeAt(fromIndex);

//    textBrowser->append(tr("animation to make space\n"));

    toSlot.insert(toIndex, mover);

//    textBrowser->append(tr("animation moving\n"));//TODO: finally some card will have front side and back side
}

/*
//this function will block
void ConsoleView::getUserInput(QString &clicktype, int &row, int &column, int player) {
    //some valid
    // l r e s
    while (true) {

        QEventLoop loop;
        connect(pushButtonArray[player], &QPushButton::click, &loop, &QEventLoop::quit);
        loop.exec();
        disconnect(pushButtonArray[player], &QPushButton::click, &loop, &QEventLoop::quit);

        QStringList stringList = lineEditArray[player]->text().split(QRegExp(" "));
        lineEditArray[player]->clear();

        if (stringList.empty()) {
            textBrowser->append(tr("no input!\n"));
        } else {

            clicktype = stringList[0];

            switch (stringList[0][0].toLatin1()) {
                case 'l'://left button

                    row = stringList[1].toInt();
                    column = stringList[2].toInt();

                    if (playerInputState == Hand ||
                        playerInputState == Target) {
                        if (posBoolValid[row][column]) {
                            return;//get a valid result
                        }
                    }
                    break;
                case 'r'://right click to cancel the selection
                    if (playerInputState == Hand ||
                        playerInputState == Target ||
                        playerInputState == Pos) {
                        return;// let logic to judge if it is valid
                    }
                    break;
                case 's'://stop follow
                    if (playerInputState == Hand) {
                        return;
                    }
                    break;
                case 'e'://escape when choose a card
                    if (playerInputState == Chooser) {
                        return;
                    }
                    break;
            }
        }
    }
}
 */
/*
void ConsoleView::showSelfDamage(const QList<QPoint> &targets) {
    //play the animation parallel
    textBrowser->append("self damage: ");
    for (auto &target:targets) {
        textBrowser->append(
                tr("%1,%2,%3").arg(target.x()).arg(target.y()).arg(cardslots[target.x()][target.y()]->getCardName()));
    }
    textBrowser->append("\n");
}
*/
void ConsoleView::showDamage(QPoint src, const QList<QPoint> &dests) {
    //play the animation parallelly
    if (src != QPoint(-1, -1)) {
        textBrowser->append(tr("damage src: (%1,%2,%3)").arg(src.x()).arg(src.y()).arg(
                cardslots[src.x()][src.y()]->getCardName()));
    }
    QString line = tr("damage target list: ");
    for (auto &target:dests) {
        line += tr("(%1,%2,%3)").arg(target.x()).arg(target.y()).arg(cardslots[target.x()][target.y()]->getCardName());
    }
    textBrowser->append(line);
}

/*
void ConsoleView::showSelfBoost(const QList<QPoint> &targets) {
    //play the animation parallel
    textBrowser->append("self boost: ");
    for (auto &target:targets) {
        textBrowser->append(
                tr("(%1,%2,%3)").arg(target.x()).arg(target.y()).arg(cardslots[target.x()][target.y()]->getCardName()));
    }
    textBrowser->append("\n");
}
*/
void ConsoleView::showBoost(QPoint src, const QList<QPoint> &dests) {
    //play the animation parallelly
    if (src != QPoint(-1, -1)) {//really has a source
        textBrowser->append(tr("boost src: (%1,%2,%3)").arg(src.x()).arg(src.y()).arg(
                cardslots[src.x()][src.y()]->getCardName()));
    }
    QString line = tr("boost target list: ");
    for (auto &target:dests) {
        line += tr("(%1,%2,%3) ").arg(target.x()).arg(target.y()).arg(
                cardslots[target.x()][target.y()]->getCardName());
    }
    textBrowser->append(line);
}

/*
void ConsoleView::showSwallow(QPoint source, const QList<QPoint> &targets) {
    //play the animation parallelly
    textBrowser->append(tr("swallow source: (%1,%2,%3)\n").arg(source.x()).arg(source.y()).arg(
            cardslots[source.x()][source.y()]->getCardName()));
    textBrowser->append("swallow target list: ");
    for (auto &target:targets) {
        textBrowser->append(
                tr("%1,%2,%3").arg(target.x()).arg(target.y()).arg(cardslots[target.x()][target.y()]->getCardName()));
    }
    textBrowser->append("\n");
}
*/
void ConsoleView::setCurrentPlayer(int player) {
    currentPlayer = player;
    textBrowser->append(tr("flipping player turn coin animation: %1").arg(currentPlayer));
}

void ConsoleView::setLocalPlayer(int player) {
    localPlayer = player;
}

void ConsoleView::setWholeRowValidPositions(int row) {
    for (bool &column : posBoolValid[row]) {
        column = true;
    }
}

void ConsoleView::playRandomCoinAnimation() {
    textBrowser->append(tr("Playing random Coin Animation"));
}

void ConsoleView::getUserInput(Command &clicktype, int &row, int &column, int player) {
    textBrowser->append(tr("the current input state is %1").arg(InputStateMSG[playerInputState[player]]));
    display();
    inputBuffer[player]->getUserInput(clicktype, row, column);
}

void ConsoleView::setButtonEnabled(int button, bool enabled) {
    pQPushButton[button]->setEnabled(enabled);
}

void ConsoleView::switchToScene(AbstractUI::Scene nextScene) {
    textBrowser->append(tr("switch Scene from %1 to %2").arg(SceneMSG[currentScene]).arg(SceneMSG[nextScene]));
    currentScene = nextScene;
}

int ConsoleView::getLocalPlayer() {
    return localPlayer;
}

AbstractUI::Scene ConsoleView::getCurrentScene() {
    return currentScene;
}

void ConsoleView::execMessageBox(const QString &title, const QString &message, int duration) {
    QEventLoop loop;
    QMessageBox messageBox;
    messageBox.setWindowTitle(title);
    messageBox.setText(message);
    QTimer::singleShot(duration, &loop, &QEventLoop::quit);
    messageBox.show();
    loop.exec();
    messageBox.hide();
}

void ConsoleView::setPlayerInputState(int player, AbstractUI::InputState state) {
    playerInputState[player] = state;
}

void ConsoleView::loadCardFromAssets(GameAssets *assets) {
    for (int row = Player1_Graveyard; row <= Player0_Graveyard; row++) {
        rowWeather[row] = assets->getRowWeather(row);
        cardslots[row].clear();
        for (auto info:assets->getCardArray(row)) {
            cardslots[row].append(info);
            posBoolValid[row].append(false);
        }
    }
}

void ConsoleView::setValidRow(int row) {
    validRow[row] = true;
}

/*
void ConsoleView::showTimer(double timeleft) {//this function should NOT block
    //initially there is not timer TODO: add a timer
}

void ConsoleView::clearTimer() {

}
*/

void ConsoleView::setRowWeather(int row, Weather weathertype) {
    textBrowser->append(tr("weather on row %1 changed from %2 to %3").arg(row).arg(WeatherMSG[rowWeather[row]]).arg(
            WeatherMSG[weathertype]));
    rowWeather[row] = weathertype;
}

void ConsoleView::removeCardFromGame(int row, int column) {
    textBrowser->append(
            tr("remove (%1,%2,%3) from game").arg(row).arg(column).arg(cardslots[row][column]->getCardName()));
    posBoolValid[row].removeAt(column);
    cardslots[row].removeAt(column);
}

void ConsoleView::showConsume(int fromR, int fromC, int toR, int toC) {
    textBrowser->append(tr("(%1,%2,%3) is consuming (%4,%5,%6)").arg(fromR).arg(fromC).arg(
            cardslots[fromR][fromC]->getCardName()).arg(toR).arg(toC).arg(cardslots[toR][toC]->getCardName()));
}

void ConsoleView::showSubtractCD(int row, int column) {
    textBrowser->append(
            tr("(%1,%2,%3) CD subtracting").arg(row).arg(column).arg(cardslots[row][column]->getCardName()));
}

void ConsoleView::showResumeCD(int row, int column) {
    textBrowser->append(tr("(%1,%2,%3) CD resuming").arg(row).arg(column).arg(cardslots[row][column]->getCardName()));
}

void ConsoleView::spawnNewCard(CardInfo *card, int row, int column) {
    cardslots[row].insert(column, card);
    posBoolValid[row].insert(column, false);
    textBrowser->append(tr("spawn at (%1,%2,%3)").arg(row).arg(column).arg(card->getCardName()));
}

void ConsoleView::clearAllWeatherOnBattlefield() {
    for (int row = Player1_Siege; row <= Player0_Siege; row++) {
        rowWeather[row] = Sunny;
    }
}

