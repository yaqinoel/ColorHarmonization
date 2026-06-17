#pragma once

#include <color_harmonization/image/Image.hpp>
#include <color_harmonization/image/Pixel.hpp>

namespace ColorHarmonization::color {

    [[nodiscard]]
    Hsv32f rgbToHsv(const Rgb8& rgb);

    [[nodiscard]]
    Rgb8 hsvToRgb(const Hsv32f& hsv);

    [[nodiscard]]
    Image<Hsv32f> rgbToHsv(const Image<Rgb8>& imageRgb);

    [[nodiscard]]
    Image<Rgb8> HsvToRgb(const Image<Hsv32f>& imageHsv);


}