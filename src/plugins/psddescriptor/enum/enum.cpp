// Copyright (C) 2024 Signal Slot Inc.
// SPDX-License-Identifier: LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include <QtPsdCore/qpsddescriptorplugin.h>
#include <QtPsdCore/qpsdenum.h>

QT_BEGIN_NAMESPACE

class QPsdDescriptorEnumPlugin : public QPsdDescriptorPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QPsdDescriptorFactoryInterface" FILE "enum.json")
public:
    // enum
    QVariant parse(QIODevice *source , quint32 *length) const override {
        return QVariant::fromValue(QPsdEnum(source, length));
    }
};

QT_END_NAMESPACE

#include "enum.moc"
