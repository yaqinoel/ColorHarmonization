#pragma once

#include <QWidget>
#include <color_harmonization/Harmonizer.hpp>

class QCheckBox;
class QComboBox;
class QDoubleSpinBox;
class QPushButton;
class QSpinBox;

class ControlPanel final : public QWidget {
    Q_OBJECT

public:
    explicit ControlPanel(QWidget* parent = nullptr);

    [[nodiscard]]
    ColorHarmonization::HarmonizationOptions harmonizationOptions() const;

signals:
    void startRequested();

private:
    void createUi();
    void connectControlSignals();
    void updateControlStates();

    QComboBox* mappingMethodComboBox = nullptr;
    QDoubleSpinBox* gaussianSigmaScaleSpinBox = nullptr;

    QCheckBox* autoTemplateCheckBox = nullptr;
    QComboBox* templateComboBox = nullptr;
    QCheckBox* autoRotationCheckBox = nullptr;
    QDoubleSpinBox* manualRotationSpinBox = nullptr;
    QDoubleSpinBox* rotationStepSpinBox = nullptr;

    QCheckBox* graphCutCheckBox = nullptr;
    QDoubleSpinBox* graphCutLambdaSpinBox = nullptr;
    QDoubleSpinBox* graphCutColorSigmaSpinBox = nullptr;

    QSpinBox* histogramBinCountSpinBox = nullptr;
    QDoubleSpinBox* saturationThresholdSpinBox = nullptr;
    QCheckBox* normalizeHistogramCheckBox = nullptr;

    QPushButton* startButton = nullptr;
};
