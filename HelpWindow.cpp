#include "HelpWindow.h"
#include "qlabel.h"

HelpWindow::HelpWindow()
{
    setFixedSize(1200, 800);

    centralWidget = new QWidget(this);
    layout = new QGridLayout(centralWidget);
	QLabel* column0 = new QLabel("\
Menus:\n\
Menu items are described top down left to right.\n\
\n\
Scene Area :\n\
	Displays a room and furniture objects.\n\
\n\
		Furniture Controls :\n\
	Movement Speed : Changes speed of furniture movement.\n\
		Rotation Speed : Changes speed of furniture rotation.\n\
		Scale Speed : Changes speed of furniture scaling\n\
		Action Select Radios : Select whether to Move, Rotate or Scale furniture.\n\
		Furniture Model Select Dropdown : Select which furniture object to load.\n\
		Load Model Button : Loads model into scene.\n\
\n\
		Wall and Floor controls :\n\
	Change Wall Color : Shows a color picker dialog to change wall color.\n\
		Change Wall Texture : Shows texture selection to change wall texture.\n\
		Change Floor Color : Shows a color picker dialog to change floor color.\n\
		Change Floor Texture : Shows texture selection to change wall texture.\n\
\n\
		Status Bar :\n\
	Status Message Text : Shows program status information such as furniture loading status or error messages.\n\
		OBB Status : Object Bounding Box status.\n\
		AABB : Axis Aligned Bounding Box.\n\
		W - Axis : Shows X, Y & Z axis of the room.\n\
		L - Axis : Shows X, Y & Z axis of a furniture object.\n\
\n\
		Camera manipulation :\n\
	Rotation and zoom achieved with mouse and keyboard keys as backup.\n\
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
	layout->addWidget(column0, 0, 0, 1, 1);
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
	layout->addWidget(column1,0,1,1,1);
    setCentralWidget(centralWidget);
	
}


HelpWindow::~HelpWindow()
{
	delete layout;
	delete centralWidget;
}
