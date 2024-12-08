// Copyright (C) 2024 Signal Slot Inc.
// SPDX-License-Identifier: LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qpsdexporterplugin.h"

#include <QtCore/QCryptographicHash>

QT_BEGIN_NAMESPACE

QMimeDatabase QPsdExporterPlugin::mimeDatabase;

QPsdExporterPlugin::QPsdExporterPlugin(QObject *parent)
    : QPsdAbstractPlugin(parent)
{}

namespace {
static const QRegularExpression notPrintable("[^a-zA-Z0-9]"_L1);
}

QString QPsdExporterPlugin::toUpperCamelCase(const QString &str, const QString &separator)
{
    QString s = str;
    s.replace(QRegularExpression("([a-z0-9])([A-Z])"_L1), "\\1 \\2"_L1);
    QStringList parts = s.split(notPrintable, Qt::SkipEmptyParts);
    for (auto &part : parts) {
        part = part.toLower();
        part[0] = part[0].toUpper();
    }
    return parts.join(separator);
}

QString QPsdExporterPlugin::toLowerCamelCase(const QString &str)
{
    QString s = str;
    s.replace(QRegularExpression("([a-z0-9])([A-Z])"_L1), "\\1 \\2"_L1);
    QStringList parts = s.split(notPrintable, Qt::SkipEmptyParts);
    bool first = true;
    for (auto &part : parts) {
        part = part.toLower();
        if (first) {
            first = false;
        } else {
            part[0] = part[0].toUpper();
        }
    }
    return parts.join(QString());
}

QString QPsdExporterPlugin::toSnakeCase(const QString &str)
{
    QString s = str;
    s.replace(QRegularExpression("([a-z0-9])([A-Z])"_L1), "\\1 \\2"_L1);
    QStringList parts = s.split(notPrintable, Qt::SkipEmptyParts);
    return parts.join("_"_L1).toLower();
}

QString QPsdExporterPlugin::toKebabCase(const QString &str)
{
    QString s = str;
    s.replace(QRegularExpression("([a-z0-9])([A-Z])"_L1), "\\1 \\2"_L1);
    QStringList parts = s.split(notPrintable, Qt::SkipEmptyParts);
    return parts.join("-"_L1).toLower();
}

QString QPsdExporterPlugin::imageFileName(const QString &name, const QString &format)
{
    QFileInfo fileInfo(name);
    QString suffix = fileInfo.suffix();
    QString basename = fileInfo.completeBaseName();
    
    QString snakeName = toSnakeCase(basename);
    if (snakeName.length() < basename.length()) {
        basename = QString::fromLatin1(QCryptographicHash::hash(basename.toUtf8(), QCryptographicHash::Sha256).toHex());
    } else {
        if (suffix.isEmpty()) {
            return u"%1.%2"_s.arg(snakeName, format.toLower());
        }

        if (suffix.compare(format, Qt::CaseInsensitive) == 0) {
            return u"%1.%2"_s.arg(basename, suffix);
        }

        if ((format.compare("jpg"_L1, Qt::CaseInsensitive) || format.compare("jpeg"_L1, Qt::CaseInsensitive))
            && (suffix.compare("jpg"_L1, Qt::CaseInsensitive) || suffix.compare("jpeg"_L1, Qt::CaseInsensitive))) {
            return u"%1.%2"_s.arg(basename, suffix);
        }

        basename = snakeName;
    }

    return u"%1.%2"_s.arg(basename, format.toLower());
}

QT_END_NAMESPACE
