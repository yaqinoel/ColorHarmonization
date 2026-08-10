#include "ControlPanel.hpp"

#include <QCheckBox>
#include <QComboBox>
#include <QDoubleSpinBox>
#include <QFormLayout>
#include <QFrame>
#include <QGroupBox>
#include <QLabel>
#include <QPushButton>
#include <QScrollArea>
#include <QSignalBlocker>
#include <QSpinBox>
#include <QVBoxLayout>

ControlPanel::ControlPanel(QWidget* parent)
    : QWidget(parent) {
    createUi();
    connectControlSignals();
    updateControlStates();
}

void ControlPanel::createUi() {
    using namespace ColorHarmonization;

    auto* titleLabel = new QLabel("Algorithm Settings", this);
    titleLabel->setAlignment(Qt::AlignCenter);

    auto* settingsWidget = new QWidget();
    auto* settingsLayout = new QVBoxLayout(settingsWidget);

    auto* mappingGroup = new QGroupBox("Hue Mapping", settingsWidget);
    auto* mappingLayout = new QFormLayout(mappingGroup);
    mappingLayout->setRowWrapPolicy(QFormLayout::WrapAllRows);
    mappingLayout->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);

    mappingMethodComboBox = new QComboBox(mappingGroup);
    mappingMethodComboBox->addItem("Nearest center", static_cast<int>(MappingMethod::NearestCenter));
    mappingMethodComboBox->addItem("Linear", static_cast<int>(MappingMethod::Linear));
    mappingMethodComboBox->addItem("Gaussian", static_cast<int>(MappingMethod::Gaussian));
    mappingMethodComboBox->setCurrentIndex(
        mappingMethodComboBox->findData(static_cast<int>(MappingMethod::Gaussian))
    );

    gaussianSigmaScaleSpinBox = new QDoubleSpinBox(mappingGroup);
    gaussianSigmaScaleSpinBox->setRange(0.01, 5.0);
    gaussianSigmaScaleSpinBox->setDecimals(2);
    gaussianSigmaScaleSpinBox->setSingleStep(0.1);
    gaussianSigmaScaleSpinBox->setValue(0.5);

    mappingLayout->addRow("Method", mappingMethodComboBox);
    mappingLayout->addRow("Gaussian sigma scale", gaussianSigmaScaleSpinBox);

    auto* templateGroup = new QGroupBox("Harmony Template", settingsWidget);
    auto* templateLayout = new QFormLayout(templateGroup);
    templateLayout->setRowWrapPolicy(QFormLayout::WrapAllRows);
    templateLayout->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);

    autoTemplateCheckBox = new QCheckBox("Automatic template", templateGroup);
    autoTemplateCheckBox->setChecked(true);

    templateComboBox = new QComboBox(templateGroup);
    templateComboBox->addItem("i", static_cast<int>(harmony::HarmonyTemplateType::i));
    templateComboBox->addItem("V", static_cast<int>(harmony::HarmonyTemplateType::V));
    templateComboBox->addItem("L", static_cast<int>(harmony::HarmonyTemplateType::L));
    templateComboBox->addItem("L mirror", static_cast<int>(harmony::HarmonyTemplateType::L_MIRROR));
    templateComboBox->addItem("I", static_cast<int>(harmony::HarmonyTemplateType::I));
    templateComboBox->addItem("T", static_cast<int>(harmony::HarmonyTemplateType::T));
    templateComboBox->addItem("Y", static_cast<int>(harmony::HarmonyTemplateType::Y));
    templateComboBox->addItem("X", static_cast<int>(harmony::HarmonyTemplateType::X));
    templateComboBox->setCurrentIndex(
        templateComboBox->findData(static_cast<int>(harmony::HarmonyTemplateType::X))
    );

    autoRotationCheckBox = new QCheckBox("Automatic rotation", templateGroup);
    autoRotationCheckBox->setChecked(true);

    manualRotationSpinBox = new QDoubleSpinBox(templateGroup);
    manualRotationSpinBox->setRange(0.0, 359.9);
    manualRotationSpinBox->setDecimals(1);
    manualRotationSpinBox->setSingleStep(0.5);
    manualRotationSpinBox->setSuffix(" deg");

    rotationStepSpinBox = new QDoubleSpinBox(templateGroup);
    rotationStepSpinBox->setRange(0.1, 359.9);
    rotationStepSpinBox->setDecimals(1);
    rotationStepSpinBox->setSingleStep(0.5);
    rotationStepSpinBox->setValue(0.5);
    rotationStepSpinBox->setSuffix(" deg");

    templateLayout->addRow(autoTemplateCheckBox);
    templateLayout->addRow("Template", templateComboBox);
    templateLayout->addRow(autoRotationCheckBox);
    templateLayout->addRow("Manual rotation", manualRotationSpinBox);
    templateLayout->addRow("Search step", rotationStepSpinBox);

    auto* graphCutGroup = new QGroupBox("Graph Cut", settingsWidget);
    auto* graphCutLayout = new QFormLayout(graphCutGroup);
    graphCutLayout->setRowWrapPolicy(QFormLayout::WrapAllRows);
    graphCutLayout->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);

    graphCutCheckBox = new QCheckBox("Enable Graph Cut", graphCutGroup);
    graphCutCheckBox->setChecked(true);

    graphCutLambdaSpinBox = new QDoubleSpinBox(graphCutGroup);
    graphCutLambdaSpinBox->setRange(0.01, 100.0);
    graphCutLambdaSpinBox->setDecimals(2);
    graphCutLambdaSpinBox->setSingleStep(0.1);
    graphCutLambdaSpinBox->setValue(0.5);

    graphCutColorSigmaSpinBox = new QDoubleSpinBox(graphCutGroup);
    graphCutColorSigmaSpinBox->setRange(0.1, 180.0);
    graphCutColorSigmaSpinBox->setDecimals(1);
    graphCutColorSigmaSpinBox->setSingleStep(1.0);
    graphCutColorSigmaSpinBox->setValue(30.0);
    graphCutColorSigmaSpinBox->setSuffix(" deg");

    graphCutLayout->addRow(graphCutCheckBox);
    graphCutLayout->addRow("Smoothness lambda", graphCutLambdaSpinBox);
    graphCutLayout->addRow("Color sigma", graphCutColorSigmaSpinBox);

    auto* histogramGroup = new QGroupBox("Histogram", settingsWidget);
    auto* histogramLayout = new QFormLayout(histogramGroup);
    histogramLayout->setRowWrapPolicy(QFormLayout::WrapAllRows);
    histogramLayout->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);

    histogramBinCountSpinBox = new QSpinBox(histogramGroup);
    histogramBinCountSpinBox->setRange(12, 720);
    histogramBinCountSpinBox->setValue(360);

    saturationThresholdSpinBox = new QDoubleSpinBox(histogramGroup);
    saturationThresholdSpinBox->setRange(0.0, 1.0);
    saturationThresholdSpinBox->setDecimals(2);
    saturationThresholdSpinBox->setSingleStep(0.01);
    saturationThresholdSpinBox->setValue(0.01);

    normalizeHistogramCheckBox = new QCheckBox("Normalize histogram", histogramGroup);
    normalizeHistogramCheckBox->setChecked(true);

    histogramLayout->addRow("Bin count", histogramBinCountSpinBox);
    histogramLayout->addRow("Saturation threshold", saturationThresholdSpinBox);
    histogramLayout->addRow(normalizeHistogramCheckBox);

    settingsLayout->addWidget(mappingGroup);
    settingsLayout->addWidget(templateGroup);
    settingsLayout->addWidget(graphCutGroup);
    settingsLayout->addWidget(histogramGroup);
    settingsLayout->addStretch(1);

    auto* scrollArea = new QScrollArea(this);
    scrollArea->setWidget(settingsWidget);
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    startButton = new QPushButton("Start", this);
    startButton->setMinimumHeight(44);

    auto* mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(titleLabel);
    mainLayout->addWidget(scrollArea, 1);
    mainLayout->addWidget(startButton);
}

void ControlPanel::connectControlSignals() {
    connect(startButton, &QPushButton::clicked, this, &ControlPanel::startRequested);

    connect(mappingMethodComboBox, &QComboBox::currentIndexChanged, this, [this](int) {
        updateControlStates();
    });
    connect(autoTemplateCheckBox, &QCheckBox::toggled, this, [this](bool) {
        updateControlStates();
        emit templatePreviewChanged();
    });
    connect(autoRotationCheckBox, &QCheckBox::toggled, this, [this](bool) {
        updateControlStates();
        emit templatePreviewChanged();
    });
    connect(templateComboBox, &QComboBox::currentIndexChanged, this, [this](int) {
        emit templatePreviewChanged();
    });
    connect(manualRotationSpinBox, &QDoubleSpinBox::valueChanged, this, [this](double) {
        emit templatePreviewChanged();
    });
    connect(graphCutCheckBox, &QCheckBox::toggled, this, [this](bool) {
        updateControlStates();
    });
}

void ControlPanel::updateControlStates() {
    using namespace ColorHarmonization;

    const auto method = static_cast<MappingMethod>(
        mappingMethodComboBox->currentData().toInt()
    );
    gaussianSigmaScaleSpinBox->setEnabled(method == MappingMethod::Gaussian);

    const bool autoTemplate = autoTemplateCheckBox->isChecked();
    if (autoTemplate && !autoRotationCheckBox->isChecked()) {
        const QSignalBlocker blocker(autoRotationCheckBox);
        autoRotationCheckBox->setChecked(true);
    }

    templateComboBox->setEnabled(!autoTemplate);
    autoRotationCheckBox->setEnabled(!autoTemplate);

    const bool autoRotation = autoRotationCheckBox->isChecked();
    manualRotationSpinBox->setEnabled(!autoTemplate && !autoRotation);
    rotationStepSpinBox->setEnabled(autoRotation);

    const bool graphCutEnabled = graphCutCheckBox->isChecked();
    graphCutLambdaSpinBox->setEnabled(graphCutEnabled);
    graphCutColorSigmaSpinBox->setEnabled(graphCutEnabled);
}

ColorHarmonization::HarmonizationOptions
ControlPanel::harmonizationOptions() const {
    using namespace ColorHarmonization;

    HarmonizationOptions options;
    options.method = static_cast<MappingMethod>(
        mappingMethodComboBox->currentData().toInt()
    );
    options.gaussianSigmaScale = static_cast<float>(
        gaussianSigmaScaleSpinBox->value()
    );

    options.autoTemplate = autoTemplateCheckBox->isChecked();
    options.manualTemplateType = static_cast<harmony::HarmonyTemplateType>(
        templateComboBox->currentData().toInt()
    );
    options.autoRotation = autoRotationCheckBox->isChecked();
    options.manualRotation = static_cast<float>(manualRotationSpinBox->value());
    options.templateMatcherOptions.rotationStepDegrees = static_cast<float>(
        rotationStepSpinBox->value()
    );

    options.graphCutEnabled = graphCutCheckBox->isChecked();
    options.graphCutLambda = static_cast<float>(graphCutLambdaSpinBox->value());
    options.graphCutColorSigma = static_cast<float>(
        graphCutColorSigmaSpinBox->value()
    );

    options.hueHistogramOptions.binCount = static_cast<std::size_t>(
        histogramBinCountSpinBox->value()
    );
    options.hueHistogramOptions.saturationThreshold = static_cast<float>(
        saturationThresholdSpinBox->value()
    );
    options.hueHistogramOptions.normalize = normalizeHistogramCheckBox->isChecked();

    return options;
}
