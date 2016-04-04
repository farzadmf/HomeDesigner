#pragma once

#include <QMainWindow>
#include <QLayout>

class HelpWindow : public QMainWindow
{
    QWidget* centralWidget;
    QGridLayout* layout;

public:
    HelpWindow();
    ~HelpWindow();
};

