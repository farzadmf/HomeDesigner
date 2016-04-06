#pragma once

#include "Includes.h"
#include <QtWidgets>

///
/// \brief The ModelComboBox class 
/// Which will provide a list of models from which one can select to load
///
class ModelComboBox : public QComboBox
{
    Q_OBJECT

public:
    /// \brief Solely used to draw the separator in our ComboBox
    class ComboBoxDelegate : public QItemDelegate
    {
    public:
        explicit ComboBoxDelegate(QObject* parent = nullptr);

    protected:
        void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
        QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const override;
    };

    ///
    /// \brief ModelComboBox creates the model combo box
    /// \param parent the parent widget
    ///
    explicit ModelComboBox(QWidget* parent = nullptr);
    ~ModelComboBox();

public slots:
    ///
    /// \brief OnButtonClicked when button is clicked in model exists it will be loaded
    ///
    void OnButtonClicked();

signals:
    ///
    /// \brief ModelChanged will determine when a model selection has changed and when we have loaded an initial scale
    /// \param modelAttributes the selected model from the combobox
    /// \param initialScale usually 1
    ///
    void ModelChanged(QString modelAttributes, GLfloat initialScale);
};

