#include <algorithm>

#include <color_harmonization/color/ColorConversion.hpp>

namespace ColorHarmonization::color {
    [[nodiscard]]
    Hsv32f rgbToHsv(const Rgb8& rgb) {
        float r = rgb.r / 255.0f;
        float g = rgb.g / 255.0f;
        float b = rgb.b / 255.0f;

        float maxValue = std::max({r,g,b});
        float minValue = std::min({r,g,b});
        float delta = maxValue - minValue;

        Hsv32f hsv;
        hsv.v = maxValue;

        if (delta == 0.0f) {
            hsv.h = 0.0f;
            hsv.s = 0.0f;
            return hsv;
        }

        hsv.s = delta / maxValue;

        if (maxValue == r) {
            hsv.h = 60.0f * ((g - b) / delta);
        } else if (maxValue == g) {
            hsv.h = 60.0f * ((b - r) / delta + 2.0f);
        } else {
            hsv.h = 60.0f * ((r - g) / delta + 4.0f);
        }

        if (hsv.h < 0.0f) {
            hsv.h += 360.0f;
        }

        return hsv;
    }

    [[nodiscard]]
    Rgb8 hsvToRgb(const Hsv32f& hsv) {


    }

    [[nodiscard]]
    Image<Hsv32f> rgbToHsv(const Image<Rgb8>& imageRgb) {

    }

    [[nodiscard]]
    Image<Rgb8> HsvToRgb(const Image<Hsv32f>& imageHsv) {

    }
}