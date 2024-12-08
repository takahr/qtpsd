// Copyright (C) 2024 Signal Slot Inc.
// SPDX-License-Identifier: LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QPSDBORDER_H
#define QPSDBORDER_H

#include <QtPsdGui/qpsdguiglobal.h>
#include <QtPsdCore/QPsdDescriptor>
#include <QtPsdCore/qpsdblend.h>

QT_BEGIN_NAMESPACE

class Q_PSDGUI_EXPORT QPsdBorder
{
public:
    QPsdBorder(const QPsdDescriptor &descriptor);
    ~QPsdBorder();

    bool isEnable() const;
    int size() const;
    enum Position {
        Outer,
        Inner,
        Center,
    };
    Position position() const;
    QPsdBlend::Mode blendMode() const;
    qreal opacity() const;
    bool isOverPrint() const;
    enum FillType {
        Solid,
        Gradient,
        Pattern,
    };
    FillType fillType() const;
    QColor color() const;

private:
    class Private;
    QScopedPointer<Private> d;
};

QT_END_NAMESPACE

#endif // QPSDBORDER_H
