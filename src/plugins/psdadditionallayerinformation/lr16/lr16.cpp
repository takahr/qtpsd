// Copyright (C) 2025 Signal Slot Inc.
// SPDX-License-Identifier: LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include <QtPsdCore/qpsdadditionallayerinformationplugin.h>
#include <QtPsdCore/qpsdlayerinfo.h>

QT_BEGIN_NAMESPACE

class QPsdAdditionalLayerInformationLr16Plugin : public QPsdAdditionalLayerInformationPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QPsdAdditionalLayerInformationFactoryInterface" FILE "lr16.json")
public:
    // Color Balance
    QVariant parse(QIODevice *source , quint32 length) const override {
        const auto layerInfo = QPsdLayerInfo(source, length);
        Q_UNUSED(layerInfo);

        return {};
    }
};

QT_END_NAMESPACE

#include "lr16.moc"
