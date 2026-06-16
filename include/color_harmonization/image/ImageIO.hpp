#pragma once

#include <filesystem>

#include <color_harmonization/image/Image.hpp>
#include <color_harmonization/image/Pixel.hpp>

namespace ColorHarmonization::ImageIO {
    [[nodiscard]]
    Image<Rgb8> loadRgb(const std::filesystem::path& path);

    void savePng(const std::filesystem::path& path, const Image<Rgb8>& image);
}