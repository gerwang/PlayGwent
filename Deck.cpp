//
// Created by Gerwa on 2017/9/12.
//

#include <QtCore/QJsonDocument>
#include "Deck.h"
#include "GameConstant.h"

void Deck::fromJson(const QJsonObject &json) {
    leader = json["leader"].toString();
    QJsonArray jsonCardArray = json["cards"].toArray();
    for (int i = 0; i < jsonCardArray.size(); i++) {
        cards.append(jsonCardArray.at(i).toString());
    }
    name = json["name"].toString();
}

void Deck::toJson(QJsonObject &json) {
    json.insert("leader", leader);
    QJsonArray jsonCardArray;
    for (const auto &card:cards) {
        jsonCardArray.append(card);
    }
    json.insert("cards", jsonCardArray);
    json.insert("name", name);
}

CardInfo::Type Deck::getTypeByName(const QString &name) {
    CardInfo *cardInstance = CardInfo::createByName(name);
    CardInfo::Type result = cardInstance->getType();
    delete cardInstance;
    return result;
}

int Deck::parseDeckFromString(const QString &str) {
    QJsonParseError error{};
    QJsonDocument document = QJsonDocument::fromJson(str.toUtf8(), &error);
    if (error.error != QJsonParseError::NoError) {
        return 1;
    }

    QJsonObject object = document.object();

    if (!object.contains("leader") || !object["leader"].isString()) {
        return 2;
    }
    leader = object["leader"].toString();
    if (!LeaderNameList.contains(leader)) {
        return 2;
    }

    if (!object.contains("name") || !object["name"].isString()) {
        return 2;
    }
    name = object["name"].toString();

    if (!object.contains("cards") || !object["cards"].isArray()) {
        return 2;
    }
    cards.clear();
    QJsonArray cardArrays = object["cards"].toArray();
    for (int i = 0; i < cardArrays.size(); i++) {
        if (!cardArrays.at(i).isString()) {
            return 2;
        }
        QString name = cardArrays.at(i).toString();
        if (!CardNameList.contains(name)) {
            return 2;
        }
        cards.append(name);
    }

    return 0;
}

QList<QString> &Deck::getCards() {
    return cards;
}

void Deck::setCards(const QList<QString> &cards) {
    Deck::cards = cards;
}

const QString &Deck::getLeader() const {
    return leader;
}

void Deck::setLeader(const QString &leader) {
    Deck::leader = leader;
}

const QString &Deck::getName() const {
    return name;
}

void Deck::setName(const QString &name) {
    Deck::name = name;
}

QByteArray Deck::toJsonStr() {
    QJsonObject object;
    toJson(object);
    QJsonDocument document(object);
    return document.toJson();
}
