#include "HelpWindowGridLayout.h"
#include <QLabel>

HelpWindowGridLayout::HelpWindowGridLayout()
{
    setContentsMargins(20, 0, 20, 0);
}

void HelpWindowGridLayout::AddHeading(QString headingText)
{
    auto heading = new QLabel(headingText);
    heading->setObjectName("mainHeading");
    addWidget(heading, row++, 0, 1, 3);
}

void HelpWindowGridLayout::AddTitle(QString titleText)
{
    auto title = new QLabel(titleText);
    title->setObjectName("title");
    addWidget(title, row, 0);
    addItem(new QSpacerItem(horizontalSpacing, 0), row, 1);
}

void HelpWindowGridLayout::AddDescription(QString descriptionText)
{
    auto description = new QLabel(descriptionText);
    description->setObjectName("description");
    addWidget(description, row++, 2);
}

void HelpWindowGridLayout::AddShortcut(QString shortcutText)
{
    auto shortcut = new QLabel(shortcutText);
    shortcut->setObjectName("shortcut");
    addWidget(shortcut, row, 0);
    addItem(new QSpacerItem(horizontalSpacing, 0), row, 1);
}
