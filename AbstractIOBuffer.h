//
// Created by Gerwa on 2017/9/7.
//

#ifndef PLAYGWENT_ABSTRACTIOBUFFER_H
#define PLAYGWENT_ABSTRACTIOBUFFER_H


#include <QtCore/QObject>

class AbstractIOBuffer {
public:
    virtual void getUserInput(QString &command, int &row, int &column)=0;
};


#endif //PLAYGWENT_ABSTRACTIOBUFFER_H
