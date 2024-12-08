// Copyright (C) 2024 Signal Slot Inc.
// SPDX-License-Identifier: LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include <QtPsdCore/qpsdadditionallayerinformationplugin.h>

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

        // Color space
        auto data = readByteArray(source, 10, &length);
        Q_UNUSED(data); // TODO

        // Opacity
        auto opacity = readU16(source, &length);
        Q_UNUSED(opacity); // TODO

        return QVariant();
    }
};

QT_END_NAMESPACE

#include "fmsk.moc"
