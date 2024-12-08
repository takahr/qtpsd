// Copyright (C) 2024 Signal Slot Inc.
// SPDX-License-Identifier: LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include <QtTest/QtTest>
#include <QtPsdCore/QPsdEngineDataParser>

#include <QThread>

class tst_QPsdEngineDataParser : public QObject
{
    Q_OBJECT
private slots:
    void parse_data();
    void parse();
};

void tst_QPsdEngineDataParser::parse_data()
{
    QTest::addColumn<QString>("ed");
    QTest::addColumn<QString>("js");
    QDir dir;
    dir.cd(QFINDTESTDATA("data/"));
    const auto fileNames = dir.entryList(QStringList() << "*.ed");
    for (const QString &filename : fileNames) {
        QString reference =  QFileInfo(filename).baseName() + ".json";
        QTest::newRow(dir.filePath(filename).toLatin1().data()) << dir.filePath(filename) << dir.filePath(reference);
    }
}

void tst_QPsdEngineDataParser::parse()
{
    QFETCH(QString, ed);
    QFETCH(QString, js);

    QFile engineDataFile(ed);
    QVERIFY(engineDataFile.open(QIODevice::ReadOnly));
    const QByteArray engineDataData = engineDataFile.readAll();
    engineDataFile.close();

    QPsdEngineDataParser::ParseError error;
    const auto cbor = QPsdEngineDataParser::parseEngineData(engineDataData, &error);
    QVERIFY(!error);

    // qDebug() << cbor;
    // qDebug().noquote().nospace() << QString::fromLatin1(QJsonDocument(cbor.toJsonObject()).toJson());

    QFile jsonFile(js);
    QVERIFY(jsonFile.open(QIODevice::ReadOnly));
    const QByteArray jsonData = jsonFile.readAll();
    jsonFile.close();

    const auto json = QJsonDocument::fromJson(jsonData);
    QCOMPARE(cbor.toJsonObject(), json.object());
}

QTEST_MAIN(tst_QPsdEngineDataParser)
#include "tst_qpsdenginedataparser.moc"
