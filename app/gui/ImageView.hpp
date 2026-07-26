#pragma once

#include <QGraphicsView>

class QGraphicsScene;
class QGraphicsPixmapItem;
class QImage;
class QMouseEvent;
class QWheelEvent;
class QResizeEvent;
class QWidget;

class ImageView final : public QGraphicsView {
public:
    explicit ImageView(QWidget* parent = nullptr);
    void setImage(const QImage &image);
    void clearImage();

private:
    QGraphicsScene* scene = nullptr;
    QGraphicsPixmapItem* imageItem = nullptr;

};