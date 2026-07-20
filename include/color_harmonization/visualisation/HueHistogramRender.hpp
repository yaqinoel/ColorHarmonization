#pragma once

#include <cstdio>
#include <color_harmonization/image/Image.hpp>
#include <color_harmonization/image/Pixel.hpp>
#include <color_harmonization/harmony/HueHistogram.hpp>

namespace ColorHarmonization::visualisation {

    [[nodiscard]]
    Image<Rgb8> renderHueHistogramWheel(harmony::HueHistogram& histogram,std::size_t width = 512);

}
