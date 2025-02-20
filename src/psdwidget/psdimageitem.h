// Copyright (C) 2024 Signal Slot Inc.
// SPDX-License-Identifier: BSD-3-Clause

#ifndef PSDIMAGEITEM_H
#define PSDIMAGEITEM_H

#include <QtPsdWidget/psdabstractitem.h>
#include <QtPsdGui/QPsdImageLayerItem>

QT_BEGIN_NAMESPACE

class PsdImageItem : public PsdAbstractItem
{
    Q_OBJECT
public:
    PsdImageItem(const QModelIndex &index, const QPsdImageLayerItem *psdData, const QPsdAbstractLayerItem *maskItem, const QMap<quint32, QString> group, QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;
};

QT_END_NAMESPACE

#endif // PSDIMAGEITEM_H
