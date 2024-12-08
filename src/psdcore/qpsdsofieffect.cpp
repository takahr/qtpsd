// Copyright (C) 2024 Signal Slot Inc.
// SPDX-License-Identifier: LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qpsdsofieffect.h"

QT_BEGIN_NAMESPACE

class QPsdSofiEffect::Private : public QSharedData
{
public:
    QPsdBlend::Mode blendMode = QPsdBlend::Invalid;
    qreal opacity = 0.0;
    QString nativeColor;
};

QPsdSofiEffect::QPsdSofiEffect()
    : QPsdAbstractEffect()
    , d(new Private)
{}

QPsdSofiEffect::QPsdSofiEffect(const QPsdSofiEffect &other)
    : QPsdAbstractEffect(other)
    , d(other.d)
{}

QPsdSofiEffect &QPsdSofiEffect::operator=(const QPsdSofiEffect &other)
{
    if (this != &other) {
        QPsdAbstractEffect::operator=(other);
        d.operator=(other.d);
    }
    return *this;
}

QPsdSofiEffect::~QPsdSofiEffect() = default;

QPsdBlend::Mode QPsdSofiEffect::blendMode() const
{
    return d->blendMode;
}

void QPsdSofiEffect::setBlendMode(const QByteArray &blendMode)
{
    d->blendMode = QPsdBlend::from(blendMode);
}

qreal QPsdSofiEffect::opacity() const
{
    return d->opacity;
}

void QPsdSofiEffect::setOpacity(quint8 opacity)
{
    d->opacity = opacity / 255.0;
}

QString QPsdSofiEffect::nativeColor() const
{
    return d->nativeColor;
}

void QPsdSofiEffect::setNativeColor(const QString &nativeColor)
{
    d->nativeColor = nativeColor;
}

QT_END_NAMESPACE
