#ifndef CARD_H
#define CARD_H

#include <QString>
#include <QList>
#include <QObject>
#include <QJsonObject>

class CardInfo : public QObject {
Q_OBJECT
public:
    CardInfo();

    void fromJsonFile(const QString &fileName);

    void readFromJson(const QJsonObject &json);

    static CardInfo *createByName(const QString &name);

    void writeToJson(QJsonObject &json) const;

    enum Type {
        Bronze, Silver, Gold, Leader
    };
    //card type
    Q_ENUM(Type)

    enum Faction {
        Monsters, Neutral
    };

    Q_ENUM(Faction)

    enum Loyalty {
        Loyal,
        Disloyal
    };
    //loyalty
    Q_ENUM(Loyalty)

    enum Row {
        Melee, Ranged, Siege, Event
    };

    Q_ENUM(Row)

    const QString &getCardName() const;

    void setCardName(const QString &cardName);

    const QList<QString> &getAttributes() const;

    void setAttributes(const QList<QString> &attributes);

    const QString &getInfomation() const;

    void setInfomation(const QString &infomation);

    int getCurrentStrength() const;

    void setCurrentStrength(int currentStrength);

    int getInitialStrength() const;

    void setInitialStrength(int initialStrength);

    int getArmor() const;

    void setArmor(int armor);

    Type getType() const;

    void setType(Type type);

    Faction getFaction() const;

    void setFaction(Faction faction);

    Loyalty getLoyalty() const;

    void setLoyalty(Loyalty loyalty);

    Row getRow() const;

    void setRow(Row row);

    bool isShield() const;

    void setShield(bool shield);

    int getCD() const;

    void setCD(int CD);

    bool hasAttribute(const QString &name);

    void resetAttributes();

private:
    QString cardName;
    QList<QString> attributes;
    QString infomation;
    //strength
    int currentStrength, initialStrength;
    int armor;
    Type type;
    Faction faction;
    Loyalty loyalty;
    Row row;
    bool shield;// the shield
    int CD;// the time to trigger an ability, -1 if no ability

    friend class CardWidget;
};

#endif // CARD_H
