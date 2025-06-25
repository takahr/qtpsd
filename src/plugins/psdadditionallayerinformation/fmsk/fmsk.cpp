// Copyright (C) 2024 Signal Slot Inc.
// SPDX-License-Identifier: LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include <QtPsdCore/qpsdadditionallayerinformationplugin.h>
#include <QtPsdCore/qpsdfiltermask.h>

QT_BEGIN_NAMESPACE

class QPsdAdditionalLayerInformationFMskPlugin : public QPsdAdditionalLayerInformationPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QPsdAdditionalLayerInformationFactoryInterface" FILE "fmsk.json")
public:
    // Filter Mask (Photoshop CS3)
    QVariant parse(QIODevice *source , quint32 length) const override {
        auto cleanup = qScopeGuard([&] {
            Q_ASSERT(length == 0);
        });

        QPsdFilterMask filterMask;

        // Color space
        const auto colorSpace = readColorSpace(source, &length);
        filterMask.setColorSpace(colorSpace);

        // Opacity
        const auto opacity = readU16(source, &length);
        filterMask.setOpacity(opacity / 100.0);

        return QVariant::fromValue(filterMask);
    }
};

QT_END_NAMESPACE

#include "fmsk.moc"
