// Copyright (C) 2024 Signal Slot Inc.
// SPDX-License-Identifier: BSD-3-Clause

#include "qpsdtreeitemmodel.h"

#include <QtPsdCore/QPsdParser>
#include <QtPsdGui/QPsdFolderLayerItem>
#include <QtPsdGui/QPsdGuiLayerTreeItemModel>
#include <QtPsdGui/QPsdLayerTree>

#include <QtCore/QDir>
#include <QtCore/QFileInfo>
#include <QtCore/QJsonArray>
#include <QtCore/QJsonDocument>
#include <QtGui/QBrush>
#include <QtGui/QIcon>

class QPsdTreeItemModel::Private
{
public:
    Private(const ::QPsdTreeItemModel *model);
    ~Private();

    QFileInfo hintFileInfo(const QString &psdFileName) const;
    QJsonDocument loadHint(const QString &hintFileName);

    bool isValidIndex(const QModelIndex &index) const;

    bool isVisible(const QModelIndex &index);
    void setVisible(const QModelIndex &index, bool visible);

    void updateLayerTreeHint();

    const ::QPsdTreeItemModel *q;
    QString fileName;
    QFileInfo fileInfo;
    QString errorMessage;

    QPsdFolderLayerItem *root = nullptr;

    QMap<QString, QPsdAbstractLayerItem::ExportHint> layerHints;
    QMap<QString, QVariantMap> exportHints;

    QMap<QString, bool> visibleMap;
};

#define HINTFILE_MAGIC_KEY "qtpsdparser.hint"_L1
#define HINTFILE_MAGIC_VERSION 1
#define HINTFILE_LAYER_HINTS_KEY "layers"_L1
#define HINTFILE_EXPORT_HINTS_KEY "exports"_L1

QPsdTreeItemModel::Private::Private(const ::QPsdTreeItemModel *model) : q(model)
{
}

QPsdTreeItemModel::Private::~Private()
{
}

QFileInfo QPsdTreeItemModel::Private::hintFileInfo(const QString &psdFileName) const
{
    QFileInfo fileInfo(psdFileName);
    QString hintFileName = u"%1.%2"_s.arg(
        fileInfo.suffix().toLower() == "psd"_L1 ? fileInfo.completeBaseName() : fileInfo.fileName(), "psd_"_L1);

    return QFileInfo(fileInfo.dir(), hintFileName);
}

QJsonDocument QPsdTreeItemModel::Private::loadHint(const QString &hintFileName)
{
    QFile file(hintFileName);
    if (file.open(QIODevice::ReadOnly)) {
        QByteArray buf = file.readAll();
        file.close();

        QJsonDocument doc = QJsonDocument::fromJson(buf);
        QJsonObject root = doc.object();
        if (root.value(HINTFILE_MAGIC_KEY) == HINTFILE_MAGIC_VERSION) {
            return doc;
        }
    }

    return {};
}

bool QPsdTreeItemModel::Private::isValidIndex(const QModelIndex &index) const
{
    return index.isValid() && index.model() == q;
}

bool QPsdTreeItemModel::Private::isVisible(const QModelIndex &index)
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

void QPsdTreeItemModel::Private::setVisible(const QModelIndex &index, bool visible)
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

void QPsdTreeItemModel::Private::updateLayerTreeHint()
{
    std::function<void(const QPsdAbstractLayerItem *)> traverseTree;
    traverseTree = [&](const QPsdAbstractLayerItem *item) {
        const QString idstr = QString::number(item->id());
        QPsdAbstractLayerItem::ExportHint exportHint = layerHints.value(idstr);
        item->setExportHint(exportHint);
        switch (item->type()) {
        case QPsdAbstractLayerItem::Folder: {
            auto folder = reinterpret_cast<const QPsdFolderLayerItem *>(item);
            for (const auto *child : folder->children()) {
                traverseTree(child);
            }
            break; }
        default:
            break;
        }
    };
    traverseTree(root);
}

QPsdTreeItemModel::QPsdTreeItemModel(QObject *parent)
    : QPsdGuiLayerTreeItemModel(parent), d(new Private(this))
{
}

QPsdTreeItemModel::~QPsdTreeItemModel()
{
}

QVariant QPsdTreeItemModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    switch (role) {
    case Qt::DisplayRole:
        switch (section) {
        case Column::Name:
            return "Layers"_L1;
        case Column::Visible:
            return "Visible"_L1;
        case Column::Export:
            return "Export"_L1;
        }
        break;
    case Qt::TextAlignmentRole:
        return Qt::AlignCenter;
    default:
        break;
    }

    return QAbstractItemModel::headerData(section, orientation, role);
}

QHash<int, QByteArray> QPsdTreeItemModel::roleNames() const
{
    auto roles = QPsdGuiLayerTreeItemModel::roleNames();
    roles.insert(Roles::VisibleRole, QByteArrayLiteral("Visible"));
    roles.insert(Roles::ExportIdRole, QByteArrayLiteral("ExportId"));

    return roles;
}

int QPsdTreeItemModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 3;
}

QVariant QPsdTreeItemModel::data(const QModelIndex &index, int role) const
{
    if (!d->isValidIndex(index))
        return QVariant();

    const QPsdAbstractLayerItem *item = layerItem(index);
    const QPsdAbstractLayerItem::ExportHint exportHint = layerHint(index);
    switch (role) {
    case Qt::DisplayRole:
        switch (index.column()) {
        case Column::Name:
            return item->name();
        case Column::Export:
            return exportHint.id;
        default:
            break;
        }
        break;
    case Qt::EditRole:
        switch (index.column()) {
        case Column::Export:
            return exportHint.id;
        default:
            break;
        }
        break;
    case Qt::TextAlignmentRole:
        switch (index.column()) {
        case Column::Visible:
            return Qt::AlignHCenter;
        default:
            break;
        }
        break;
    case Qt::CheckStateRole:
        switch (index.column()) {
        case Column::Visible:
            return d->isVisible(index) ? Qt::Checked : Qt::Unchecked;
        default:
            break;
        }
        break;
    case Qt::BackgroundRole: {
        QColor color = item->color();
        int h, s, l, a;
        color.getHsl(&h, &s, &l, &a);
        color.setHsl(h, 128, 128, 128 * a / 255);
        return QBrush(color); }
    case Qt::DecorationRole:
        switch (index.column()) {
        case Column::Name:
            return QIcon(QPixmap::fromImage(item->image()));
        default:
            break;
        }
        break;
    case Roles::VisibleRole:
        return isVisible(index);
    case Roles::ExportIdRole:
        return exportHint.id;
    default:
        return QPsdGuiLayerTreeItemModel::data(index, role);
    }

    return {};
}

bool QPsdTreeItemModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    switch (index.column()) {
    case Column::Visible:
        if (role == Qt::CheckStateRole) {
            d->setVisible(index, value.toBool());

            emit dataChanged(index, index);
            return true;
        }
        break;
    case Column::Export: {
        QPsdAbstractLayerItem::ExportHint exportHint = layerHint(index);
        exportHint.id = value.toString();
        setLayerHint(index, exportHint);
        return true; }
    default:
        break;
    }

    return false;
}

Qt::ItemFlags QPsdTreeItemModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags flags = QAbstractItemModel::flags(index);
    switch (index.column()) {
    case Column::Visible:
        return flags | Qt::ItemIsUserCheckable;
    case Column::Export:
        return flags | Qt::ItemIsEditable;
    default:
        break;
    }

    return flags;
}

bool QPsdTreeItemModel::isVisible(const QModelIndex &index) const
{
    return d->isVisible(index);
}

QString QPsdTreeItemModel::exportId(const QModelIndex &index) const
{
    QPsdAbstractLayerItem::ExportHint exportHint = layerHint(index);

    return exportHint.id;
}

QFileInfo QPsdTreeItemModel::fileInfo() const
{
    return d->fileInfo;
}

QString QPsdTreeItemModel::fileName() const
{
    return d->fileName;
}

QString QPsdTreeItemModel::errorMessage() const
{
    return d->errorMessage;
}

const QPsdFolderLayerItem *QPsdTreeItemModel::layerTree() const
{
    d->updateLayerTreeHint();
    return d->root;
}

QVariantMap QPsdTreeItemModel::exportHint(const QString& exporterKey) const
{
    return d->exportHints.value(exporterKey);
}

void QPsdTreeItemModel::updateExportHint(const QString &exporterKey, const QVariantMap &hint)
{
    d->exportHints.insert(exporterKey, hint);
}

QPsdAbstractLayerItem::ExportHint QPsdTreeItemModel::layerHint(const QModelIndex &index) const
{
    const QPsdAbstractLayerItem *item = layerItem(index);
    const QString idstr = QString::number(item->id());

    return d->layerHints.value(idstr);
}

void QPsdTreeItemModel::setLayerHint(const QModelIndex &index, const QPsdAbstractLayerItem::ExportHint exportHint)
{
    const QPsdAbstractLayerItem *item = layerItem(index);
    const QString idstr = QString::number(item->id());

    d->layerHints.insert(idstr, exportHint);
    item->setExportHint(exportHint);

    emit dataChanged(index, index);
}

void QPsdTreeItemModel::load(const QString &fileName)
{
    d->fileInfo = QFileInfo(fileName);
    d->fileName = fileName;
    if (!d->fileInfo.exists()) {
        setErrorMessage(tr("File not found"));
        return;
    }
    emit fileInfoChanged(d->fileInfo);

    QPsdParser parser;
    parser.load(fileName);

    fromParser(parser);

    const auto header = parser.fileHeader();
    if (!header.errorString().isEmpty()) {
        setErrorMessage(header.errorString());
        return;
    }

    QFileInfo hintFileInfo = d->hintFileInfo(fileName);
    if (hintFileInfo.exists()) {
        QJsonDocument hintDoc = d->loadHint(hintFileInfo.absoluteFilePath());
        QJsonObject root = hintDoc.object();
        QJsonObject layerHints = root.value(HINTFILE_LAYER_HINTS_KEY).toObject();
        for (const auto &idstr: layerHints.keys()) {
            QVariantMap settings = layerHints.value(idstr).toObject().toVariantMap();
            QStringList properties = settings.value("properties"_L1).toStringList();
            QPsdFolderLayerItem::ExportHint exportHint {
                settings.value("id"_L1).toString(),
                static_cast<QPsdAbstractLayerItem::ExportHint::Type>(settings.value("type"_L1).toInt()),
                settings.value("name"_L1).toString(),
                static_cast<QPsdAbstractLayerItem::ExportHint::NativeComponent>(settings.value("native"_L1).toInt()),
                settings.value("visible"_L1).toBool(),
                QSet<QString>(properties.begin(), properties.end()),
            };
            d->layerHints.insert(idstr, exportHint);
        }

        QJsonObject exportHints = root.value(HINTFILE_EXPORT_HINTS_KEY).toObject();
        for (const auto &exporterKey : exportHints.keys()) {
            QVariantMap map = exportHints.value(exporterKey).toObject().toVariantMap();
            d->exportHints.insert(exporterKey, map);
        }
    }
    d->root = QPsdLayerTree::fromParser(parser);

    std::function<void(const QModelIndex &parent)> traverseTree;
    traverseTree = [&](const QModelIndex &parent) {
        for (int row = 0; row < rowCount(parent); row++) {
            const QModelIndex childIndex = index(row, 0, parent);
            const QPsdAbstractLayerItem *item = layerItem(childIndex);
            const QString idstr = QString::number(item->id());
            QPsdAbstractLayerItem::ExportHint exportHint = d->layerHints.value(idstr);
            exportHint.visible = item->isVisible();
            d->layerHints.insert(idstr, exportHint);
            item->setExportHint(exportHint);

            if (hasChildren(childIndex)) {
                traverseTree(childIndex);
            }
        }
    };
    traverseTree(QModelIndex());
}

void QPsdTreeItemModel::save()
{
    QFileInfo hintFileInfo = d->hintFileInfo(fileName());

    QJsonDocument doc;
    QJsonObject layerHints;
    std::function<void(const QModelIndex &)> traverse = [&](const QModelIndex &index) {
        if (index.isValid()) {
            const auto layer = layerItem(index);
            const auto lyid = layer->id();
            const auto idstr = QString::number(lyid);

            const auto exportHint = layerHint(index);
            if (!exportHint.isDefaultValue()) {
                QStringList propList = exportHint.properties.values();
                std::sort(propList.begin(), propList.end(), std::less<QString>());
                QJsonObject object;
                if (!exportHint.id.isEmpty()) {
                    object.insert("id"_L1, exportHint.id);
                }
                object.insert("type"_L1, static_cast<int>(exportHint.type));
                if (!exportHint.componentName.isEmpty()) {
                    object.insert("name"_L1, exportHint.componentName);
                }
                object.insert("native"_L1, static_cast<int>(exportHint.baseElement));
                object.insert("visible"_L1, exportHint.visible);
                if (!propList.isEmpty())
                    object.insert("properties"_L1, QJsonArray::fromStringList(propList));
                layerHints.insert(idstr, object);
            }
        }
        for (int i = 0; i < rowCount(index); i++) {
            traverse(this->index(i, 0, index));
        }
    };
    traverse({});

    QJsonObject exportHints;
    for (const auto &exporterKey : d->exportHints.keys()) {
        exportHints.insert(exporterKey, QJsonObject::fromVariantMap(d->exportHints.value(exporterKey)));
    }

    QJsonObject root;
    root.insert(HINTFILE_MAGIC_KEY, HINTFILE_MAGIC_VERSION);
    root.insert(HINTFILE_LAYER_HINTS_KEY, layerHints);
    root.insert(HINTFILE_EXPORT_HINTS_KEY, exportHints);

    doc.setObject(root);

    QFile file(hintFileInfo.absoluteFilePath());
    if (!file.open(QIODevice::WriteOnly))
        return;
    file.write(doc.toJson());
    file.close();
}

void QPsdTreeItemModel::setErrorMessage(const QString &errorMessage)
{
    if (d->errorMessage == errorMessage) return;
    d->errorMessage = errorMessage;
    emit errorOccurred(errorMessage);
}
