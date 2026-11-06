#include <cmath>
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
            if (!std::isfinite(options.graphCutLambda) || options.graphCutLambda < 0.0f ||
                !std::isfinite(options.graphCutColorSigma) || options.graphCutColorSigma <= 0.0f) {
                throw std::invalid_argument("Graph Cut lambda must be non-negative and color sigma must be positive");
            }
        }

        if (options.autoTemplate && !options.autoRotation) {
            throw std::invalid_argument("Automatic template selection requires automatic rotation");
        }

        Image<Hsv32f> hsvImage = color::rgbToHsv(image);
        harmony::TemplateMatcher templateMatcher = harmony::TemplateMatcher(options.templateMatcherOptions);
        const harmony::HueHistogram histogram = harmony::HueHistogram::fromImage(hsvImage, options.hueHistogramOptions);

        harmony::TemplateMatchResult matchResult;

        if (options.autoTemplate && options.autoRotation) {
            matchResult = templateMatcher.findBestTemplate(histogram);
        } else if (!options.autoTemplate && options.autoRotation) {
            matchResult = templateMatcher.findBestRotation(histogram, options.manualTemplateType);
        } else if (!options.autoTemplate && !options.autoRotation) {
            matchResult.templateType = options.manualTemplateType;
            matchResult.rotationDegrees = harmony::normalizeHue(options.manualRotation);
        }

        Image<Hsv32f> mappedHsvImage = hsvImage;

        harmony::HarmonyTemplate selectedTemplate = harmony::getTemplateByType(matchResult.templateType);

        if (!options.graphCutEnabled) {
            switch (options.method) {
                case MappingMethod::NearestCenter:
                    mappedHsvImage = harmony::mapImageHuesToNearestCenter(hsvImage, selectedTemplate, matchResult.rotationDegrees);
                    break;
                case MappingMethod::Linear:
                    mappedHsvImage = harmony::mapImageHuesLinearly(hsvImage, selectedTemplate, matchResult.rotationDegrees);
                    break;
                case MappingMethod::Gaussian:
                    mappedHsvImage = harmony::mapImageHuesWithGaussian(hsvImage, selectedTemplate, matchResult.rotationDegrees, options.gaussianSigmaScale);
                    break;
            }
        } else {
            std::vector<harmony::MoveDirection> directionsLabelMap = harmony::calculateLabelMapByGraphCut(hsvImage, selectedTemplate, matchResult.rotationDegrees, options.graphCutLambda, options.graphCutColorSigma);
            switch (options.method) {
                case MappingMethod::NearestCenter:
                    mappedHsvImage = harmony::mapImageHuesToNearestCenter(hsvImage, selectedTemplate, matchResult.rotationDegrees, directionsLabelMap);
                    break;
                case MappingMethod::Linear:
                    mappedHsvImage = harmony::mapImageHuesLinearly(hsvImage, selectedTemplate, matchResult.rotationDegrees, directionsLabelMap);
                    break;
                case MappingMethod::Gaussian:
                    mappedHsvImage = harmony::mapImageHuesWithGaussian(hsvImage, selectedTemplate, matchResult.rotationDegrees, options.gaussianSigmaScale, directionsLabelMap);
                    break;
            }
        }

        Image<Rgb8> mappedRgbImage = color::hsvToRgb(mappedHsvImage);

        return HarmonizationResult {
            mappedRgbImage,
            matchResult
        };
    }
}
