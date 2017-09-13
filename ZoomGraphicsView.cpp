#include "ZoomGraphicsView.h"
#include <QtOpenGL>

ZoomGraphicsView::ZoomGraphicsView(QWidget *parent) : QGraphicsView(parent) {
    setViewport(new QGLWidget(QGLFormat(QGL::SampleBuffers)));
    setAlignment(Qt::AlignLeft | Qt::AlignTop);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setMouseTracking(true);
}

void ZoomGraphicsView::resizeEvent(QResizeEvent *event) {
    fitInView(sceneRect(), Qt::IgnoreAspectRatio);
    QGraphicsView::resizeEvent(event);
}