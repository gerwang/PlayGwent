//
// Created by Gerwa on 2017/9/12.
//

#ifndef PLAYGWENT_NETWORKMANAGER_H
#define PLAYGWENT_NETWORKMANAGER_H

#include <QObject>
#include <QtNetwork>
#include <QTcpSocket>
#include <QJsonObject>
#include <QJsonDocument>
#include "AbstractInputBuffer.h"
#include "AbstractOutputBuffer.h"

class NetworkManager : public QObject, public AbstractInputBuffer, public AbstractOutputBuffer {
Q_OBJECT
private:
    QTcpSocket socket;

public:

    const static QString hostname;
    const static int port;

    NetworkManager();

    void writeJsonObject(const QJsonObject &json);

    QJsonObject readJsonObject();

    int login(const QString &username, const QString &password);

    void logout();

    void getUserInput(Command &command, int &row, int &column) override;

    void writeUserOutput(Command command, int row, int column) override;

public slots:

    void slotOnNetWorkErrorQuit();
};


#endif //PLAYGWENT_NETWORKMANAGER_H
