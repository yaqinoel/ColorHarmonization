#pragma once
#include <QLabel>
#include <QWidget>

class ImageView;

class ImagePanel final : public QWidget {
public:
    explicit ImagePanel(const QString& title, QWidget* parent = nullptr);

    void setImage(const QImage& image);
    void setHistogramImage(const QImage& image);
    void clear();

private:
    QLabel* titleLabel = nullptr;
    ImageView* imageView = nullptr;
    QLabel* histogramLabel= nullptr;
};
