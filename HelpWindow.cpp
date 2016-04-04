#include "HelpWindow.h"
#include "qlabel.h"
#include <QFile>
#include <QFontDatabase>
#include <HelpWindowGridLayout.h>

#include <iostream>
using std::cout;
using std::endl;

void HelpWindow::AddToColumn(QGridLayout* gridLayout, int column, int& row) const
{
    layout->addLayout(gridLayout, row++, column);
    layout->addItem(new QSpacerItem(0, verticalSpacing), row++, 0);
}

HelpWindow::HelpWindow()
{
    setFixedSize(1200, 800);
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

    auto row = 0;

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
    AddToColumn(furnitureGrid, 0, row);
//    layout->addLayout(furnitureGrid, row++, 0);
//    layout->addItem(new QSpacerItem(0, verticalSpacing), row++, 0);

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
    AddToColumn(wallFloorControlsGrid, 0, row);
//    layout->addLayout(wallFloorControlsGrid, row++, 0);
//    layout->addItem(new QSpacerItem(0, verticalSpacing), row++, 0);

    auto statusBarGrid = new HelpWindowGridLayout();
    statusBarGrid->AddHeading("Status Bar");
    statusBarGrid->AddTitle("Status Message Text");
    statusBarGrid->AddDescription("Shows program status information such as furniture\nloading status or error messages");
    statusBarGrid->AddTitle("OBB Status");
    statusBarGrid->AddDescription("Furnitures' oriented bounding box display status");
    statusBarGrid->AddTitle("AABB Status");
    statusBarGrid->AddDescription("Furnitures' axis-aligned bounding box display status");
    statusBarGrid->AddTitle("W-Axis (World Axis)");
    statusBarGrid->AddDescription("Room's center axis display status");
    statusBarGrid->AddTitle("L-Axis (Local Axis)");
    statusBarGrid->AddDescription("Furnitures' axis display status");
    AddToColumn(statusBarGrid, 0, row);
//    layout->addLayout(statusBarGrid, row++, 0);
//    layout->addItem(new QSpacerItem(0, verticalSpacing), row++, 0);

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
    cameraMovementGrid->AddDescription("Change camera angle (only when there is at least\none object in the scene)");
    cameraMovementGrid->AddShortcut("CTRL + Right Click Drag");
    cameraMovementGrid->AddDescription("Rotate around the scene (only when there is at least\none object in the scene)");
    cameraMovementGrid->AddShortcut("Mouse Wheel");
    cameraMovementGrid->AddDescription("Change camera FOV (field of view)");
    cameraMovementGrid->AddShortcut("R");
    cameraMovementGrid->AddDescription("Reset camera to its original position");
    row = 0;
    AddToColumn(cameraMovementGrid, 1, row);

    QLabel* column0 = new QLabel("\
Menus:\n\
Menu items are described top down left to right.\n\
\n\
Scene Area :\n\
    Displays a room and furniture objects.\n\
\n\
        Camera movement :\n\
    W : Move Camera Forward\n\
        S : Move Camera  Backward\n\
        A : Move Camera  Left\n\
        D : Move Camera  Right\n\
        Shift + W : Move Camera  UP\n\
        Shift + S : Move Camera  Down\n\
        Arrow  Up : Rotate Camera Up\n\
        Arrow  Down : Rotate Camera Down\n\
        Arrow  Left : Rotate Camera Left\n\
        Arrow  Right : Rotate Camera Right\n\
        Right Click Drag : Change Camera angle(Only when there is at least one object in scene).\n\
        CTRL + Right Click Drag : Rotate around the scene(Only when there is at least one object in scene).\n\
        Mouse Scroll : Change Camera Field Of View\n\
        R : Reset Camera to original position\n");
//    layout->addWidget(column0, 0, 0, 1, 1);
    QLabel* column1 = new QLabel("\
Furniture Object Manipulation :\n\
        Furniture objects are selected and loaded from Furniture Control Menus as seen above.\n\
        Furniture Objects are manipulated using keyboard and mouse.\n\
\n\
            Movement :\n\
            X + Left Click :\n\
        Move mouse Down : Move object Left\n\
            Move mouse Up : Move object Right\n\
            Z + Left Click :\n\
        Move mouse Down : Move object Backward\n\
            Move mouse Up : Move object Forward\n\
            Y + Left Click :\n\
        Move mouse Down : Move object Down\n\
            Move mouse Up : Move object UP\n\
            Shift + N : Switch wall mounted Furniture Object to other walls\n\
            Rotation :\n\
        X + Left Click : Rotate around the X - Axis\n\
            Z + Left Click : Rotate around the Z - Axis\n\
            Y + Left Click : Rotate around the Y - Axis\n\
            Scale :\n\
        Left Click :\n\
        Move mouse Up : Scale Up\n\
            Move mouse Down : Scale Down\n\
            Delete :\n\
        Delete button : Deletes Currently Selected Furniture Object\n\
            Selection :\n\
        Space Bar : Cycles through Furniture Objects In Scene.Highlighted object is the currently active object.\n\
            Esc : Deselect Object\n\
\n\
            Utilities :\n\
        B : Display Furniture Object's Bounding Box\n\
            Shift + B : Display Furniture Object's Axis - Aligned Bounding Box\n\
            L : Display Furniture Object's x,y,z axis\n\
            X : Displays X - axis For Current Furniture Object Or For Whole Scene\n\
            Y : Displays Y - axis For Current Furniture  Object Or For Whole Scene\n\
            Z : Displays Z - axis For Current Furniture  Object Or For Whole Scene\n\
            Q : Quit Application\n\
            World Axis controls :\n\
            Shift + L : Show World Axis\n\
            { : Move Axis Up(Only when World Axis is Shown)\n\
                / : Move Axis Down(Only when World Axis is \n\
                ; : Decrease Size(Only when World Axis is Shown)\n\
                Shift + / : Reset World Axis Position(Only when World Axis is Shown)n");
//    layout->addWidget(column1,0,1,1,1);
    setCentralWidget(centralWidget);
    
}


HelpWindow::~HelpWindow()
{
    delete layout;
    delete centralWidget;
}
