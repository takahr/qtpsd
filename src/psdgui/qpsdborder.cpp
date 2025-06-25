// Copyright (C) 2024 Signal Slot Inc.
// SPDX-License-Identifier: LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qpsdborder.h"

QT_BEGIN_NAMESPACE

class QPsdBorder::Private
{
public:
    Private(const QHash<QByteArray, QVariant> &descriptor)
    {
        // Enabled
        enabled = descriptor.value("enab").toBool();

        // Size
        const auto sz__ = descriptor.value("Sz  ").value<QPsdUnitFloat>();
        Q_ASSERT(sz__.unit() == QPsdUnitFloat::Pixels);
        size = sz__.value();

        // Position
        const auto styl = descriptor.value("Styl").value<QPsdEnum>();
        Q_ASSERT(styl.type() == "FStl");
        if (styl.value() == "OutF")
            position = Outer;
        else if (styl.value() == "InsF")
            position = Inner;
        else if (styl.value() == "CtrF")
            position = Center;
        else
            Q_ASSERT(false);

        // Blend Mode
        const auto md__ = descriptor.value("Md  ").value<QPsdEnum>();
        Q_ASSERT(md__.type() == "BlnM");
        blendMode = QPsdBlend::from(md__.value());

        // Opacity
        const auto opct = descriptor.value("Opct").value<QPsdUnitFloat>();
        Q_ASSERT(opct.unit() == QPsdUnitFloat::Percent);
        opacity = opct.value() / 100.0;

        // Over Print
        overPrint = descriptor.value("overprint").toBool();

        // Fill Type
        const auto pntT = descriptor.value("PntT").value<QPsdEnum>();
        Q_ASSERT(pntT.type() == "FrFl");
        if (pntT.value() == "SClr") {
            fillType = Solid;
        } else if (pntT.value() == "GrFl") {
            fillType = Gradient;
            const auto angl = descriptor.value("Angl").value<QPsdUnitFloat>();
            Q_ASSERT(angl.unit() == QPsdUnitFloat::Angle);
            qDebug() << "Gradient Fill" << angl.value();
            const auto type = descriptor.value("Type").value<QPsdEnum>();
            Q_ASSERT(type.type() == "GrdT");
            qDebug() << "Gradient Type" << type.value();
            const auto grad = descriptor.value("Grad").value<QPsdDescriptor>().data();
            qDebug() << grad;
            const auto algn = grad.value("Algn").toBool();
            qDebug() << "Align" << algn;
            const auto ofst = grad.value("Ofst").value<QPsdDescriptor>().data();
            qDebug() << "Offset" << ofst;
            const auto rvrs = grad.value("Rvrs").toBool();
            qDebug() << "Reverse" << rvrs;
            const auto dthr = grad.value("Dthr").toBool();
            qDebug() << "Dither" << dthr;
            const auto scl_ = grad.value("Scl ").value<QPsdUnitFloat>();
            qDebug() << "Scale" << scl_.value() << scl_.unit();
        } else {
            qDebug() << descriptor;
            // qFatal() << pntT.value();
        }
        // Color
        const auto clr_ = descriptor.value("Clr ").value<QPsdDescriptor>().data();
        const auto rd__ = clr_.value("Rd  ").toDouble();
        const auto grn_ = clr_.value("Grn ").toDouble();
        const auto bl__ = clr_.value("Bl  ").toDouble();
        color = QColor::fromRgbF(rd__ / 255.0, grn_ / 255.0, bl__ / 255.0);

    }

    bool enabled;
    int size;
    Position position;
    QPsdBlend::Mode blendMode;
    qreal opacity;
    bool overPrint;
    FillType fillType;
    QColor color;
};

QPsdBorder::QPsdBorder(const QPsdDescriptor &descriptor)
    : d(new Private(descriptor.data()))
{}

QPsdBorder::~QPsdBorder() = default;

bool QPsdBorder::isEnable() const
{
    return d->enabled;
}

int QPsdBorder::size() const
{
    return d->size;
}

QPsdBorder::Position QPsdBorder::position() const
{
    return d->position;
}

QPsdBlend::Mode QPsdBorder::blendMode() const
{
    return d->blendMode;
}

qreal QPsdBorder::opacity() const
{
    return d->opacity;
}

bool QPsdBorder::isOverPrint() const
{
    return d->overPrint;
}

QPsdBorder::FillType QPsdBorder::fillType() const
{
    return d->fillType;
}

QColor QPsdBorder::color() const
{
    return d->color;
}

QT_END_NAMESPACE
