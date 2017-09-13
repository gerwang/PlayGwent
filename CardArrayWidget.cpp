//
// Created by Gerwa on 2017/9/13.
//

#include "CardArrayWidget.h"
#include <QParallelAnimationGroup>
#include <QPropertyAnimation>
#include <QEventLoop>
#include <QGraphicsItemAnimation>
#include <QTimeLine>
#include <QGraphicsSceneWheelEvent>
#include <QDebug>

const QColor CardArrayWidget::weatherColor[4] = {
        QColor(0, 0, 0, 0),//sunny
        QColor(0, 255, 0, 128),//Rainy
        QColor(255, 0, 0, 128),//Foggy todo toggle the value
        QColor(0, 0, 255, 128)//frozen
};


CardArrayWidget::CardArrayWidget() {
    weatherRect.setParentItem(this);
    weatherRect.setPen(QColor(0, 0, 0, 0));
    weatherRect.setBrush(weatherColor[0]);
}

void CardArrayWidget::spawnCardAt(int index, CardInfo *info, bool enableAnimation) {
    auto *widget = new CardWidget(info);
    widget->setFace(1);//default render back
    QPointF pos = makeSpaceForCardAt(index, enableAnimation);
    widget->setParentItem(this);
    widget->setPos(pos);
    performAddCard(widget, index);
    if (defaultFace != widget->getFace()) {
        if (enableAnimation) {
            widget->flip();
        } else {
            widget->setFace(defaultFace);
        }
    }
}

QPointF CardArrayWidget::makeSpaceForCardAt(int index, bool enableAnimation) {
    int cardSize = cards.size();
    if (index != -1) {
        cardSize++;
    }
    QPointF result{};
    if (mode == FixedWidthAlignCenter) {

        QList<QPointF> destinationPos;
        qreal cardDistance = 0, startOffset = 0;
        if (cardSize > 1) {
            if (cardSize * cardWidth <= fixedWidth) {//no need to overlap
                qreal spacing = 0;
                spacing = (fixedWidth - cardSize * cardWidth) / (cardSize - 1);
                spacing = std::min(spacing, maxSpacing);
                qreal totalWidth = cardSize * (cardWidth + spacing) - spacing;
                startOffset = (fixedWidth - totalWidth) / 2;//align center
                cardDistance = cardWidth + spacing;
            } else {
                startOffset = 0;
                cardDistance = (fixedWidth - cardWidth) / (cardSize - 1);
            }
        } else {
            startOffset = (fixedWidth - cardWidth) / 2;
        }
        qreal currentPos = startOffset;
        for (int i = 0; i < cardSize; i++) {
            destinationPos.append(QPointF(currentPos, 0));
            currentPos += cardDistance;
        }
        if (enableAnimation) {
            auto *group = new QParallelAnimationGroup;
            for (int newPos = 0; newPos < cardSize; newPos++) {
                if (newPos == index) {

                    result = destinationPos[newPos];

                } else {
                    int pos = newPos;
                    if (index != -1 && pos > index) {
                        pos--;
                    }
                    QPropertyAnimation *animation = new QPropertyAnimation(cards[pos], "pos", group);
                    animation->setEndValue(destinationPos[newPos]);
                    animation->setDuration(CardArrayMakePlaceDuration);
                    group->addAnimation(animation);
                }
            }
            group->start(QParallelAnimationGroup::DeleteWhenStopped);
        } else {
            for (int newPos = 0; newPos < cardSize; newPos++) {
                if (newPos == index) {

                    result = destinationPos[newPos];

                } else {
                    int pos = newPos;
                    if (index != -1 && pos > index) {
                        pos--;
                    }
                    cards[pos]->setPos(destinationPos[newPos]);
                }
            }
        }

        for (int newPos = 0; newPos < cardSize; newPos++) {
            if (newPos == index) {
                continue;
            } else {
                int pos = newPos;
                if (index != -1 && pos > index) {
                    pos--;
                }
                cards[pos]->setZValue(newPos);
            }
        }

    } else if (mode == FixedSpacingRollable) {
        QList<QPointF> destinationPos;

        if (enableAnimation) {
            auto *group = new QParallelAnimationGroup();
            int currentPos = 0;
            for (int newPos = 0; newPos < cardSize; newPos++) {
                if (newPos == index) {
                    result = QPointF(currentPos, 0);
                } else {
                    int pos = newPos;
                    if (index != -1 && pos > index) {
                        pos--;
                    }
                    QPropertyAnimation *animation = new QPropertyAnimation(cards[pos], "pos", group);
                    animation->setEndValue(QPointF(currentPos, 0));
                    animation->setDuration(CardArrayMakePlaceDuration);
                    group->addAnimation(animation);
                }
                currentPos += cardWidth + fixedSpacing;
            }
            resize(currentPos, cardWidth * 378 / 266);
            group->start(QParallelAnimationGroup::DeleteWhenStopped);
        } else {
            int currentPos = 0;
            for (int newPos = 0; newPos < cardSize; newPos++) {
                if (newPos == index) {
                    result = QPointF(currentPos, 0);
                } else {
                    int pos = newPos;
                    if (index != -1 && pos > index) {
                        pos--;
                    }
                    cards[pos]->setPos(currentPos, 0);
                }
                currentPos += cardWidth + fixedSpacing;
            }
            resize(currentPos, cardWidth * 378 / 266);
        }
    }
    return result;
}

bool CardArrayWidget::isClickValid() const {
    return clickValid;
}

void CardArrayWidget::setClickValid(bool clickValid) {
    CardArrayWidget::clickValid = clickValid;
}

QList<CardWidget *> &CardArrayWidget::getCards() {
    return cards;
}

void CardArrayWidget::changeWeatherTo(Weather nextWeather) {
    if (mode != FixedWidthAlignCenter) {
        qWarning() << "only battlefield can change weather";
        return;
    }
    weather = nextWeather;
    auto *timeLine = new QTimeLine(WeatherChangeDuration);
    auto *animation = new QGraphicsItemAnimation;
    animation->setItem(&weatherRect);
    animation->setTimeLine(timeLine);
    for (int i = 0; i < 10; i++) {
        animation->setScaleAt(i / 10.0, i / 10.0, 1);
    }
    QEventLoop loop;
    connect(timeLine, &QTimeLine::finished, &loop, &QEventLoop::quit);
    weatherRect.setBrush(weatherColor[nextWeather]);
    timeLine->start();
    loop.exec();
    delete animation;
    delete timeLine;
}

int CardArrayWidget::getDefaultFace() const {
    return defaultFace;
}

void CardArrayWidget::setDefaultFace(int defaultFace) {
    CardArrayWidget::defaultFace = defaultFace;
}

CardArrayWidget::LayoutMode CardArrayWidget::getMode() const {
    return mode;
}

void CardArrayWidget::setMode(CardArrayWidget::LayoutMode mode) {
    CardArrayWidget::mode = mode;
}

void CardArrayWidget::addCardAt(CardWidget *card, int index, bool enableAnimation) {
    //now the card is in the scene coordinate
    performAddCard(card, index);
    makeSpaceForCardAt(-1, enableAnimation);
}

void CardArrayWidget::removeCardAt(int index, bool enableAnimation) {
    CardWidget *card = cards.takeAt(index);
    card->setPos(mapToScene(card->pos()));
    card->setParentItem(nullptr);
    makeSpaceForCardAt(-1, enableAnimation);
}

void CardArrayWidget::eraseCardFromGameAt(int index) {
    CardWidget *widget = cards.takeAt(index);
    widget->deleteLater();
    makeSpaceForCardAt(-1, false);
}

void CardArrayWidget::setFixedWidthMode(qreal fixedWidth, qreal maxSpacing, qreal cardWidth) {
    mode = FixedWidthAlignCenter;
    this->fixedWidth = fixedWidth;
    this->maxSpacing = maxSpacing;
    setCardWidth(cardWidth);
    weatherRect.setRect(0, 0, fixedWidth, cardWidth * 378 / 266);
    resize(fixedWidth, cardWidth * 378 / 266);
}

void CardArrayWidget::setRollableMode(qreal fixedSpacing, qreal rollerSmoothing, qreal cardWidth) {
    mode = FixedSpacingRollable;
    this->fixedSpacing = fixedSpacing;
    this->rollerSmoothing = rollerSmoothing;
    setCardWidth(cardWidth);
}

void CardArrayWidget::setCardWidth(qreal cardWidth) {
    CardArrayWidget::cardWidth = cardWidth;
//    resize(cardWidth, cardWidth * 378 / 266);//TODO what does this code do?
}

void CardArrayWidget::wheelEvent(QGraphicsSceneWheelEvent *event) {
    if (mode != FixedSpacingRollable) {
        return;
    }
    qreal moveDelta = event->delta();
    if (moveDelta > 0) {
        moveDelta = std::min(moveDelta, rollerSmoothing - geometry().left());
        moveDelta = std::max(moveDelta, 0.0);
    } else {
        moveDelta = std::max(moveDelta, rollerSmoothing - geometry().right());
        moveDelta = std::min(moveDelta, 0.0);
    }
    QPropertyAnimation *animation = new QPropertyAnimation(this, "pos");
    animation->setEndValue(pos() + QPointF(moveDelta, 0));
    animation->setDuration(static_cast<int>(1 + fabs(moveDelta / MouseWheelSpeed)));
    animation->start(QPropertyAnimation::DeleteWhenStopped);
    QGraphicsItem::wheelEvent(event);
}

void CardArrayWidget::performAddCard(CardWidget *card, int index) {
    card->setWidth(cardWidth);
    card->setParentItem(this);
    if (renderInfo) {
        card->setRenderFlag(card->getRenderFlag() | CardWidget::DrawInfo);
    } else {
        card->setRenderFlag(card->getRenderFlag() & ~CardWidget::DrawInfo);
    }
    cards.insert(index, card);
}

bool CardArrayWidget::isRenderInfo() const {
    return renderInfo;
}

void CardArrayWidget::setRenderInfo(bool renderInfo) {
    CardArrayWidget::renderInfo = renderInfo;
}

void CardArrayWidget::clearWeather() {
    weather = Sunny;
    weatherRect.setBrush(weatherColor[Sunny]);
}

void CardArrayWidget::performSetWeather(Weather nextWeather) {
    weather = nextWeather;
    weatherRect.setBrush(weatherColor[nextWeather]);
}

CardWidget *CardArrayWidget::operator[](int column) {
    return cards[column];
}


