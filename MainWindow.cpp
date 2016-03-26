#include "MainWindow.h"

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent)
{
    resize(800, 800);

    centralWidget = new QWidget(this);
    QVBoxLayout *verticalLayout = new QVBoxLayout(centralWidget);
    centralWidget->setLayout(verticalLayout);

    InitializeControlsGrid();
    SetupSliders();
    AddMoveControlsToControlsGrid();
    AddRotateControlsToControlsGrid();
    AddScaleControlsToControlsGrid();
    AddModelControlsToControlsGrid();
    SetupStatusBar();

    floorAndWallModifiersLayout = new QHBoxLayout();
    controlsGridLayout->addLayout(floorAndWallModifiersLayout, 3, 0, 1, 9);

    // Set two custom colors for the color dialog: first is the initial color for the walls,
    //      and second one is for the floor (we get the values from 'HomeDesignerOpenGLWidget.cpp')
    extern glm::vec3 initialWallColor;
    extern glm::vec3 initialFloorColor;
    QColorDialog::setCustomColor(0, QColor(initialWallColor.r * 255, initialWallColor.g * 255, initialWallColor.b * 255));
    QColorDialog::setCustomColor(1, QColor(initialFloorColor.r * 255, initialFloorColor.g * 255, initialFloorColor.b * 255));

    SetupWallModifiers();
    SetupFloorModifiers();

    openglWidget = new HomeDesignerOpenGLWidget(centralWidget);
    openglWidget->setFocusPolicy(Qt::StrongFocus);
    openglWidget->setFocus();
    openglWidget->setMouseTracking(true);

    ConnectSignalsAndSlots();
    InitializeSliderValues();

    verticalLayout->addWidget(openglWidget);
    verticalLayout->addLayout(controlsGridLayout);

    setCentralWidget(centralWidget);

}

void MainWindow::OnDisplayError(QString message)
{
    QMessageBox::critical(this, "Error!", message);
}

void MainWindow::OnUpdateStatus(bool boundingBox, bool aaBoudningBox, bool axis) const
{
    QString bbString = QString("BB: <b>%1</b>").arg(boundingBox ? "ON" : "OFF");
    QString aabbString = QString("AABB: <b>%1</b>").arg(aaBoudningBox ? "ON" : "OFF");
    QString axisString = QString("Axis: <b>%1</b>").arg(axis ? "ON" : "OFF");
    bbStatusLabel->setText(bbString);
    aabbStatusLabel->setText(aabbString);
    axisStatusLabel->setText(axisString);
}

void MainWindow::OnWallColorButtonClicked()
{
    grabKeyboard();
    releaseKeyboard();
    auto color = QColorDialog::getColor();

    if (!color.isValid())
        return;

    emit ChangeRoomWallColor(color);
    openglWidget->grabKeyboard();
}

void MainWindow::OnWallTextureButtonClicked()
{
    grabKeyboard();
    releaseKeyboard();
    auto wallTexturePath = QFileDialog::getOpenFileName(this, "Wall Texture File Location",
                                                        "textures", "Image Files (*.png *.jpg *.bmp)");

    if (wallTexturePath.isEmpty())
        return;

    emit ChangeRoomWallTexture(wallTexturePath);
    openglWidget->grabKeyboard();
}

void MainWindow::OnFloorColorButtonClicked()
{
    grabKeyboard();
    releaseKeyboard();
    auto color = QColorDialog::getColor();

    if (!color.isValid())
        return;

    emit ChangeRoomFloorColor(color);
    openglWidget->grabKeyboard();
}

void MainWindow::OnFloorTextureButtonClicked()
{
    grabKeyboard();
    releaseKeyboard();
    QFileDialog fileDialog(this);
    auto floorTexturePath = QFileDialog::getOpenFileName(this,  "Floor Texture Path",
                                                         "textures", "Image Files (*.png *.jpg *.bmp)");
    if (floorTexturePath.isEmpty())
        return;

    emit ChangeRoomFloorTexture(floorTexturePath);
    openglWidget->grabKeyboard();
}

MainWindow::~MainWindow()
{
    delete messageLabel;
    delete controlsGridLayout;
    delete statusBar;
    delete openglWidget;
    delete modelsCombo;
    delete loadModelButton;
    delete moveRadioButton;
    delete rotateRadioButton;
    delete scaleRadioButton;
    delete moveSlider;
    delete rotateSlider;
    delete scaleSlider;
    delete moveValueLabel;
    delete rotateValueLabel;
    delete scaleValueLabel;
    delete wallColorButton;
    delete wallTextureButton;
    delete floorColorButton;
    delete floorTextureButton;
    delete centralWidget;
}

void MainWindow::OnChangeButtons(bool enable) const
{
    loadModelButton->setEnabled(enable);
    wallColorButton->setEnabled(enable);
    wallTextureButton->setEnabled(enable);
    floorColorButton->setEnabled(enable);
    floorTextureButton->setEnabled(enable);
}

void MainWindow::OnClearMessage() const
{
    messageLabel->setText("");
}

void MainWindow::OnDisplayMessage(QString message, int timeout) const
{
    messageLabel->setText(message);

    if (timeout)
    {
        QCoreApplication::processEvents();
        QTimer::singleShot(timeout, this, SLOT(OnClearMessage()));
    }
}

void MainWindow::SetupSliders()
{
    moveSlider = new QSlider(Qt::Horizontal, centralWidget);
    moveSlider->setSingleStep(1);
    moveSlider->setTickPosition(QSlider::TicksBelow);
    moveSlider->setTickInterval(2);
    moveSlider->setMinimum(1);
    moveSlider->setMaximum(39);
    moveSlider->setValue(1);

    rotateSlider = new QSlider(Qt::Horizontal, centralWidget);
    rotateSlider->setSingleStep(1);
    rotateSlider->setTickPosition(QSlider::TicksBelow);
    rotateSlider->setTickInterval(2);
    rotateSlider->setMinimum(1);
    rotateSlider->setMaximum(19);
    rotateSlider->setValue(1);

    scaleSlider = new QSlider(Qt::Horizontal, centralWidget);
    scaleSlider->setSingleStep(1);
    scaleSlider->setTickPosition(QSlider::TicksBelow);
    scaleSlider->setTickInterval(2);
    scaleSlider->setMinimum(1);
    scaleSlider->setMaximum(19);
    scaleSlider->setValue(1);
}

void MainWindow::InitializeControlsGrid()
{
    controlsGridLayout = new QGridLayout();

    controlsGridLayout->setColumnStretch(0, 1);
    controlsGridLayout->setColumnStretch(1, 3);
    controlsGridLayout->setColumnStretch(2, 1);
    controlsGridLayout->setColumnStretch(3, 1);
    controlsGridLayout->setColumnStretch(4, 3);
    controlsGridLayout->setColumnStretch(5, 1);
    controlsGridLayout->setColumnStretch(6, 1);
    controlsGridLayout->setColumnStretch(7, 3);
    controlsGridLayout->setColumnStretch(8, 1);
}

void MainWindow::AddMoveControlsToControlsGrid()
{
    auto moveLabel = new QLabel("Movement Speed:");
    moveValueLabel = new QLabel("1");

    moveRadioButton = new QRadioButton("Move", centralWidget);

    auto moveGridLayout = new QGridLayout();
    moveGridLayout->addWidget(moveLabel, 0, 0);
    moveGridLayout->addWidget(moveValueLabel, 0, 1, Qt::AlignLeft);
    moveGridLayout->addWidget(moveSlider, 1, 0, 1, 2);
    moveGridLayout->setColumnStretch(0, 1);
    moveGridLayout->setColumnStretch(1, 2);

    controlsGridLayout->addWidget(moveRadioButton, 0, 0);
    controlsGridLayout->addLayout(moveGridLayout, 0, 1);
    controlsGridLayout->addItem(new QSpacerItem(20, 5, QSizePolicy::Fixed, QSizePolicy::Fixed), 0, 2);
}

void MainWindow::AddRotateControlsToControlsGrid()
{
    auto rotateLabel = new QLabel("Rotation Speed:");
    rotateValueLabel = new QLabel("1");

    rotateRadioButton = new QRadioButton("Rotate", centralWidget);

    auto rotateGridLayout = new QGridLayout();
    rotateGridLayout->addWidget(rotateLabel, 0, 0);
    rotateGridLayout->addWidget(rotateValueLabel, 0, 1, Qt::AlignLeft);
    rotateGridLayout->addWidget(rotateSlider, 1, 0, 1, 2);
    rotateGridLayout->setColumnStretch(0, 1);
    rotateGridLayout->setColumnStretch(1, 2);

    controlsGridLayout->addWidget(rotateRadioButton, 0, 3);
    controlsGridLayout->addLayout(rotateGridLayout, 0, 4);
    controlsGridLayout->addItem(new QSpacerItem(20, 5, QSizePolicy::Fixed, QSizePolicy::Fixed), 0, 5);
}

void MainWindow::AddScaleControlsToControlsGrid()
{
    auto scaleLabel = new QLabel("Scale Speed:");
    scaleValueLabel = new QLabel("1");

    scaleRadioButton = new QRadioButton("Scale", centralWidget);

    auto scaleGridLayout = new QGridLayout();
    scaleGridLayout->addWidget(scaleLabel, 0, 0);
    scaleGridLayout->addWidget(scaleValueLabel, 0, 1, Qt::AlignLeft);
    scaleGridLayout->addWidget(scaleSlider, 1, 0, 1, 2);
    scaleGridLayout->setColumnStretch(0, 1);
    scaleGridLayout->setColumnStretch(1, 2);

    controlsGridLayout->addWidget(scaleRadioButton, 0, 6);
    controlsGridLayout->addLayout(scaleGridLayout, 0, 7);
    controlsGridLayout->addItem(new QSpacerItem(20, 5, QSizePolicy::Fixed, QSizePolicy::Fixed), 0, 8);
}

void MainWindow::AddModelControlsToControlsGrid()
{
    modelsCombo = new ModelComboBox(centralWidget);
    loadModelButton = new QPushButton("Load Model");
    modelsCombo->addItem("--- Please select a model ---");
	modelsCombo->addItem("Tool chest", "models/toolChest/model.obj|floor");
	modelsCombo->addItem("Rolled Towels", "models/rolledTowels/model.obj");
	modelsCombo->addItem("Arm Chair", "models/armChair/model.obj");
    modelsCombo->addItem("Nanosuit Model (Not Bound)"              , "models/nanosuit/nanosuit.obj");
    modelsCombo->addItem("Nanosuit Model (Bound to Floor)"         , "models/nanosuit/nanosuit.obj|floor");
    modelsCombo->addItem("Nanosuit Model (Bound to [Default] Wall)", "models/nanosuit/nanosuit.obj|wall");
    modelsCombo->addItem("Nanosuit Model (Bound to Left Wall)"     , "models/nanosuit/nanosuit.obj|wall|left");
    modelsCombo->addItem("Nanosuit Model (Bound to Right Wall)"    , "models/nanosuit/nanosuit.obj|wall|right");
    modelsCombo->addItem("Nanosuit Model (Bound to Back Wall)"     , "models/nanosuit/nanosuit.obj|wall|back");
    controlsGridLayout->addWidget(modelsCombo    , 1, 0, 1, 6);
    controlsGridLayout->addWidget(loadModelButton, 1, 6, 1, 3);
}

void MainWindow::ConnectSignalsAndSlots() const
{
    connect(moveRadioButton,   SIGNAL(toggled(bool)), openglWidget, SLOT(OnMoveSelectedChanged(bool)));
    connect(rotateRadioButton, SIGNAL(toggled(bool)), openglWidget, SLOT(OnRotateSelectedChanged(bool)));
    connect(scaleRadioButton,  SIGNAL(toggled(bool)), openglWidget, SLOT(OnScaleSelectedChanged(bool)));

    connect(moveSlider, SIGNAL(valueChanged(int)), openglWidget,   SLOT(OnMoveSpeedChanged(int)));
    connect(moveSlider, SIGNAL(valueChanged(int)), moveValueLabel, SLOT(setNum(int)));

    connect(rotateSlider, SIGNAL(valueChanged(int)), openglWidget,     SLOT(OnRotateSpeedChanged(int)));
    connect(rotateSlider, SIGNAL(valueChanged(int)), rotateValueLabel, SLOT(setNum(int)));

    connect(scaleSlider, SIGNAL(valueChanged(int)), openglWidget,    SLOT(OnScaleSpeedChanged(int)));
    connect(scaleSlider, SIGNAL(valueChanged(int)), scaleValueLabel, SLOT(setNum(int)));

    connect(openglWidget, SIGNAL(Exit()),                          this, SLOT(close()));
    connect(openglWidget, SIGNAL(DisplayMessage(QString, int)),    this, SLOT(OnDisplayMessage(QString, int)));
    connect(openglWidget, SIGNAL(StatusUpdated(bool, bool, bool)), this, SLOT(OnUpdateStatus(bool, bool, bool)));
    connect(openglWidget, SIGNAL(DisplayError(QString)),           this, SLOT(OnDisplayError(QString)));
    connect(openglWidget, SIGNAL(ClearMessage()),                  this, SLOT(OnClearMessage()));
    connect(openglWidget, SIGNAL(CollisionDetected(bool)),         this, SLOT(OnChangeButtons(bool)));

    connect(loadModelButton, SIGNAL(clicked()), modelsCombo, SLOT(OnButtonClicked()));

    connect(modelsCombo, SIGNAL(ModelChanged(QString, GLfloat)), openglWidget, SLOT(OnLoadModel(QString, GLfloat)));

    // Connect color/texture buttons' signals to appropriate slots
    connect(wallColorButton,    SIGNAL(clicked()), this, SLOT(OnWallColorButtonClicked()));
    connect(wallTextureButton,  SIGNAL(clicked()), this, SLOT(OnWallTextureButtonClicked()));
    connect(floorColorButton,   SIGNAL(clicked()), this, SLOT(OnFloorColorButtonClicked()));
    connect(floorTextureButton, SIGNAL(clicked()), this, SLOT(OnFloorTextureButtonClicked()));

    // Connect color/texture signals and slots
    connect(this, SIGNAL(ChangeRoomWallColor(QColor)),     openglWidget, SLOT(OnChangeRoomWallColor(QColor)));
    connect(this, SIGNAL(ChangeRoomWallTexture(QString)),  openglWidget, SLOT(OnChangeRoomWallTexture(QString)));
    connect(this, SIGNAL(ChangeRoomFloorColor(QColor)),    openglWidget, SLOT(OnChangeRoomFloorColor(QColor)));
    connect(this, SIGNAL(ChangeRoomFloorTexture(QString)), openglWidget, SLOT(OnChangeRoomFloorTexture(QString)));
}

// Initialize slider values to a medium value
void MainWindow::InitializeSliderValues()
{
    moveSlider->setValue(12);
    rotateSlider->setValue(5);
    scaleSlider->setValue(4);
}

void MainWindow::SetupStatusBar()
{
    statusBar = new QStatusBar(centralWidget);

    messageLabel = new QLabel(statusBar);
    bbStatusLabel = new QLabel(statusBar);
    bbStatusLabel->setAlignment(Qt::AlignRight);
    aabbStatusLabel = new QLabel(statusBar);
    aabbStatusLabel->setAlignment(Qt::AlignRight);
    axisStatusLabel = new QLabel(statusBar);
    axisStatusLabel->setAlignment(Qt::AlignRight);
    statusBar->addPermanentWidget(messageLabel, 4);
    statusBar->addPermanentWidget(bbStatusLabel, 0);
    statusBar->addPermanentWidget(aabbStatusLabel, 0);
    statusBar->addPermanentWidget(axisStatusLabel, 0);
    statusBar->setStyleSheet("QStatusBar{border-top: 2px outset grey;}");
    setStatusBar(statusBar);
}

void MainWindow::SetupWallModifiers()
{
    wallColorButton = new QPushButton("Change Wall Color ...");
    wallTextureButton = new QPushButton("Change Wall Texture ...");

    QGroupBox* wallGroupBox = new QGroupBox("Wall Controls:");
    wallGroupBox->setFixedHeight(60);
    auto wallLayout = new QHBoxLayout();
    wallLayout->addWidget(wallColorButton);
    wallLayout->addWidget(wallTextureButton);

    wallGroupBox->setLayout(wallLayout);
    floorAndWallModifiersLayout->addWidget(wallGroupBox);

    // Add a space between wall modifiers and floor modifiers
    floorAndWallModifiersLayout->addSpacing(40);
}

void MainWindow::SetupFloorModifiers()
{
    floorColorButton = new QPushButton("Change Floor Color ...");
    floorTextureButton = new QPushButton("Change Floor Texture ...");

    QGroupBox* floorGroupBox = new QGroupBox("Floor Controls:");
    floorGroupBox->setFixedHeight(60);
    auto wallLayout = new QHBoxLayout();
    wallLayout->addWidget(floorColorButton);
    wallLayout->addWidget(floorTextureButton);

    floorGroupBox->setLayout(wallLayout);
    floorAndWallModifiersLayout->addWidget(floorGroupBox);
}
