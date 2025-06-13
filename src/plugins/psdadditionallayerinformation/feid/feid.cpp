// Copyright (C) 2025 Signal Slot Inc.
// SPDX-License-Identifier: LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include <QtPsdCore/qpsdadditionallayerinformationplugin.h>

QT_BEGIN_NAMESPACE

class QPsdAdditionalLayerInformationFeidPlugin : public QPsdAdditionalLayerInformationPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QPsdAdditionalLayerInformationFactoryInterface" FILE "feid.json")
public:
    // Filter Effect
    QVariant parse(QIODevice *source , quint32 length) const override {
        auto cleanup = qScopeGuard([&] {
            Q_ASSERT(length < 3);
        });

        const auto version = readU32(source, &length);
        Q_ASSERT(version == 1 || version == 2 || version == 3);

        int index = 0;
        while (length > 8) {
            const auto dataLength = readU64(source, &length);
            Q_UNUSED(dataLength);
            const auto identifier = readPascalString(source, 1, &length);
            Q_UNUSED(identifier);
            const auto version1 = readU32(source, &length);
            Q_ASSERT(version1 == 1);
            const auto len1 = readU64(source, &length);
            Q_UNUSED(len1);
            const auto rect = readRectangle(source, &length);
            Q_UNUSED(rect);
            const auto depth = readU32(source, &length);
            Q_UNUSED(depth);
            const auto channels = readU32(source, &length);

            for (quint32 i = 0; i < channels + 2; i++) {
                const auto written = readU32(source, &length);
                if (written) {
                    const auto len2 = readU64(source, &length);
                    Q_UNUSED(len2);
                    const auto compressionMode = readU16(source, &length);
                    Q_UNUSED(compressionMode);
                    const auto data = readByteArray(source, len2 - 2, &length);
                    Q_UNUSED(data);
                }
            }

            if (length > 0) {
                const auto flag = readU8(source, &length);
                if (flag) {
                    const auto rect = readRectangle(source, &length);
                    Q_UNUSED(rect);
                    const auto extraLength = readU64(source, &length);
                    const auto compressionMode = readU16(source, &length);
                    Q_UNUSED(compressionMode);
                    const auto data = readByteArray(source, extraLength - 2, &length);
                    Q_UNUSED(data);
                }
            }

            const auto padding = length % 4;
            if (padding > 0) {
                skip(source, padding, &length);
            }
            index++;
        }
        Q_UNUSED(index);

        return {};
    }
};

QT_END_NAMESPACE

#include "feid.moc"
