// Copyright (C) 2025 Signal Slot Inc.
// SPDX-License-Identifier: LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include <QtPsdCore/qpsdadditionallayerinformationplugin.h>

QT_BEGIN_NAMESPACE

class QPsdAdditionalLayerInformationCurvPlugin : public QPsdAdditionalLayerInformationPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QPsdAdditionalLayerInformationFactoryInterface" FILE "curv.json")
public:
    // Curve
    QVariant parse(QIODevice *source , quint32 length) const override {
        auto cleanup = qScopeGuard([&] {
            Q_ASSERT(length <= 3);
        });

        const auto v1 = readU8(source, &length);
        Q_UNUSED(v1);
        const auto version = readU16(source, &length);
        Q_ASSERT(version == 1);
        const auto channelsVersion = readU16(source, &length);
        Q_ASSERT(channelsVersion == 1 || channelsVersion == 4 || channelsVersion == 0);
        const auto channels = readU16(source, &length);

        if (channelsVersion != 4) {
            if (channels & 1) {
                const auto rgb = readCurve(source, &length);
                Q_UNUSED(rgb);
            }
            if (channels & 2) {
                const auto red = readCurve(source, &length);
                Q_UNUSED(red);
            }
            if (channels & 4) {
                const auto green = readCurve(source, &length);
                Q_UNUSED(green);
            }
            if (channels & 8) {
                const auto blue = readCurve(source, &length);
                Q_UNUSED(blue);
            }
        } else {
            for (int i = 0; i < channels; i++) {
                const auto curve = readCurve(source, &length);
                Q_UNUSED(curve);
            }            
        }

        const auto signature = readByteArray(source, 4, &length);
        Q_ASSERT(signature == "Crv ");
        const auto version2 = readU16(source, &length);
        Q_UNUSED(version2);
        const auto version3 = readU16(source, &length);
        Q_UNUSED(version3);
        const auto channels2 = readU16(source, &length);
        for (int i = 0; i < channels2; i++) {
            const auto index = readU16(source, &length);
            const auto curve = readCurve(source, &length);

            Q_UNUSED(index);
            Q_UNUSED(curve);
        }

        return {};
    }

    QVariant readCurve(QIODevice *source, quint32 *length) const {
        const auto count = readU16(source, length);
        Q_ASSERT(count * 2 <= *length);
        for (int i = 0; i < count; i++) {
            const auto input = readS16(source, length);
            const auto output = readS16(source, length);

            Q_UNUSED(input);
            Q_UNUSED(output);
        }

        return {};
    }
};

QT_END_NAMESPACE

#include "curv.moc"
