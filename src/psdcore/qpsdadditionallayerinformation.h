// Copyright (C) 2024 Signal Slot Inc.
// SPDX-License-Identifier: LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QPSDADDITIONALLAYERINFORMATION_H
#define QPSDADDITIONALLAYERINFORMATION_H

#include <QtPsdCore/qpsdsection.h>
#include <QtCore/QVariant>
#include <QtCore/qplugin.h>
#include <QtCore/qfactoryinterface.h>

QT_BEGIN_NAMESPACE

class Q_PSDCORE_EXPORT QPsdAdditionalLayerInformation : public QPsdSection
{
public:
    QPsdAdditionalLayerInformation();
    QPsdAdditionalLayerInformation(QIODevice *source, int padding = 0);
    QPsdAdditionalLayerInformation(const QPsdAdditionalLayerInformation &other);
    QPsdAdditionalLayerInformation &operator=(const QPsdAdditionalLayerInformation &other);
    ~QPsdAdditionalLayerInformation() override;

    QByteArray key() const;
    QVariant data() const;

private:
    class Private;
    QSharedDataPointer<Private> d;
};

QT_END_NAMESPACE

#endif // QPSDADDITIONALLAYERINFORMATION_H
