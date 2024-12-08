// Copyright (C) 2024 Signal Slot Inc.
// SPDX-License-Identifier: LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include <QtTest/QtTest>
#include <QtPsdCore/QPsdParser>

class tst_QAgPsd : public QObject
{
    Q_OBJECT
private slots:
    void parse_data();
    void parse();
};

void tst_QAgPsd::parse_data()
{
    QTest::addColumn<QString>("psd");
    QDir dir;
    dir.cd(QFINDTESTDATA("ag-psd/test/"));

    std::function<void(QDir *dir)> findPsd;
    findPsd = [&](QDir *dir) {
        for (const QString &subdir : dir->entryList(QDir::Dirs | QDir::NoDotAndDotDot)) {
            dir->cd(subdir);
            findPsd(dir);
            dir->cdUp();
        }
        for (const QString &fileName : dir->entryList(QStringList() << "*.psd")) {
            QTest::newRow(dir->filePath(fileName).toLatin1().data()) << dir->filePath(fileName);
        }
    };

    findPsd(&dir);
}

void tst_QAgPsd::parse()
{
    QFETCH(QString, psd);

    QPsdParser parser;
    parser.load(psd);
}

QTEST_MAIN(tst_QAgPsd)
#include "tst_ag_psd.moc"
