#include <stdexcept>
#include <vector>

#include <color_harmonization/Harmonizer.hpp>
#include <color_harmonization/color/ColorConversion.hpp>
#include <color_harmonization/harmony/HueHistogram.hpp>
#include <color_harmonization/harmony/HueMapper.hpp>

namespace ColorHarmonization {
    HarmonizationResult harmonize(const Image<Rgb8>& image, const HarmonizationOptions& options) {
        if (image.empty()) {
            throw std::invalid_argument("Input image must not be empty");
        }

        if (options.method == MappingMethod::Gaussian && options.gaussianSigmaScale <= 0.0f) {
            throw std::invalid_argument("Gaussian sigma scale must be positive");
        }

        if (options.graphCutEnabled) {
            if (options.graphCutLambda <= 0.0f || options.graphCutColorSigma <= 0.0f) {
                throw std::invalid_argument("Graph Cut parameters must be positive");
            }
        }

        Image<Hsv32f> hsvImage = color::rgbToHsv(image);

        const harmony::HueHistogram histogram = harmony::HueHistogram::fromImage(hsvImage);





    }
}