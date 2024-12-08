// Copyright (C) 2024 Signal Slot Inc.
// SPDX-License-Identifier: LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qpsdbevleffect.h"

QT_BEGIN_NAMESPACE

class QPsdBevlEffect::Private : public QSharedData
{
public:
    quint32 angle = 0;
    quint32 strength = 0;
    quint32 blur = 0;
    QPsdBlend::Mode highlightBlendMode = QPsdBlend::Invalid;
    QPsdBlend::Mode shadowBlendMode = QPsdBlend::Invalid;
    QString highlightColor;
    QString shadowColor;
    quint8 bevelStyle = 0;
    qreal highlightOpacity = 0;
    qreal shadowOpacity = 0;
    bool useGlobalAngle = false;
    bool upOrDown = false;
    QString realHighlightColor;
    QString realShadowColor;
};

QPsdBevlEffect::QPsdBevlEffect()
    : QPsdAbstractEffect()
    , d(new Private)
{}

QPsdBevlEffect::QPsdBevlEffect(const QPsdBevlEffect &other)
    : QPsdAbstractEffect(other)
    , d(other.d)
{}

QPsdBevlEffect &QPsdBevlEffect::operator=(const QPsdBevlEffect &other)
{
    if (this != &other) {
        QPsdAbstractEffect::operator=(other);
        d.operator=(other.d);
    }
    return *this;
}

QPsdBevlEffect::~QPsdBevlEffect() = default;

quint32 QPsdBevlEffect::angle() const
{
    return d->angle;
}

void QPsdBevlEffect::setAngle(quint32 angle)
{
    d->angle = angle;
}

quint32 QPsdBevlEffect::strength() const
{
    return d->strength;
}

void QPsdBevlEffect::setStrength(quint32 strength)
{
    d->strength = strength;
}

quint32 QPsdBevlEffect::blur() const
{
    return d->blur;
}

void QPsdBevlEffect::setBlur(quint32 blur)
{
    d->blur = blur;
}

QPsdBlend::Mode QPsdBevlEffect::highlightBlendMode() const
{
    return d->highlightBlendMode;
}

void QPsdBevlEffect::setHighlightBlendMode(const QByteArray &highlightBlendMode)
{
    d->highlightBlendMode = QPsdBlend::from(highlightBlendMode);
}

QPsdBlend::Mode QPsdBevlEffect::shadowBlendMode() const
{
    return d->shadowBlendMode;
}

void QPsdBevlEffect::setShadowBlendMode(const QByteArray &shadowBlendMode)
{
    d->shadowBlendMode = QPsdBlend::from(shadowBlendMode);
}

QString QPsdBevlEffect::highlightColor() const
{
    return d->highlightColor;
}

void QPsdBevlEffect::setHighlightColor(const QString &highlightColor)
{
    d->highlightColor = highlightColor;
}

QString QPsdBevlEffect::shadowColor() const
{
    return d->shadowColor;
}

void QPsdBevlEffect::setShadowColor(const QString &shadowColor)
{
    d->shadowColor = shadowColor;
}

quint8 QPsdBevlEffect::bevelStyle() const
{
    return d->bevelStyle;
}

void QPsdBevlEffect::setBevelStyle(quint8 bevelStyle)
{
    d->bevelStyle = bevelStyle;
}

qreal QPsdBevlEffect::highlightOpacity() const
{
    return d->highlightOpacity;
}

void QPsdBevlEffect::setHighlightOpacity(quint8 highlightOpacity)
{
    d->highlightOpacity = highlightOpacity / 255.0;
}

qreal QPsdBevlEffect::shadowOpacity() const
{
    return d->shadowOpacity;
}

void QPsdBevlEffect::setShadowOpacity(quint8 shadowOpacity)
{
    d->shadowOpacity = shadowOpacity / 255.0;
}

bool QPsdBevlEffect::useGlobalAngle() const
{
    return d->useGlobalAngle;
}

void QPsdBevlEffect::setUseGlobalAngle(quint8 useGlobalAngle)
{
    d->useGlobalAngle = useGlobalAngle;
}

bool QPsdBevlEffect::upOrDown() const
{
    return d->upOrDown;
}

void QPsdBevlEffect::setUpOrDown(quint8 upOrDown)
{
    d->upOrDown = upOrDown;
}

QString QPsdBevlEffect::realHighlightColor() const
{
    return d->realHighlightColor;
}

void QPsdBevlEffect::setRealHighlightColor(const QString &realHighlightColor)
{
    d->realHighlightColor = realHighlightColor;
}

QString QPsdBevlEffect::realShadowColor() const
{
    return d->realShadowColor;
}

void QPsdBevlEffect::setRealShadowColor(const QString &realShadowColor)
{
    d->realShadowColor = realShadowColor;
}

QT_END_NAMESPACE
