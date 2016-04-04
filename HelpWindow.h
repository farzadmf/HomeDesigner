#pragma once

#include <QMainWindow>
#include <QLayout>

class HelpWindow : public QMainWindow
{
    QWidget* centralWidget;
    QGridLayout* layout;

    int verticalSpacing = 25;

    void AddToColumn(QGridLayout* gridLayout, int column, int& row) const;
public:
    HelpWindow();
    ~HelpWindow();
};

