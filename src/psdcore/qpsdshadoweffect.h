// Copyright (C) 2024 Signal Slot Inc.
// SPDX-License-Identifier: LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QPSDSHADOWEFFECT_H
#define QPSDSHADOWEFFECT_H

#include <QtPsdCore/qpsdoglweffect.h>

QT_BEGIN_NAMESPACE

class Q_PSDCORE_EXPORT QPsdShadowEffect : public QPsdOglwEffect
{
public:
    QPsdShadowEffect();
    QPsdShadowEffect(Type type);
    QPsdShadowEffect(const QPsdShadowEffect &other);
    QPsdShadowEffect &operator=(const QPsdShadowEffect &other);
    void swap(QPsdShadowEffect &other) noexcept { d.swap(other.d); }
    ~QPsdShadowEffect() override;

    Type type() const override;

    quint32 angle() const;
    void setAngle(quint32 angle);
    quint32 distance() const;
    void setDistance(quint32 distance);
    bool useAngleInAllEffects() const;
    void setUseAngleInAllEffects(quint8 useAngleInAllEffects);

private:
    class Private;
    QSharedDataPointer<Private> d;
};

QT_END_NAMESPACE

#endif // QPSDSHADOWEFFECT_H
