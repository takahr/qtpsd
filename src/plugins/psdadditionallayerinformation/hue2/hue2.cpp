// Copyright (C) 2025 Signal Slot Inc.
// SPDX-License-Identifier: LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include <QtPsdCore/qpsdadditionallayerinformationplugin.h>

QT_BEGIN_NAMESPACE

class QPsdAdditionalLayerInformationHue2Plugin : public QPsdAdditionalLayerInformationPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QPsdAdditionalLayerInformationFactoryInterface" FILE "hue2.json")
public:
    // New Hue/saturation, Photoshop 5.0
    QVariant parse(QIODevice *source , quint32 length) const override {
        auto cleanup = qScopeGuard([&] {
            Q_ASSERT(length <= 3);
        });

       const auto version = readU16(source, &length);
        Q_ASSERT(version == 2);

        const auto master = readHue(source, &length);
        Q_UNUSED(master);
        const auto reds = readHue(source, &length);
        Q_UNUSED(reds);
        const auto yellows = readHue(source, &length);
        Q_UNUSED(yellows);
        const auto greens = readHue(source, &length);
        Q_UNUSED(greens);
        const auto cyans = readHue(source, &length);
        Q_UNUSED(cyans);
        const auto blues = readHue(source, &length);
        Q_UNUSED(blues);
        const auto magentas = readHue(source, &length);
        Q_UNUSED(magentas);

        return {};
    }

    QVariant readHue(QIODevice *source, quint32 *length) const {
        const auto a1 = readU16(source, length);
        Q_UNUSED(a1);
        const auto a2 = readU16(source, length);
        Q_UNUSED(a2);
        const auto a3 = readU16(source, length);
        Q_UNUSED(a3);
        const auto a4 = readU16(source, length);
        Q_UNUSED(a4);

        const auto hue = readS16(source, length);
        Q_UNUSED(hue);
        const auto saturation = readS16(source, length);
        Q_UNUSED(saturation);
        const auto lightness = readS16(source, length);
        Q_UNUSED(lightness);

        return {};
    }
};

QT_END_NAMESPACE

#include "hue2.moc"
