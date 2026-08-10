#pragma once
#include <QGraphicsView>

#include "color_harmonization/harmony/HarmonyTemplate.hpp"

class QGraphicsScene;
class QGraphicsPixmapItem;
class QImage;
class QMouseEvent;
class QWheelEvent;
class QResizeEvent;
class QWidget;
class QLabel;
class QGraphicsItemGroup;

class HistogramView final : public QGraphicsView {
public:
    explicit HistogramView(QWidget* parent = nullptr);
    void setHistogram(const QImage& histogram);
    void clearImage();
    void fitImage();
    void setEmptyLabelText(const QString& text);
    void setTemplateOverlay(const ColorHarmonization::harmony::HarmonyTemplate& harmonyTemplate, float rotationDegrees);
    void clearTemplateOverlay();

protected:
    void resizeEvent(QResizeEvent* event) override;

private:
    QGraphicsScene* scene = nullptr;
    QGraphicsPixmapItem* imageItem = nullptr;
    QLabel* emptyLabel = nullptr;
    QGraphicsItemGroup* templateOverlayGroup = nullptr;
};
