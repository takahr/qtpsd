// Copyright (C) 2024 Signal Slot Inc.
// SPDX-License-Identifier: LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include <QtTest/QtTest>
#include <QtGui/QImage>
#include <QtGui/QPainter>
#include <QtWidgets/QApplication>
#include <QtPsdCore/QPsdParser>
#include <QtPsdCore/QPsdFileHeader>
#include <QtPsdCore/QPsdImageData>
#include <QtPsdGui/qpsdguiglobal.h>
#include <QtPsdWidget/QPsdView>
#include <QtPsdWidget/QPsdWidgetTreeItemModel>

class tst_QPsdView : public QObject
{
    Q_OBJECT
private slots:
    void compareRendering_data();
    void compareRendering();

private:
    void addPsdFiles();
    QImage renderPsdView(const QString &filePath);
    double compareImages(const QImage &img1, const QImage &img2);
};

void tst_QPsdView::addPsdFiles()
{
    QTest::addColumn<QString>("psd");

    const QString basePath = QFINDTESTDATA("../../psdcore/ag-psd/ag-psd/test/");
    QDir baseDir(basePath);
    QDir dir(basePath);

    std::function<void(QDir *dir)> findPsd;
    findPsd = [&](QDir *dir) {
        for (const QString &subdir : dir->entryList(QDir::Dirs | QDir::NoDotAndDotDot)) {
            dir->cd(subdir);
            findPsd(dir);
            dir->cdUp();
        }
        for (const QString &fileName : dir->entryList(QStringList() << "*.psd")) {
            // Use relative path from base directory for test row name
            QString relativePath = baseDir.relativeFilePath(dir->filePath(fileName));
            QTest::newRow(relativePath.toLatin1().data()) << dir->filePath(fileName);
        }
    };

    findPsd(&dir);
}

QImage tst_QPsdView::renderPsdView(const QString &filePath)
{
    // Load the PSD file
    QPsdParser parser;
    parser.load(filePath);

    // Create the model
    QPsdWidgetTreeItemModel model;
    model.fromParser(parser);

    // Create the view
    QPsdView view;
    view.setModel(&model);
    view.setShowChecker(false);
    view.reset();

    // Get the size from the model
    const QSize canvasSize = model.size();
    if (canvasSize.isEmpty()) {
        return QImage();
    }

    // Set widget attributes for offscreen rendering
    view.setAttribute(Qt::WA_DontShowOnScreen);

    // Ensure the widget is properly sized and laid out
    view.resize(canvasSize);
    view.show(); // Still need to "show" for layout to work, but WA_DontShowOnScreen prevents actual display
    QApplication::processEvents();

    // Render to image using grab()
    QPixmap pixmap = view.grab();
    QImage rendered = pixmap.toImage();

    return rendered;
}

double tst_QPsdView::compareImages(const QImage &img1, const QImage &img2)
{
    if (img1.size() != img2.size()) {
        return 0.0;
    }

    if (img1.isNull() || img2.isNull()) {
        return 0.0;
    }

    // Convert both images to the same format for comparison
    // Handle special cases where conversion might fail
    QImage a, b;

    // Convert to a common format for comparison
    if (img1.format() == QImage::Format_RGB888 || img1.format() == QImage::Format_BGR888) {
        a = img1.convertToFormat(QImage::Format_RGB32);
    } else {
        a = img1.convertToFormat(QImage::Format_ARGB32);
    }

    if (img2.format() == QImage::Format_RGB888 || img2.format() == QImage::Format_BGR888) {
        b = img2.convertToFormat(QImage::Format_RGB32);
    } else {
        b = img2.convertToFormat(QImage::Format_ARGB32);
    }

    if (a.isNull() || b.isNull()) {
        return 0.0;
    }

    qint64 totalDiff = 0;
    qint64 maxPossibleDiff = 0;

    for (int y = 0; y < a.height(); ++y) {
        const QRgb *lineA = reinterpret_cast<const QRgb *>(a.scanLine(y));
        const QRgb *lineB = reinterpret_cast<const QRgb *>(b.scanLine(y));

        for (int x = 0; x < a.width(); ++x) {
            QRgb pixelA = lineA[x];
            QRgb pixelB = lineB[x];

            int dr = qAbs(qRed(pixelA) - qRed(pixelB));
            int dg = qAbs(qGreen(pixelA) - qGreen(pixelB));
            int db = qAbs(qBlue(pixelA) - qBlue(pixelB));
            int da = qAbs(qAlpha(pixelA) - qAlpha(pixelB));

            totalDiff += dr + dg + db + da;
            maxPossibleDiff += 4 * 255; // Maximum difference per pixel
        }
    }

    // Return similarity as percentage (100% = identical)
    if (maxPossibleDiff == 0) {
        return 100.0;
    }

    return 100.0 * (1.0 - static_cast<double>(totalDiff) / static_cast<double>(maxPossibleDiff));
}

void tst_QPsdView::compareRendering_data()
{
    addPsdFiles();
}

void tst_QPsdView::compareRendering()
{
    QFETCH(QString, psd);

    // Load the PSD and get the toplevel image data
    QPsdParser parser;
    parser.load(psd);

    const auto header = parser.fileHeader();
    const auto imageData = parser.imageData();

    // Skip if no image data
    if (imageData.width() == 0 || imageData.height() == 0) {
        QSKIP("No image data in PSD file");
    }

    // Convert toplevel imageData to QImage
    QImage toplevelImage = QtPsdGui::imageDataToImage(imageData, header);
    QVERIFY(!toplevelImage.isNull());

    // Render using QPsdView
    QImage viewRendering = renderPsdView(psd);
    QVERIFY(!viewRendering.isNull());

    // Compare sizes
    QCOMPARE(viewRendering.size(), toplevelImage.size());

    // Compare images directly
    double similarity = compareImages(toplevelImage, viewRendering);

    // Log the similarity for information
    qDebug() << "File:" << QFileInfo(psd).fileName() << "Similarity:" << similarity << "%";

    QVERIFY2(similarity > 50.0,
             qPrintable(QString("Images differ significantly. Similarity: %1%").arg(similarity)));

    // Save images for manual inspection if they differ
    if (similarity < 99.0) {
        QString testName = QFileInfo(psd).dir().dirName() + "_" + QFileInfo(psd).baseName();
        QString outputDir = QDir::current().absolutePath();

        // Save both images for comparison
        toplevelImage.save(QString("%1/%2_toplevel.png").arg(outputDir, testName));
        viewRendering.save(QString("%1/%2_view.png").arg(outputDir, testName));

        // Create difference image
        QImage diff(toplevelImage.size(), QImage::Format_ARGB32);
        for (int y = 0; y < diff.height(); ++y) {
            for (int x = 0; x < diff.width(); ++x) {
                QRgb p1 = toplevelImage.pixel(x, y);
                QRgb p2 = viewRendering.pixel(x, y);

                int dr = qAbs(qRed(p1) - qRed(p2));
                int dg = qAbs(qGreen(p1) - qGreen(p2));
                int db = qAbs(qBlue(p1) - qBlue(p2));

                // Amplify differences for visibility
                dr = qMin(255, dr * 10);
                dg = qMin(255, dg * 10);
                db = qMin(255, db * 10);

                diff.setPixel(x, y, qRgb(dr, dg, db));
            }
        }
        diff.save(QString("%1/%2_diff.png").arg(outputDir, testName));

        qDebug() << "Saved comparison images for" << testName;
    }
}

QTEST_MAIN(tst_QPsdView)

#include "tst_qpsdview.moc"
