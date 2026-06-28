#include <iostream>

#include <color_harmonization/image/Image.hpp>
#include <color_harmonization/image/Pixel.hpp>
#include <color_harmonization/image/ImageIO.hpp>
#include <color_harmonization/color/ColorConversion.hpp>

#include "color_harmonization/harmony/HueHistogram.hpp"


int main() {
    using ColorHarmonization::Image;
    using ColorHarmonization::Rgb8;
    using ColorHarmonization::Hsv32f;
    using ColorHarmonization::harmony::HueHistogram;
    using ColorHarmonization::harmony::HueHistogramOptions;

    Image<Hsv32f> testImage(3,1);
    testImage.at(0, 0) = Hsv32f{0.0F, 1.0F, 1.0F};    // red
    testImage.at(1, 0) = Hsv32f{120.0F, 1.0F, 1.0F};  // green
    testImage.at(2, 0) = Hsv32f{240.0F, 1.0F, 1.0F};  // blue

    HueHistogramOptions options;
    options.binCount = 360;
    options.normalize = true;
    options.saturationThreshold = 0.01f;

    const auto histogram = HueHistogram::fromImage(testImage, options);
    histogram.outputData(std::cout);

    try {
        auto image = ColorHarmonization::ImageIO::loadRgb("assets/input/lena.png");

        std::cout << "loaded image..." << std::endl;

        auto hsv = ColorHarmonization::color::rgbToHsv(image);
        std::cout << "convert rgb image to hsv..." << std::endl;

        const auto lenaHistogram = HueHistogram::fromImage(hsv, options);
        for (std::size_t bin = 0; bin < lenaHistogram.binCount(); ++bin) {
            const float value = lenaHistogram.valueAt(bin);

            if (value > 0.0F) {
                std::cout
                    << "bin=" << bin
                    << ", angle=" << lenaHistogram.angleAt(bin)
                    << ", value=" << value
                    << '\n';
            }
        }

        auto roundtrip = ColorHarmonization::color::hsvToRgb(hsv);
        std::cout << "convert hsv image to rgb..." << std::endl;

        ColorHarmonization::ImageIO::savePng("assets/output/lena_roundtrip.png",roundtrip);
        std::cout << "svade image..." << std::endl;


    }catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}
