#include "HistogramView.hpp"

#include <algorithm>

#include <QBrush>
#include <QColor>
#include <QGraphicsItemGroup>
#include <QGraphicsPathItem>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QImage>
#include <QPainter>
#include <QPainterPath>
#include <QPen>
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
    clearTemplateOverlay();
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

void HistogramView::setTemplateOverlay(const ColorHarmonization::harmony::HarmonyTemplate& harmonyTemplate, float rotationDegrees) {
    clearTemplateOverlay();
    if (imageItem == nullptr || harmonyTemplate.sectors().empty()) {
        return;
    }
    const QRectF histogramRect = imageItem->boundingRect();
    const qreal wheelSize = std::min(histogramRect.width(),histogramRect.height());
    const QPointF center = histogramRect.center();
    const qreal innerRadius = wheelSize * 4.0 / 12.0;
    const QRectF innerCircleRect(center.x() - innerRadius,center.y() - innerRadius,innerRadius * 2.0,innerRadius * 2.0);
    templateOverlayGroup = new QGraphicsItemGroup();
    scene->addItem(templateOverlayGroup);
    templateOverlayGroup->setZValue(1.0);
    const QBrush overlayBrush(QColor(100, 100, 100, 90));
    const QPen overlayPen(QColor(70, 70, 70, 190),2.0);

    for (const auto& sector : harmonyTemplate.sectors()) {
        if (sector.width <= 0.0F) {
            continue;
        }
        const float centerHue = ColorHarmonization::harmony::normalizeHue(sector.center + rotationDegrees);
        const float startHue = centerHue - sector.width / 2.0F;
        const qreal qtStartAngle = -static_cast<qreal>(startHue);
        const qreal qtSweepAngle = -static_cast<qreal>(sector.width);

        QPainterPath sectorPath;
        sectorPath.moveTo(center);
        sectorPath.arcTo(innerCircleRect,qtStartAngle,qtSweepAngle);
        sectorPath.closeSubpath();

        auto* sectorItem = new QGraphicsPathItem(sectorPath,templateOverlayGroup);
        sectorItem->setBrush(overlayBrush);
        sectorItem->setPen(overlayPen);
    }
}

void HistogramView::clearTemplateOverlay() {
    if (templateOverlayGroup == nullptr) {
        return;
    }
    scene->removeItem(templateOverlayGroup);
    delete templateOverlayGroup;
    templateOverlayGroup = nullptr;
}
