// Copyright (C) 2024 Signal Slot Inc.
// SPDX-License-Identifier: LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qpsdresolutioninfo.h"
#include "qpsdimageresourceblock.h"

#include <QtCore/QDataStream>

QT_BEGIN_NAMESPACE

class QPsdResolutionInfo::Private : public QSharedData
{
public:
    bool valid = false;
    double horizontalResolution = 72.0;
    double verticalResolution = 72.0;
    QPsdResolutionInfo::Unit horizontalResolutionDisplayUnit = QPsdResolutionInfo::Inches;
    QPsdResolutionInfo::Unit verticalResolutionDisplayUnit = QPsdResolutionInfo::Inches;
    QPsdResolutionInfo::Unit widthUnit = QPsdResolutionInfo::Inches;
    QPsdResolutionInfo::Unit heightUnit = QPsdResolutionInfo::Inches;
};

QPsdResolutionInfo::QPsdResolutionInfo()
    : d(new Private)
{
}

QPsdResolutionInfo::QPsdResolutionInfo(const QPsdImageResourceBlock &block)
    : d(new Private)
{
    if (block.id() != 1005) {
        qWarning() << "QPsdResolutionInfo: Invalid block ID" << block.id() << "expected 1005";
        return;
    }

    const QByteArray data = block.data();
    if (data.size() < 16) {
        qWarning() << "QPsdResolutionInfo: Insufficient data size" << data.size() << "expected at least 16";
        return;
    }

    QDataStream stream(data);
    stream.setByteOrder(QDataStream::BigEndian);

    // ResolutionInfo structure:
    // 4 bytes: Horizontal resolution in pixels per inch as a fixed point number (16.16)
    // 2 bytes: Display unit for horizontal resolution (1=inches, 2=cm)
    // 2 bytes: Width unit (1=inches, 2=cm, 3=points, 4=picas, 5=columns)
    // 4 bytes: Vertical resolution in pixels per inch as a fixed point number (16.16)
    // 2 bytes: Display unit for vertical resolution (1=inches, 2=cm)
    // 2 bytes: Height unit (1=inches, 2=cm, 3=points, 4=picas, 5=columns)

    quint32 hRes;
    quint16 hResDisplayUnit;
    quint16 widthUnit;
    quint32 vRes;
    quint16 vResDisplayUnit;
    quint16 heightUnit;

    stream >> hRes >> hResDisplayUnit >> widthUnit >> vRes >> vResDisplayUnit >> heightUnit;

    // Convert fixed point 16.16 to double
    d->horizontalResolution = hRes / 65536.0;
    d->verticalResolution = vRes / 65536.0;

    // Set units
    d->horizontalResolutionDisplayUnit = static_cast<Unit>(hResDisplayUnit);
    d->verticalResolutionDisplayUnit = static_cast<Unit>(vResDisplayUnit);
    d->widthUnit = static_cast<Unit>(widthUnit);
    d->heightUnit = static_cast<Unit>(heightUnit);

    d->valid = true;
}

QPsdResolutionInfo::QPsdResolutionInfo(const QPsdResolutionInfo &other)
    : d(other.d)
{
}

QPsdResolutionInfo &QPsdResolutionInfo::operator=(const QPsdResolutionInfo &other)
{
    if (this != &other) {
        d = other.d;
    }
    return *this;
}

QPsdResolutionInfo::~QPsdResolutionInfo() = default;

bool QPsdResolutionInfo::isValid() const
{
    return d->valid;
}

double QPsdResolutionInfo::horizontalResolution() const
{
    return d->horizontalResolution;
}

double QPsdResolutionInfo::verticalResolution() const
{
    return d->verticalResolution;
}

QPsdResolutionInfo::Unit QPsdResolutionInfo::horizontalResolutionDisplayUnit() const
{
    return d->horizontalResolutionDisplayUnit;
}

QPsdResolutionInfo::Unit QPsdResolutionInfo::verticalResolutionDisplayUnit() const
{
    return d->verticalResolutionDisplayUnit;
}

QPsdResolutionInfo::Unit QPsdResolutionInfo::widthUnit() const
{
    return d->widthUnit;
}

QPsdResolutionInfo::Unit QPsdResolutionInfo::heightUnit() const
{
    return d->heightUnit;
}

QT_END_NAMESPACE
