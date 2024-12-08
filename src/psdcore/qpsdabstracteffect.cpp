// Copyright (C) 2024 Signal Slot Inc.
// SPDX-License-Identifier: LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qpsdabstracteffect.h"

QT_BEGIN_NAMESPACE

class QPsdAbstractEffect::Private : public QSharedData
{
public:
    bool enabled = false;
};

QPsdAbstractEffect::QPsdAbstractEffect()
    : d(new Private)
{}

QPsdAbstractEffect::QPsdAbstractEffect(const QPsdAbstractEffect &other)
    : d(other.d)
{}

QPsdAbstractEffect &QPsdAbstractEffect::operator=(const QPsdAbstractEffect &other)
{
    if (this != &other)
        d.operator=(other.d);
    return *this;
}

QPsdAbstractEffect::~QPsdAbstractEffect() = default;

bool QPsdAbstractEffect::isEnabled() const
{
    return d->enabled;
}

void QPsdAbstractEffect::setEnabled(int enabled)
{
    d->enabled = (enabled != 0);
}
