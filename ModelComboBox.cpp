#include "ModelComboBox.h"

ModelComboBox::ComboBoxDelegate::ComboBoxDelegate(QObject* parent) :
    QItemDelegate(parent)
{
}

void ModelComboBox::ComboBoxDelegate::paint(QPainter* painter,
                                            const QStyleOptionViewItem& option,
                                            const QModelIndex& index) const
{
    if (index.data(Qt::AccessibleDescriptionRole).toString() == QLatin1String("separator"))
    {
        painter->setPen(QPen(QBrush(Qt::black), 2, Qt::DashDotLine));
        painter->drawLine(option.rect.left(), option.rect.center().y(),
                          option.rect.right(), option.rect.center().y());
    }
    else
        QItemDelegate::paint(painter, option, index);
}

QSize ModelComboBox::ComboBoxDelegate::sizeHint(const QStyleOptionViewItem& option,
                                                const QModelIndex& index) const
{
    auto type = index.data(Qt::AccessibleDescriptionRole).toString();
    if (type == QLatin1String("separator"))
        return QSize(0, 20);
    
    return QItemDelegate::sizeHint(option, index);
}

ModelComboBox::ModelComboBox(QWidget* parent) : QComboBox(parent) { }

ModelComboBox::~ModelComboBox() { }

void ModelComboBox::OnButtonClicked()
{
    int index = this->currentIndex();

    // Index 0 just displays the message
    if (!index)
        return;

    auto modelAttributes = this->currentData().toString().toStdString();

    // Make sure enough information is included with the model
    auto attributes = QString::fromStdString(modelAttributes).split('|');
    if (attributes.size() == 0)
    {
        QMessageBox::critical(this, "Error!", QString::fromStdString(
            "Date defined for the model isn't enough!"));
        return;
    }

    // Check to see if the specified path exists
    auto modelPath = attributes[0];
    QFileInfo modelFile(modelPath);

    if (!modelFile.exists() || !modelFile.isFile())
    {
        QMessageBox::critical(this, "Error!", QString::fromStdString(
            "File specified in '" + modelAttributes + "' doesn't exist!"));
        return;
    }

    grabKeyboard();
    releaseKeyboard();
    bool ok;
    QString message = QString::fromStdString("Please specify the <b>initial scale</b> of the") +
        QString::fromStdString(" model (minimum <b>0.1</b> ") +
        QString::fromStdString(" and maximum <b>20.0</b>)");

    GLfloat initialScale = QInputDialog::getDouble(this, "Initial Scale", message,
                                                   1.0, 0.1, 20.0f, 2, &ok);

    if (!ok)
        return;

    emit ModelChanged(QString::fromStdString(modelAttributes), initialScale);
}
