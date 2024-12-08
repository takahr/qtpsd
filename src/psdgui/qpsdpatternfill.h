// Copyright (C) 2024 Signal Slot Inc.
// SPDX-License-Identifier: LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QPSDPATTERNFILL_H
#define QPSDPATTERNFILL_H

#include <QtPsdGui/qpsdguiglobal.h>
#include <QtPsdCore/QPsdDescriptor>
#include <QtPsdCore/qpsdblend.h>

QT_BEGIN_NAMESPACE

class Q_PSDGUI_EXPORT QPsdPatternFill
{
public:
    QPsdPatternFill(const QPsdDescriptor &descriptor);
    ~QPsdPatternFill();

    QPsdBlend::Mode blendMode() const;
    qreal opacity() const;
    QString patternID() const;
    bool aligned() const;
    qreal angle() const;
    qreal scale() const;
    QPointF phase() const;

private:
    class Private;
    QScopedPointer<Private> d;
};

#endif // QPSDPATTERNFILL_H
