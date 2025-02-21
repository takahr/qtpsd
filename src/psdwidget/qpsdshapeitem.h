// Copyright (C) 2024 Signal Slot Inc.
// SPDX-License-Identifier: BSD-3-Clause

#ifndef QPSDSHAPEITEM_H
#define QPSDSHAPEITEM_H

#include <QtPsdWidget/qpsdabstractitem.h>
#include <QtPsdGui/QPsdShapeLayerItem>

QT_BEGIN_NAMESPACE

class QPsdShapeItem : public QPsdAbstractItem
{
    Q_OBJECT
public:
    QPsdShapeItem(const QModelIndex &index, const QPsdShapeLayerItem *psdData, const QPsdAbstractLayerItem *maskItem, const QMap<quint32, QString> group, QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;
};

QT_END_NAMESPACE

#endif // QPSDSHAPEITEM_H
