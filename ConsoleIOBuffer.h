//
// Created by Gerwa on 2017/9/7.
//

#ifndef PLAYGWENT_CONSOLEIOBUFFER_H
#define PLAYGWENT_CONSOLEIOBUFFER_H

#include "AbstractIOBuffer.h"
#include <QObject>
#include <QPushButton>
#include <QLineEdit>

class ConsoleIOBuffer : public QObject, public AbstractIOBuffer {
Q_OBJECT
public:
    ConsoleIOBuffer(QPushButton *button, QLineEdit *edit) : button(button), edit(edit) {}

    void getUserInput(QString &command, int &row, int &column) override;

private:
    QPushButton *button;
    QLineEdit *edit;
};


#endif //PLAYGWENT_CONSOLEIOBUFFER_H
