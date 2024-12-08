// Copyright (C) 2024 Signal Slot Inc.
// SPDX-License-Identifier: LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qpsdimagelayeritem.h"

#include <QtCore/QBuffer>
#include <QtGui/QImageReader>

QT_BEGIN_NAMESPACE

class QPsdImageLayerItem::Private
{
public:
};

QPsdImageLayerItem::QPsdImageLayerItem(const QPsdLayerRecord &record)
    : QPsdAbstractLayerItem(record)
    , d(new Private)
{}

QPsdImageLayerItem::QPsdImageLayerItem()
    : QPsdAbstractLayerItem()
    , d(new Private)
{}

QPsdImageLayerItem::~QPsdImageLayerItem() = default;

QImage QPsdImageLayerItem::linkedImage() const
{
    const auto file = linkedFile();
    if (!file.type.isEmpty()) {
        QBuffer buffer;
        buffer.setData(file.data);
        buffer.open(QBuffer::ReadOnly);
        QImageReader reader(&buffer, file.type.trimmed());
        if (reader.canRead()) {
            QImage image;
            if (reader.read(&image)) {
                return image;
            }
            qWarning() << reader.errorString();
        } else {
            qWarning() << file.type << "not supported for" << file.name;
        }
    }
    return QImage();
}

QT_END_NAMESPACE
