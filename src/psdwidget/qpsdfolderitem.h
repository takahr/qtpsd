// Copyright (C) 2024 Signal Slot Inc.
// SPDX-License-Identifier: BSD-3-Clause

#ifndef QPSDFOLDERITEM_H
#define QPSDFOLDERITEM_H

#include <QtPsdWidget/psdabstractitem.h>
#include <QtPsdGui/QPsdFolderLayerItem>

QT_BEGIN_NAMESPACE

class QPsdFolderItem : public QPsdAbstractItem
{
    Q_OBJECT
public:
    QPsdFolderItem(const QModelIndex &index, const QPsdFolderLayerItem *psdData, const QPsdAbstractLayerItem *maskItem, const QMap<quint32, QString> group, QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;
};

QT_END_NAMESPACE

#endif // QPSDFOLDERITEM_H
