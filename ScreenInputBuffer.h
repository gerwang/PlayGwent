//
// Created by Gerwa on 2017/9/12.
//

#ifndef PLAYGWENT_SCREENINPUTBUFFER_H
#define PLAYGWENT_SCREENINPUTBUFFER_H

#include "AbstractIOBuffer.h"

class ScreenInputBuffer : public QObject, public AbstractIOBuffer {
Q_OBJECT
    //only input for logic will be here, input for look and feel can be reponded immediately

    void getUserInput(Command &command, int &row, int &column);

signals:

    void onComingInput();
};


#endif //PLAYGWENT_SCREENINPUTBUFFER_H
