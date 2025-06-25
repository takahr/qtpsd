// Copyright (C) 2025 Signal Slot Inc.
// SPDX-License-Identifier: LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qpsdfiltermask.h"

#include <QtCore/QSharedData>

QT_BEGIN_NAMESPACE

class QPsdFilterMask::Private : public QSharedData
{
public:
    Private() = default;
    Private(const Private &other) = default;

    QPsdColorSpace colorSpace;
    qreal opacity = 1.0;
};

QPsdFilterMask::QPsdFilterMask()
    : d(new Private)
{
}

QPsdFilterMask::QPsdFilterMask(const QPsdFilterMask &other) = default;

QPsdFilterMask &QPsdFilterMask::operator=(const QPsdFilterMask &other) = default;

QPsdFilterMask::QPsdFilterMask(QPsdFilterMask &&other) noexcept = default;

QPsdFilterMask &QPsdFilterMask::operator=(QPsdFilterMask &&other) noexcept = default;

QPsdFilterMask::~QPsdFilterMask() = default;

QPsdColorSpace QPsdFilterMask::colorSpace() const
{
    return d->colorSpace;
}

void QPsdFilterMask::setColorSpace(const QPsdColorSpace &colorSpace)
{
    d->colorSpace = colorSpace;
}

qreal QPsdFilterMask::opacity() const
{
    return d->opacity;
}

void QPsdFilterMask::setOpacity(qreal opacity)
{
    d->opacity = qBound(0.0, opacity, 1.0);
}

bool QPsdFilterMask::isValid() const
{
    // A filter mask is valid if it has a valid color space
    return d->colorSpace.isValid();
}

bool QPsdFilterMask::operator==(const QPsdFilterMask &other) const
{
    return d->colorSpace == other.d->colorSpace &&
           qFuzzyCompare(d->opacity, other.d->opacity);
}

QT_END_NAMESPACE