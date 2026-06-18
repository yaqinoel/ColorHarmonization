#include <iostream>

#include <color_harmonization/image/Image.hpp>
#include <color_harmonization/image/Pixel.hpp>
#include <color_harmonization/image/ImageIO.hpp>
#include <color_harmonization/color/ColorConversion.hpp>


int main() {
    using ColorHarmonization::Image;
    using ColorHarmonization::Rgb8;

    try {
        auto image = ColorHarmonization::ImageIO::loadRgb("assets/input/lena.png");

        std::cout << "loaded image..." << std::endl;

        auto hsv = ColorHarmonization::color::rgbToHsv(image);
        std::cout << "convert rgb image to hsv..." << std::endl;

        auto roundtrip = ColorHarmonization::color::hsvToRgb(hsv);
        std::cout << "convert hsv image to rgb..." << std::endl;

        ColorHarmonization::ImageIO::savePng("assets/output/lena_roundtrip.png",roundtrip);
        std::cout << "svade image..." << std::endl;

    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}
