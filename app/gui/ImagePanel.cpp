#include "ImagePanel.hpp"
#include "ImageView.hpp"

#include <QFrame>
#include <QImage>
#include <QLabel>
#include <QPixmap>
#include <QSplitter>
#include <QString>
#include <QVBoxLayout>

ImagePanel::ImagePanel(const QString& title, QWidget* parent) : QWidget(parent) {
    titleLabel = new QLabel(title, this);
    titleLabel->setAlignment(Qt::AlignCenter);

    imageView = new ImageView(this);

    histogramLabel = new QLabel("Histogram",this);
    histogramLabel->setAlignment(Qt::AlignCenter);
    histogramLabel->setFrameStyle(QFrame::StyledPanel);
    histogramLabel->setMinimumHeight(160);

    auto* contentSplitter = new QSplitter(Qt::Vertical,this);
    contentSplitter->addWidget(imageView);
    contentSplitter->addWidget(histogramLabel);

    contentSplitter->setStretchFactor(0, 3);
    contentSplitter->setStretchFactor(1, 1);
    contentSplitter->setChildrenCollapsible(false);

    auto* layout = new QVBoxLayout(this);
    layout->addWidget(titleLabel);
    layout->addWidget(contentSplitter);
}

void ImagePanel::setImage(const QImage& image) {
    imageView->setImage(image);
}

void ImagePanel::setHistogramImage(const QImage& image) {
    if (image.isNull()) {
        histogramLabel->clear();
        histogramLabel->setText("Histogram");
        return;
    }

    const QPixmap pixMap = QPixmap::fromImage(image);
    histogramLabel->setPixmap(pixMap.scaled(
        histogramLabel->size(),
        Qt::KeepAspectRatio,
        Qt::SmoothTransformation
        ));

}

void ImagePanel::clear() {
    imageView->clearImage();
    histogramLabel->clear();
    histogramLabel->setText("Histogram");
}