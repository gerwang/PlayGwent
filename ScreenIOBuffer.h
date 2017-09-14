//
// Created by Gerwa on 2017/9/14.
//

#ifndef PLAYGWENT_SCREENIOBUFFER_H
#define PLAYGWENT_SCREENIOBUFFER_H

#include "AbstractInputBuffer.h"
#include "AbstractOutputBuffer.h"
#include <QObject>
#include <QList>

struct Event {
    Command command;
    int row, column;
};

class ScreenIOBuffer : public QObject, public AbstractInputBuffer, public AbstractOutputBuffer {
Q_OBJECT
public:
    void getUserInput(Command &command, int &row, int &column) override;

    void writeUserOutput(Command command, int row, int column) override;

private:
    QList<Event> eventQueue;

signals:

    void ready();
};


#endif //PLAYGWENT_SCREENIOBUFFER_H
