// Copyright (C) 2025 Signal Slot Inc.
// SPDX-License-Identifier: LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include <QtPsdCore/qpsdadditionallayerinformationplugin.h>
#include <QtPsdCore/qpsddescriptor.h>

QT_BEGIN_NAMESPACE

class QPsdAdditionalLayerInformationLevlPlugin : public QPsdAdditionalLayerInformationPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QPsdAdditionalLayerInformationFactoryInterface" FILE "levl.json")
public:
    // Levels
    QVariant parse(QIODevice *source , quint32 length) const override {
        auto cleanup = qScopeGuard([&] {
            Q_ASSERT(length == 0);
        });

        Q_ASSERT(length == 0x0278);

        const auto rgb = readLevels(source, &length);
        Q_UNUSED(rgb);
        const auto red = readLevels(source, &length);
        Q_UNUSED(red);
        const auto green = readLevels(source, &length);
        Q_UNUSED(green);
        const auto blue = readLevels(source, &length);
        Q_UNUSED(blue);

        // has many unknown data.. (592 bytes)
        skip(source, length, &length);

        return QVariant{};
    }

    QVariant readLevels(QIODevice *source, quint32 *length) const {
        const auto shadowInput = readU16(source, length);
        Q_UNUSED(shadowInput);
        const auto highlightInput = readU16(source, length);
        Q_UNUSED(highlightInput);
        const auto shadowOutput = readU16(source, length);
        Q_UNUSED(shadowOutput);
        const auto highlightOutput = readU16(source, length);
        Q_UNUSED(highlightOutput);
        const auto midtoneInput = readU16(source, length);
        Q_UNUSED(midtoneInput);

        return QVariant{};
    }
};

QT_END_NAMESPACE

#include "levl.moc"
