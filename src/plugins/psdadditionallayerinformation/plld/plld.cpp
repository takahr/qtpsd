// Copyright (C) 2024 Signal Slot Inc.
// SPDX-License-Identifier: LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include <QtPsdCore/qpsdadditionallayerinformationplugin.h>
#include <QtPsdCore/qpsdplacedlayer.h>

QT_BEGIN_NAMESPACE

class QPsdAdditionalLayerInformationPlLdPlugin : public QPsdAdditionalLayerInformationPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QPsdAdditionalLayerInformationFactoryInterface" FILE "plld.json")
public:
    // Placed Layer (replaced by SoLd in Photoshop CS3)
    QVariant parse(QIODevice *source , quint32 length) const override {
        return QVariant::fromValue(QPsdPlacedLayer(source, length));
    }
};

QT_END_NAMESPACE

#include "plld.moc"
