#pragma once

#include "Includes.h"
#include <QtWidgets>

class ModelComboBox : public QComboBox
{
    Q_OBJECT

public:
    explicit ModelComboBox(QWidget* parent = nullptr);
    ~ModelComboBox();

public slots:
    void OnButtonClicked();

signals:
    void ModelChanged(int index, string modelPath, GLfloat initialScale);
};

