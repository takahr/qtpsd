// Copyright (C) 2024 Signal Slot Inc.
// SPDX-License-Identifier: LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qpsdshadoweffect.h"

QT_BEGIN_NAMESPACE

class QPsdShadowEffect::Private : public QSharedData
{
public:
    Type type = Unknown;
    quint32 angle = 0;
    quint32 distance = 0;
    bool useAngleInAllEffects = false;
};

QPsdShadowEffect::QPsdShadowEffect()
    : QPsdOglwEffect()
    , d(new Private)
{}

QPsdShadowEffect::QPsdShadowEffect(Type type)
    : QPsdOglwEffect()
    , d(new Private)
{
    d->type = type;
}

QPsdShadowEffect::QPsdShadowEffect(const QPsdShadowEffect &other)
    : QPsdOglwEffect(other)
    , d(other.d)
{}

QPsdShadowEffect &QPsdShadowEffect::operator=(const QPsdShadowEffect &other)
{
    if (this != &other) {
        QPsdOglwEffect::operator=(other);
        d.operator=(other.d);
    }
    return *this;
}

QPsdShadowEffect::~QPsdShadowEffect() = default;

QPsdAbstractEffect::Type QPsdShadowEffect::type() const
{
    return d->type;
}

quint32 QPsdShadowEffect::angle() const
{
    return d->angle;
}

void QPsdShadowEffect::setAngle(quint32 angle)
{
    d->angle = angle;
}

quint32 QPsdShadowEffect::distance() const
{
    return d->distance;
}

void QPsdShadowEffect::setDistance(quint32 distance)
{
    d->distance = distance;
}

bool QPsdShadowEffect::useAngleInAllEffects() const
{
    return d->useAngleInAllEffects;
}

void QPsdShadowEffect::setUseAngleInAllEffects(quint8 useAngleInAllEffects)
{
    d->useAngleInAllEffects = (useAngleInAllEffects != 0);
}

QT_END_NAMESPACE
