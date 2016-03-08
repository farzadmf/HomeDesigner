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
    void ChangeModelLoadingButton(bool enable) const;
    void ClearMessage() const;
    void DisplayMessage(QString message, int timeout) const;
    void DisplayError(QString message);
    void UpdateStatus(bool boundingBox, bool aaBoudningBox, bool axis) const;

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

    void SetupSliders();
    void InitializeControlsGrid();
    void AddMoveControlsToControlsGrid();
    void AddRotateControlsToControlsGrid();
    void AddScaleControlsToControlsGrid();
    void AddModelControlsToControlsGrid();
    void ConnectSignalsAndSlots() const;
    void SetupStatusBar();
};

