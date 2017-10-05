//
// Created by Gerwa on 2017/9/12.
//

#include "NetworkManager.h"
#include <QMessageBox>
#include <cstdlib>

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
    if (socket.error() != QAbstractSocket::RemoteHostClosedError) {
        std::exit(0);
    }
}

NetworkManager::NetworkManager() {
    connect(&socket, static_cast<void (QTcpSocket::*)(QAbstractSocket::SocketError)>(&QTcpSocket::error), this,
            &NetworkManager::slotOnNetWorkErrorQuit);
}

int NetworkManager::login(const QString &username, const QString &password) {
    socket.connectToHost(hostname, (quint16) port);
    QEventLoop loop;
    connect(&socket, &QTcpSocket::connected, &loop, &QEventLoop::quit);
    loop.exec();
    QJsonObject loginInfo;
    loginInfo.insert("username", username);
    loginInfo.insert("password", password);
    loginInfo.insert("version", GameVersion);
    writeJsonObject(loginInfo);
    QJsonObject replyInfo = readJsonObject();
    bool validation = replyInfo["validation"].toBool();
    if (!validation) {
        QString reason = replyInfo["reason"].toString();
        QMessageBox::warning(nullptr, tr("Login failed"), reason);
        socket.disconnectFromHost();
        return -1;
    } else {
        bool resume = replyInfo["resume"].toBool();
        return resume ? 1 : 0;
    }
}

void NetworkManager::logout() {
    socket.disconnectFromHost();
    socket.waitForDisconnected();
}

void NetworkManager::getUserInput(Command &command, int &row, int &column) {
    QJsonObject cmdObject = readJsonObject();
    if (cmdObject["command"].toString() != "transmit") {
        qWarning() << "not proper command" << cmdObject["command"].toString();
    }
    bool onlineStatus = cmdObject["online"].toBool();
    if (!onlineStatus) {
        command = Command::Offline;
        row = -1;
        column = -1;
    } else {
        command = static_cast<Command>(cmdObject["operation"].toInt());
        row = cmdObject["row"].toInt();
        column = cmdObject["column"].toInt();
    }
}

void NetworkManager::writeUserOutput(Command command, int row, int column) {
    QJsonObject cmdObject;
    cmdObject.insert("command", "transmit");
    cmdObject.insert("operation", static_cast<int>(command));
    cmdObject.insert("row", row);
    cmdObject.insert("column", column);
    writeJsonObject(cmdObject);
}
