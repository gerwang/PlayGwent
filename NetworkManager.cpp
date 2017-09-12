//
// Created by Gerwa on 2017/9/12.
//

#include "NetworkManager.h"
#include <QMessageBox>

const QString NetworkManager::hostname = "127.0.0.1";
const int NetworkManager::port = 2333;

void NetworkManager::writeJsonObject(const QJsonObject &json) {
    QJsonDocument document(json);
    socket.write(document.toJson().replace('\n', ' ') + "\n");
}

QJsonObject NetworkManager::readJsonObject() {
    while (!socket.canReadLine()) {
        QEventLoop loop;
        connect(&socket, &QTcpSocket::readyRead, &loop, &QEventLoop::quit);
        loop.exec();
    }
    QByteArray line = socket.readLine();
    QJsonParseError error{};
    QJsonDocument document = QJsonDocument::fromJson(line, &error);
    if (error.error == QJsonParseError::NoError) {
        return document.object();
    } else {
        qWarning() << error.errorString();
    }
}

void NetworkManager::slotOnNetWorkErrorQuit() {
    QMessageBox::warning(nullptr, tr("Network Error"), socket.errorString());
    QCoreApplication::quit();
}

NetworkManager::NetworkManager() {
    connect(&socket, static_cast<void (QTcpSocket::*)(QAbstractSocket::SocketError)>(&QTcpSocket::error), this,
            &NetworkManager::slotOnNetWorkErrorQuit);
}
