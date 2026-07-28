#include "MainWindow.hpp"

#include <QAction>
#include <QFrame>
#include <QLabel>
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

    QImage placeHolderImage ("assets/input/starry_night.jpg");

    originalPanel_ = new ImagePanel("Original Image",mainSplitter_);
    originalPanel_->setImage(placeHolderImage);
    originalPanel_->setHistogram(placeHolderImage);

    resultPanel_ = new ImagePanel("Harmonized Result",mainSplitter_);
    resultPanel_->setImage(placeHolderImage);
    resultPanel_->setHistogram(placeHolderImage);

    controlPanel = new ControlPanel(mainSplitter_);

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

    originalImage = loadedImage;
    originalPanel_->setImage(originalImage);
    originalPanel_->setHistogram(originalImage);

    statusBar()->showMessage(
        QString("Loaded image %1 × %2")
            .arg(originalImage.width())
            .arg(originalImage.height())
    );
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