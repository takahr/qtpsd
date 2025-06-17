// Copyright (C) 2024 Signal Slot Inc.
// SPDX-License-Identifier: LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QPSDRESOLUTIONINFO_H
#define QPSDRESOLUTIONINFO_H

#include <QtPsdCore/qpsdcoreglobal.h>
#include <QtCore/QSharedDataPointer>

QT_BEGIN_NAMESPACE

class QPsdImageResourceBlock;

class Q_PSDCORE_EXPORT QPsdResolutionInfo
{
public:
    enum Unit {
        Inches = 1,
        Centimeters = 2,
        Points = 3,
        Picas = 4,
        Columns = 5
    };

    QPsdResolutionInfo();
    explicit QPsdResolutionInfo(const QPsdImageResourceBlock &block);
    QPsdResolutionInfo(const QPsdResolutionInfo &other);
    QPsdResolutionInfo &operator=(const QPsdResolutionInfo &other);
    ~QPsdResolutionInfo();

    bool isValid() const;
    
    // Resolution values in DPI (dots per inch)
    double horizontalResolution() const;
    double verticalResolution() const;
    
    // Display units for the resolution values
    Unit horizontalResolutionDisplayUnit() const;
    Unit verticalResolutionDisplayUnit() const;
    
    // Units for width and height
    Unit widthUnit() const;
    Unit heightUnit() const;

private:
    class Private;
    QSharedDataPointer<Private> d;
};

QT_END_NAMESPACE

#endif // QPSDRESOLUTIONINFO_H