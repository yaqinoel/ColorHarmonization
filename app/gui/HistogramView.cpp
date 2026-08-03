#include "HistogramView.hpp"

#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QImage>
#include <QPainter>
#include <QPixmap>
#include <QRectF>
#include <QResizeEvent>
#include <QLabel>

HistogramView::HistogramView(QWidget *parent) : QGraphicsView(parent), scene(new QGraphicsScene(this)) {
    setScene(scene);
    setAlignment(Qt::AlignCenter);
    setRenderHint(QPainter::SmoothPixmapTransform, true);

    emptyLabel = new QLabel("No histogram to display", this);
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

void HistogramView::setHistogram(const QImage &image) {
    if (image.isNull()) {
        clearImage();
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
    emptyLabel->hide();
    fitImage();
}

void HistogramView::clearImage() {
    scene->clear();
    imageItem = nullptr;
    resetTransform();
    scene->setSceneRect(QRectF{});
    emptyLabel->show();
    emptyLabel->raise();
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
    emptyLabel->setGeometry(viewport()->rect());
    fitImage();
}

void HistogramView::setEmptyLabelText(const QString& text) {
    emptyLabel->setText(text);
}

