#include "ImageView.hpp"

#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QImage>
#include <QPainter>
#include <QPixmap>
#include <QRectF>

ImageView::ImageView(QWidget* parent) : QGraphicsView(parent), scene(new QGraphicsScene(this)){
    setScene(scene);
    setAlignment(Qt::AlignCenter);
    setRenderHint(QPainter::SmoothPixmapTransform, true);
}

void ImageView::setImage(const QImage& image) {
    if (image.isNull()) {
        scene->clear();
        imageItem = nullptr;
        return;
    }

    const QPixmap pixMap = QPixmap::fromImage(image);

    if (imageItem == nullptr) {
        imageItem = scene->addPixmap(pixMap);
    }else {
        imageItem->setPixmap(pixMap);
    }

    imageItem->setPos(0.0, 0.0);

    scene->setSceneRect(imageItem->sceneBoundingRect());

    resetTransform();

    fitInView(imageItem, Qt::KeepAspectRatio);
}

void ImageView::clearImage() {
    scene->clear();
    imageItem = nullptr;
    resetTransform();
    scene->setSceneRect(QRectF{});
}


