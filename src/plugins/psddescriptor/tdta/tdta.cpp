// Copyright (C) 2024 Signal Slot Inc.
// SPDX-License-Identifier: LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include <QtPsdCore/qpsddescriptorplugin.h>

QT_BEGIN_NAMESPACE

Q_LOGGING_CATEGORY(lcQPsdDescriptorTdtaPlugin, "qt.psdcore.plugins.descriptor.tdta")

class QPsdDescriptorTdtaPlugin : public QPsdDescriptorPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QPsdDescriptorFactoryInterface" FILE "tdta.json")
public:
    // Raw Data
    QVariant parse(QIODevice *source , quint32 *length) const override {
        auto size = readS32(source, length);
        qCDebug(lcQPsdDescriptorTdtaPlugin) << size;
        return readByteArray(source, size, length);
    }
};

QT_END_NAMESPACE

#include "tdta.moc"
