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

void ZoomGraphicsView::closeEvent(QCloseEvent *event) {
    QMessageBox::StandardButton resBtn = QMessageBox::question(this, "PlayGwent",
                                                               tr("Are you sure?\n"),
                                                               QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes,
                                                               QMessageBox::Yes);
    if (resBtn != QMessageBox::Yes) {
        event->ignore();
    } else {
        event->accept();
        exit(0);
    }
}