//
// Created by Gerwa on 2017/9/14.
//

#ifndef PLAYGWENT_ABSTRACTOUTPUTBUFFER_H
#define PLAYGWENT_ABSTRACTOUTPUTBUFFER_H

#endif //PLAYGWENT_ABSTRACTOUTPUTBUFFER_H

#include "GameConstant.h"

class AbstractOutputBuffer {
public:
    virtual void writeUserOutput(Command command, int row, int column)=0;
};