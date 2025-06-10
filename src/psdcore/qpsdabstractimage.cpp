// Copyright (C) 2024 Signal Slot Inc.
// SPDX-License-Identifier: LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qpsdabstractimage.h"
#include "qpsdfileheader.h"

QT_BEGIN_NAMESPACE

class QPsdAbstractImage::Private : public QSharedData
{
public:
    quint32 width = 0;
    quint32 height = 0;
    quint16 depth = 0;
    quint8 opacity = 0;
};

QPsdAbstractImage::QPsdAbstractImage()
    : QPsdSection()
    , d(new Private)
{}

QPsdAbstractImage::QPsdAbstractImage(const QPsdAbstractImage &other)
    : QPsdSection(other)
    , d(other.d)
{}

QPsdAbstractImage &QPsdAbstractImage::operator=(const QPsdAbstractImage &other)
{
    if (this != &other) {
        QPsdSection::operator=(other);
        d.operator=(other.d);
    }
    return *this;
}

QPsdAbstractImage::~QPsdAbstractImage() = default;

quint32 QPsdAbstractImage::width() const
{
    return d->width;
}

void QPsdAbstractImage::setWidth(quint32 width)
{
    d->width = width;
}

quint32 QPsdAbstractImage::height() const
{
    return d->height;
}

void QPsdAbstractImage::setHeight(quint32 height)
{
    d->height = height;
}

quint16 QPsdAbstractImage::depth() const
{
    return d->depth;
}

void QPsdAbstractImage::setDepth(quint16 depth)
{
    d->depth = depth;
}

quint8 QPsdAbstractImage::opacity() const
{
    return d->opacity;
}

void QPsdAbstractImage::setOpacity(quint8 opacity)
{
    d->opacity = opacity;
}

QByteArray QPsdAbstractImage::readRLE(QIODevice *source, int height, quint32 *length)
{
    QByteArray ret;
    QList<qint16> byteCounts;
    for (int y = 0; y < height; y++) {
        byteCounts.append(readS16(source, length));
    }
    for (qint16 byteCount : byteCounts) {
        EnsureSeek es(source, byteCount);
        while (es.bytesAvailable() > 0) {
            auto size = readS8(source, length);
            if (size == -128) {
                // ignore size == -128 for padding
            } else if (size < 0) {
                ret.append(-size + 1, readByteArray(source, 1, length).at(0));
            } else if (size >= 0) {
                ret.append(readByteArray(source, size + 1, length));
            }
        }
    }
    return ret;
}

QByteArray QPsdAbstractImage::readZip(QIODevice *source, quint32 *length)
{
    quint32 beLength = qToBigEndian(*length);
    QByteArray zipData = readByteArray(source, *length, length);
    zipData.prepend(reinterpret_cast<char *>(&beLength), sizeof(quint32));

    return qUncompress(zipData);
}

QByteArray QPsdAbstractImage::toImage(QPsdFileHeader::ColorMode colorMode) const
{
    QByteArray ret;
    switch (colorMode) {
    case QPsdFileHeader::Bitmap:
    case QPsdFileHeader::Grayscale:
        ret = imageData();
        break;
    case QPsdFileHeader::RGB: {
        auto pr = r();
        auto pg = g();
        auto pb = b();
        auto pa = a();
        double o = opacity();
        const auto size = width() * height();
        for (quint32 i = 0; i < size; i++) {
            ret.append(*pb++);
            ret.append(*pg++);
            ret.append(*pr++);
            if (pa) {
                ret.append(*pa++ * o / 0xff);
            }
        }
        break; }
    case QPsdFileHeader::CMYK: {
        // For CMYK, channels are mapped as:
        // Channel 0 (r()) = Cyan
        // Channel 1 (g()) = Magenta
        // Channel 2 (b()) = Yellow
        // Channel 3 needs special handling for Black (K)
        auto pc = r();  // Cyan
        auto pm = g();  // Magenta
        auto py = b();  // Yellow
        // For now, we'll handle CMYK without the K channel
        // as the abstract interface doesn't provide access to a 4th channel
        const auto size = width() * height();
        for (quint32 i = 0; i < size; i++) {
            // Simple CMYK to RGB conversion (without K channel)
            // R = 255 * (1 - C)
            // G = 255 * (1 - M)
            // B = 255 * (1 - Y)
            if (pc && pm && py) {
                quint8 c = *pc++;
                quint8 m = *pm++;
                quint8 y = *py++;
                ret.append(255 - y);  // Blue
                ret.append(255 - m);  // Green
                ret.append(255 - c);  // Red
            } else {
                // If any channel is missing, skip this pixel
                ret.append('\0');
                ret.append('\0');
                ret.append('\0');
            }
        }
        break; }
    default:
        qFatal("Color mode %d not supported", colorMode);
    }
    return ret;
}

QT_END_NAMESPACE
