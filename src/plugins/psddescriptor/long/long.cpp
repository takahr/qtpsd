// Copyright (C) 2024 Signal Slot Inc.
// SPDX-License-Identifier: LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include <QtPsdCore/qpsddescriptorplugin.h>

QT_BEGIN_NAMESPACE

class QPsdDescriptorLongPlugin : public QPsdDescriptorPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QPsdDescriptorFactoryInterface" FILE "long.json")
public:
    // Integer
    QVariant parse(QIODevice *source , quint32 *length) const override {
        return readS32(source, length);
    }
};

QT_END_NAMESPACE

#include "long.moc"
