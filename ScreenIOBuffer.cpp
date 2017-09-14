//
// Created by Gerwa on 2017/9/14.
//

#include <QtCore/QEventLoop>
#include "ScreenIOBuffer.h"

void ScreenIOBuffer::getUserInput(Command &command, int &row, int &column) {
    if (eventQueue.empty()) {
        QEventLoop loop;
        connect(this, &ScreenIOBuffer::ready, &loop, &QEventLoop::quit);
        loop.exec();
    }
    const Event &event = eventQueue.takeFirst();
    command = event.command;
    row = event.row;
    column = event.column;
}

void ScreenIOBuffer::writeUserOutput(Command command, int row, int column) {
    eventQueue.append({command, row, column});
    emit ready();
}
