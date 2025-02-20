// Copyright (C) 2024 Signal Slot Inc.
// SPDX-License-Identifier: BSD-3-Clause

#ifndef PSDTEXTITEM_H
#define PSDTEXTITEM_H

#include <QtPsdWidget/psdabstractitem.h>
#include <QtPsdGui/QPsdTextLayerItem>

QT_BEGIN_NAMESPACE

class PsdTextItem : public PsdAbstractItem
{
    Q_OBJECT
public:
    PsdTextItem(const QModelIndex &index, const QPsdTextLayerItem *psdData, const QPsdAbstractLayerItem *maskItem, const QMap<quint32, QString> group, QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;
};

QT_END_NAMESPACE

#endif // PSDTEXTITEM_H
