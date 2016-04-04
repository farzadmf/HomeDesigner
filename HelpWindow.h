#pragma once

#include <QMainWindow>
#include <QLayout>
#include <vector>

class HelpWindow : public QMainWindow
{
    QWidget* centralWidget;
    QGridLayout* layout;
//    std::map<int, QVBoxLayout*> columnLayouts;
    std::vector<QVBoxLayout*> columnLayouts;

    int verticalSpacing = 25;

    void AddToColumn(QGridLayout* gridLayout, int column);
public:
    HelpWindow();
    ~HelpWindow();
};

