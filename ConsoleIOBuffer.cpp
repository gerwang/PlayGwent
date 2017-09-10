//
// Created by Gerwa on 2017/9/7.
//

#include "ConsoleIOBuffer.h"
#include <QEventLoop>
#include <QStringList>
#include <QDebug>
#include "GameConstant.h"

void ConsoleIOBuffer::getUserInput(Command &command, int &row, int &column) {
    QEventLoop loop;
    connect(button, &QPushButton::clicked, &loop, &QEventLoop::quit);
    loop.exec();
    QStringList stringList = edit->text().split(QRegExp(" "));
    edit->clear();
    QString stringCommand = stringList[0];
    if (stringCommand[0].toLatin1() == 'l') {
        command = Command::LeftClick;
        row = stringList[1].toInt();
        column = stringList[2].toInt();
    } else if (stringCommand[0].toLatin1() == 'r') {
        command = Command::RightClick;
    } else if (stringCommand[0].toLatin1() == 'p') {
        command = Command::Pass;
    } else if (stringCommand[0].toLatin1() == 'e') {
        command = Command::Escape;
    } else if (stringCommand[0].toLatin1() == 's') {
        command = Command::SaveDeck;
    } else {
        qWarning() << "unhandled user input!";
    }
}
