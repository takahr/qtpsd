// Copyright (C) 2024 Signal Slot Inc.
// SPDX-License-Identifier: BSD-3-Clause

#ifndef PSDFOLDERITEM_H
#define PSDFOLDERITEM_H

#include "psdabstractitem.h"
#include <QtPsdGui/QPsdFolderLayerItem>

class PsdFolderItem : public PsdAbstractItem
{
    Q_OBJECT
public:
    PsdFolderItem(const QPsdFolderLayerItem *psdData, const QPsdAbstractLayerItem *maskItem, QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;
};

#endif // PSDFOLDERITEM_H
