#include <iostream>

#include <color_harmonization/image/Image.hpp>
#include <color_harmonization/image/Pixel.hpp>
#include <color_harmonization/image/ImageIO.hpp>


int main() {
    using ColorHarmonization::Image;
    using ColorHarmonization::Rgb8;

    try {
        auto image = ColorHarmonization::ImageIO::loadRgb("assets/input/lena.png");

        std::cout << "loaded image..." << std::endl;

        ColorHarmonization::ImageIO::savePng("assets/output/lena_copy.png", image);

        std::cout << "saved image..." << std::endl;

    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}