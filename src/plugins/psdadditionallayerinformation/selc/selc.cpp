// Copyright (C) 2025 Signal Slot Inc.
// SPDX-License-Identifier: LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include <QtPsdCore/qpsdadditionallayerinformationplugin.h>

QT_BEGIN_NAMESPACE

class QPsdAdditionalLayerInformationSelcPlugin : public QPsdAdditionalLayerInformationPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QPsdAdditionalLayerInformationFactoryInterface" FILE "selc.json")
public:
    // Selective color
    QVariant parse(QIODevice *source , quint32 length) const override {
        auto cleanup = qScopeGuard([&] {
            Q_ASSERT(length <= 3);
        });

       const auto version = readU16(source, &length);
        Q_ASSERT(version == 1);

        const auto mode = readU16(source, &length);
        Q_UNUSED(mode);

        skip(source, 8, &length);

        const auto reds = readSelectiveColors(source, &length);
        Q_UNUSED(reds);
        const auto yellows = readSelectiveColors(source, &length);
        Q_UNUSED(yellows);
        const auto greens = readSelectiveColors(source, &length);
        Q_UNUSED(greens);
        const auto cyans = readSelectiveColors(source, &length);
        Q_UNUSED(cyans);
        const auto blues = readSelectiveColors(source, &length);
        Q_UNUSED(blues);
        const auto magentas = readSelectiveColors(source, &length);
        Q_UNUSED(magentas);
        const auto whites = readSelectiveColors(source, &length);
        Q_UNUSED(whites);
        const auto neutrals = readSelectiveColors(source, &length);
        Q_UNUSED(neutrals);
        const auto blacks = readSelectiveColors(source, &length);
        Q_UNUSED(blacks);

        return {};
    }

    QVariant readSelectiveColors(QIODevice *source, quint32 *length) const {
        const auto c = readS16(source, length);
        Q_UNUSED(c);
        const auto m = readS16(source, length);
        Q_UNUSED(m);
        const auto y = readS16(source, length);
        Q_UNUSED(y);
        const auto k = readS16(source, length);
        Q_UNUSED(k);

        return {};
    }
};

QT_END_NAMESPACE

#include "selc.moc"
