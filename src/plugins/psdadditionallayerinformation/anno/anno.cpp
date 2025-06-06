// Copyright (C) 2025 Signal Slot Inc.
// SPDX-License-Identifier: LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include <QtPsdCore/qpsdadditionallayerinformationplugin.h>

QT_BEGIN_NAMESPACE

class QPsdAdditionalLayerInformationAnnoPlugin : public QPsdAdditionalLayerInformationPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QPsdAdditionalLayerInformationFactoryInterface" FILE "anno.json")
public:
    // Annotations (Photoshop 6.0)
    QVariant parse(QIODevice *source , quint32 length) const override {
        auto cleanup = qScopeGuard([&] {
            Q_ASSERT(length <= 3);
        });

        const auto major = readU16(source, &length);
        const auto minor = readU16(source, &length);
        Q_ASSERT(major == 2 && minor == 1);

        const auto count = readU32(source, &length);
        for (int i = 0; i < count; i++) {
            const auto len = readU32(source,  &length);
            Q_UNUSED(len);
            const auto type = readByteArray(source, 4, &length);
            const auto open = readU8(source, &length);
            const auto flags = readU8(source, &length);
            const auto optionalBlocks = readU16(source, &length);
            const auto iconLocation = readRectangle(source, &length);
            const auto popupLocation = readRectangle(source, &length);
            const auto color = readColor(source, &length);
            const auto author = readPascalString(source, 2, &length);
            const auto name = readPascalString(source, 2, &length);
            const auto date = readPascalString(source, 2, &length);
            const auto contentLength = readU32(source, &length);
            const auto dataType = readByteArray(source, 4, &length);
            const auto dataLength = readU32(source, &length);
//            Q_ASSERT(dataLength == contentLength - 8);

            const auto rawData = readByteArray(source, dataLength, &length);
            if (dataType == "txtC"_ba) {
                if (dataLength > 2) {
                    const quint16 bom = (rawData[0] << 8) + rawData[1];
                    if (bom == 0xfeff) {
                        static QStringDecoder decoder(QStringDecoder::Utf16BE);
                        const auto data = decoder.decode(rawData);
                        Q_UNUSED(data);
                    } else {
                        const auto data = rawData;
                        Q_UNUSED(data);
                    }
                }
            } else if (dataType == "sndA"_ba) {
                const auto data = rawData;
                Q_UNUSED(data);
            } else {
                qFatal() << "unknown annotation data type" << dataType;
            }
        }

        return {};
    }
};

QT_END_NAMESPACE

#include "anno.moc"
