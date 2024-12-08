// Copyright (C) 2024 Signal Slot Inc.
// SPDX-License-Identifier: LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qpsdlayerinfo.h"
#include "qpsdfileheader.h"

QT_BEGIN_NAMESPACE

class QPsdLayerInfo::Private : public QSharedData
{
public:
    Private();
    QList<QPsdLayerRecord> records;
    QList<QPsdChannelImageData> channelImageData;
};

QPsdLayerInfo::Private::Private()
{}

QPsdLayerInfo::QPsdLayerInfo()
    : QPsdSection()
    , d(new Private)
{}

QPsdLayerInfo::QPsdLayerInfo(QIODevice *source)
    : QPsdLayerInfo()
{
    // Layer info
    // https://www.adobe.com/devnet-apps/photoshop/fileformatashtml/#50577409_16000

    // Length of the layers info section, rounded up to a multiple of 2. (**PSB** length is 8 bytes.)
    auto length = readU32(source);
    EnsureSeek es(source, length);

    const auto count = readS16(source);

    for (int i = 0; i < std::abs(count); i++) {
        d->records.append(QPsdLayerRecord(source));
    }

    for (const QPsdLayerRecord &record : d->records) {
        QPsdChannelImageData imageData(record, source);
        d->channelImageData.append(imageData);
    }
}

QPsdLayerInfo::QPsdLayerInfo(const QPsdLayerInfo &other)
    : QPsdSection(other)
    , d(other.d)
{}

QPsdLayerInfo &QPsdLayerInfo::operator=(const QPsdLayerInfo &other)
{
    if (this != &other) {
        QPsdSection::operator=(other);
        d.operator=(other.d);
    }
    return *this;
}

QPsdLayerInfo::~QPsdLayerInfo() = default;

QList<QPsdLayerRecord> QPsdLayerInfo::records() const
{
    return d->records;
}

QList<QPsdChannelImageData> QPsdLayerInfo::channelImageData() const
{
    return d->channelImageData;
}

QT_END_NAMESPACE
