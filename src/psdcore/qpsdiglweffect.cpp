// Copyright (C) 2024 Signal Slot Inc.
// SPDX-License-Identifier: LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qpsdiglweffect.h"

QT_BEGIN_NAMESPACE

class QPsdIglwEffect::Private : public QSharedData
{
public:
    bool invert = false;
};

QPsdIglwEffect::QPsdIglwEffect()
    : QPsdOglwEffect()
    , d(new Private)
{}

QPsdIglwEffect::QPsdIglwEffect(const QPsdIglwEffect &other)
    : QPsdOglwEffect(other)
    , d(other.d)
{}

QPsdIglwEffect &QPsdIglwEffect::operator=(const QPsdIglwEffect &other)
{
    if (this != &other) {
        QPsdOglwEffect::operator=(other);
        d.operator=(other.d);
    }
    return *this;
}

QPsdIglwEffect::~QPsdIglwEffect() = default;

bool QPsdIglwEffect::invert() const
{
    return d->invert;
}

void QPsdIglwEffect::setInvert(quint8 invert)
{
    d->invert = (invert != 0);
}

QT_END_NAMESPACE
