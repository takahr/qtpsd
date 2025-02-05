// Copyright (C) 2024 Signal Slot Inc.
// SPDX-License-Identifier: LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qpsdexporterplugin.h"

#include <QtCore/QCryptographicHash>

QT_BEGIN_NAMESPACE

QMimeDatabase QPsdExporterPlugin::mimeDatabase;

class QPsdExporterPlugin::Private {
public:
    Private(QPsdExporterPlugin *parent);
    ~Private() = default;
        
    static void findChildren(const QPsdAbstractLayerItem *item, QRect *rect);
    void generateRectMap(const QPsdAbstractLayerItem *item, const QPoint &topLeft) const;
    bool generateMergeData(const QPsdAbstractLayerItem *item) const;

    void generateChildrenRectMap(const QPersistentModelIndex &index) const;
    void generateIndexMap(const QPersistentModelIndex &index, const QPoint &topLeft) const;

    QPsdExporterPlugin *q;
    const PsdTreeItemModel *model = nullptr;
};

QPsdExporterPlugin::Private::Private(QPsdExporterPlugin *parent) : q(parent)
{}

void QPsdExporterPlugin::Private::findChildren(const QPsdAbstractLayerItem *item, QRect *rect)
{
    switch (item->type()) {
    case QPsdAbstractLayerItem::Folder: {
        const auto folder = reinterpret_cast<const QPsdFolderLayerItem *>(item);
        for (const auto *child : folder->children()) {
            findChildren(child, rect);
        }
        break; }
    default:
        *rect |= item->rect();
        break;
    }
}

void QPsdExporterPlugin::Private::generateRectMap(const QPsdAbstractLayerItem *item, const QPoint &topLeft) const
{
    switch (item->type()) {
    case QPsdAbstractLayerItem::Folder: {
        const auto folder = reinterpret_cast<const QPsdFolderLayerItem *>(item);
        QRect contentRect;
        if (!item->parent()->parent()) {
            contentRect = item->parent()->rect();
            // if (item->parent() == tree) {
            //     contentRect = tree->rect();
        } else {
            for (const auto *child : folder->children()) {
                findChildren(child, &contentRect);
            }
        }
        q->rectMap.insert(item, contentRect.translated(-topLeft));
        for (const auto *child : folder->children()) {
            generateRectMap(child, contentRect.topLeft());
        }
        break; }
    default:
        q->rectMap.insert(item, item->rect().translated(-topLeft));
        break;
    }
}

bool QPsdExporterPlugin::Private::generateMergeData(const QPsdAbstractLayerItem *item) const
{
    switch (item->type()) {
    case QPsdAbstractLayerItem::Folder: {
        const auto folder = reinterpret_cast<const QPsdFolderLayerItem *>(item);
        auto children = folder->children();
        std::reverse(children.begin(), children.end());
        for (const auto *child : children) {
            if (!generateMergeData(child))
                return false;
        }
        break; }
    default: {
        const auto hint = item->exportHint();
        if (hint.type != QPsdAbstractLayerItem::ExportHint::Merge)
            return true;
        const auto group = item->group();
        for (const auto *i : group) {
            if (i == item)
                continue;
            if (i->name() == hint.componentName) {
                q->mergeMap.insert(i, item);
            }
        }
        break; }
    }

    return true;
}

void QPsdExporterPlugin::Private::generateChildrenRectMap(const QPersistentModelIndex &index) const
{
    QRect childrenRect;

    bool hasChildren = model->hasChildren(index);
    if (hasChildren) {
        for (int i = model->rowCount(index) -1; i >= 0; i--) {
            QModelIndex childIndex = model->index(i, 0, index);
            generateChildrenRectMap(childIndex);

            childrenRect |= q->childrenRectMap.value(childIndex);
        }
    }

    if (index.isValid()) {
        if (hasChildren) {
            if (!model->parent(index).isValid()) {
                q->childrenRectMap.insert(index, QRect { { 0, 0}, model->size() });
            } else {
                q->childrenRectMap.insert(index, childrenRect);
            }
        } else {
            q->childrenRectMap.insert(index, model->rect(index));
        }
    }
}

void QPsdExporterPlugin::Private::generateIndexMap(const QPersistentModelIndex &index, const QPoint &topLeft) const
{
    QPoint childTopLeft = topLeft;
    if (index.isValid()) {
        QRect contentRect = q->childrenRectMap.value(index);
        q->indexRectMap.insert(index, contentRect.translated(-topLeft));
        
        childTopLeft = contentRect.topLeft();

        const auto hint = model->layerHint(index);
        if (hint.type == QPsdAbstractLayerItem::ExportHint::Merge) {
            const auto groupIndexes = model->groupIndexes(index);
            for (const auto &i : groupIndexes) {
                if (i == index)
                    continue;
                if (model->layerName(i) == hint.componentName) {
                    q->indexMergeMap.insert(i, index);
                }
            }
        }
    }

    for (int i = 0; i < model->rowCount(index); i++) {
        QModelIndex childIndex = model->index(i, 0, index);

        generateIndexMap(childIndex, childTopLeft);
    }
}

QPsdExporterPlugin::QPsdExporterPlugin(QObject *parent)
    : QPsdAbstractPlugin(parent), d(new Private(this))
{}

QPsdExporterPlugin::~QPsdExporterPlugin()
{}

const PsdTreeItemModel *QPsdExporterPlugin::model() const
{
    return d->model;
}

void QPsdExporterPlugin::setModel(const PsdTreeItemModel *model) const
{
    d->model = model;
}

namespace {
static const QRegularExpression notPrintable("[^a-zA-Z0-9]"_L1);
}

QString QPsdExporterPlugin::toUpperCamelCase(const QString &str, const QString &separator)
{
    QString s = str;
    s.replace(QRegularExpression("([a-z0-9])([A-Z])"_L1), "\\1 \\2"_L1);
    QStringList parts = s.split(notPrintable, Qt::SkipEmptyParts);
    for (auto &part : parts) {
        part = part.toLower();
        part[0] = part[0].toUpper();
    }
    return parts.join(separator);
}

QString QPsdExporterPlugin::toLowerCamelCase(const QString &str)
{
    QString s = str;
    s.replace(QRegularExpression("([a-z0-9])([A-Z])"_L1), "\\1 \\2"_L1);
    QStringList parts = s.split(notPrintable, Qt::SkipEmptyParts);
    bool first = true;
    for (auto &part : parts) {
        part = part.toLower();
        if (first) {
            first = false;
        } else {
            part[0] = part[0].toUpper();
        }
    }
    return parts.join(QString());
}

QString QPsdExporterPlugin::toSnakeCase(const QString &str)
{
    QString s = str;
    s.replace(QRegularExpression("([a-z0-9])([A-Z])"_L1), "\\1 \\2"_L1);
    QStringList parts = s.split(notPrintable, Qt::SkipEmptyParts);
    return parts.join("_"_L1).toLower();
}

QString QPsdExporterPlugin::toKebabCase(const QString &str)
{
    QString s = str;
    s.replace(QRegularExpression("([a-z0-9])([A-Z])"_L1), "\\1 \\2"_L1);
    QStringList parts = s.split(notPrintable, Qt::SkipEmptyParts);
    return parts.join("-"_L1).toLower();
}

QString QPsdExporterPlugin::imageFileName(const QString &name, const QString &format)
{
    QFileInfo fileInfo(name);
    QString suffix = fileInfo.suffix();
    QString basename = fileInfo.completeBaseName();
    
    QString snakeName = toSnakeCase(basename);
    if (snakeName.length() < basename.length()) {
        basename = QString::fromLatin1(QCryptographicHash::hash(basename.toUtf8(), QCryptographicHash::Sha256).toHex());
    } else {
        if (suffix.isEmpty()) {
            return u"%1.%2"_s.arg(snakeName, format.toLower());
        }

        if (suffix.compare(format, Qt::CaseInsensitive) == 0) {
            return u"%1.%2"_s.arg(basename, suffix);
        }

        if ((format.compare("jpg"_L1, Qt::CaseInsensitive) || format.compare("jpeg"_L1, Qt::CaseInsensitive))
            && (suffix.compare("jpg"_L1, Qt::CaseInsensitive) || suffix.compare("jpeg"_L1, Qt::CaseInsensitive))) {
            return u"%1.%2"_s.arg(basename, suffix);
        }

        basename = snakeName;
    }

    return u"%1.%2"_s.arg(basename, format.toLower());
}

bool QPsdExporterPlugin::generateMaps() const
{
    rectMap.clear();
    mergeMap.clear();

    auto children = d->model->layerTree()->children();
    for (const auto *child : children) {
        d->generateRectMap(child, QPoint(0, 0));
        if (!d->generateMergeData(child))
            return false;
    }

    childrenRectMap.clear();
    indexRectMap.clear();
    indexMergeMap.clear();

    d->generateChildrenRectMap({});
    d->generateIndexMap({}, QPoint(0, 0));

    return true;
}

QT_END_NAMESPACE
