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
    void OnChangeModelLoadingButton(bool enable) const;
    void OnClearMessage() const;
    void OnDisplayMessage(QString message, int timeout) const;
    void OnDisplayError(QString message);
    void OnUpdateStatus(bool boundingBox, bool aaBoudningBox, bool axis) const;

private:
    QWidget* centralWidget;
    QGridLayout* controlsGridLayout;

    QLabel* bbStatusLabel;
    QLabel* aabbStatusLabel;
    QLabel* axisStatusLabel;
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
    void SetupStatusBar();
    void SetupWallModifiers();
    void SetupFloorModifiers();
};

