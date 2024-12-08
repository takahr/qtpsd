// Copyright (C) 2024 Signal Slot Inc.
// SPDX-License-Identifier: LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include <QtPsdCore/qpsdadditionallayerinformationplugin.h>
#include <QtPsdCore/qpsdvectorstrokecontentsetting.h>

QT_BEGIN_NAMESPACE

class QPsdAdditionalLayerInformationVscgPlugin : public QPsdAdditionalLayerInformationPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QPsdAdditionalLayerInformationFactoryInterface" FILE "vscg.json")
public:
    QVariant parse(QIODevice *source , quint32 length) const override {
        return QVariant::fromValue(QPsdVectorStrokeContentSetting(source, length));
    }
};

QT_END_NAMESPACE

#include "vscg.moc"
