#pragma once

#include <color_harmonization/harmony/HarmonyTemplate.hpp>
#include <color_harmonization/harmony/HueHistogram.hpp>

namespace ColorHarmonization::harmony {
    struct TemplateMatchResult {
        HarmonyTemplateType templateType = HarmonyTemplateType::i;
        float rotationDegrees = 0.0f;
        float cost = 0.0f;
    };

    struct TemplateMatcherOptions {
        float rotationStepDegrees = 0.5f;
    };

    class TemplateMatcher {
    public:
        explicit TemplateMatcher(TemplateMatcherOptions options = {});

        [[nodiscard]]
        TemplateMatchResult findBestTemplate(const HueHistogram& histogram) const;

        [[nodiscard]]
        TemplateMatchResult findBestRotation(const HueHistogram& histogram, HarmonyTemplateType templateType) const;

    private:
        TemplateMatcherOptions options_;

    };
}