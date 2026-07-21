#include <color_harmonization/visualisation/HueHistogramRender.hpp>

#include "color_harmonization/color/ColorConversion.hpp"
#include "color_harmonization/harmony/HarmonyTemplate.hpp"

namespace ColorHarmonization::visualisation {

    Image<Rgb8> renderHueHistogramWheel(const harmony::HueHistogram& histogram,std::size_t width) {
        Image<Hsv32f> resultHsv(width, width);
        if (histogram.binCount() <= 0) {
            return color::hsvToRgb(resultHsv);
        }

        const std::size_t centerX = width / 2;
        const std::size_t centerY = width / 2;
        const std::size_t radiusOut = 5 * width / 12;
        const std::size_t radiusIn = 4 * width / 12;
        const std::size_t borderWidth = 6;
        const std::size_t centerPointRadius = 20;
        const std::size_t binCount = histogram.binCount();

        // draw background
        resultHsv.forEachPixel([](std::size_t, std::size_t, Hsv32f& pixel) {
            pixel.h = 0.0f;
            pixel.s = 0.0f;
            pixel.v = 1.0f;
        });

        resultHsv.forEachPixel([&](std::size_t x, std::size_t y, Hsv32f& pixel) {
            const float dx = static_cast<float>(x) - static_cast<float>(centerX);
            const float dy = static_cast<float>(y) - static_cast<float>(centerY);

            float distanceToCenter = std::sqrt(dx * dx + dy * dy);
            float angle = std::atan2(dy, dx) * 180.f / M_PI;

            // draw bars
            std::size_t bin = histogram.binForHue(angle);
            const float binHeight = histogram.valueAt(bin);
            if (distanceToCenter <= (radiusIn - borderWidth) && distanceToCenter >= (radiusIn - radiusIn * binHeight) ) {
                pixel.h = angle;
                pixel.s = 0.8f;
                pixel.v = 1.0f;
            }

            // draw colorWheel
            if (distanceToCenter <= radiusOut && distanceToCenter >= radiusIn) {
                angle = harmony::normalizeHue(angle);
                pixel.h = angle;
                pixel.s = 1.0f;
                pixel.v = 1.0f;
            }

            // draw gray border line
            if ((distanceToCenter <= (radiusOut + borderWidth) && distanceToCenter >= radiusOut) || (distanceToCenter <= radiusIn && distanceToCenter >= (radiusIn - borderWidth))) {
                pixel.h = 0.0f;
                pixel.s = 0.0f;
                pixel.v = 0.5f;
            }

            // draw center point
            if (distanceToCenter <= centerPointRadius) {
                pixel.h = 0.0f;
                pixel.s = 0.0f;
                pixel.v = 0.0f;
            }
        });

        return color::hsvToRgb(resultHsv);
    }

}
