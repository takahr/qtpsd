// Copyright (C) 2025 Signal Slot Inc.
// SPDX-License-Identifier: LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include <QtPsdCore/qpsdadditionallayerinformationplugin.h>

QT_BEGIN_NAMESPACE

class QPsdAdditionalLayerInformationMixrPlugin : public QPsdAdditionalLayerInformationPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QPsdAdditionalLayerInformationFactoryInterface" FILE "mixr.json")
public:
    // Channel Mixer
    QVariant parse(QIODevice *source , quint32 length) const override {
        auto cleanup = qScopeGuard([&] {
            Q_ASSERT(length <= 3);
        });

        const auto monochrome = readU16(source, &length) != 0;

        if (!monochrome) {
            const auto red = readMixer(source, &length);
            Q_UNUSED(red);
            const auto green = readMixer(source, &length);
            Q_UNUSED(green);
            const auto blue = readMixer(source, &length);
            Q_UNUSED(blue);
        }
        const auto gray = readMixer(source, &length);
        Q_UNUSED(gray);

        if (monochrome) {
            skip(source, 3 * 5 * 2, &length);
        }

        return {};
    }

    QVariant readMixer(QIODevice *source, quint32 *length) const {
        const auto red = readS16(source, length);
        Q_UNUSED(red);
        const auto green = readS16(source, length);
        Q_UNUSED(green);
        const auto blue = readS16(source, length);
        Q_UNUSED(blue);

        const auto v1 = readU16(source, length);
        Q_UNUSED(v1);

        const auto constant = readS16(source, length);
        Q_UNUSED(constant);

        return {};
    }
};

QT_END_NAMESPACE

#include "mixr.moc"
