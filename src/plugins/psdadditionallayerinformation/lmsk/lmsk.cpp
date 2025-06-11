// Copyright (C) 2025 Signal Slot Inc.
// SPDX-License-Identifier: LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include <QtPsdCore/qpsdadditionallayerinformationplugin.h>

QT_BEGIN_NAMESPACE

class QPsdAdditionalLayerInformationLMskPlugin : public QPsdAdditionalLayerInformationPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QPsdAdditionalLayerInformationFactoryInterface" FILE "lmsk.json")
public:
    // User Mask
    QVariant parse(QIODevice *source , quint32 length) const override {
        auto cleanup = qScopeGuard([&] {
            Q_ASSERT(length == 0);
        });

        const auto color = readColor(source, &length);
        Q_UNUSED(color);
        const auto opacity = readU16(source, &length);
        Q_UNUSED(opacity);
        const auto flag = readU8(source, &length);
        Q_ASSERT(flag == 128);
        skip(source, 1, &length);

        return {};
    }
};

QT_END_NAMESPACE

#include "lmsk.moc"
