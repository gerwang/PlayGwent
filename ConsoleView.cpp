//
// Created by Gerwa on 2017/9/5.
//

#include <QtCore/QMetaEnum>
#include "ConsoleView.h"
#include <QDebug>
#include <QtWidgets/QMessageBox>

ConsoleView::ConsoleView(QObject *parent) {
    mainwindow = new ConsoleWindow();
    QLabel *tempLabel[10] = {mainwindow->ui->label,
                             mainwindow->ui->label_2,
                             mainwindow->ui->label_3,
                             mainwindow->ui->label_4,
                             mainwindow->ui->label_5,
                             mainwindow->ui->label_6,
                             mainwindow->ui->label_7,
                             mainwindow->ui->label_8,
                             mainwindow->ui->label_9,
                             mainwindow->ui->label_10};
    for (int i = 0; i < 10; i++) {
        textLabelArray[i] = tempLabel[i];
    }
    pushButtonArray[0] = mainwindow->ui->pushButton;
    pushButtonArray[1] = mainwindow->ui->pushButton_2;
    lineEditArray[0] = mainwindow->ui->lineEdit;
    lineEditArray[1] = mainwindow->ui->lineEdit_2;
    textBrowser = mainwindow->ui->textBrowser;
}

ConsoleView::~ConsoleView() {
    delete mainwindow;
}

void ConsoleView::setSource(int row, int column) {
    source = QPoint(row, column);
    textBrowser->append(
            tr("set source (%1, %2, %3)\n").arg(row).arg(column).arg(cardslots[row][column]->getCardName()));
}

void ConsoleView::releaseSource() {
    source = QPoint(-1, -1);//the dummy value -1
    textBrowser->append(tr("source removed"));
}

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

void ConsoleView::setUIState(AbstractUI::State state) {
    m_state = state;
    textBrowser->append(
            tr("current state switched: %1").arg(QMetaEnum::fromType<State>().valueToKey(state)));
}

void ConsoleView::setValidRows(const QList<int> &validRows) {
    for (auto row:validRows) {
        validRow[row] = true;
    }
}

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
    textLabelArray[index]->setText(text);
}

void ConsoleView::displayCardInfo(CardInfo *info) {
    const char *nameColor[] = {"chocolate", "silver", "gold", "gray"};
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
    qDebug() << result;//TODO: delete test code
    textBrowser->append(result);
}

void ConsoleView::displaySlotIndexInfo(int index) {
    textBrowser->append(
            tr("<font color=\"orangered\">[") + QString::number(index) + tr("] ") +
            cardslotMsg + tr("</font>: "));
    displaySlotInfo(cardslots[index]);
}

void ConsoleView::displaySlotInfo(const QList<CardInfo *> &slot) {
    for (auto info:slot) {
        displayCardInfo(info);
        textBrowser->append(" ");
    }
    textBrowser->append("\n");
}

void ConsoleView::display() {
    textBrowser->append(tr("Player %1's turn\n").arg(currentPlayer));
    for (int i = 0; i < ROW_NUM; i++) {
        displaySlotIndexInfo(i);
    }
    if (source.x() != -1) {
        textBrowser->append(tr("source: (%1, %2, %3)\n").arg(source.x()).arg(source.y()).arg(
                cardslots[source.x()][source.y()]->getCardName()));
    }
    textBrowser->append(tr("target: "));
    for (auto &target:targets) {
        textBrowser->append(tr("(%1, %2, %3) ").arg(target.x()).arg(target.y()).arg(
                cardslots[target.x()][target.y()]->getCardName()));
    }
    textBrowser->append("\n\n");
}

void ConsoleView::moveCard(int fromR, int fromC, int toR, int toC) {
    textBrowser->append(
            tr("moving %1(%2,%3)->(%4,%5)\n").arg(cardslots[fromR][fromC]->getCardName()).arg(fromR).arg(fromC).arg(
                    toR).arg(toC));
    bool valid = posBoolValid[fromR][fromC];
    posBoolValid[fromR].removeAt(fromC);
    posBoolValid[toR].insert(toC, valid);
    moveCard(cardslots[fromR], fromC, cardslots[toR], toC);
}

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

void ConsoleView::moveCard(QList<CardInfo *> &fromSlot, int fromIndex, QList<CardInfo *> &toSlot, int toIndex) {
    if (&fromSlot == &toSlot) {
        qDebug() << "same slot! 65468435";//consider gell
    }
    CardInfo *mover = fromSlot[fromIndex];
    fromSlot.removeAt(fromIndex);

    textBrowser->append(tr("animation to make space\n"));

    toSlot.insert(toIndex, mover);

    textBrowser->append(tr("animation moving\n"));//TODO: finally some card will have front side and back side
}

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

                    if (m_state == Hand ||
                        m_state == Target) {
                        if (posBoolValid[row][column]) {
                            return;//get a valid result
                        }
                    }
                    break;
                case 'r'://right click to cancel the selection
                    if (m_state == Hand ||
                        m_state == Target ||
                        m_state == Pos) {
                        return;// let logic to judge if it is valid
                    }
                    break;
                case 's'://stop follow
                    if (m_state == Hand) {
                        return;
                    }
                    break;
                case 'e'://escape when choose a card
                    if (m_state == Chooser) {
                        return;
                    }
                    break;
            }
        }
    }
}

void ConsoleView::showSelfDamage(const QList<QPoint> &targets) {
    //play the animation parallel
    textBrowser->append("self damage: ");
    for (auto &target:targets) {
        textBrowser->append(
                tr("%1,%2,%3").arg(target.x()).arg(target.y()).arg(cardslots[target.x()][target.y()]->getCardName()));
    }
    textBrowser->append("\n");
}

void ConsoleView::showDamage(QPoint source, const QList<QPoint> &targets) {
    //play the animation parallelly
    textBrowser->append(tr("damage source: (%1,%2,%3)\n").arg(source.x()).arg(source.y()).arg(
            cardslots[source.x()][source.y()]->getCardName()));
    textBrowser->append("damage target list: ");
    for (auto &target:targets) {
        textBrowser->append(
                tr("(%1,%2,%3)").arg(target.x()).arg(target.y()).arg(cardslots[target.x()][target.y()]->getCardName()));
    }
    textBrowser->append("\n");
}

void ConsoleView::showSelfBoost(const QList<QPoint> &targets) {
    //play the animation parallel
    textBrowser->append("self boost: ");
    for (auto &target:targets) {
        textBrowser->append(
                tr("(%1,%2,%3)").arg(target.x()).arg(target.y()).arg(cardslots[target.x()][target.y()]->getCardName()));
    }
    textBrowser->append("\n");
}

void ConsoleView::showBoost(QPoint source, const QList<QPoint> &targets) {
    //play the animation parallelly
    textBrowser->append(tr("boost source: (%1,%2,%3)\n").arg(source.x()).arg(source.y()).arg(
            cardslots[source.x()][source.y()]->getCardName()));
    textBrowser->append("boost target list: ");
    for (auto &target:targets) {
        textBrowser->append(
                tr("(%1,%2,%3) ").arg(target.x()).arg(target.y()).arg(
                        cardslots[target.x()][target.y()]->getCardName()));
    }
    textBrowser->append("\n");
}

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

void ConsoleView::setCurrentPlayer(int player) {
    currentPlayer = player;
    textBrowser->append(tr("flipping player turn coin animation: %1\n").arg(currentPlayer));
}

void ConsoleView::setLocalPlayer(int player) {
    localPlayer = player;
}

void ConsoleView::showTimer(double timeleft) {//this function should NOT block
    //initially there is not timer TODO: add a timer
}

void ConsoleView::clearTimer() {

}

void ConsoleView::applyWeatherOnRow(Weather weathertype, int row) {
    textBrowser->append(tr("weather on row %1 changed from %2 to %3\n").arg(row).arg(rowWeather[row]).arg(
            weathertype));//TODO: check if the enum can be outputed normally
    rowWeather[row] = weathertype;
}