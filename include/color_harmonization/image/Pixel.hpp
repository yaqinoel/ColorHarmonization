#pragma once

#include <cstdint>

namespace ColorHarmonization {
    struct Rgb8 {
        std::uint8_t r = 0;
        std::uint8_t g = 0;
        std::uint8_t b = 0;
    };

    struct Hsv32f {
        float h = 0.0f; // [0-360)
        float s = 0.0f; // [0-1]
        float v = 0.0f; // [0-1]
    };
} // namespace ColorHarmonization