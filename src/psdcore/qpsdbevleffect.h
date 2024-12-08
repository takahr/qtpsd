// Copyright (C) 2024 Signal Slot Inc.
// SPDX-License-Identifier: LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QPSDBEVLEFFECT_H
#define QPSDBEVLEFFECT_H

#include <QtPsdCore/qpsdabstracteffect.h>
#include <QtPsdCore/qpsdblend.h>

QT_BEGIN_NAMESPACE

class Q_PSDCORE_EXPORT QPsdBevlEffect : public QPsdAbstractEffect
{
public:
    QPsdBevlEffect();
    QPsdBevlEffect(const QPsdBevlEffect &other);
    QPsdBevlEffect &operator=(const QPsdBevlEffect &other);
    void swap(QPsdBevlEffect &other) noexcept { d.swap(other.d); }
    ~QPsdBevlEffect() override;

    Type type() const override { return Bevel; }

    quint32 angle() const;
    void setAngle(quint32 angle);
    quint32 strength() const;
    void setStrength(quint32 strength);
    quint32 blur() const;
    void setBlur(quint32 blur);
    QPsdBlend::Mode highlightBlendMode() const;
    void setHighlightBlendMode(const QByteArray &highlightBlendMode);
    QPsdBlend::Mode shadowBlendMode() const;
    void setShadowBlendMode(const QByteArray &shadowBlendMode);
    QString highlightColor() const;
    void setHighlightColor(const QString &highlightColor);
    QString shadowColor() const;
    void setShadowColor(const QString &shadowColor);
    quint8 bevelStyle() const;
    void setBevelStyle(quint8 bevelStyle);
    qreal highlightOpacity() const;
    void setHighlightOpacity(quint8 highlightOpacity);
    qreal shadowOpacity() const;
    void setShadowOpacity(quint8 shadowOpacity);
    bool useGlobalAngle() const;
    void setUseGlobalAngle(quint8 useGlobalAngle);
    bool upOrDown() const;
    void setUpOrDown(quint8 upOrDown);
    QString realHighlightColor() const;
    void setRealHighlightColor(const QString &realHighlightColor);
    QString realShadowColor() const;
    void setRealShadowColor(const QString &realShadowColor);

private:
    class Private;
    QSharedDataPointer<Private> d;
};
#endif // QPSDBEVLEFFECT_H
