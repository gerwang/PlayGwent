//
// Created by Gerwa on 2017/9/7.
//

#include "ConsoleIOBuffer.h"
#include <QEventLoop>
#include <QStringList>
#include <QDebug>

void ConsoleIOBuffer::getUserInput(QString &command, int &row, int &column) {
    QEventLoop loop;
    connect(button, &QPushButton::clicked, &loop, &QEventLoop::quit);
    loop.exec();
    QStringList stringList = edit->text().split(QRegExp(" "));
    command = stringList[0];
    if (command[0].toLatin1() == 'l') {
        command = "leftclick";
        row = stringList[1].toInt();
        column = stringList[2].toInt();
    } else if (command[0].toLatin1() == 'r') {
        command = "rightclick";
    } else if (command[0].toLatin1() == 'p') {
        command = "pass";
    } else if (command[0].toLatin1() == 'e') {
        command = "escape";
    } else if (command[0].toLatin1() == 's') {
        command = "savedeck";
    } else {
        qWarning() << "unhandled user input!";
    }
}
