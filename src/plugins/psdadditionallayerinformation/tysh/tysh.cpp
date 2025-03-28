// Copyright (C) 2024 Signal Slot Inc.
// SPDX-License-Identifier: LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include <QtPsdCore/qpsdadditionallayerinformationplugin.h>
#include <QtPsdCore/qpsdtypetoolobjectsetting.h>

QT_BEGIN_NAMESPACE

class QPsdAdditionalLayerInformationTyShPlugin : public QPsdAdditionalLayerInformationPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QPsdAdditionalLayerInformationFactoryInterface" FILE "tysh.json")
public:
    // Type tool object setting
    QVariant parse(QIODevice *source , quint32 length) const override {
        auto cleanup = qScopeGuard([&] {
            // Q_ASSERT(length == 0);
        });
        QPsdTypeToolObjectSetting ret(source, &length);
        return QVariant::fromValue(ret);
    }
};

QT_END_NAMESPACE

#include "tysh.moc"
