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

class NetworkManager : public QObject {
Q_OBJECT
private:
    QTcpSocket socket;

public:

    const static QString hostname;
    const static int port;

    NetworkManager();

    void writeJsonObject(const QJsonObject &json);

    QJsonObject readJsonObject();

public slots:

    void slotOnNetWorkErrorQuit();
};


#endif //PLAYGWENT_NETWORKMANAGER_H
