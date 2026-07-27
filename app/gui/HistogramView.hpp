#pragma once
#include <QGraphicsView>

class QGraphicsScene;
class QGraphicsPixmapItem;
class QImage;
class QMouseEvent;
class QWheelEvent;
class QResizeEvent;
class QWidget;

class HistogramView final : public QGraphicsView {
public:
    explicit HistogramView(QWidget* parent = nullptr);
    void setHistogram(const QImage& histogram);
    void clearImage();
    void fitImage();

protected:
    void resizeEvent(QResizeEvent* event) override;

private:
    QGraphicsScene* scene = nullptr;
    QGraphicsPixmapItem* imageItem = nullptr;
};
