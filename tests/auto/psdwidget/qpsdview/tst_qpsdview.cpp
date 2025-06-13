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
#include <QtCore/QTextStream>
#include <QtCore/QFile>
#include <QtCore/QDateTime>
#include <algorithm>

class tst_QPsdView : public QObject
{
    Q_OBJECT
private slots:
    void initTestCase();
    void compareRendering_data();
    void compareRendering();
    void cleanupTestCase();

private:
    void addPsdFiles();
    QImage renderPsdView(const QString &filePath);
    double compareImages(const QImage &img1, const QImage &img2);

    struct SimilarityResult {
        QString fileName;
        double similarity;
        bool passed;
        QString imagePaths[3]; // 0: toplevel, 1: view, 2: diff
    };
    QList<SimilarityResult> m_similarityResults;
    bool m_generateSummary;
    QString m_outputBaseDir;
};

void tst_QPsdView::initTestCase()
{
    // Check if summary generation is enabled via environment variable
    QString summaryPath = qEnvironmentVariable("QTPSD_SIMILARITY_SUMMARY_PATH");
    m_generateSummary = !summaryPath.isEmpty();

    if (m_generateSummary) {
        qDebug() << "Similarity summary generation enabled";

        // If path is relative, make it relative to the project root
        QDir projectRoot = QDir::current();
        // Go up until we find CMakeLists.txt (project root)
        while (!projectRoot.exists("CMakeLists.txt") && projectRoot.cdUp()) {
            // Keep going up
        }

        if (QDir::isRelativePath(summaryPath)) {
            m_outputBaseDir = projectRoot.absoluteFilePath(summaryPath);
        } else {
            m_outputBaseDir = summaryPath;
        }

        // Create output directory structure
        QDir dir;
        dir.mkpath(m_outputBaseDir);
        dir.mkpath(m_outputBaseDir + "/images");

        qDebug() << "Output directory:" << m_outputBaseDir;
    }
}

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
        if (m_generateSummary) {
            // Record as skipped with 0% similarity
            SimilarityResult result;
            result.fileName = QDir(QFINDTESTDATA("../../psdcore/ag-psd/ag-psd/test/")).relativeFilePath(psd);
            result.similarity = 0.0;
            result.passed = false;
            result.imagePaths[0] = QString();
            result.imagePaths[1] = QString();
            result.imagePaths[2] = QString();
            m_similarityResults.append(result);
            qDebug() << "No image data in PSD:" << QFileInfo(psd).fileName();
            return;
        } else {
            QSKIP("No image data in PSD file");
        }
    }

    // Convert toplevel imageData to QImage
    QImage toplevelImage = QtPsdGui::imageDataToImage(imageData, header);
    if (toplevelImage.isNull()) {
        if (m_generateSummary) {
            SimilarityResult result;
            result.fileName = QDir(QFINDTESTDATA("../../psdcore/ag-psd/ag-psd/test/")).relativeFilePath(psd);
            result.similarity = 0.0;
            result.passed = false;
            result.imagePaths[0] = QString();
            result.imagePaths[1] = QString();
            result.imagePaths[2] = QString();
            m_similarityResults.append(result);
            qDebug() << "Failed to convert image data:" << QFileInfo(psd).fileName();
            return;
        } else {
            QFAIL("Failed to convert image data to QImage");
        }
    }

    // Render using QPsdView
    QImage viewRendering = renderPsdView(psd);
    if (viewRendering.isNull()) {
        if (m_generateSummary) {
            SimilarityResult result;
            result.fileName = QDir(QFINDTESTDATA("../../psdcore/ag-psd/ag-psd/test/")).relativeFilePath(psd);
            result.similarity = 0.0;
            result.passed = false;
            result.imagePaths[0] = QString();
            result.imagePaths[1] = QString();
            result.imagePaths[2] = QString();
            m_similarityResults.append(result);
            qDebug() << "Failed to render view:" << QFileInfo(psd).fileName();
            return;
        } else {
            QFAIL("Failed to render QPsdView");
        }
    }

    // Compare sizes
    if (viewRendering.size() != toplevelImage.size()) {
        if (m_generateSummary) {
            SimilarityResult result;
            result.fileName = QDir(QFINDTESTDATA("../../psdcore/ag-psd/ag-psd/test/")).relativeFilePath(psd);
            result.similarity = 0.0;
            result.passed = false;
            result.imagePaths[0] = QString();
            result.imagePaths[1] = QString();
            result.imagePaths[2] = QString();
            m_similarityResults.append(result);
            qDebug() << "Size mismatch:" << QFileInfo(psd).fileName()
                     << "toplevel:" << toplevelImage.size()
                     << "view:" << viewRendering.size();
            return;
        } else {
            QCOMPARE(viewRendering.size(), toplevelImage.size());
        }
    }

    // Compare images directly
    double similarity = compareImages(toplevelImage, viewRendering);

    // Log the similarity for information
    qDebug() << "File:" << QFileInfo(psd).fileName() << "Similarity:" << similarity << "%";

    // Store result if summary generation is enabled
    if (m_generateSummary) {
        SimilarityResult result;
        result.fileName = QDir(QFINDTESTDATA("../../psdcore/ag-psd/ag-psd/test/")).relativeFilePath(psd);
        result.similarity = similarity;
        result.passed = similarity > 50.0;
        // Initialize image paths
        result.imagePaths[0] = QString();
        result.imagePaths[1] = QString();
        result.imagePaths[2] = QString();
        m_similarityResults.append(result);

        // Create a safe filename from the PSD path
        QString relativePsdPath = QDir(QFINDTESTDATA("../../psdcore/ag-psd/ag-psd/test/")).relativeFilePath(psd);
        QString safeFileName = relativePsdPath;
        safeFileName.replace('/', '_');
        safeFileName.replace('\\', '_');
        safeFileName = safeFileName.left(safeFileName.lastIndexOf('.')); // Remove .psd extension

        QString imageDir = m_generateSummary ? (m_outputBaseDir + "/images") : QDir::current().absolutePath();

        // Save both images for comparison
        QString toplevelPath = QString("%1/%2_toplevel.png").arg(imageDir, safeFileName);
        QString viewPath = QString("%1/%2_view.png").arg(imageDir, safeFileName);
        QString diffPath = QString("%1/%2_diff.png").arg(imageDir, safeFileName);

        toplevelImage.save(toplevelPath);
        viewRendering.save(viewPath);

        // Create difference image with transparency for matching pixels
        QImage diff(toplevelImage.size(), QImage::Format_ARGB32);
        diff.fill(Qt::transparent); // Start with fully transparent

        for (int y = 0; y < diff.height(); ++y) {
            for (int x = 0; x < diff.width(); ++x) {
                QRgb p1 = toplevelImage.pixel(x, y);
                QRgb p2 = viewRendering.pixel(x, y);

                int dr = qAbs(qRed(p1) - qRed(p2));
                int dg = qAbs(qGreen(p1) - qGreen(p2));
                int db = qAbs(qBlue(p1) - qBlue(p2));
                int da = qAbs(qAlpha(p1) - qAlpha(p2));

                // If pixels match exactly, keep transparent
                if (dr == 0 && dg == 0 && db == 0 && da == 0) {
                    continue;
                }

                // Amplify differences for visibility
                dr = qMin(255, dr * 10);
                dg = qMin(255, dg * 10);
                db = qMin(255, db * 10);

                // Set opaque red for differences, with intensity based on difference magnitude
                int maxDiff = qMax(qMax(dr, dg), db);
                diff.setPixel(x, y, qRgba(255, 0, 0, maxDiff));
            }
        }
        diff.save(diffPath);

        // Store relative paths for the summary if enabled
        if (m_generateSummary && m_similarityResults.size() > 0) {
            // Find the last result (the one we just added)
            auto& lastResult = m_similarityResults.last();
            lastResult.imagePaths[0] = QString("images/%1_toplevel.png").arg(safeFileName);
            lastResult.imagePaths[1] = QString("images/%1_view.png").arg(safeFileName);
            lastResult.imagePaths[2] = QString("images/%1_diff.png").arg(safeFileName);
        }

        qDebug() << "Saved comparison images for" << relativePsdPath;
    } else {
        QVERIFY2(similarity > 50.0,
                 qPrintable(QString("Images differ significantly. Similarity: %1%").arg(similarity)));
    }
}

void tst_QPsdView::cleanupTestCase()
{
    if (!m_generateSummary || m_similarityResults.isEmpty()) {
        return;
    }

    // Sort results by similarity (lowest first)
    std::sort(m_similarityResults.begin(), m_similarityResults.end(),
              [](const SimilarityResult &a, const SimilarityResult &b) {
                  return a.similarity < b.similarity;
              });

    // Generate markdown summary
    QString summaryPath = m_outputBaseDir + "/similarity_report.md";
    QFile file(summaryPath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qWarning() << "Failed to create summary file:" << summaryPath;
        return;
    }

    QTextStream stream(&file);
    stream << "# QPsdView Similarity Test Results\n\n";
    stream << "Generated on: " << QDateTime::currentDateTime().toString(Qt::ISODate) << "\n\n";

    // Summary statistics
    int totalTests = m_similarityResults.size();
    int passedTests = 0;
    double totalSimilarity = 0;
    double minSimilarity = 100;
    double maxSimilarity = 0;

    for (const auto &result : m_similarityResults) {
        if (result.passed) passedTests++;
        totalSimilarity += result.similarity;
        minSimilarity = qMin(minSimilarity, result.similarity);
        maxSimilarity = qMax(maxSimilarity, result.similarity);
    }

    double avgSimilarity = totalSimilarity / totalTests;

    stream << "## Summary Statistics\n\n";
    stream << "| Metric | Value |\n";
    stream << "|--------|-------|\n";
    stream << "| Total Tests | " << totalTests << " |\n";
    stream << "| Passed Tests | " << passedTests << " ("
           << QString::number(100.0 * passedTests / totalTests, 'f', 1) << "%) |\n";
    stream << "| Failed Tests | " << (totalTests - passedTests) << " |\n";
    stream << "| Average Similarity | " << QString::number(avgSimilarity, 'f', 2) << "% |\n";
    stream << "| Minimum Similarity | " << QString::number(minSimilarity, 'f', 2) << "% |\n";
    stream << "| Maximum Similarity | " << QString::number(maxSimilarity, 'f', 2) << "% |\n\n";

    // All results in a single table
    stream << "## Test Results\n\n";
    stream << "| File | Similarity | Status | PSD | QtPsd | Difference |\n";
    stream << "|------|------------|--------|-----|-------|------------|\n";

    for (const auto &result : m_similarityResults) {
        QString status;
        QString statusEmoji;
        if (!result.passed) {
            status = "FAILED";
            statusEmoji = "❌";
        } else if (result.similarity >= 99.0) {
            status = "PERFECT";
            statusEmoji = "✅";
        } else if (result.similarity >= 90.0) {
            status = "GOOD";
            statusEmoji = "✅";
        } else {
            status = "LOW";
            statusEmoji = "⚠️";
        }

        // Create GitHub link to the PSD file
        QString encodedFileName = result.fileName;
        encodedFileName.replace(" ", "%20");
        QString githubUrl = QString("https://github.com/Agamnentzar/ag-psd/tree/master/test/%1").arg(encodedFileName);
        
        stream << "| [" << result.fileName << "](" << githubUrl << ")"
               << " | " << QString::number(result.similarity, 'f', 2) << "% "
               << " | " << statusEmoji << " " << status;

        // Add image thumbnails with links
        if (!result.imagePaths[0].isEmpty()) {
            // URL-encode spaces in image paths
            QString encodedPath0 = result.imagePaths[0];
            QString encodedPath1 = result.imagePaths[1];
            QString encodedPath2 = result.imagePaths[2];
            encodedPath0.replace(" ", "%20");
            encodedPath1.replace(" ", "%20");
            encodedPath2.replace(" ", "%20");
            
            stream << " | [<img src=\"" << encodedPath0
                   << "\" width=\"100\">](" << encodedPath0 << ")";
            stream << " | [<img src=\"" << encodedPath1
                   << "\" width=\"100\">](" << encodedPath1 << ")";
            stream << " | [<img src=\"" << encodedPath2
                   << "\" width=\"100\">](" << encodedPath2 << ")";
        } else {
            stream << " | - | - | -";
        }

        stream << " |\n";
    }

    file.close();
    qDebug() << "Similarity summary written to:" << QFileInfo(file).absoluteFilePath();
}

QTEST_MAIN(tst_QPsdView)

#include "tst_qpsdview.moc"
