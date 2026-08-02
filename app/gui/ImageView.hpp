#pragma once

#include <QGraphicsView>

class QGraphicsScene;
class QGraphicsPixmapItem;
class QImage;
class QMouseEvent;
class QWheelEvent;
class QResizeEvent;
class QWidget;
class QLabel;

class ImageView final : public QGraphicsView {
public:
    explicit ImageView(QWidget* parent = nullptr);
    void setImage(const QImage &image);
    void clearImage();
    void fitImage();
    void setEmptyLabelText(const QString& text);

protected:
    void resizeEvent(QResizeEvent* event) override;

private:
    QGraphicsScene* scene = nullptr;
    QGraphicsPixmapItem* imageItem = nullptr;
    QLabel* emptyLabel = nullptr;
};