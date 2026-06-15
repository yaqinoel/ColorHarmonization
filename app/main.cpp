#include <iostream>

#include <color_harmonization/image/Image.hpp>
#include "color_harmonization/image/Pixel.hpp"


int main() {
    using ColorHarmonization::Image;
    using ColorHarmonization::Rgb8;

    Image<Rgb8> image(3, 2);

    image.at(1, 0) = Rgb8{255, 128, 0};

    const Rgb8& pixel = image.at(1, 0);

    std::cout
        << "Width: " << image.width()
        << ", Height: " << image.height()
        << '\n';

    std::cout
        << "Pixel: "
        << static_cast<int>(pixel.r) << ", "
        << static_cast<int>(pixel.g) << ", "
        << static_cast<int>(pixel.b) << '\n';
}