#include <algorithm>
#include <cmath>
#include <cstdint>

#include <color_harmonization/color/ColorConversion.hpp>

namespace {
    std::uint8_t toByte(float value) {
        value = std::clamp(value, 0.0F, 1.0F);
        return static_cast<std::uint8_t>(std::round(value * 255.0F));
    }
}

namespace ColorHarmonization::color {

    [[nodiscard]]
    Hsv32f rgbToHsv(const Rgb8& rgb) {
        const float r = rgb.r / 255.0f;
        const float g = rgb.g / 255.0f;
        const float b = rgb.b / 255.0f;

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
        float h = hsv.h;
        const float s = std::clamp(hsv.s, 0.0f, 1.0f);
        const float v = std::clamp(hsv.v, 0.0f, 1.0f);

        if (h < 0.0f) {
            h = std::fmod(h, 360.0f) + 360.0f;
        }else {
            h = std::fmod(h, 360.0f);
        }

        const float c = s * v;
        const float x = c * (1.0f - std::fabs(std::fmod(h / 60.0f, 2.0f) - 1.0f));
        const float m = v - c;

        float r1 = 0.0f;
        float g1 = 0.0f;
        float b1 = 0.0f;

        if (h < 60.0F) {
            r1 = c;
            g1 = x;
        } else if (h < 120.0F) {
            r1 = x;
            g1 = c;
        } else if (h < 180.0F) {
            g1 = c;
            b1 = x;
        } else if (h < 240.0F) {
            g1 = x;
            b1 = c;
        } else if (h < 300.0F) {
            r1 = x;
            b1 = c;
        } else {
            r1 = c;
            b1 = x;
        }

        return Rgb8{
            toByte(r1 + m),
            toByte(g1 + m),
            toByte(b1 + m)
        };
    }

    [[nodiscard]]
    Image<Hsv32f> rgbToHsv(const Image<Rgb8>& imageRgb) {
        const size_t width = imageRgb.width();
        const size_t height = imageRgb.height();

        Image<Hsv32f> imageHsv(width, height);

        for (size_t y = 0; y < height; ++y) {
            for (size_t x = 0; x < width; ++x) {
                const Rgb8& pixelRgb = imageRgb.at(x, y);
                imageHsv.at(x,y) = rgbToHsv(pixelRgb);
            }
        }

        return imageHsv;
    }

    [[nodiscard]]
    Image<Rgb8> hsvToRgb(const Image<Hsv32f>& imageHsv) {
        const size_t width = imageHsv.width();
        const size_t height = imageHsv.height();

        Image<Rgb8> imageRgb(width, height);

        for (size_t y = 0; y < height; ++y) {
            for (size_t x = 0; x < width; ++x) {
                const Hsv32f& pixelHsv = imageHsv.at(x, y);
                imageRgb.at(x,y) = hsvToRgb(pixelHsv);
            }
        }

        return imageRgb;
    }
}