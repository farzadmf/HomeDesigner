#pragma once

#include <QGridLayout>
///
/// \brief The HelpWindowGridLayout class sets default styling of a grid and allows the adding of heading ,titles,descriptions and shortcuts
///
class HelpWindowGridLayout : public QGridLayout
{
    int row = 0;
    int horizontalSpacing = 20;

public:
    ///
    /// \brief HelpWindowGridLayout Sets the styling of the grid
    ///
    HelpWindowGridLayout();

    void AddHeading(QString headingText);
    void AddTitle(QString titleText);
    void AddDescription(QString descriptionText);
    void AddShortcut(QString shortcutText);
};

