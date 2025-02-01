// Copyright (C) 2024 Signal Slot Inc.
// SPDX-License-Identifier: LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qpsdexporterplugin.h"

#include <QtCore/QCryptographicHash>

QT_BEGIN_NAMESPACE

QMimeDatabase QPsdExporterPlugin::mimeDatabase;

QPsdExporterPlugin::QPsdExporterPlugin(QObject *parent)
    : QPsdAbstractPlugin(parent)
{}

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

void QPsdExporterPlugin::findChildren(const QPsdAbstractLayerItem *item, QRect *rect)
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

void QPsdExporterPlugin::generateRectMap(const QPsdAbstractLayerItem *item, const QPoint &topLeft) const
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
        rectMap.insert(item, contentRect.translated(-topLeft));
        for (const auto *child : folder->children()) {
            generateRectMap(child, contentRect.topLeft());
        }
        break; }
    default:
        rectMap.insert(item, item->rect().translated(-topLeft));
        break;
    }
}

bool QPsdExporterPlugin::generateMergeData(const QPsdAbstractLayerItem *item) const
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
                mergeMap.insert(i, item);
            }
        }
        break; }
    }

    return true;
}

bool QPsdExporterPlugin::generateMaps(const PsdTreeItemModel *model) const
{
    rectMap.clear();
    mergeMap.clear();

    auto children = model->layerTree()->children();
    for (const auto *child : children) {
        generateRectMap(child, QPoint(0, 0));
        if (!generateMergeData(child))
            return false;
    }

    return true;
}

QT_END_NAMESPACE
