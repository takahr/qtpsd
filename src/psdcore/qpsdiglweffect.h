// Copyright (C) 2024 Signal Slot Inc.
// SPDX-License-Identifier: LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QPSDIGLWEFFECT_H
#define QPSDIGLWEFFECT_H

#include <QtPsdCore/qpsdoglweffect.h>

QT_BEGIN_NAMESPACE

class Q_PSDCORE_EXPORT QPsdIglwEffect : public QPsdOglwEffect
{
public:
    QPsdIglwEffect();
    QPsdIglwEffect(const QPsdIglwEffect &other);
    QPsdIglwEffect &operator=(const QPsdIglwEffect &other);
    void swap(QPsdIglwEffect &other) noexcept { d.swap(other.d); }
    ~QPsdIglwEffect() override;

    Type type() const override { return InnerGlow; }

    bool invert() const;
    void setInvert(quint8 invert);

private:
    class Private;
    QSharedDataPointer<Private> d;
};

QT_END_NAMESPACE

#endif // QPSDIGLWEFFECT_H
