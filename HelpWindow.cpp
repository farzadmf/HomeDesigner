#include "HelpWindow.h"
#include "qlabel.h"
#include <QFile>
#include <QFontDatabase>
#include <HelpWindowGridLayout.h>

#include <iostream>
using std::cout;
using std::endl;

void HelpWindow::AddToColumn(QGridLayout* gridLayout, int column)
{
    if (columnLayouts.size() < column + 1)
    {
        auto vLayout = new QVBoxLayout();
        vLayout->setAlignment(Qt::AlignTop);
        columnLayouts.push_back(vLayout);
        layout->addLayout(columnLayouts[column], 0, column);
    }

    columnLayouts[column]->addLayout(gridLayout);
    columnLayouts[column]->addSpacerItem(new QSpacerItem(0, verticalSpacing));
}

HelpWindow::HelpWindow()
{
    setFixedSize(1300, 900);
    QFile stylesheetFile(":/styles/HelpWindow.qss");

    if (stylesheetFile.open(QIODevice::ReadOnly))
    {
        QFontDatabase::addApplicationFont(":/fonts/Oswald-Bold.ttf");
        QFontDatabase::addApplicationFont(":/fonts/Bangers.ttf");
        QFontDatabase::addApplicationFont(":/fonts/OxygenMono-Regular.ttf");
        QString stylesheet = stylesheetFile.readAll();
        setStyleSheet(stylesheet);
    }

    centralWidget = new QWidget(this);
    layout = new QGridLayout(centralWidget);

    auto furnitureGrid = new HelpWindowGridLayout();
    furnitureGrid->AddHeading("Furniture Controls");
    furnitureGrid->AddTitle("Movement Speed");
    furnitureGrid->AddDescription("Changes speed of furniture movement");
    furnitureGrid->AddTitle("Rotation Speed");
    furnitureGrid->AddDescription("Changes speed of furniture rotation");
    furnitureGrid->AddTitle("Scale Speed");
    furnitureGrid->AddDescription("Changes speed of furniture scaling");
    furnitureGrid->AddTitle("Action Selection Radio Buttons");
    furnitureGrid->AddDescription("Select whether to move, rotate, or scale furniture");
    furnitureGrid->AddTitle("Furniture Model Selection Dropdown");
    furnitureGrid->AddDescription("Select which furniture object to load");
    AddToColumn(furnitureGrid, 0);

    auto wallFloorControlsGrid = new HelpWindowGridLayout();
    wallFloorControlsGrid->AddHeading("Wall and Floor Controls");
    wallFloorControlsGrid->AddTitle("Change Wall Color");
    wallFloorControlsGrid->AddDescription("Shows a color picker dialog to change wall color");
    wallFloorControlsGrid->AddTitle("Change Wall Texture");
    wallFloorControlsGrid->AddDescription("Shows texture selection to change wall texture");
    wallFloorControlsGrid->AddTitle("Change Floor Texture");
    wallFloorControlsGrid->AddDescription("Shows a color picker dialog to change floor color");
    wallFloorControlsGrid->AddTitle("Change Floor Texture");
    wallFloorControlsGrid->AddDescription("Shows texture selection to change floor texture");
    AddToColumn(wallFloorControlsGrid, 0);

    auto statusBarGrid = new HelpWindowGridLayout();
    statusBarGrid->AddHeading("Status Bar");
    statusBarGrid->AddTitle("Status Message Text");
    statusBarGrid->AddDescription(QString("Shows program status information such as furniture")
                                  .append("loading status or error messages"));
    statusBarGrid->AddTitle("OBB Status");
    statusBarGrid->AddDescription("Furnitures' oriented bounding box display status");
    statusBarGrid->AddTitle("AABB Status");
    statusBarGrid->AddDescription("Furnitures' axis-aligned bounding box display status");
    statusBarGrid->AddTitle("W-Axis (World Axis)");
    statusBarGrid->AddDescription("Room's center axis display status");
    statusBarGrid->AddTitle("L-Axis (Local Axis)");
    statusBarGrid->AddDescription("Furnitures' axis display status");
    AddToColumn(statusBarGrid, 0);

    auto cameraMovementGrid = new HelpWindowGridLayout();
    cameraMovementGrid->AddHeading("Camera Movement");
    cameraMovementGrid->AddShortcut("W");
    cameraMovementGrid->AddDescription("Move camera forward");
    cameraMovementGrid->AddShortcut("S");
    cameraMovementGrid->AddDescription("Move camera backward");
    cameraMovementGrid->AddShortcut("A");
    cameraMovementGrid->AddDescription("Move camera left");
    cameraMovementGrid->AddShortcut("D");
    cameraMovementGrid->AddDescription("Move camera right");
    cameraMovementGrid->AddShortcut("Shoft + W");
    cameraMovementGrid->AddDescription("Move camera up");
    cameraMovementGrid->AddShortcut("Shift + S");
    cameraMovementGrid->AddDescription("Move camera down");
    cameraMovementGrid->AddShortcut("Arrow Up");
    cameraMovementGrid->AddDescription("Look up");
    cameraMovementGrid->AddShortcut("Arrow Down");
    cameraMovementGrid->AddDescription("Look down");
    cameraMovementGrid->AddShortcut("Arrow Left");
    cameraMovementGrid->AddDescription("Look left");
    cameraMovementGrid->AddShortcut("Arrow Right");
    cameraMovementGrid->AddDescription("Look right");
    cameraMovementGrid->AddShortcut("Right Click Drag");
    cameraMovementGrid->AddDescription(QString("Change camera angle (only when there is at least")
                                       .append("one object in the scene)"));
    cameraMovementGrid->AddShortcut("CTRL + Right Click Drag");
    cameraMovementGrid->AddDescription(QString("Rotate around the scene (only when there")
                                       .append(" is at least one object in the scene)"));
    cameraMovementGrid->AddShortcut("Mouse Wheel");
    cameraMovementGrid->AddDescription("Change camera FOV (field of view)");
    cameraMovementGrid->AddShortcut("R");
    cameraMovementGrid->AddDescription("Reset camera to its original position");
    AddToColumn(cameraMovementGrid, 1);

    auto furnitureManiuplationGrid = new HelpWindowGridLayout();
    furnitureManiuplationGrid->AddHeading("Furniture Object Manipulation");
    furnitureManiuplationGrid->AddTitle("Movement");
    furnitureManiuplationGrid->AddDescription("");
    furnitureManiuplationGrid->AddShortcut("X + Left Click Drag\nY + Left Click Drag\nZ + Left Click Drag");
    furnitureManiuplationGrid->AddDescription(QString("<b>Move mouse up/down:</b> Move object ")
                                              .append("in the positive/negative direction of the axis ")
                                              .append("corresponding to X, Y, and Z"));
    furnitureManiuplationGrid->AddTitle("Rotation");
    furnitureManiuplationGrid->AddDescription("");
    furnitureManiuplationGrid->AddShortcut("X + Left Click Drag\nY + Left Click Drag\nZ + Left Click Drag");
    furnitureManiuplationGrid->AddDescription(QString("<b>Move mouse up/down:</b> rotate object ")
                                              .append("in the positive/negative angle direction of the axis ")
                                              .append("corresponding to X, Y, and Z"));
    furnitureManiuplationGrid->AddTitle("Scale");
    furnitureManiuplationGrid->AddDescription("");
    furnitureManiuplationGrid->AddShortcut("Left Click Drag");
    furnitureManiuplationGrid->AddDescription("<b>Move mouse up/down</b>: uniformly scale the object up/down");
    furnitureManiuplationGrid->AddShortcut("N");
    furnitureManiuplationGrid->AddDescription("Cycle wall-mounted furnitures between the walls");
    furnitureManiuplationGrid->AddShortcut("DELETE");
    furnitureManiuplationGrid->AddDescription("Remove the currently selected object from the scene");
    furnitureManiuplationGrid->AddShortcut("SPACE");
    furnitureManiuplationGrid->AddDescription("Cycle through exising objects");
    furnitureManiuplationGrid->AddShortcut("ESCAPE");
    furnitureManiuplationGrid->AddDescription("De-select the currently selected object");
    AddToColumn(furnitureManiuplationGrid, 1);

    auto utilitiesGrid = new HelpWindowGridLayout();
    utilitiesGrid->AddHeading("Utilities");
    utilitiesGrid->AddShortcut("B");
    utilitiesGrid->AddDescription("Display objects' oriented bounding boxes");
    utilitiesGrid->AddShortcut("SHIFT + B");
    utilitiesGrid->AddDescription("Display objects' axis-aligned bounding boxes");
    utilitiesGrid->AddShortcut("L");
    utilitiesGrid->AddDescription("Display objects' pivots");
    utilitiesGrid->AddShortcut("X\nY\nZ");
    utilitiesGrid->AddDescription(QString("Display <b>X, Y</b> or <b>Z</b> axis aligned to ")
                                  .append("the center of the room ")
                                  .append("or aligned to currently selected object's location"));
    utilitiesGrid->AddShortcut("Q");
    utilitiesGrid->AddDescription("Quit the application (no confirmation window!)");
    AddToColumn(utilitiesGrid, 2);

    auto worldAxisGrid = new HelpWindowGridLayout();
    worldAxisGrid->AddHeading("World (room) Axis Controls");
    worldAxisGrid->AddTitle("");
    worldAxisGrid->AddDescription("(only when the axis is being displayed)");
    worldAxisGrid->AddShortcut("SHIFT + L");
    worldAxisGrid->AddDescription("Display room's pivot");
    worldAxisGrid->AddShortcut("{");
    worldAxisGrid->AddDescription("Move axis up");
    worldAxisGrid->AddShortcut("/");
    worldAxisGrid->AddDescription("Move axis down");
    worldAxisGrid->AddShortcut("'");
    worldAxisGrid->AddDescription("Move axis to the right");
    worldAxisGrid->AddShortcut(";");
    worldAxisGrid->AddDescription("Move axis to the left");
    worldAxisGrid->AddShortcut("SHIFT + /");
    worldAxisGrid->AddDescription("Reset axis' location to its original position");
    AddToColumn(worldAxisGrid, 2);

    setCentralWidget(centralWidget);
}


HelpWindow::~HelpWindow()
{
    delete layout;
    delete centralWidget;
}
