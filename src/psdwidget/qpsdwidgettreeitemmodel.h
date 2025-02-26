#ifndef QPSDWIDGETTREEITEMMODEL_H
#define QPSDWIDGETTREEITEMMODEL_H

#include <qpsdwidgetglobal.h>
#include <QtPsdGui/QPsdGuiLayerTreeItemModel>

QT_BEGIN_NAMESPACE

class Q_PSDWIDGET_EXPORT QPsdWidgetTreeItemModel : public QPsdGuiLayerTreeItemModel
{
    Q_OBJECT
public:
    enum Roles {
        VisibleRole = QPsdGuiLayerTreeItemModel::LayerItemObjectRole + 1,
    };

    explicit QPsdWidgetTreeItemModel(QObject *parent = nullptr);
    ~QPsdWidgetTreeItemModel() override;

    QHash<int, QByteArray> roleNames() const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    bool isVisible(const QModelIndex &index) const;
    void setVisible(const QModelIndex &index, bool visible);

private:
    class Private;
    QScopedPointer<Private> d;
};

QT_END_NAMESPACE

#endif // QPSDWIDGETTREEITEMMODEL_H
