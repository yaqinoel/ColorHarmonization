#include "MainWindow.hpp"

#include <QAction>
#include <QSplitter>
#include <QStatusBar>
#include <QToolBar>
#include <QVBoxLayout>
#include <QWidget>
#include <QFileDialog>
#include <QImage>
#include <QMessageBox>

#include "ControlPanel.hpp"
#include "ImagePanel.hpp"
#include "ImageView.hpp"

#include <color_harmonization/image/ImageIO.hpp>
#include <color_harmonization/image/Image.hpp>
#include <color_harmonization/image/Pixel.hpp>

#include "QtImageConverter.hpp"
#include "color_harmonization/color/ColorConversion.hpp"
#include "color_harmonization/visualisation/HueHistogramRender.hpp"

namespace {
    QWidget* createPlaceholderPanel(const QString& title, QWidget* parent) {
        auto* panel = new QFrame(parent);
        panel->setFrameShape(QFrame::StyledPanel);

        auto* layout = new QVBoxLayout(panel);

        auto* label = new QLabel(title, panel);
        label->setAlignment(Qt::AlignCenter);

        layout->addWidget(label);

        return panel;
    }
}

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent) {
    setWindowTitle("Color Harmonization");
    resize(1000, 700);

    createToolBar();
    createCentralArea();

    statusBar()->showMessage("Ready");
}

void MainWindow::createToolBar() {
    auto* toolBar = addToolBar("Main toolbar");

    toolBar->setMovable(false);

    openImageAction_ = toolBar->addAction("Open image");
    saveImageAction_ = toolBar->addAction("Save image");
    resetAction_ = toolBar->addAction("Reset");

    saveImageAction_->setEnabled(false);
    resetAction_->setEnabled(false);

    connect(
        openImageAction_,
        &QAction::triggered,
        this,
        &MainWindow::openImage
    );

    connect(
        saveImageAction_,
        &QAction::triggered,
        this,
        &MainWindow::saveResult
    );

    connect(
        resetAction_,
        &QAction::triggered,
        this,
        &MainWindow::resetResult
    );
}

void MainWindow::createCentralArea() {
    mainSplitter_ = new QSplitter(Qt::Horizontal,this);

    originalPanel_ = new ImagePanel("Original Image",mainSplitter_);
    resultPanel_ = new ImagePanel("Harmonized Result",mainSplitter_);
    controlPanel = new ControlPanel(mainSplitter_);

    originalPanel_->setEmptyLabelText("No image selected\nOpen an image from the toolbar", "Select an image to see the histogram");
    resultPanel_->setEmptyLabelText("Run harmonization to see the result", "Run harmonization to see the result");

    mainSplitter_->addWidget(originalPanel_);
    mainSplitter_->addWidget(resultPanel_);
    mainSplitter_->addWidget(controlPanel);

    mainSplitter_->setStretchFactor(0, 4);
    mainSplitter_->setStretchFactor(1, 4);
    mainSplitter_->setStretchFactor(2, 2);

    mainSplitter_->setChildrenCollapsible(false);

    originalPanel_->setMinimumWidth(200);
    resultPanel_->setMinimumWidth(200);
    controlPanel->setMinimumWidth(200);

    mainSplitter_->setSizes({500, 500, 300});

    setCentralWidget(mainSplitter_);

    connect(
        controlPanel,
        &ControlPanel::startRequested,
        this,
        &MainWindow::startHarmonization
    );
}

void MainWindow::openImage() {
    const QString filePath = QFileDialog::getOpenFileName(
        this,
        "Open Image",
        {},
        "Images (*.png *.jpg *.jpeg)"
        );

    if (filePath.isEmpty()) {
        return;
    }

    QImage loadedImage(filePath);

    if (loadedImage.isNull()) {
        QMessageBox::warning(
            this,
            "Open Image",
            "Failed to load the selected image."
        );
        return;
    }

    try {
        QImage histogramImage = calculateHistogramImage(loadedImage);
        originalHueHistogramImage = histogramImage;
        originalImage = loadedImage;

        originalPanel_->setImage(originalImage);
        originalPanel_->setHistogram(originalHueHistogramImage);
    }catch (const std::exception& exception) {
        QMessageBox::critical(this, "Processing failed", exception.what());
    }

    statusBar()->showMessage(
        QString("Loaded image %1 × %2")
            .arg(originalImage.width())
            .arg(originalImage.height())
    );

    // clean old result
    resultImage = {};
    resultHueHistogramImage = {};
    resultPanel_->clear();
}

void MainWindow::saveResult() {
    statusBar()->showMessage(QString("Saved result"));
}

void MainWindow::resetResult() {
    statusBar()->showMessage(QString("Reseted result"));
}

void MainWindow::startHarmonization() {
    statusBar()->showMessage(QString("Starting Harmonization"));
}

QImage MainWindow::calculateHistogramImage(const QImage& image) const{
    ColorHarmonization::harmony::HueHistogramOptions options;
    options.binCount = 360;
    options.normalize = true;
    options.saturationThreshold = 0.01f;

    ColorHarmonization::Image<ColorHarmonization::Rgb8> rgbImage = ColorHarmonization::gui::fromQtImage(image);

    auto imageHsv = ColorHarmonization::color::rgbToHsv(rgbImage);
    const auto histogram = ColorHarmonization::harmony::HueHistogram::fromImage(imageHsv, options);
    ColorHarmonization::Image<ColorHarmonization::Rgb8> imageHueHistogram = ColorHarmonization::visualisation::renderHueHistogramWheel(histogram, 1024);

    return ColorHarmonization::gui::toQtImage(imageHueHistogram);
}