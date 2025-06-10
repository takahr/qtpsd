#include "qpsdguiglobal.h"

QT_BEGIN_NAMESPACE

namespace QtPsdGui {
QImage imageDataToImage(const QPsdAbstractImage &imageData, const QPsdFileHeader &fileHeader)
{
    QImage image;
    const auto w = imageData.width();
    const auto h = imageData.height();
    const auto depth = imageData.depth();
    const auto data = imageData.toImage(fileHeader.colorMode());

    switch (fileHeader.colorMode()) {
    case QPsdFileHeader::Bitmap:
        // Bitmap mode is 1-bit per pixel, but toImage() converts it to 8-bit grayscale
        if (depth == 1) {
            image = QImage(reinterpret_cast<const uchar *>(data.constData()),
                           w, h, w, QImage::Format_Grayscale8);
        }
        break;
        
    case QPsdFileHeader::Grayscale:
        if (depth == 8) {
            image = QImage(reinterpret_cast<const uchar *>(data.constData()),
                           w, h, depth / 8 * w, QImage::Format_Grayscale8);
        } else if (depth == 16) {
            image = QImage(reinterpret_cast<const uchar *>(data.constData()),
                           w, h, depth / 8 * w, QImage::Format_Grayscale16);
        } else if (depth == 32) {
            // Convert 32-bit float grayscale to 16-bit
            image = QImage(w, h, QImage::Format_Grayscale16);
            const auto src = reinterpret_cast<const float *>(data.constData());
            auto dst = reinterpret_cast<quint16 *>(image.bits());
            const auto pixelCount = w * h;
            for (int i = 0; i < pixelCount; ++i) {
                // Convert float (0.0-1.0) to 16-bit (0-65535)
                float value = src[i];
                value = qBound(0.0f, value, 1.0f);
                dst[i] = static_cast<quint16>(value * 65535.0f);
            }
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
        } else if (depth == 16) {
            if (imageData.hasAlpha()) {
                image = QImage(reinterpret_cast<const uchar *>(data.constData()),
                               w, h, 8 * w, QImage::Format_RGBA64);
            } else {
                image = QImage(reinterpret_cast<const uchar *>(data.constData()),
                               w, h, 8 * w, QImage::Format_RGBX64);
            }
        } else if (depth == 32) {
            // Convert 32-bit float RGB to 16-bit for display using RGBA64 format
            if (imageData.hasAlpha()) {
                image = QImage(w, h, QImage::Format_RGBA64);
                const auto src = reinterpret_cast<const float *>(data.constData());
                auto dst = reinterpret_cast<quint16 *>(image.bits());
                for (int y = 0; y < h; ++y) {
                    for (int x = 0; x < w; ++x) {
                        const int srcIdx = (y * w + x) * 4;
                        const int dstIdx = (y * w + x) * 4;
                        // Convert float (0.0-1.0) to 16-bit (0-65535)
                        // Note: PSD stores in BGR order, but we need RGB
                        dst[dstIdx + 2] = static_cast<quint16>(qBound(0.0f, src[srcIdx], 1.0f) * 65535.0f);     // B -> B
                        dst[dstIdx + 1] = static_cast<quint16>(qBound(0.0f, src[srcIdx + 1], 1.0f) * 65535.0f); // G -> G
                        dst[dstIdx] = static_cast<quint16>(qBound(0.0f, src[srcIdx + 2], 1.0f) * 65535.0f);     // R -> R
                        dst[dstIdx + 3] = static_cast<quint16>(qBound(0.0f, src[srcIdx + 3], 1.0f) * 65535.0f); // A -> A
                    }
                }
            } else {
                image = QImage(w, h, QImage::Format_RGBX64);
                const auto src = reinterpret_cast<const float *>(data.constData());
                auto dst = reinterpret_cast<quint16 *>(image.bits());
                for (int y = 0; y < h; ++y) {
                    for (int x = 0; x < w; ++x) {
                        const int srcIdx = (y * w + x) * 3;
                        const int dstIdx = (y * w + x) * 4;
                        // Convert float (0.0-1.0) to 16-bit (0-65535)
                        // Note: PSD stores in BGR order, but we need RGB
                        dst[dstIdx + 2] = static_cast<quint16>(qBound(0.0f, src[srcIdx], 1.0f) * 65535.0f);     // B -> B
                        dst[dstIdx + 1] = static_cast<quint16>(qBound(0.0f, src[srcIdx + 1], 1.0f) * 65535.0f); // G -> G
                        dst[dstIdx] = static_cast<quint16>(qBound(0.0f, src[srcIdx + 2], 1.0f) * 65535.0f);     // R -> R
                        dst[dstIdx + 3] = 65535; // Alpha = 1.0
                    }
                }
            }
        }
        break;

    case QPsdFileHeader::CMYK:
        if (depth == 8) {
            // CMYK is converted to RGB by toImage(), so we treat it as RGB
            image = QImage(reinterpret_cast<const uchar *>(data.constData()),
                           w, h, 3 * w, QImage::Format_RGB888);
        }
        break;

    default:
        qFatal() << fileHeader.colorMode() << "not supported";
    }
    return image;
}
}

QT_END_NAMESPACE
