// Copyright (C) 2025 Signal Slot Inc.
// SPDX-License-Identifier: LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include <QtPsdCore/qpsddescriptorplugin.h>
#include <QtPsdCore/qpsddescriptor.h>
#include <QtPsdCore/qpsdunitfloat.h>

QT_BEGIN_NAMESPACE

class QPsdDescriptorObArPlugin : public QPsdDescriptorPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QPsdDescriptorFactoryInterface" FILE "obar.json")
public:
    // Descriptor
    QVariant parse(QIODevice *source , quint32 *length) const override {
        const auto version = readU32(source, length);
        Q_ASSERT(version == 16);
        const auto name = readString(source, length);
        Q_UNUSED(name);
        const auto sizeClassID = readU32(source, length);
        const auto classID = readByteArray(source, sizeClassID == 0 ? 4 : sizeClassID, length);
        Q_UNUSED(classID);

        QVariantMap res;
        const auto count = readS32(source, length);
        for (int i = 0; i < count; i++) {
            const auto sizeType1 = readS32(source, length);
            const auto type1 = readByteArray(source, sizeType1 == 0 ? 4 : sizeType1, length);
            const auto signature = readByteArray(source, 4, length);
            Q_UNUSED(signature);

            const auto unit = readByteArray(source ,4, length);
            const auto countValues = readS32(source, length);
            QVariantList values;
            for (int j = 0; j < countValues; j++) {
                const auto value = readDouble(source, length);
                const auto unitValue = QPsdUnitFloat(unit, value);
                values.append(QVariant::fromValue(unitValue));
            }

            res.insert(type1, values);
        }

        return res;
    }
};

QT_END_NAMESPACE

#include "obar.moc"
