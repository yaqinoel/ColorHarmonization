#pragma once

#include <QMainWindow>

class QAction;
class QSplitter;
class QWidget;

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

    QWidget* originalPanel_ = nullptr;
    QWidget* resultPanel_ = nullptr;
    QWidget* controlPanel_ = nullptr;
};
