#pragma once

#include <color_harmonization/image/Image.hpp>
#include <color_harmonization/image/Pixel.hpp>
#include <color_harmonization/harmony/HarmonyTemplate.hpp>
#include <color_harmonization/harmony/TemplateMatcher.hpp>

namespace ColorHarmonization {

    enum class MappingMethod {
        NearestCenter,
        Linear,
        Gaussian
    };

    struct HarmonizationOptions {
        bool autoTemplate = true;
        bool autoRotation = true;
        bool graphCutEnabled = true;

        float manualRotation = 0.f;
        float gaussianSigmaScale = 0.5f;
        float graphCutColorSigma = 30.f;
        float graphCutLambda = 0.5f;

        harmony::HarmonyTemplateType manualTemplateType = harmony::HarmonyTemplateType::X;
        MappingMethod method = MappingMethod::Gaussian;
        harmony::HueHistogramOptions hueHistogramOptions;
        harmony::TemplateMatcherOptions templateMatcherOptions;
    };

    struct HarmonizationResult {
        Image<Rgb8> image;
        harmony::TemplateMatchResult matchResult;
    };

    [[nodiscard]]
    HarmonizationResult harmonize(const Image<Rgb8>& image, const HarmonizationOptions& options = {});

}
