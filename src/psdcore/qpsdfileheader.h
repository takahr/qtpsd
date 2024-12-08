// Copyright (C) 2024 Signal Slot Inc.
// SPDX-License-Identifier: LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QPSDFILEHEADER_H
#define QPSDFILEHEADER_H

#include <QtPsdCore/qpsdsection.h>

QT_BEGIN_NAMESPACE

class Q_PSDCORE_EXPORT QPsdFileHeader : public QPsdSection
{
public:
    enum ColorMode : qint16 {
        Bitmap = 0,
        Grayscale = 1,
        Indexed = 2,
        RGB = 3,
        CMYK = 4,
        Multichannel = 7,
        Duotone = 8,
        Lab = 9
    };
    QPsdFileHeader();
    QPsdFileHeader(QIODevice *source);
    QPsdFileHeader(const QPsdFileHeader &other);
    QPsdFileHeader &operator=(const QPsdFileHeader &other);
    ~QPsdFileHeader() override;

    quint16 channels() const;
    quint32 height() const;
    quint32 width() const;
    quint16 depth() const;
    ColorMode colorMode() const;

    QSize size() const { return QSize(width(), height()); }

private:
    class Private;
    QSharedDataPointer<Private> d;
};

QT_END_NAMESPACE

#endif // QPSDFILEHEADER_H
