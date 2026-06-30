#include <iostream>

#include <color_harmonization/image/Image.hpp>
#include <color_harmonization/image/Pixel.hpp>
#include <color_harmonization/image/ImageIO.hpp>
#include <color_harmonization/color/ColorConversion.hpp>

#include "color_harmonization/harmony/HarmonyTemplate.hpp"
#include "color_harmonization/harmony/HueHistogram.hpp"
#include "color_harmonization/harmony/TemplateMatcher.hpp"


int main() {
    using ColorHarmonization::Image;
    using ColorHarmonization::Rgb8;
    using ColorHarmonization::Hsv32f;
    using ColorHarmonization::harmony::HueHistogram;
    using ColorHarmonization::harmony::HueHistogramOptions;
    using ColorHarmonization::harmony::HarmonyTemplate;
    using ColorHarmonization::harmony::TemplateMatcher;
    using ColorHarmonization::harmony::TemplateMatcherOptions;
    using ColorHarmonization::harmony::TemplateMatchResult;

    HueHistogramOptions options;
    options.binCount = 360;
    options.normalize = true;
    options.saturationThreshold = 0.01f;

    TemplateMatcherOptions matcherOptions;
    matcherOptions.rotationStepDegrees = 1.0f;

    try {
        auto image = ColorHarmonization::ImageIO::loadRgb("assets/input/lena.png");

        std::cout << "loaded image..." << std::endl;

        auto hsv = ColorHarmonization::color::rgbToHsv(image);
        std::cout << "convert rgb image to hsv..." << std::endl;

        const auto histogram = HueHistogram::fromImage(hsv, options);
        std::cout << "calculate hue histogram..." << std::endl;

        auto templateMatcher = TemplateMatcher(matcherOptions);
        TemplateMatchResult templateMatchResult = templateMatcher.findBestTemplate(histogram);

        std::cout<< "Best hramony template:"
                 << "\ntype:" << toString(templateMatchResult.templateType)
                 << "\nrotation:" << templateMatchResult.rotationDegrees
                 << "\ncost" << templateMatchResult.cost
                 << std::endl;

    }catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }





    return 0;
}
