// Copyright (C) 2024 Signal Slot Inc.
// SPDX-License-Identifier: LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QPSDSOFIEFFECT_H
#define QPSDSOFIEFFECT_H

#include <QtPsdCore/qpsdabstracteffect.h>
#include <QtPsdCore/qpsdblend.h>

QT_BEGIN_NAMESPACE

class Q_PSDCORE_EXPORT QPsdSofiEffect : public QPsdAbstractEffect
{
public:
    QPsdSofiEffect();
    QPsdSofiEffect(const QPsdSofiEffect &other);
    QPsdSofiEffect &operator=(const QPsdSofiEffect &other);
    void swap(QPsdSofiEffect &other) noexcept { d.swap(other.d); }
    ~QPsdSofiEffect() override;

    Type type() const override { return SolidFill; }

    QPsdBlend::Mode blendMode() const;
    void setBlendMode(const QByteArray &blendMode);
    qreal opacity() const;
    void setOpacity(quint8 opacity);
    QString nativeColor() const;
    void setNativeColor(const QString &nativeColor);

private:
    class Private;
    QSharedDataPointer<Private> d;
};

QT_END_NAMESPACE

#endif // QPSDSOFIEFFECT_H
