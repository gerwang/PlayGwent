//
// Created by Gerwa on 2017/9/7.
//

#include "ConsoleIOBuffer.h"
#include <QEventLoop>
#include <QStringList>

void ConsoleIOBuffer::getUserInput(QString &command, int &row, int &column) {
    QEventLoop loop;
    connect(button, &QPushButton::click, &loop, &QEventLoop::quit);
    loop.exec();
    QStringList stringList = edit->text().split(QRegExp(" "));
    command = stringList[0];
    if (command[0].toLatin1() == 'l') {
        row = stringList[1].toInt();
        column = stringList[2].toInt();
    }
}
