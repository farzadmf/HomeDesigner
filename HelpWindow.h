#pragma once

#include <QMainWindow>
#include <QLayout>
#include <vector>
///
/// \brief The HelpWindow class 
///  Another MainWindow that displays all the help keyboard shorcuts
///
class HelpWindow : public QMainWindow
{
    QWidget* centralWidget;
    QGridLayout* layout;
//    std::map<int, QVBoxLayout*> columnLayouts;
    std::vector<QVBoxLayout*> columnLayouts;

    int verticalSpacing = 25;

    void AddToColumn(QGridLayout* gridLayout, int column);
public:
    ///
    /// \brief HelpWindow sets the size and loads stylesheets for styling the window then adds all the widgets to the window
    ///
    HelpWindow();
    ~HelpWindow();
};

