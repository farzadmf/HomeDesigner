#include "HelpWindow.h"

HelpWindow::HelpWindow()
{
    setFixedSize(800, 600);

    centralWidget = new QWidget(this);
    layout = new QGridLayout(centralWidget);

    setCentralWidget(centralWidget);
}


HelpWindow::~HelpWindow()
{
}
