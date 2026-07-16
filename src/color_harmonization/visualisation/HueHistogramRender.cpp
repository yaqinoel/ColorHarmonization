#include <color_harmonization/visualisation/HueHistogramRender.hpp>

#include "color_harmonization/color/ColorConversion.hpp"
#include "color_harmonization/harmony/HarmonyTemplate.hpp"

namespace ColorHarmonization::visualisation {

    Image<Rgb8> renderHueHistogramWheel(const harmony::HueHistogram& histogram,std::size_t width) {
        Image<Hsv32f> resultHsv(width, width);
        const std::size_t centerX = width / 2;
        const std::size_t centerY = width / 2;
        const std::size_t radiusOut = 5 * width / 12;
        const std::size_t radiusIn = 4 * width / 12;

        // draw background
        resultHsv.forEachPixel([](std::size_t, std::size_t, Hsv32f& pixel) {
            pixel.h = 0.0f;
            pixel.s = 0.0f;
            pixel.v = 1.0f;
        });

        // draw colorWheel
        resultHsv.forEachPixel([&](std::size_t x, std::size_t y, Hsv32f& pixel) {
            const float dx = static_cast<float>(x) - static_cast<float>(centerX);
            const float dy = static_cast<float>(y) - static_cast<float>(centerY);

            float distanceToCenter = std::sqrt(dx * dx + dy * dy);
            if (distanceToCenter <= radiusOut && distanceToCenter >= radiusIn) {
                float angle = std::atan2(dy, dx) * 180.f / M_PI;
                angle = harmony::normalizeHue(angle);

                pixel.h = angle;
                pixel.s = 1.0f;
                pixel.v = 1.0f;
            }
        });

        return color::hsvToRgb(resultHsv);
    }

}
