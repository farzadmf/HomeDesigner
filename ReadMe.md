# HomeDesigner Project

## Screenshots
### Main Window
<img src="https://github.com/farzadmf/HomeDesigner/blob/master/Screenshots/01-Main.jpg" />

### Adding a Model
<img src="https://github.com/farzadmf/HomeDesigner/blob/master/Screenshots/02-AddingModel.jpg" />

### Moving a model in the scene
<img src="https://github.com/farzadmf/HomeDesigner/blob/master/Screenshots/03-MoveModel.jpg" />

### Changing color (wall or floor)
<img src="https://github.com/farzadmf/HomeDesigner/blob/master/Screenshots/04-ChangeColor.jpg" />

### Changing texture (wall or floor)
<img src="https://github.com/farzadmf/HomeDesigner/blob/master/Screenshots/04-ChangeTexture.jpg" />

### Room with different textures
<img src="https://github.com/farzadmf/HomeDesigner/blob/master/Screenshots/05-ChangedTextures.jpg" />

### Collision detection
<img src="https://github.com/farzadmf/HomeDesigner/blob/master/Screenshots/06-CollisionDetection.jpg" />

### Resolving collision (by moving the object)
<img src="https://github.com/farzadmf/HomeDesigner/blob/master/Screenshots/07-CollisionResolved.jpg" />

## To use the project:
1. Install Visual Studio 2015  
    1. [This is the link to the Community version](https://go.microsoft.com/fwlink/?LinkId=691978&clcid=0x409)
    2. You can get the Enterprise version [from DreamSpark here](https://aits.encs.concordia.ca/aits/sec/msdnaa) (enter your ENCS username and password)
2. Download Qt (32 bit - latest version) --- [This is the link to 5.7 installer](http://download.qt.io/official_releases/qt/5.7/5.7.0/qt-opensource-windows-x86-msvc2015-5.7.0.exe)
    --- **NOTE:** the file size is about 800MB, so be patient!
3. Install Qt into the default location (**C:\qt\Qt5.7.0\5.7\msvc2015**). You don't need to enter this location; just accept the defaults
4. Open Visual Studio, go to **Tools/Extensions and Updates...**
5. Click on the **Online** section
6. In the upper right of the dialog, search by typing **qt** (no need to press **Enter**)
7. Install the **Qt5Package** plugin
8. After Visual Studio has been restarted, open the menu item **Qt5/Qt Options**, then click **Add**
9. Enter **5.7** for **Version Name** and the path should be the default install location (**C:\Qt\Qt5.7.0\5.7\msvc2015**)
10. Clone the repository and you should be able to run the application inside Visual Studio

**TIP**: not required for this project, but if you want to create new Qt projects from scratch (using the templates installed by the Add-in), you need to create an environment variable with name `QTDIR`, and the value being the default Qt installation directory (**C:\Qt\Qt5.7.0\5.7\msvc2015**)
