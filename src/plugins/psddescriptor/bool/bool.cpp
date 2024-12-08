// Copyright (C) 2024 Signal Slot Inc.
// SPDX-License-Identifier: LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include <QtPsdCore/qpsddescriptorplugin.h>

QT_BEGIN_NAMESPACE

class QPsdDescriptorBoolPlugin : public QPsdDescriptorPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QPsdDescriptorFactoryInterface" FILE "bool.json")
public:
    // Boolean
    QVariant parse(QIODevice *source , quint32 *length) const override {
        return readU8(source, length) == 1;
    }
};

QT_END_NAMESPACE

#include "bool.moc"
