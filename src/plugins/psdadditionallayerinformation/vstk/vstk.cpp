// Copyright (C) 2024 Signal Slot Inc.
// SPDX-License-Identifier: LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include <QtPsdCore/qpsdadditionallayerinformationplugin.h>
#include <QtPsdCore/qpsdvectorstrokedata.h>

QT_BEGIN_NAMESPACE

class QPsdAdditionalLayerInformationVstkPlugin : public QPsdAdditionalLayerInformationPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QPsdAdditionalLayerInformationFactoryInterface" FILE "vstk.json")
public:
    QVariant parse(QIODevice *source , quint32 length) const override {
        return QVariant::fromValue(QPsdVectorStrokeData(source, length));
    }
};

QT_END_NAMESPACE

#include "vstk.moc"
