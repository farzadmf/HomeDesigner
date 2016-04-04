#pragma once

#include <QGridLayout>

class HelpWindowGridLayout : public QGridLayout
{
    int row = 0;
    int horizontalSpacing = 20;

public:
    HelpWindowGridLayout();

    void AddHeading(QString headingText);
    void AddTitle(QString titleText);
    void AddDescription(QString descriptionText);
    void AddShortcut(QString shortcutText);
};

