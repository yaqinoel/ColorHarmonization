#include "ControlPanel.hpp"

#include <QVBoxLayout>
#include <QLabel>
#include <QScrollArea>
#include <QPushButton>
#include <QFrame>

ControlPanel::ControlPanel(QWidget *parent) : QWidget(parent) {
    auto* titleLabel = new QLabel("Algorithm Settings", this);
    titleLabel->setAlignment(Qt::AlignCenter);

    auto* settingsWidget = new QWidget();

    auto* settingsLayout = new QVBoxLayout(settingsWidget);
    auto* placeHolderLabel = new QLabel("settings",settingsWidget);
    placeHolderLabel->setAlignment(Qt::AlignCenter);
    settingsLayout->addWidget(placeHolderLabel);
    settingsLayout->addStretch(1);

    auto* scrollArea = new QScrollArea(this);
    scrollArea->setWidget(settingsWidget);
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);

    startButton = new QPushButton("Start", this);
    startButton->setMinimumHeight(44);
    // startButton->setEnabled(false);
    connect(
        startButton,
        &QPushButton::clicked,
        this,
        &ControlPanel::startRequested
    );

    auto* mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(titleLabel);
    mainLayout->addWidget(scrollArea, 1);
    mainLayout->addWidget(startButton, 0);
}

void ControlPanel::setEnabled(bool enabled) {
    startButton->setEnabled(enabled);
}