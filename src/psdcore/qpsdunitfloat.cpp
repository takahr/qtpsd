// Copyright (C) 2024 Signal Slot Inc.
// SPDX-License-Identifier: LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qpsdunitfloat.h"

QT_BEGIN_NAMESPACE

class QPsdUnitFloat::Private : public QSharedData
{
public:
    Unit unit = None;
    double value = 0;
};

QPsdUnitFloat::QPsdUnitFloat()
    : d(new Private)
{}

QPsdUnitFloat::QPsdUnitFloat(QIODevice *source , quint32 *length)
    : QPsdUnitFloat(readByteArray(source, 4, length), readDouble(source, length))
{}

QPsdUnitFloat::QPsdUnitFloat(const QPsdUnitFloat &other)
    : d(other.d)
{}

QPsdUnitFloat::QPsdUnitFloat(const QByteArray &unit, double value) : d(new Private)
{
         if (unit == "#Pnt") d->unit = Points;
    else if (unit == "#Mlm") d->unit = MilliMeters;
    else if (unit == "#Ang") d->unit = Angle;
    else if (unit == "#Rsl") d->unit = Density;
    else if (unit == "#Rlt") d->unit = Distance;
    else if (unit == "#Nne") d->unit = None;
    else if (unit == "#Prc") d->unit = Percent;
    else if (unit == "#Pxl") d->unit = Pixels;
    else qWarning() << unit << "is not a valid unit";

    d->value = value;
}

QPsdUnitFloat &QPsdUnitFloat::operator=(const QPsdUnitFloat &other)
{
    if (this != &other)
        d.operator=(other.d);
    return *this;
}

QPsdUnitFloat::~QPsdUnitFloat() = default;

QPsdUnitFloat::Unit QPsdUnitFloat::unit() const
{
    return d->unit;
}

double QPsdUnitFloat::value() const
{
    return d->value;
}

QDebug operator<<(QDebug s, const QPsdUnitFloat &value)
{
    QDebugStateSaver saver(s);
    s.nospace() << "QPsdUnitFloat(" << value.value() << ", ";
    switch (value.unit()) {
#define CASE(x) case QPsdUnitFloat::x: s << #x; break
    CASE(Points);
    CASE(MilliMeters);
    CASE(Angle);
    CASE(Density);
    CASE(Distance);
    CASE(None);
    CASE(Percent);
    CASE(Pixels);
#undef CASE
    }
    s << ")";
    return s;
}

QT_END_NAMESPACE
