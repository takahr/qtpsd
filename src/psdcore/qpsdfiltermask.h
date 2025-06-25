// Copyright (C) 2025 Signal Slot Inc.
// SPDX-License-Identifier: LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QPSDFILTERMASK_H
#define QPSDFILTERMASK_H

#include <QtPsdCore/qpsdcoreglobal.h>
#include <QtPsdCore/qpsdcolorspace.h>

#include <QtCore/QSharedDataPointer>
#include <QtCore/QMetaType>

QT_BEGIN_NAMESPACE

class Q_PSDCORE_EXPORT QPsdFilterMask
{
public:
    QPsdFilterMask();
    QPsdFilterMask(const QPsdFilterMask &other);
    QPsdFilterMask &operator=(const QPsdFilterMask &other);
    QPsdFilterMask(QPsdFilterMask &&other) noexcept;
    QPsdFilterMask &operator=(QPsdFilterMask &&other) noexcept;
    ~QPsdFilterMask();

    void swap(QPsdFilterMask &other) noexcept
    { d.swap(other.d); }

    QPsdColorSpace colorSpace() const;
    void setColorSpace(const QPsdColorSpace &colorSpace);

    qreal opacity() const;
    void setOpacity(qreal opacity);

    bool isValid() const;

    bool operator==(const QPsdFilterMask &other) const;
    bool operator!=(const QPsdFilterMask &other) const
    { return !(*this == other); }

private:
    class Private;
    QSharedDataPointer<Private> d;
};

Q_DECLARE_SHARED(QPsdFilterMask)

QT_END_NAMESPACE

Q_DECLARE_METATYPE(QPsdFilterMask)

#endif // QPSDFILTERMASK_H