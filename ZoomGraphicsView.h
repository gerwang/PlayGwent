#ifndef ZOOMGRAPHICSVIEW_H
#define ZOOMGRAPHICSVIEW_H

#include <QObject>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QCloseEvent>

class ZoomGraphicsView : public QGraphicsView {
Q_OBJECT
public:
    explicit ZoomGraphicsView(QWidget *parent = 0);

    void resizeEvent(QResizeEvent *event) override;

    void closeEvent(QCloseEvent *event) override;
};

#endif // ZOOMGRAPHICSVIEW_H
