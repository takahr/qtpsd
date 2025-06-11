// Copyright (C) 2025 Signal Slot Inc.
// SPDX-License-Identifier: LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include <QtPsdCore/qpsddescriptorplugin.h>
#include <QtPsdCore/qpsddescriptor.h>
#include <QtPsdCore/qpsdunitfloat.h>

QT_BEGIN_NAMESPACE

class QPsdDescriptorPthPlugin : public QPsdDescriptorPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QPsdDescriptorFactoryInterface" FILE "pth.json")
public:
    // File Path
    QVariant parse(QIODevice *source , quint32 *length) const override {
        const auto len = readU32(source, length);
        Q_UNUSED(len);
        const auto signature = readByteArray(source, 4, length);
        const auto pathSize = readU32LE(source, length);
        Q_UNUSED(pathSize);
        const auto path = readStringLE(source, length);
        
        QVariantMap res;
        res.insert(signature, path);

        return res;
    }
};

QT_END_NAMESPACE

#include "pth.moc"
