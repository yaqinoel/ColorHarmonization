#include "MainWindow.hpp"

#include <QAction>
#include <QFrame>
#include <QLabel>
#include <QSplitter>
#include <QStatusBar>
#include <QToolBar>
#include <QVBoxLayout>
#include <QWidget>

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

    controlPanel_ = createPlaceholderPanel("Controls",mainSplitter_);

    mainSplitter_->addWidget(originalPanel_);
    mainSplitter_->addWidget(resultPanel_);
    mainSplitter_->addWidget(controlPanel_);

    mainSplitter_->setStretchFactor(0, 4);
    mainSplitter_->setStretchFactor(1, 4);
    mainSplitter_->setStretchFactor(2, 2);

    mainSplitter_->setChildrenCollapsible(false);

    originalPanel_->setMinimumWidth(200);
    resultPanel_->setMinimumWidth(200);
    controlPanel_->setMinimumWidth(200);

    mainSplitter_->setSizes({500, 500, 300});

    setCentralWidget(mainSplitter_);
}
