#pragma once

#include "HomeDesignerOpenGLWidget.h"
#include <QtWidgets>
#include "ModelComboBox.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void OnChangeButtons(bool enable) const;
    void OnClearMessage() const;
    void OnDisplayMessage(QString message, int timeout) const;
    void OnDisplayError(QString message);
    void OnUpdateStatus(bool boundingBox, bool aaBoudningBox, bool worldAxis, bool localAxis) const;

    // Color/texture handlers
    void OnWallColorButtonClicked();
    void OnWallTextureButtonClicked();
    void OnFloorColorButtonClicked();
    void OnFloorTextureButtonClicked();

signals:
    void ChangeRoomWallColor(QColor color);
    void ChangeRoomWallTexture(QString textureFilePath);
    void ChangeRoomFloorColor(QColor color);
    void ChangeRoomFloorTexture(QString textureFilePath);

private:
    QWidget* centralWidget;
    QGridLayout* controlsGridLayout;

    QLabel* bbStatusLabel;
    QLabel* aabbStatusLabel;
    QLabel* worldAxisStatusLabel;
    QLabel* localAxisStatusLabel;
    QLabel* messageLabel;
    QStatusBar* statusBar;

    QOpenGLWidget* openglWidget;

    ModelComboBox* modelsCombo;
    QPushButton* loadModelButton;

    QRadioButton* moveRadioButton;
    QRadioButton* rotateRadioButton;
    QRadioButton* scaleRadioButton;

    QSlider* moveSlider;
    QSlider* rotateSlider;
    QSlider* scaleSlider;

    QLabel* moveValueLabel;
    QLabel* rotateValueLabel;
    QLabel* scaleValueLabel;

    QHBoxLayout* floorAndWallModifiersLayout;
    QPushButton* wallColorButton;
    QPushButton* wallTextureButton;
    QPushButton* floorColorButton;
    QPushButton* floorTextureButton;

    void SetupSliders();
    void InitializeControlsGrid();
    void AddMoveControlsToControlsGrid();
    void AddRotateControlsToControlsGrid();
    void AddScaleControlsToControlsGrid();
    void AddModelControlsToControlsGrid();
    void ConnectSignalsAndSlots() const;
    void InitializeSliderValues() const;
    void SetupStatusBar();
    void SetupWallModifiers();
    void SetupFloorModifiers();
};

