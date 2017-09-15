#include "CardWidget.h"
#include "GameConstant.h"
#include <QPainter>
#include <QDebug>
#include <QPropertyAnimation>
#include <QGraphicsItem>
#include <QPen>
#include <QEventLoop>
#include <QGraphicsScene>
#include <QGraphicsItemAnimation>
#include <QTimeLine>
#include <QMap>

const QPixmap &CardWidget::getPixmap(const QString &path) {
    static QMap<QString, QPixmap> resourceMap;
    if (!resourceMap.contains(path)) {
        resourceMap.insert(path, QPixmap(path));
    }
    return resourceMap[path];
}

CardWidget::CardWidget(CardInfo *_cardinfo, QGraphicsItem *parent) :
        QGraphicsWidget(parent), cardinfo(_cardinfo) {
    //read the card front
    front = getPixmap("assets/card_front/" + cardinfo->cardName + ".png").scaledToWidth(266, Qt::SmoothTransformation);

    //read the card back and banner
    QString backFilePath = QString("assets/card_back/");
    QString bannerPath = QString("assets/card_frame/");

    static const QString factionBackPattern[2] = {"monsters.png", "neutral.png"};
    backFilePath += factionBackPattern[cardinfo->faction];

    static const QString factionBannerPattern[2] = {"monsters_", "neutral_"};
    bannerPath += factionBannerPattern[cardinfo->faction];
    static const QString typePattern[4] = {"bronze.png", "silver.png", "gold.png", "gold.png"};
    bannerPath += typePattern[cardinfo->type];

    back = getPixmap(backFilePath).scaledToWidth(266, Qt::SmoothTransformation);
    banner = getPixmap(bannerPath).scaledToWidth(370, Qt::SmoothTransformation);

    timerIcon = getPixmap("assets/card_icon/timer_icon.tex.png").scaledToWidth(60, Qt::SmoothTransformation);
    armorIcon = getPixmap("assets/card_icon/helmet_icon.tex.png").scaledToWidth(45, Qt::SmoothTransformation);
    divider = getPixmap("assets/card_icon/LegendaryBigCardPreviewDivider.tex.png").scaledToHeight(200,
                                                                                                  Qt::SmoothTransformation);

    QString rowPath = QString("assets/card_icon/icon_");
    static const QString rowPattern[4] = {"melee_", "ranged_", "siege_", "agile_"};
    rowPath += rowPattern[cardinfo->row];
    static const QString loyaltyPattern[2] = {"loyal.tex.png", "disloyal.tex.png"};
    rowPath += loyaltyPattern[cardinfo->loyalty];

    rowIcon = getPixmap(rowPath).scaledToWidth(60, Qt::SmoothTransformation);

    setWidth(200);
    face = 0;
    setRenderFlag(DrawFrame);
}

void CardWidget::paintFront(QPainter *truePainter, const QStyleOptionGraphicsItem *, QWidget *) {
    static QPixmap pixmap(400, 850);
    pixmap.fill(Qt::transparent);
    auto *painter = new QPainter(&pixmap);

    if (renderFlag & DrawTransparent) {
        painter->setOpacity(0.5);
    }

    painter->setRenderHint(QPainter::Antialiasing);
    painter->setRenderHint(QPainter::SmoothPixmapTransform);
    qreal hTrans = pixmap.width() - front.width() - 10;
    qreal vTrans = pixmap.height() - front.height() - 410;
    painter->translate(hTrans, vTrans);
    painter->drawPixmap(0, 0, front);// draw front image

    if ((renderFlag & DrawFrame) && !cardinfo->hasAttribute("ui")) {

        painter->drawPixmap(front.width() - banner.width(), front.height() - banner.height(), banner);//draw frame

        if (cardinfo->initialStrength > 0) {//should render strength
            //render current strength
            painter->setFont(QFont("Arial", 40, 100, false));

            QColor strenghColor = QColor(220, 220, 220); //grey
            if (cardinfo->currentStrength < cardinfo->initialStrength) {
                strenghColor = QColor(255, 0, 0); //red
            } else if (cardinfo->currentStrength > cardinfo->initialStrength) {
                strenghColor = QColor(0, 255, 0); //green
            }
            painter->setPen(strenghColor);

            painter->drawText(-15, 50, QString::number(cardinfo->currentStrength));

            //render row
            painter->drawPixmap(-30, 60, rowIcon);
        }

        if (cardinfo->armor > 0) {
            painter->setFont(QFont("Arial", 30, 100, false));
            painter->setPen(QColor(0, 0, 200));
            painter->drawPixmap(-24, 120, armorIcon);
            painter->drawText(-15, 160, QString::number(cardinfo->armor));
        }

        if (cardinfo->CD >= 0) {
            painter->setFont(QFont("Arial", 26, 100, false));
            painter->drawPixmap(-30, 185, timerIcon);
            painter->drawText(-12, 230, QString::number(cardinfo->CD));
        }

        if (cardinfo->shield) {
            painter->drawPixmap(-24, 210, armorIcon);
        }

    }

    if (renderFlag & DrawInfo) {
        painter->setFont(QFont("Arial", 20, 100, false));
        painter->setPen(QColor(209, 209, 208));
        painter->drawText(QRectF(0, front.height() + 10, front.width(), 84), Qt::AlignHCenter, cardinfo->cardName);
        painter->drawPixmap(0, front.height() + 10, divider.scaled(divider.width(), painter->fontMetrics().height()));

        qreal infoStartHeight = front.height() + 20 + painter->fontMetrics().height();
        painter->setFont(QFont("Arial", 15, 50, false));

        int totalWidth = painter->fontMetrics().width(cardinfo->infomation);
        int lines = (totalWidth - 1) / front.width() + 1;//ceil
        int realHeight = painter->fontMetrics().lineSpacing() * lines;

        painter->drawText(QRectF(20, infoStartHeight, front.width(), realHeight), Qt::TextWordWrap,
                          cardinfo->infomation);
        painter->drawPixmap(0, static_cast<int>(infoStartHeight), divider.scaled(divider.width(), realHeight));
    }

    delete painter;
    qreal scaleRatio = size().width() / front.width();

    truePainter->setRenderHint(QPainter::Antialiasing);
    truePainter->setRenderHint(QPainter::SmoothPixmapTransform);
    truePainter->drawPixmap(static_cast<int>(-hTrans * scaleRatio), static_cast<int>(-vTrans * scaleRatio),
                            pixmap.scaledToWidth(static_cast<int>(pixmap.width() * scaleRatio),
                                                 Qt::SmoothTransformation));

//    truePainter->setPen(Qt::black);
//    truePainter->drawRect(boundingRect());
}

void CardWidget::paintBack(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) {
    painter->drawPixmap(0, 0, back.scaledToWidth(static_cast<int>(size().width()), Qt::SmoothTransformation));
}

void CardWidget::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    if (face == 0) {
        paintFront(painter, option, widget);
    } else {
        paintBack(painter, option, widget);
    }
}

QRectF CardWidget::boundingRect() const {
//    return QGraphicsWidget::boundingRect();
    return {-size().width() * 0.5, -size().height() * 0.3, size().width() * 1.6, size().height() * 2.5};
}

void CardWidget::setWidth(qreal width) {
    resize(width, width / front.width() * front.height());
    defaultSize = size();
}

void CardWidget::setHeight(qreal height) {
    resize(height / front.height() * front.width(), height);
    defaultSize = size();
}

void CardWidget::expand() {
    qreal halfWidth = defaultSize.width() / 2;
    qreal halfHeight = defaultSize.height() / 2;
    halfWidth *= CardExpandAnimScale;
    halfHeight *= CardExpandAnimScale;
    QRectF nextGeometry(geometry().center() - QPointF(halfWidth, halfHeight),
                        geometry().center() + QPointF(halfWidth, halfHeight));
    QPropertyAnimation *expandAnim = new QPropertyAnimation(this, "geometry");
    expandAnim->setEndValue(nextGeometry);
    expandAnim->setDuration(CardExpandAnimDuration);
    expandAnim->start(QPropertyAnimation::DeleteWhenStopped);
}

void CardWidget::shrink() {
    qreal halfWidth = defaultSize.width() / 2;
    qreal halfHeight = defaultSize.height() / 2;
    QRectF nextGeometry(geometry().center() - QPointF(halfWidth, halfHeight),
                        geometry().center() + QPointF(halfWidth, halfHeight));
    QPropertyAnimation *expandAnim = new QPropertyAnimation(this, "geometry");
    expandAnim->setEndValue(nextGeometry);
    expandAnim->setDuration(CardExpandAnimDuration);
    expandAnim->start(QPropertyAnimation::DeleteWhenStopped);
}

int CardWidget::getRenderFlag() const {
    return renderFlag;
}

void CardWidget::setRenderFlag(int renderFlag) {
    CardWidget::renderFlag = renderFlag;
}

int CardWidget::getFace() const {
    return face;
}

void CardWidget::setFace(int face) {
    CardWidget::face = face;
    update();
}

void CardWidget::flip() {
    QGraphicsRotation rotation;
    rotation.setAxis(Qt::YAxis);
    rotation.setOrigin(QVector3D(static_cast<float>(size().width() / 2), static_cast<float>(size().height() / 2), 0));
    setTransformations({&rotation});

    QPropertyAnimation rotateAnimation(&rotation, "angle");
    rotateAnimation.setEndValue(90);
    rotateAnimation.setDuration(CardFlipHalfDuration);

    QEventLoop loop;
    connect(&rotateAnimation, &QPropertyAnimation::finished, &loop, &QEventLoop::quit);
    rotateAnimation.start();
    loop.exec();

    setFace(face ^ 1);
    rotation.setAngle(-90);

    rotateAnimation.setEndValue(0);
    rotateAnimation.start();
    loop.exec();

    setTransformations({});
}

bool CardWidget::isValidTarget() const {
    return validTarget;
}

void CardWidget::setValidTarget(bool validTarget) {
    CardWidget::validTarget = validTarget;
}

void CardWidget::spiritEnterScene() {
    spiritRotation = new QGraphicsRotation(this);
    spiritRotation->setAxis(Qt::YAxis);
    spiritRotation->setOrigin(
            QVector3D(static_cast<float>(size().width() + 30), 0, 0));//TODO toggle the value
    spiritRotation->setAngle(90);
    setTransformations({spiritRotation});
    QPropertyAnimation *animation = new QPropertyAnimation(spiritRotation, "angle");
    animation->setEndValue(0);
    animation->setDuration(CardFlipHalfDuration);
    animation->start(QPropertyAnimation::DeleteWhenStopped);
}

void CardWidget::spiritExitScene() {
    if (spiritRotation == nullptr) {
        return;
    }
    QPropertyAnimation *animation = new QPropertyAnimation(spiritRotation, "angle");
    animation->setEndValue(90);
    animation->setDuration(CardFlipHalfDuration);
    connect(animation, &QPropertyAnimation::finished, this, &QObject::deleteLater);
    animation->start(QPropertyAnimation::DeleteWhenStopped);
}

void CardWidget::rotateCDAnimation(int direction, int loopCount) {
    QGraphicsPixmapItem timerItem(timerIcon.scaledToWidth(80, Qt::SmoothTransformation), this);
    timerItem.setPos(size().width() / 2, size().height() / 2);
    QGraphicsItemAnimation animation;
    animation.setItem(&timerItem);
    QTimeLine timeLine(CardRotateCDDuration);
    animation.setTimeLine(&timeLine);
    for (int i = 0; i < 360; i++) {
        animation.setTranslationAt(i / 360., -timerItem.pixmap().size().width() / 2,
                                   -timerItem.pixmap().size().height() / 2);
        animation.setRotationAt(i / 360., i * direction);
    }
    timeLine.setLoopCount(loopCount);
    QEventLoop loop;
    connect(&timeLine, &QTimeLine::finished, &loop, &QEventLoop::quit);
    timeLine.start();
    loop.exec();
    update();
}

const QPixmap &CardWidget::getFront() const {
    return front;
}

void CardWidget::setFront(const QPixmap &front) {
    CardWidget::front = front;
}

const QPixmap &CardWidget::getBack() const {
    return back;
}

void CardWidget::setBack(const QPixmap &back) {
    CardWidget::back = back;
}

CardInfo *CardWidget::getCardinfo() const {
    return cardinfo;
}

bool CardWidget::isAnimated() const {
    return animated;
}

void CardWidget::setAnimated(bool animated) {
    CardWidget::animated = animated;
}

void CardWidget::unlockAnimated() {
    animated = false;
}

void CardWidget::lockAnimated() {
    animated = true;
}

