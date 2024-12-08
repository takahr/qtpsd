// Copyright (C) 2024 Signal Slot Inc.
// SPDX-License-Identifier: LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include <QtPsdCore/qpsddescriptorplugin.h>

QT_BEGIN_NAMESPACE

class QPsdDescriptorDoubPlugin : public QPsdDescriptorPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QPsdDescriptorFactoryInterface" FILE "doub.json")
public:
        // Double
    QVariant parse(QIODevice *source , quint32 *length) const override {
        return readDouble(source, length);
    }
};

QT_END_NAMESPACE

#include "doub.moc"
