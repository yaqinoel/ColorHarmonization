#pragma once
#include <QLabel>
#include <QWidget>

class ImageView;
class HistogramView;

namespace ColorHarmonization::harmony {
    class HarmonyTemplate;
}

class ImagePanel final : public QWidget {
public:
    explicit ImagePanel(const QString& title, QWidget* parent = nullptr);

    void setImage(const QImage& image);
    void setHistogram(const QImage& image);
    void setTemplateOverlay(
        const ColorHarmonization::harmony::HarmonyTemplate& harmonyTemplate,
        float rotationDegrees
    );
    void clearTemplateOverlay();
    void clear();
    void setEmptyLabelText(const QString& imageText, const QString& histogramText);

private:
    QLabel* titleLabel = nullptr;
    ImageView* imageView = nullptr;
    HistogramView* histogramView = nullptr;
};
