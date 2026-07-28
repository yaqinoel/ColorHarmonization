#pragma once
#include <QWidget>

class QAction;
class QPushButton;
class QLabel;

class ControlPanel final : public QWidget {
    Q_OBJECT

public:
    explicit ControlPanel(QWidget* parent = nullptr);

signals:
    void startRequested();

private:
    QPushButton* startButton = nullptr;
};