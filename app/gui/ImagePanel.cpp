#include "ImagePanel.hpp"
#include "ImageView.hpp"
#include "HistogramView.hpp"

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
    histogramView = new HistogramView(this);

    auto* contentSplitter = new QSplitter(Qt::Vertical,this);
    contentSplitter->addWidget(imageView);
    contentSplitter->addWidget(histogramView);

    contentSplitter->setStretchFactor(0, 1);
    contentSplitter->setStretchFactor(1, 1);
    contentSplitter->setChildrenCollapsible(false);

    auto* layout = new QVBoxLayout(this);
    layout->addWidget(titleLabel);
    layout->addWidget(contentSplitter);
}

void ImagePanel::setImage(const QImage& image) {
    imageView->setImage(image);
}

void ImagePanel::setHistogram(const QImage& image) {
    histogramView->setHistogram(image);
}

void ImagePanel::clear() {
    imageView->clearImage();
    histogramView->clearImage();
}

void ImagePanel::setEmptyLabelText(const QString& imageText, const QString& histogramText) {
    imageView->setEmptyLabelText(imageText);
    histogramView->setEmptyLabelText(histogramText);
}