#ifndef ZOOMGRAPHICSVIEW_H
#define ZOOMGRAPHICSVIEW_H

#include <QObject>
#include <QGraphicsView>
#include <QGraphicsScene>

class ZoomGraphicsView : public QGraphicsView {
Q_OBJECT
public:
    explicit ZoomGraphicsView(QWidget *parent = 0);

    void resizeEvent(QResizeEvent *event) override;
};

#endif // ZOOMGRAPHICSVIEW_H
