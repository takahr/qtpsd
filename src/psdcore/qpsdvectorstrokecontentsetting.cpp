// Copyright (C) 2024 Signal Slot Inc.
// SPDX-License-Identifier: LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qpsdvectorstrokecontentsetting.h"

#include "qpsddescriptor.h"

#include <QtCore/QLoggingCategory>

QT_BEGIN_NAMESPACE

Q_LOGGING_CATEGORY(lcQPsdVectorStrokeContentSetting, "qt.psdcore.vscg")

class QPsdVectorStrokeContentSetting::Private : public QSharedData
{
public:
    Type type;
    QString solidColor;
    GradientType gradientType = Liner;
    qreal angle;
    QList<QPair<qreal, qreal>> opacities;
    QList<QPair<qreal, QString>> colors;
    bool dither = false;
};

QPsdVectorStrokeContentSetting::QPsdVectorStrokeContentSetting()
    : QPsdSection()
    , d(new Private)
{}

QPsdVectorStrokeContentSetting::QPsdVectorStrokeContentSetting(QIODevice *source, quint32 length)
    : QPsdVectorStrokeContentSetting()
{
    // Vector stroke content setting
    auto cleanup = qScopeGuard([&] {
        // TODO
        // Q_ASSERT(length == 0);
    });

    // Key for data
    const auto key = readByteArray(source, 4, &length);

    // Version ( = 16 )
    const auto version = readU32(source, &length);
    Q_ASSERT(version == 16);

    // Descriptor of placed layer information
    QPsdDescriptor descriptor(source, &length);

    if (key == "SoCo") {
        d->type = SolidColor;
        const auto clr_ = descriptor.data().value("Clr ").value<QPsdDescriptor>().data();
        const int rd__ = clr_.value("Rd  ").toDouble();
        const int grn_ = clr_.value("Grn ").toDouble();
        const int bl__ = clr_.value("Bl  ").toDouble();
        d->solidColor = QString("#%1%2%3"_L1).arg(rd__, 2, 16, '0'_L1).arg(grn_, 2, 16, '0'_L1).arg(bl__, 2, 16, '0'_L1);
    } else if (key == "GdFl") {
        d->type = GradientFill;
        const auto grad = descriptor.data().value("Grad").value<QPsdDescriptor>().data();
        const auto trns = grad.value("Trns").toList();
        for (const auto &t : trns) {
            const auto trn = t.value<QPsdDescriptor>().data();
            const auto mdpn = trn.value("Mdpn").toInt();
            Q_UNUSED(mdpn);
            const auto opct = trn.value("Opct").value<QPsdUnitFloat>();
            const auto lctn = trn.value("Lctn").toDouble();
            d->opacities.append(qMakePair(lctn / 4096, opct.value() / 100.0));
        }
        const auto clrs = grad.value("Clrs").toList();
        for (const auto &c : clrs) {
            const auto clr = c.value<QPsdDescriptor>().data();
            const auto mdpn = clr.value("Mdpn").toInt();
            Q_UNUSED(mdpn);
            const auto lctn = clr.value("Lctn").toDouble();
            const auto type = clr.value("Type").value<QPsdEnum>();
            Q_ASSERT(type.type() == "Clry" && type.value() == "UsrS");

            const auto clr_ = clr.value("Clr ").value<QPsdDescriptor>().data();
            const int rd__ = clr_.value("Rd  ").toDouble();
            const int grn_ = clr_.value("Grn ").toDouble();
            const int bl__ = clr_.value("Bl  ").toDouble();
            const auto color = QString("#%1%2%3"_L1).arg(rd__, 2, 16, '0'_L1).arg(grn_, 2, 16, '0'_L1).arg(bl__, 2, 16, '0'_L1);
            d->colors.append(qMakePair(lctn / 4096, color));
        }

        const auto type = descriptor.data().value("Type").value<QPsdEnum>();
        Q_ASSERT(type.type() == "GrdT");
        if (type.value() == "Lnr ") {
            d->gradientType = Liner;
        } else if (type.value() == "Rdl ") {
            d->gradientType = Radial;
        } else if (type.value() == "Angl") {
            d->gradientType = Angle;
        } else if (type.value() == "Rflc") {
            d->gradientType = Reflected;
        } else if (type.value() == "Dmnd") {
            d->gradientType = Diamond;
        } else {
            qWarning() << type.value() << "not supported";
        }

        const auto angl = descriptor.data().value("Angl").value<QPsdUnitFloat>();
        // accept None: e.g. ag-psd/test/read/blend-if/src.psd
        Q_ASSERT(angl.unit() == QPsdUnitFloat::Angle || angl.unit() == QPsdUnitFloat::None);
        d->angle = angl.value();

        d->dither = descriptor.data().value("Dthr").toBool();

        const auto gim = descriptor.data().value("gradientsInterpolationMethod").value<QPsdEnum>();
        qCDebug(lcQPsdVectorStrokeContentSetting) << "gradientsInterpolationMethod" << gim.value();
    } else {
        qFatal() << key << "not implemented";
    }
}

QPsdVectorStrokeContentSetting::QPsdVectorStrokeContentSetting(const QPsdVectorStrokeContentSetting &other)
    : QPsdSection(other)
    , d(other.d)
{}

QPsdVectorStrokeContentSetting &QPsdVectorStrokeContentSetting::operator=(const QPsdVectorStrokeContentSetting &other)
{
    if (this != &other) {
        QPsdSection::operator=(other);
        d.operator=(other.d);
    }
    return *this;
}

QPsdVectorStrokeContentSetting::~QPsdVectorStrokeContentSetting() = default;

QPsdVectorStrokeContentSetting::Type QPsdVectorStrokeContentSetting::type() const
{
    return d->type;
}

QString QPsdVectorStrokeContentSetting::solidColor() const
{
    return d->solidColor;
}

QList<QPair<qreal, qreal>> QPsdVectorStrokeContentSetting::opacities() const
{
    return d->opacities;
}

QList<QPair<qreal, QString>> QPsdVectorStrokeContentSetting::colors() const
{
    return d->colors;
}

QPsdVectorStrokeContentSetting::GradientType QPsdVectorStrokeContentSetting::gradientType() const
{
    return d->gradientType;
}

qreal QPsdVectorStrokeContentSetting::angle() const
{
    return d->angle;
}

bool QPsdVectorStrokeContentSetting::isDither() const
{
    return d->dither;
}

QT_END_NAMESPACE
