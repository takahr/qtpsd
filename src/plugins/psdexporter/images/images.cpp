// Copyright (C) 2024 Signal Slot Inc.
// SPDX-License-Identifier: LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include <QtPsdGui/qpsdexporterplugin.h>
#include <QtGui/QImage>
#include <QtCore/QDir>

QT_BEGIN_NAMESPACE

class QPsdExporterImagePlugin : public QPsdExporterPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QPsdExporterFactoryInterface" FILE "images.json")
public:
    int priority() const override { return 100; }
    QIcon icon() const override {
        auto mimeType = mimeDatabase.mimeTypeForName("image/png");
        return QIcon::fromTheme(mimeType.iconName());
    }
    QString name() const override {
        return tr("&Images");
    }
    ExportType exportType() const override { return QPsdExporterPlugin::Directory; }

    bool exportTo(const QPsdFolderLayerItem *tree, const QString &to, const QVariantMap &hint) const override;
};

bool QPsdExporterImagePlugin::exportTo(const QPsdFolderLayerItem *tree, const QString &to, const QVariantMap &hint) const
{
    const auto imageScaling = hint.value("imageScaling", false).toBool();
    std::function<void(const QPsdAbstractLayerItem *, QDir *)> traverseTree;
    traverseTree = [&](const QPsdAbstractLayerItem *item, QDir *directory) {
        switch (item->type()) {
        case QPsdAbstractLayerItem::Folder: {
            auto folder = reinterpret_cast<const QPsdFolderLayerItem *>(item);
            directory->mkdir(folder->name());
            directory->cd(folder->name());
            for (const auto *child : folder->children()) {
                traverseTree(child, directory);
            }
            bool isEmpty = directory->isEmpty();
            directory->cdUp();
            if (isEmpty)
                directory->rmdir(folder->name());
            break; }
        case QPsdAbstractLayerItem::Image: {
            auto imageItem = reinterpret_cast<const QPsdImageLayerItem *>(item);
            QImage image = imageItem->linkedImage();
            QString name = imageItem->linkedFile().name;
            if (image.isNull()) {
                image = item->image();
                name = item->name() + ".png"_L1;
            }
            if (imageScaling)
                image = image.scaled(item->rect().size(), Qt::KeepAspectRatio);
            image.save(directory->filePath(name));
            break; }
        default:
            break;
        }
    };

    QDir dir(to);
    for (const auto *item : tree->children()) {
        traverseTree(item, &dir);
    }
    return true;
}

QT_END_NAMESPACE

#include "images.moc"
