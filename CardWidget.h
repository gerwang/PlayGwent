#ifndef SMALLCARDWIDGET_H
#define SMALLCARDWIDGET_H

#include "CardInfo.h"

#include <QGraphicsWidget>
#include <QPixmap>
#include <QStyleOptionGraphicsItem>
#include <QWidget>
#include <QPainter>
#include <QSizeF>
#include <QtCore/QPropertyAnimation>
#include <QPropertyAnimation>
#include <QGraphicsRotation>

class CardWidget : public QGraphicsWidget {
Q_OBJECT
public:

    enum Flags {
        DrawFrame = 1,
        DrawInfo = 2,
        DrawTransparent = 4
    };

    const QPixmap &getPixmap(const QString &path);

    CardWidget() = default;//from some ui item

    explicit CardWidget(CardInfo *_cardinfo, QGraphicsItem *parent = 0);

    void paintFront(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    void paintBack(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) override;

    QRectF boundingRect() const override;

    void setWidth(qreal width);

    void setHeight(qreal height);

    void expand();

    void shrink();

    void flip();//this function will block

    int getFace() const;

    void setFace(int face);

    int getRenderFlag() const;

    void setRenderFlag(int renderFlag);

    bool isValidTarget() const;

    void setValidTarget(bool validTarget);

    void spiritEnterScene(); //this function will NOT block

    void spiritExitScene();//this function will NOT block and will delete itself

    void rotateCDAnimation(int direction, int loopCount);//this function will block

    const QPixmap &getFront() const;

    void setFront(const QPixmap &front);

    const QPixmap &getBack() const;

    void setBack(const QPixmap &back);

    CardInfo *getCardinfo() const;

private:
    CardInfo *cardinfo{};
// the date structure stores the card infomation
    QPixmap banner; // the pixmap store the frame
    QPixmap front;
    // store picture of characters
    QPixmap back; // store the back of the card (already has a banner)
    QPixmap timerIcon; // CD icon
    QPixmap rowIcon;//which row and loyalty
    QPixmap armorIcon; //armor icon
    QPixmap divider;
    QSizeF defaultSize;
    int face{};
    int renderFlag{};
    bool validTarget{};
    QGraphicsRotation *spiritRotation{};
};

#endif // SMALLCARDWIDGET_H
