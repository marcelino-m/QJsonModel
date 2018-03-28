#include <QStyledItemDelegate>



class ItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    ItemDelegate(QObject* parent=0);
    ~ItemDelegate();

    QWidget*
    createEditor(
        QWidget*                    parent,
        QStyleOptionViewItem const& option,
        QModelIndex const&          index
        ) const;

    void
    setEditorData(QWidget* editor, QModelIndex const& index) const;

    void
    setModelData(
        QWidget*            editor,
        QAbstractItemModel* model,
        QModelIndex const&  index
        ) const;
};
