// Copyright (C) 2024 Signal Slot Inc.
// SPDX-License-Identifier: LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qpsdoglweffect.h"

QT_BEGIN_NAMESPACE

class QPsdOglwEffect::Private : public QSharedData
{
public:
    quint32 blur = 0;
    quint32 intensity = 0;
    QString color;
};

QPsdOglwEffect::QPsdOglwEffect()
    : QPsdSofiEffect()
    , d(new Private)
{}

QPsdOglwEffect::QPsdOglwEffect(const QPsdOglwEffect &other)
    : QPsdSofiEffect(other)
    , d(other.d)
{}

QPsdOglwEffect &QPsdOglwEffect::operator=(const QPsdOglwEffect &other)
{
    if (this != &other) {
        QPsdSofiEffect::operator=(other);
        d.operator=(other.d);
    }
    return *this;
}

QPsdOglwEffect::~QPsdOglwEffect() = default;

quint32 QPsdOglwEffect::blur() const
{
    return d->blur;
}

void QPsdOglwEffect::setBlur(quint32 blur)
{
    d->blur = blur;
}

quint32 QPsdOglwEffect::intensity() const
{
    return d->intensity;
}

void QPsdOglwEffect::setIntensity(quint32 intensity)
{
    d->intensity = intensity;
}

QString QPsdOglwEffect::color() const
{
    return d->color;
}

void QPsdOglwEffect::setColor(const QString &color)
{
    d->color = color;
}

QT_END_NAMESPACE
