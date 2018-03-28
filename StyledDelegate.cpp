#include "StyledDelegate.h"
#include <QComboBox>
#include <QFileDialog>
#include "qjsonmodel.h"
#include <QDebug>
#include <QCheckBox>


ItemDelegate::ItemDelegate(QObject* parent) : QStyledItemDelegate(parent) { }

ItemDelegate::~ItemDelegate() { }

QWidget*
ItemDelegate::createEditor(
    QWidget*             parent,
    QStyleOptionViewItem const& option,
    QModelIndex          const& index
    ) const
{
    auto col = index.column();
    auto item = static_cast<QJsonTreeItem*>(index.internalPointer());
    if (col != 1) {
        return QStyledItemDelegate::createEditor(parent, option, index);
    } else if (nullptr == item ) {
        return QStyledItemDelegate::createEditor(parent, option, index);
    }

    auto keys = item->key().split(":");
    auto k = 1 == keys.size() ? keys.at(0).trimmed() : keys.at(1).trimmed();

    if ("Archivo" == k) {
        auto fd = new QFileDialog(parent);
        fd->setNameFilter("*.obj *.csv *.txt");
        return fd;
    } else if (
        ("Visibilidad inicial"          == k) ||
        ("Forzar superficie doble cara" == k) ||
        ("Forzar color en mallas"       == k)) {

        auto v = item->value();
        QComboBox* cb = new QComboBox(parent);
        cb->addItem("Si", true);
        cb->addItem("No", false);

        if (v.trimmed() == "false") {
            cb->setCurrentIndex(1);
        } else {
            cb->setCurrentIndex(0);
        }
        return cb;

    } else {
        return QStyledItemDelegate::createEditor(parent, option, index);
    }


}


void ItemDelegate::setEditorData(QWidget* editor, QModelIndex const& index) const
{
    if (QComboBox* cb = qobject_cast<QComboBox*>(editor)) {
        // get the index of the text in the combobox that matches the current value of the itenm
        QString currentText = index.data(Qt::EditRole).toString();
        int cbIndex = cb->findText(currentText);
        // if it is valid, adjust the combobox
        if (cbIndex >= 0)
            cb->setCurrentIndex(cbIndex);
    } else {
        QStyledItemDelegate::setEditorData(editor, index);
    }
}


void ItemDelegate::setModelData(
    QWidget*            editor,
    QAbstractItemModel* model,
    QModelIndex const&  index
    ) const
{

    auto item = static_cast<QJsonTreeItem*>(index.internalPointer());
    auto keys = item->key().split(":");
    auto k = 1 == keys.size() ? keys.at(0).trimmed() : keys.at(1).trimmed();

    if ("Archivo" == k) {
        if (QFileDialog* fd = qobject_cast<QFileDialog*>(editor)) {
            auto files = fd->selectedFiles();
            if (files.size() > 0) {
                model->setData(index, files.at(0), Qt::EditRole);
            }
        }
        else {
            QStyledItemDelegate::setModelData(editor, model, index);
        }
    } else if (
        ("Visibilidad inicial"          == k) ||
        ("Forzar superficie doble cara" == k) ||
        ("Forzar color en mallas"       == k)) {
        if (QComboBox* cb = qobject_cast<QComboBox*>(editor)) {
            QString data = cb->currentData().toBool() ? "true" : "false";
            model->setData(index, data, Qt::EditRole);
        }
        else {
            QStyledItemDelegate::setModelData(editor, model, index);
        }
    }

}
