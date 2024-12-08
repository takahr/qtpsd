// Copyright (C) 2024 Signal Slot Inc.
// SPDX-License-Identifier: LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QPSDIMAGELAYERITEM_H
#define QPSDIMAGELAYERITEM_H

#include <QtPsdGui/qpsdabstractlayeritem.h>

QT_BEGIN_NAMESPACE

class Q_PSDGUI_EXPORT QPsdImageLayerItem : public QPsdAbstractLayerItem
{
public:
    QPsdImageLayerItem(const QPsdLayerRecord &record);
    QPsdImageLayerItem();
    ~QPsdImageLayerItem() override;
    Type type() const override { return Image; }

    QImage linkedImage() const;

private:
    class Private;
    QScopedPointer<Private> d;
};

QT_END_NAMESPACE

#endif // QPSDIMAGELAYERITEM_H
