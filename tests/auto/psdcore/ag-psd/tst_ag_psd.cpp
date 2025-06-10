// Copyright (C) 2024 Signal Slot Inc.
// SPDX-License-Identifier: LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include <QtGui/QImage>
#include <QtPsdCore/QPsdFileHeader>
#include <QtPsdCore/QPsdImageData>
#include <QtPsdCore/QPsdLayerRecord>
#include <QtPsdCore/QPsdParser>
#include <QtPsdGui/qpsdguiglobal.h>
#include <QtTest/QtTest>

class tst_QAgPsd : public QObject
{
    Q_OBJECT
private slots:
    void parse_data();
    void parse();
    void imageData_data();
    void imageData();
    void layerImageData_data();
    void layerImageData();

private:
    void addPsdFiles();
};

void tst_QAgPsd::addPsdFiles()
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

void tst_QAgPsd::parse_data()
{
    addPsdFiles();
}

void tst_QAgPsd::parse()
{
    QFETCH(QString, psd);

    QPsdParser parser;
    parser.load(psd);
}

void tst_QAgPsd::imageData_data()
{
    addPsdFiles();
}

void tst_QAgPsd::imageData()
{
    QFETCH(QString, psd);

    QPsdParser parser;
    parser.load(psd);

    const auto header = parser.fileHeader();

    const auto imageData = parser.imageData();
    if (imageData.width() > 0 && imageData.height() > 0) {
        const  QImage image = QtPsdGui::imageDataToImage(imageData, header);
        QVERIFY(!image.isNull());
        QCOMPARE(image.width(), imageData.width());
        QCOMPARE(image.height(), imageData.height());
    }
}

void tst_QAgPsd::layerImageData_data()
{
    addPsdFiles();
}

void tst_QAgPsd::layerImageData()
{
    QFETCH(QString, psd);

    QPsdParser parser;
    parser.load(psd);

    const auto header = parser.fileHeader();

    const auto layerAndMaskInfo = parser.layerAndMaskInformation();
    const auto layerInfo = layerAndMaskInfo.layerInfo();
    const auto layers = layerInfo.records();

    for (int i = 0; i < layers.size(); ++i) {
        const auto &layer = layers[i];
        const auto imageData = layer.imageData();

        // Skip empty layers
        if (imageData.width() == 0 || imageData.height() == 0) {
            continue;
        }

        const  QImage image = QtPsdGui::imageDataToImage(imageData, header);
        const auto depth = imageData.depth();
        QVERIFY(!image.isNull());
        QCOMPARE(image.width(), imageData.width());
        QCOMPARE(image.height(), imageData.height());
        // Additional validation based on format
        if (header.colorMode() == QPsdFileHeader::RGB && imageData.hasAlpha() && depth == 8) {
            QVERIFY(image.hasAlphaChannel());
        }
    }
}

QTEST_MAIN(tst_QAgPsd)
#include "tst_ag_psd.moc"
