#include "QtImageConverter.hpp"

#include <QColor>

namespace ColorHarmonization::gui {

    Image<Rgb8> fromQtImage(const QImage& image) {
        if (image.isNull()) {
            throw std::invalid_argument("Cannot convert a null QImage");
        }

        const QImage rgbImage = image.convertToFormat(QImage::Format_RGB888);
        Image<Rgb8> result(rgbImage.width(), rgbImage.height());

        result.forEachPixel([&rgbImage](std::size_t x, std::size_t y, Rgb8& pixel) {
            const QColor color = rgbImage.pixelColor(x, y);
            pixel.r = static_cast<std::uint8_t>(color.red());
            pixel.g = static_cast<std::uint8_t>(color.green());
            pixel.b = static_cast<std::uint8_t>(color.blue());
        });

        return result;
    }

    QImage toQtImage(const Image<Rgb8>& image) {
        QImage result(image.width(), image.height(), QImage::Format_RGB888);

        image.forEachPixel([&result](std::size_t x, std::size_t y, const Rgb8& pixel) {
            result.setPixelColor(x, y, QColor(pixel.r, pixel.g, pixel.b));
        });

        return result;
    }
}