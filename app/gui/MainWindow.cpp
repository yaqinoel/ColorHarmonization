#include "MainWindow.hpp"

#include <QLabel>
#include <QVBoxLayout>
#include <QWidget>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent) {
    setWindowTitle("Color Harmonization");
    resize(1000, 700);

    auto* centralWidget = new QWidget(this);
    auto* layout = new QVBoxLayout(centralWidget);

    auto* titleLabel = new QLabel("Color Harmonization", centralWidget);
    titleLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(titleLabel);

    setCentralWidget(centralWidget);
}
