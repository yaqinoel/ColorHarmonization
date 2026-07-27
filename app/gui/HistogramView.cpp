#include "HistogramView.hpp"

#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QImage>
#include <QPainter>
#include <QPixmap>
#include <QRectF>
#include <QResizeEvent>

HistogramView::HistogramView(QWidget *parent) : QGraphicsView(parent), scene(new QGraphicsScene(this)) {
    setScene(scene);
    setAlignment(Qt::AlignCenter);
    setRenderHint(QPainter::SmoothPixmapTransform, true);
}

void HistogramView::setHistogram(const QImage &image) {
    if (image.isNull()) {
        scene->clear();
        imageItem = nullptr;
        return;
    }

    const QPixmap pixmap = QPixmap::fromImage(image);

    if (imageItem == nullptr) {
        imageItem = scene->addPixmap(pixmap);
    } else {
        imageItem->setPixmap(pixmap);
    }

    imageItem->setPos(0.0, 0.0);
    scene->setSceneRect(imageItem->sceneBoundingRect());
    fitImage();
}

void HistogramView::clearImage() {
    scene->clear();
    imageItem = nullptr;
    resetTransform();
    scene->setSceneRect(QRectF{});
}

void HistogramView::fitImage() {
    if (imageItem == nullptr) {
        return;
    }
    resetTransform();
    fitInView(imageItem, Qt::KeepAspectRatio);
}

void HistogramView::resizeEvent(QResizeEvent* event) {
    QGraphicsView::resizeEvent(event);
    fitImage();
}

