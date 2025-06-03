// Copyright (C) 2025 Signal Slot Inc.
// SPDX-License-Identifier: LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include <QtPsdCore/qpsdadditionallayerinformationplugin.h>

QT_BEGIN_NAMESPACE

class QPsdAdditionalLayerInformationBlncPlugin : public QPsdAdditionalLayerInformationPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QPsdAdditionalLayerInformationFactoryInterface" FILE "blnc.json")
public:
    // Color Balance
    QVariant parse(QIODevice *source , quint32 length) const override {
        auto cleanup = qScopeGuard([&] {
            Q_ASSERT(length <= 3);
        });

        const auto shadows = readBalance(source, &length);
        Q_UNUSED(shadows);
        const auto midtones = readBalance(source, &length);
        Q_UNUSED(midtones);
        const auto highlights = readBalance(source, &length);
        Q_UNUSED(highlights);
        const auto preserveLuminosity = readU8(source, &length);
        Q_UNUSED(preserveLuminosity);

        return {};
    }

    QVariant readBalance(QIODevice *source, quint32 *length) const {
        const auto cyanRed = readS16(source, length);
        Q_UNUSED(cyanRed);
        const auto magentaGreen = readS16(source, length);
        Q_UNUSED(magentaGreen);
        const auto yellowBlue = readS16(source, length);
        Q_UNUSED(yellowBlue);

        return {};
    }
};

QT_END_NAMESPACE

#include "blnc.moc"
