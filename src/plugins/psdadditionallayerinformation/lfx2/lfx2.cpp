// Copyright (C) 2024 Signal Slot Inc.
// SPDX-License-Identifier: LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include <QtPsdCore/qpsdadditionallayerinformationplugin.h>
#include <QtPsdCore/qpsddescriptor.h>

QT_BEGIN_NAMESPACE

class QPsdAdditionalLayerInformationLfx2Plugin : public QPsdAdditionalLayerInformationPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QPsdAdditionalLayerInformationFactoryInterface" FILE "lfx2.json")
public:
    // Object Based Effects Layer info
    QVariant parse(QIODevice *source , quint32 length) const override {
        auto cleanup = qScopeGuard([&] {
            // Q_ASSERT(length == 0);
        });

        // Object effects version: 0
        auto version = readU32(source, &length);
        Q_ASSERT(version == 0);
        // Descriptor version ( = 16 for Photoshop 6.0).
        auto descriptorVersion = readU32(source, &length);
        Q_ASSERT(descriptorVersion == 16);
        // Descriptor. Based on the Action file format structure
        QPsdDescriptor descriptor(source, &length);
        return QVariant::fromValue(descriptor);
    }
};

QT_END_NAMESPACE

#include "lfx2.moc"
