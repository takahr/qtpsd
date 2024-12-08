// Copyright (C) 2024 Signal Slot Inc.
// SPDX-License-Identifier: LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QPSDOGLWEFFECT_H
#define QPSDOGLWEFFECT_H

#include <QtPsdCore/qpsdsofieffect.h>

QT_BEGIN_NAMESPACE

class Q_PSDCORE_EXPORT QPsdOglwEffect : public QPsdSofiEffect
{
public:
    QPsdOglwEffect();
    QPsdOglwEffect(const QPsdOglwEffect &other);
    QPsdOglwEffect &operator=(const QPsdOglwEffect &other);
    void swap(QPsdOglwEffect &other) noexcept { d.swap(other.d); }
    ~QPsdOglwEffect() override;

    Type type() const override { return OuterGlow; }

    quint32 blur() const;
    void setBlur(quint32 blur);
    quint32 intensity() const;
    void setIntensity(quint32 intensity);
    QString color() const;
    void setColor(const QString &color);

private:
    class Private;
    QSharedDataPointer<Private> d;
};

QT_END_NAMESPACE

#endif // QPSDOGLWEFFECT_H
