#pragma once

#include <QMainWindow>

class QAction;
class QSplitter;
class QWidget;
class ImagePanel;

class MainWindow final : public QMainWindow {
public:
    explicit MainWindow(QWidget* parent = nullptr);

private:
    void createToolBar();
    void createCentralArea();

    QAction* openImageAction_ = nullptr;
    QAction* saveImageAction_ = nullptr;
    QAction* resetAction_ = nullptr;

    QSplitter* mainSplitter_ = nullptr;

    ImagePanel* originalPanel_ = nullptr;
    ImagePanel* resultPanel_ = nullptr;
    QWidget* controlPanel_ = nullptr;
};
