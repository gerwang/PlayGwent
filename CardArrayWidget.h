//
// Created by Gerwa on 2017/9/13.
//

#ifndef PLAYGWENT_CARDARRAYWIDGET_H
#define PLAYGWENT_CARDARRAYWIDGET_H

#include "CardWidget.h"
#include "GameConstant.h"
#include <QGraphicsWidget>

class CardArrayWidget : public QGraphicsWidget {
Q_OBJECT

public:

    static const QColor weatherColor[4];

    enum LayoutMode {
        FixedWidthAlignCenter,
        FixedSpacingRollable
    };

    CardArrayWidget();

    void spawnCardAt(int index, CardInfo *info, bool enableAnimation);

    void eraseCardFromGameAt(int index);

    QPointF makeSpaceForCardAt(int index, bool enableAnimation);//index=-1 if no room to make

    bool isClickValid() const;

    void setClickValid(bool clickValid);

    QList<CardWidget *> &getCards();

    void changeWeatherTo(Weather nextWeather);

    int getDefaultFace() const;

    void setDefaultFace(int defaultFace);

    void addCardAt(CardWidget *card, int index, bool enableAnimation);

    void removeCardAt(int index, bool enableAnimation);//erase the card, map it properly  to scene

    void wheelEvent(QGraphicsSceneWheelEvent *event) override;

    void performAddCard(CardWidget *card, int index);

    void setCardWidth(qreal cardWidth);

    LayoutMode getMode() const;

    void setMode(LayoutMode mode);

    void setFixedWidthMode(qreal fixedWidth, qreal maxSpacing, qreal cardWidth);

    void setRollableMode(qreal fixedSpacing, qreal rollerSmoothing, qreal cardWidth);

    bool isRenderInfo() const;

    void setRenderInfo(bool renderInfo);

    void clearWeather();

    void performSetWeather(Weather nextWeather);

    CardWidget *operator[](int column);
private:
    LayoutMode mode;

    QList<CardWidget *> cards;
    int defaultFace;
//0 front 1 back
    bool clickValid;
    Weather weather;

    QGraphicsRectItem weatherRect;

    qreal cardWidth;

    qreal fixedWidth;
    qreal maxSpacing;

    qreal fixedSpacing;
    qreal rollerSmoothing;

    bool renderInfo{};
};


#endif //PLAYGWENT_CARDARRAYWIDGET_H
