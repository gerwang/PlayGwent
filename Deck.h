//
// Created by Gerwa on 2017/9/12.
//

#ifndef PLAYGWENT_DECK_H
#define PLAYGWENT_DECK_H

#include "cardinfo.h"
#include <QObject>
#include <QJsonObject>
#include <QJsonArray>
#include <QList>
#include <QString>
#include <QByteArray>

class Deck {
public:
    int parseDeckFromString(const QString &str);
    //return 0 if success, 1 if parse error, 2 if invalid deck

    void fromJson(const QJsonObject &json);

    void toJson(QJsonObject &json);

    static CardInfo::Type getTypeByName(const QString &name);

    QList<QString> &getCards();

    void setCards(const QList<QString> &cards);

    const QString &getLeader() const;

    void setLeader(const QString &leader);

    const QString &getName() const;

    void setName(const QString &name);

    QByteArray toJsonStr();

private:
    QList<QString> cards;
    QString leader;
    QString name;
};


#endif //PLAYGWENT_DECK_H
