// Copyright (C) 2024 Signal Slot Inc.
// SPDX-License-Identifier: LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include <QtPsdCore/qpsdeffectslayerplugin.h>

QT_BEGIN_NAMESPACE

class QPsdEffectsLayerCmnSPlugin : public QPsdEffectsLayerPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QPsdEffectsLayerFactoryInterface" FILE "cmns.json")
public:
    // Effects layer, common state info
    QVariant parse(QByteArrayView key, QIODevice *source , quint32 *length) const override {
        Q_ASSERT(key == "cmnS");
        // https://www.adobe.com/devnet-apps/photoshop/fileformatashtml/PhotoshopFileFormats.htm#50577409_41831

        // Size of next three items: 7
        const auto size = readU32(source, length);
        Q_ASSERT(size == 7);

        // Version: 0
        const auto version = readU32(source, length);
        Q_ASSERT(version == 0);

        // Visible: always true
        const auto visible = readU8(source, length);
        Q_ASSERT(visible == 1);

        // Unused: always 0
        skip(source, 2, length);

        return QVariant();
    }
};

QT_END_NAMESPACE

#include "cmns.moc"
