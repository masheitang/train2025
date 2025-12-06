#pragma once

#include <QtWidgets/QMainWindow>
// 注意：这里的 ui_xxx.h 文件名需要与你的 .ui 文件名保持一致
#include "ui_mainwindow.h" 
#include "MainWindow.h"
#include <CustomerMainWindow.cpp>

class MainWindow : public QMainWindow // <--- 重点：类名改为 MainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow ui; // <--- 修复：将 MainWindowClass 改为 MainWindow，与 ui_mainwindow.h 中的类名一致
};