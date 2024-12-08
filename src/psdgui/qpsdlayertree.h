// Copyright (C) 2024 Signal Slot Inc.
// SPDX-License-Identifier: LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QPSDLAYERTREE_H
#define QPSDLAYERTREE_H

#include <QtPsdGui/qpsdguiglobal.h>
#include <QtPsdCore/qpsdparser.h>

QT_BEGIN_NAMESPACE

class QPsdFolderLayerItem;

class Q_PSDGUI_EXPORT QPsdLayerTree
{
public:
    static QPsdFolderLayerItem *fromParser(const QPsdParser &parser);
};

QT_END_NAMESPACE

#endif // QPSDLAYERTREE_H
