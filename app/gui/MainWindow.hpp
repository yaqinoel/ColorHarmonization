#pragma once

#include <QMainWindow>
#include <QImage>

#include <color_harmonization/harmony/TemplateMatcher.hpp>

class ControlPanel;
class QAction;
class QSplitter;
class QWidget;
class ImagePanel;

using ColorHarmonization::harmony::TemplateMatcherOptions;
using ColorHarmonization::harmony::TemplateMatcher;
using ColorHarmonization::harmony::TemplateMatchResult;
using ColorHarmonization::harmony::HarmonyTemplate;

class MainWindow final : public QMainWindow {
public:
    explicit MainWindow(QWidget* parent = nullptr);

private:
    void createToolBar();
    void createCentralArea();
    void openImage();
    void saveResult();
    void resetResult();
    void startHarmonization();

    [[nodiscard]]
    QImage calculateHistogramImage(const QImage& image) const;

    QAction* openImageAction_ = nullptr;
    QAction* saveImageAction_ = nullptr;
    QAction* resetAction_ = nullptr;

    QSplitter* mainSplitter_ = nullptr;

    ImagePanel* originalPanel_ = nullptr;
    ImagePanel* resultPanel_ = nullptr;
    ControlPanel* controlPanel = nullptr;

    QImage originalImage;
    QImage originalHueHistogramImage;
    QImage resultImage;
    QImage resultHueHistogramImage;
};
