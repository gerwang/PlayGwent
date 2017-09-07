#include "cardinfo.h"

#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QMetaEnum>
#include <QDir>
#include <QDebug>

CardInfo::CardInfo() {

}

void CardInfo::fromJsonFile(const QString &filePath) {
    QFile jsonFile;
    jsonFile.setFileName(filePath);
    jsonFile.open(QIODevice::ReadOnly | QIODevice::Text);
    QString jsonStr = jsonFile.readAll();
    jsonFile.close();
    QJsonParseError jsonErr;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonStr.toUtf8(), &jsonErr);
    if (jsonErr.error == QJsonParseError::NoError) {
        QJsonObject jsonObj = jsonDoc.object();
        readFromJson(jsonObj);
    } else {
        qDebug() << jsonStr << "Error!";
    }
}

void CardInfo::readFromJson(const QJsonObject &json) {

    //read card name
    cardName = json["cardName"].toString();

    //read attributes
    attributes.clear();
    QJsonArray attributeArray = json["attributes"].toArray();
    for (int i = 0; i < attributeArray.size(); i++) {
        attributes.push_back(attributeArray.at(i).toString());
    }

    //read infomation
    infomation = json["infomation"].toString();

    //read strength and armor
    initialStrength = json["initialStrength"].toInt();
    currentStrength = json["currentStrength"].toInt();
    armor = json["armor"].toInt();

    //read card type
    QMetaEnum cardTypeEnum = QMetaEnum::fromType<CardInfo::Type>();
    QString typeStr = json["type"].toString();
    type = static_cast<Type>(cardTypeEnum.keyToValue(typeStr.toUtf8()));

    //read faction
    QMetaEnum factionEnum = QMetaEnum::fromType<CardInfo::Faction>();
    QString factionStr = json["faction"].toString();
    faction = static_cast<Faction>(factionEnum.keyToValue(factionStr.toUtf8()));

    //read loyalty
    QMetaEnum loyaltyEnum = QMetaEnum::fromType<CardInfo::Loyalty>();
    QString loyaltyStr = json["loyalty"].toString();
    loyalty = static_cast<Loyalty>(loyaltyEnum.keyToValue(loyaltyStr.toUtf8()));

    //read row
    QMetaEnum rowEnum = QMetaEnum::fromType<CardInfo::Row>();
    QString rowStr = json["row"].toString();
    row = static_cast<Row>(rowEnum.keyToValue(rowStr.toUtf8()));

    //read CD
    CD = json["CD"].toInt();

    //read shield
    shield = json["shield"].toBool();
}


void CardInfo::writeToJson(QJsonObject &json) const {

    json.insert("cardName", cardName);

    //write attributes
    QJsonArray attributeArray;
    for (int i = 0; i < attributes.size(); i++) {
        attributeArray.append(attributes[i]);
    }
    json.insert("attributes", attributeArray);

    //write infomation
    json["infomation"] = infomation;

    //write strength and armor
    json.insert("initialStrength", initialStrength);
    json.insert("currentStrength", currentStrength);
    json.insert("armor", armor);

    //write card type
    QMetaEnum cardTypeEnum = QMetaEnum::fromType<CardInfo::Type>();
    json.insert("type", cardTypeEnum.valueToKey(type));

    //write faction
    QMetaEnum factionEnum = QMetaEnum::fromType<CardInfo::Faction>();
    json.insert("faction", factionEnum.valueToKey(faction));

    //write loyalty
    QMetaEnum loyaltyEnum = QMetaEnum::fromType<CardInfo::Loyalty>();
    json.insert("loyalty", loyaltyEnum.valueToKey(loyalty));

    //write row
    QMetaEnum rowEnum = QMetaEnum::fromType<CardInfo::Row>();
    json.insert("row", rowEnum.valueToKey(row));

    //write CD
    json.insert("CD", CD);

    //write shield
    json.insert("shield", shield);
}

const QString &CardInfo::getCardName() const {
    return cardName;
}

void CardInfo::setCardName(const QString &cardName) {
    CardInfo::cardName = cardName;
}

const QList<QString> &CardInfo::getAttributes() const {
    return attributes;
}

void CardInfo::setAttributes(const QList<QString> &attributes) {
    CardInfo::attributes = attributes;
}

const QString &CardInfo::getInfomation() const {
    return infomation;
}

void CardInfo::setInfomation(const QString &infomation) {
    CardInfo::infomation = infomation;
}

int CardInfo::getCurrentStrength() const {
    return currentStrength;
}

void CardInfo::setCurrentStrength(int currentStrength) {
    CardInfo::currentStrength = currentStrength;
}

int CardInfo::getInitialStrength() const {
    return initialStrength;
}

void CardInfo::setInitialStrength(int initialStrength) {
    CardInfo::initialStrength = initialStrength;
}

int CardInfo::getArmor() const {
    return armor;
}

void CardInfo::setArmor(int armor) {
    CardInfo::armor = armor;
}

CardInfo::Type CardInfo::getType() const {
    return type;
}

void CardInfo::setType(CardInfo::Type type) {
    CardInfo::type = type;
}

CardInfo::Faction CardInfo::getFaction() const {
    return faction;
}

void CardInfo::setFaction(CardInfo::Faction faction) {
    CardInfo::faction = faction;
}

CardInfo::Loyalty CardInfo::getLoyalty() const {
    return loyalty;
}

void CardInfo::setLoyalty(CardInfo::Loyalty loyalty) {
    CardInfo::loyalty = loyalty;
}

CardInfo::Row CardInfo::getRow() const {
    return row;
}

void CardInfo::setRow(CardInfo::Row row) {
    CardInfo::row = row;
}

bool CardInfo::isShield() const {
    return shield;
}

void CardInfo::setShield(bool shield) {
    CardInfo::shield = shield;
}

int CardInfo::getCD() const {
    return CD;
}

void CardInfo::setCD(int CD) {
    CardInfo::CD = CD;
}

