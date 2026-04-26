#pragma once
#include <QMainWindow>

class MainWindow : public QMainWindow {
    Q_OBJECT
    QWidget* root;
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
};
