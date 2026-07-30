#pragma once

#include <QMainWindow>
#include <QImage>

#include "color_harmonization/harmony/HueHistogram.hpp"

class ControlPanel;
class QAction;
class QSplitter;
class QWidget;
class ImagePanel;

class MainWindow final : public QMainWindow {
public:
    explicit MainWindow(QWidget* parent = nullptr);
    void startHarmonization();

private:
    void createToolBar();
    void createCentralArea();
    void openImage();
    void saveResult();
    void resetResult();

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
    QImage origianlHueHistogramImage;
    QImage resultImage;
    QImage resultHueHistogramImage;
};
