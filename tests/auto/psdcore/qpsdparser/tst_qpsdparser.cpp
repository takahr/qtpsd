// Copyright (C) 2024 Signal Slot Inc.
// SPDX-License-Identifier: LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include <QtPsdCore/QPsdFileHeader>
#include <QtPsdCore/QPsdImageData>
#include <QtPsdCore/QPsdLayerRecord>
#include <QtPsdCore/QPsdParser>
#include <QtTest/QtTest>

class tst_QPsdParser : public QObject
{
    Q_OBJECT
private slots:
    void parse_data();
    void parse();

private:
    void addPsdFiles();
};

void tst_QPsdParser::addPsdFiles()
{
    QTest::addColumn<QString>("psd");

    std::function<void(QDir *dir, const QDir &baseDir)> findPsd;
    findPsd = [&](QDir *dir, const QDir &baseDir) {
        for (const QString &subdir : dir->entryList(QDir::Dirs | QDir::NoDotAndDotDot)) {
            dir->cd(subdir);
            findPsd(dir, baseDir);
            dir->cdUp();
        }
        for (const QString &fileName : dir->entryList(QStringList() << "*.psd")) {
            // Use relative path from base directory for test row name
            QString relativePath = baseDir.relativeFilePath(dir->filePath(fileName));
            QTest::newRow(relativePath.toLatin1().data()) << dir->filePath(fileName);
        }
    };

    QDir agPsd(QFINDTESTDATA("../../3rdparty/ag-psd/test/"));
    findPsd(&agPsd, QDir(agPsd));

    QDir psdTools(QFINDTESTDATA("../../3rdparty/psd-tools/tests/psd_files/"));
    findPsd(&psdTools, QDir(psdTools));
}

void tst_QPsdParser::parse_data()
{
    addPsdFiles();
}

void tst_QPsdParser::parse()
{
    QFETCH(QString, psd);

    QPsdParser parser;
    parser.load(psd);
}

QTEST_MAIN(tst_QPsdParser)
#include "tst_qpsdparser.moc"
