// Copyright (C) 2024 Signal Slot Inc.
// SPDX-License-Identifier: LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include <QtTest/QtTest>
#include <QtPsdCore/QPsdParser>
#include <QtPsdCore/QPsdLayerRecord>
#include <QtPsdCore/QPsdImageData>
#include <QtPsdCore/QPsdFileHeader>
#include <QImage>

class tst_QAgPsd : public QObject
{
    Q_OBJECT
private slots:
    void parse_data();
    void parse();
    void imageData_data();
    void imageData();

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
    
    // Test parser's imageData first
    const auto parserImageData = parser.imageData();
    if (parserImageData.width() > 0 && parserImageData.height() > 0) {
        const auto data = parserImageData.toImage(header.colorMode());
        QVERIFY2(!data.isEmpty(), "Parser imageData: Failed to generate image data");
        
        // Create QImage based on color mode and depth
        QImage image;
        const auto w = parserImageData.width();
        const auto h = parserImageData.height();
        const auto depth = parserImageData.depth();
        
        switch (header.colorMode()) {
        case QPsdFileHeader::Grayscale:
            if (depth == 8) {
                image = QImage(reinterpret_cast<const uchar *>(data.constData()),
                              w, h, depth / 8 * w, QImage::Format_Grayscale8);
            }
            break;
            
        case QPsdFileHeader::RGB:
            if (depth == 8) {
                // For RGB images, we always create RGB888 format
                // The toImage() method should handle alpha channel internally
                image = QImage(reinterpret_cast<const uchar *>(data.constData()),
                              w, h, 3 * w, QImage::Format_RGB888);
            }
            break;
            
        default:
            break;
        }
        
        if (!image.isNull()) {
            QCOMPARE(image.width(), w);
            QCOMPARE(image.height(), h);
        }
    }
    
    const auto layerAndMaskInfo = parser.layerAndMaskInformation();
    const auto layerInfo = layerAndMaskInfo.layerInfo();
    const auto layers = layerInfo.records();

    // Test image generation for each layer
    for (int i = 0; i < layers.size(); ++i) {
        const auto &layer = layers[i];
        const auto imageData = layer.imageData();

        // Skip empty layers
        if (imageData.width() == 0 || imageData.height() == 0) {
            continue;
        }

        // Generate raw image data
        const auto data = imageData.toImage(header.colorMode());
        QVERIFY2(!data.isEmpty(),
                 qPrintable(QString("Layer %1: Failed to generate image data").arg(i)));

        // Create QImage based on color mode and depth
        QImage image;
        const auto w = imageData.width();
        const auto h = imageData.height();
        const auto depth = imageData.depth();

        switch (header.colorMode()) {
        case QPsdFileHeader::Grayscale:
            if (depth == 8) {
                image = QImage(reinterpret_cast<const uchar *>(data.constData()),
                              w, h, depth / 8 * w, QImage::Format_Grayscale8);
            }
            break;

        case QPsdFileHeader::RGB:
            if (depth == 8) {
                if (imageData.hasAlpha()) {
                    image = QImage(reinterpret_cast<const uchar *>(data.constData()),
                                  w, h, 4 * w, QImage::Format_ARGB32);
                } else {
                    image = QImage(reinterpret_cast<const uchar *>(data.constData()),
                                  w, h, 3 * w, QImage::Format_RGB888);
                }
            }
            break;

        default:
            // Skip unsupported color modes for now
            continue;
        }

        // Verify the image is not null for supported formats
        if (!image.isNull()) {
            QCOMPARE(image.width(), w);
            QCOMPARE(image.height(), h);
            
            // Additional validation based on format
            if (header.colorMode() == QPsdFileHeader::RGB && imageData.hasAlpha() && depth == 8) {
                QVERIFY(image.hasAlphaChannel());
            }
        } else if (depth == 8) {
            // For 8-bit images in supported color modes, we expect a valid QImage
            QFAIL(qPrintable(QString("Layer %1: Generated QImage is null for 8-bit image").arg(i)));
        }
    }
}

QTEST_MAIN(tst_QAgPsd)
#include "tst_ag_psd.moc"
