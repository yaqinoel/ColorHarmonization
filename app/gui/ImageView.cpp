#include "ImageView.hpp"

#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QImage>
#include <QLabel>
#include <QPainter>
#include <QPixmap>
#include <QRectF>
#include <QResizeEvent>

ImageView::ImageView(QWidget* parent) : QGraphicsView(parent), scene(new QGraphicsScene(this)){
    setScene(scene);
    setAlignment(Qt::AlignCenter);
    setRenderHint(QPainter::SmoothPixmapTransform, true);

    emptyLabel = new QLabel("No image to display", this);
    emptyLabel->setAlignment(Qt::AlignCenter);
    emptyLabel->setWordWrap(true);
    emptyLabel->setAttribute(Qt::WA_TransparentForMouseEvents);
    emptyLabel->setGeometry(viewport()->geometry());
    emptyLabel->setStyleSheet(
        "QLabel {"
        "    color: #888888;"
        "    font-size: 16px;"
        "    background: transparent;"
        "}"
    );
}

void ImageView::setImage(const QImage& image) {
    if (image.isNull()) {
        clearImage();
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
    emptyLabel->hide();
    fitImage();
}

void ImageView::clearImage() {
    scene->clear();
    imageItem = nullptr;
    resetTransform();
    scene->setSceneRect(QRectF{});
    emptyLabel->show();
    emptyLabel->raise();
}

void ImageView::fitImage() {
    if (imageItem == nullptr) {
        return;
    }
    resetTransform();
    fitInView(imageItem, Qt::KeepAspectRatio);
}

void ImageView::resizeEvent(QResizeEvent* event) {
    QGraphicsView::resizeEvent(event);
    emptyLabel->setGeometry(viewport()->rect());
    fitImage();
}

void ImageView::setEmptyLabelText(const QString& text) {
    emptyLabel->setText(text);
}
