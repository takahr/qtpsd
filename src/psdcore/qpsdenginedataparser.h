// Copyright (C) 2024 Signal Slot Inc.
// SPDX-License-Identifier: LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QPSDENGINEDATACORE_H
#define QPSDENGINEDATACORE_H

#include <QtPsdCore/qpsdcoreglobal.h>

QT_BEGIN_NAMESPACE

class Q_PSDCORE_EXPORT QPsdEngineDataParser
{
public:
    class ParseError {
    public:
        enum Error {
            NoError,
            IOError,
            UnexpectedEndOfData,
            InvalidData,
            UnknownError
        };
        ParseError(const QString &_errorMessage = QString())
            : errorMessage(_errorMessage)
        {}
        operator bool() const { return !errorMessage.isEmpty(); }
        // Error error = NoError;
        QString errorMessage;
    };
    static QCborMap parseEngineData(const QByteArray &data, ParseError* error = nullptr);

private:
    QPsdEngineDataParser() = delete;
    class Private;
};

QT_END_NAMESPACE

#endif // QPSDENGINEDATACORE_H
