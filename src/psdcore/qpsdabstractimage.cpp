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
    const auto size = width() * height();
    const auto bytesPerChannel = depth() / 8;
    switch (colorMode) {
    case QPsdFileHeader::Bitmap:
    case QPsdFileHeader::Grayscale:
        // For grayscale, imageData() should already contain the correct format
        // but we should verify it matches the expected size
        ret = imageData();
        break;
    case QPsdFileHeader::RGB: {
        auto pr = r();
        auto pg = g();
        auto pb = b();
        auto pa = a();
        double o = opacity();

        if (bytesPerChannel == 1) {
            // 8-bit per channel
            for (quint32 i = 0; i < size; i++) {
                ret.append(*pb++);
                ret.append(*pg++);
                ret.append(*pr++);
                if (pa) {
                    ret.append(*pa++ * o / 0xff);
                }
            }
        } else if (bytesPerChannel == 2) {
            // 16-bit per channel
            for (quint32 i = 0; i < size; i++) {
                // Append 2 bytes for each channel
                ret.append(reinterpret_cast<const char*>(pb), 2);
                pb += 2;
                ret.append(reinterpret_cast<const char*>(pg), 2);
                pg += 2;
                ret.append(reinterpret_cast<const char*>(pr), 2);
                pr += 2;
                if (pa) {
                    // For 16-bit, we need to scale opacity differently
                    quint16 alpha = *reinterpret_cast<const quint16*>(pa);
                    alpha = static_cast<quint16>(alpha * o / 0xff);
                    ret.append(reinterpret_cast<const char*>(&alpha), 2);
                    pa += 2;
                }
            }
        } else if (bytesPerChannel == 4) {
            // 32-bit per channel (float)
            for (quint32 i = 0; i < size; i++) {
                ret.append(reinterpret_cast<const char*>(pb), 4);
                pb += 4;
                ret.append(reinterpret_cast<const char*>(pg), 4);
                pg += 4;
                ret.append(reinterpret_cast<const char*>(pr), 4);
                pr += 4;
                if (pa) {
                    float alpha = *reinterpret_cast<const float*>(pa);
                    alpha = alpha * static_cast<float>(o / 0xff);
                    ret.append(reinterpret_cast<const char*>(&alpha), 4);
                    pa += 4;
                }
            }
        }
        break; }
    case QPsdFileHeader::CMYK: {
        if (bytesPerChannel == 0) {
            // TODO: what is 0?
        } else if (bytesPerChannel == 1) {
            auto pc = c();  // Channel 0 = Cyan
            auto pm = m();  // Channel 1 = Magenta
            auto py = y();  // Channel 2 = Yellow
            auto pk = k();  // Channel 3 = Black (K) - might be null

            const auto size = width() * height();
            for (quint32 i = 0; i < size; i++) {
                // CMYK order for QImage::Format_CMYK8888
                ret.append(255 - *pc++);  // C
                ret.append(255 - *pm++);  // M
                ret.append(255 - *py++);  // Y
                ret.append(255 - *pk++);  // K
            }
        } else {
            qWarning() << "bytesPerChannel" << bytesPerChannel << "not supported";
        }
        break; }
    default:
        qFatal("Color mode %d not supported", colorMode);
    }
    return ret;
}

QT_END_NAMESPACE
