// Copyright (C) 2025 Signal Slot Inc.
// SPDX-License-Identifier: LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include <QtPsdCore/qpsddescriptorplugin.h>
#include <QtPsdCore/qpsddescriptor.h>

QT_BEGIN_NAMESPACE

class QPsdDescriptorObjPlugin : public QPsdDescriptorPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QPsdDescriptorFactoryInterface" FILE "obj.json")
public:
    // Descriptor
    QVariant parse(QIODevice *source , quint32 *length) const override {
        const auto count = readS32(source, length);
        QVariantList res;
        for (int i = 0; i < count; i++) {
            const auto type = readByteArray(source, 4, length);
            if (type == "prop"_ba) {
                const auto name = readString(source, length);
                Q_UNUSED(name);
                const auto sizeClassID = readU32(source, length);
                const auto classID = readByteArray(source, sizeClassID == 0 ? 4 : sizeClassID, length);
                Q_UNUSED(classID);

                const auto sizeKeyID = readU32(source, length);
                const auto keyID = readByteArray(source, sizeKeyID == 0 ? 4 : sizeKeyID, length);
                Q_UNUSED(keyID);
            } else if (type == "Clss"_ba) {
                const auto name = readString(source, length);
                Q_UNUSED(name);
                const auto sizeClassID = readU32(source, length);
                const auto classID = readByteArray(source, sizeClassID == 0 ? 4 : sizeClassID, length);
                Q_UNUSED(classID);
            } else if (type == "Enmr"_ba) {
                const auto name = readString(source, length);
                Q_UNUSED(name);
                const auto sizeClassID = readU32(source, length);
                const auto classID = readByteArray(source, sizeClassID == 0 ? 4 : sizeClassID, length);
                Q_UNUSED(classID);

                const auto sizeTypeID = readU32(source, length);
                const auto typeID = readByteArray(source, sizeTypeID == 0 ? 4 : sizeTypeID, length);
                Q_UNUSED(typeID);

                const auto sizeValue = readU32(source, length);
                const auto value = readByteArray(source, sizeValue == 0 ? 4 : sizeValue, length);
                Q_UNUSED(sizeValue);
            } else if (type == "rele"_ba) {
                const auto name = readString(source, length);
                Q_UNUSED(name);
                const auto sizeClassID = readU32(source, length);
                const auto classID = readByteArray(source, sizeClassID == 0 ? 4 : sizeClassID, length);
                Q_UNUSED(classID);

                const auto offset = readS32(source, length);
                Q_UNUSED(offset);
            } else if (type == "Idnt"_ba) {
                const auto ref = readS32(source, length);
                Q_UNUSED(ref);
            } else if (type == "indx"_ba) {
                const auto index = readS32(source, length);
                Q_UNUSED(index);
            } else if (type == "name"_ba) {
                const auto name = readString(source, length);
                Q_UNUSED(name);
                const auto sizeClassID = readU32(source, length);
                const auto classID = readByteArray(source, sizeClassID == 0 ? 4 : sizeClassID, length);
                Q_UNUSED(classID);

                const auto nameRef = readString(source, length);
                Q_UNUSED(nameRef);
            }
        }

        return res;
    }
};

QT_END_NAMESPACE

#include "obj.moc"
