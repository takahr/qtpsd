// Copyright (C) 2024 Signal Slot Inc.
// SPDX-License-Identifier: LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QPSDABSTRACTEFFECT_H
#define QPSDABSTRACTEFFECT_H

#include <QtPsdCore/qpsdcoreglobal.h>
#include <QtCore/QSharedDataPointer>

QT_BEGIN_NAMESPACE

class Q_PSDCORE_EXPORT QPsdAbstractEffect
{
public:
    enum Type {
        Unknown,
        CommonState,
        DropShadow,
        InnerShadow,
        OuterGlow,
        InnerGlow,
        Bevel,
        SolidFill,
    };
    QPsdAbstractEffect();
    QPsdAbstractEffect(const QPsdAbstractEffect &other);
    QPsdAbstractEffect &operator=(const QPsdAbstractEffect &other);
    void swap(QPsdAbstractEffect &other) noexcept { d.swap(other.d); }
    virtual ~QPsdAbstractEffect();

    virtual Type type() const = 0;
    bool isEnabled() const;
    void setEnabled(int enabled);

private:
    class Private;
    QSharedDataPointer<Private> d;
};

QT_END_NAMESPACE

#endif // QPSDABSTRACTEFFECT_H
