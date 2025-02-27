#include "qpsdwidgettreeitemmodel.h"

class QPsdWidgetTreeItemModel::Private
{
public:
    Private(const ::QPsdWidgetTreeItemModel *model);
    ~Private();

    bool isValidIndex(const QModelIndex &index) const;

    bool isVisible(const QModelIndex &index);
    void setVisible(const QModelIndex &index, bool visible);

    const ::QPsdWidgetTreeItemModel *q;
    QMap<QString, bool> visibleMap;
};

QPsdWidgetTreeItemModel::Private::Private(const ::QPsdWidgetTreeItemModel *model) : q(model)
{
}

QPsdWidgetTreeItemModel::Private::~Private()
{
}

bool QPsdWidgetTreeItemModel::Private::isValidIndex(const QModelIndex &index) const
{
    return index.isValid() && index.model() == q;
}

bool QPsdWidgetTreeItemModel::Private::isVisible(const QModelIndex &index)
{
    if (!isValidIndex(index)) {
        return false;
    }

    const auto *item = q->layerItem(index);
    if (item == nullptr) {
        return false;
    }
    QString idstr = QString::number(item->id());

    if (!visibleMap.contains(idstr)) {
        visibleMap.insert(idstr, item->isVisible());
    }

    return visibleMap.value(idstr);
}

void QPsdWidgetTreeItemModel::Private::setVisible(const QModelIndex &index, bool visible)
{
    if (!isValidIndex(index)) {
        return;
    }

    const auto *item = q->layerItem(index);
    if (item == nullptr) {
        return;
    }
    QString idstr = QString::number(item->id());

    visibleMap.insert(idstr, visible);
}

QPsdWidgetTreeItemModel::QPsdWidgetTreeItemModel(QObject *parent)
    : QPsdGuiLayerTreeItemModel(parent), d(new Private(this))
{
}

QPsdWidgetTreeItemModel::~QPsdWidgetTreeItemModel()
{
}

QHash<int, QByteArray> QPsdWidgetTreeItemModel::roleNames() const
{
    auto roles = QPsdGuiLayerTreeItemModel::roleNames();
    roles.insert(Roles::VisibleRole, QByteArrayLiteral("Visible"));

    return roles;
}

QVariant QPsdWidgetTreeItemModel::data(const QModelIndex &index, int role) const
{
    if (!d->isValidIndex(index))
        return QVariant();

    switch (role) {
    case Roles::VisibleRole:
        return isVisible(index);
    default:
        return QPsdGuiLayerTreeItemModel::data(index, role);
    }

    return {};
}

bool QPsdWidgetTreeItemModel::isVisible(const QModelIndex &index) const
{
    return d->isVisible(index);
}

void QPsdWidgetTreeItemModel::setVisible(const QModelIndex &index, bool visible)
{
    return d->setVisible(index, visible);
}
