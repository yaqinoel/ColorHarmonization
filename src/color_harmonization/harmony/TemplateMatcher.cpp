#include <iostream>
#include <limits>
#include <stdexcept>

#include <color_harmonization/harmony/TemplateMatcher.hpp>

namespace ColorHarmonization::harmony {

    TemplateMatcher::TemplateMatcher(TemplateMatcherOptions options) : options_(options) {
        if (options_.rotationStepDegrees <= 0.0f || options_.rotationStepDegrees >= 360.0f) {
            throw std::invalid_argument("Invalid rotation step degrees");
        }
    }

    TemplateMatchResult TemplateMatcher::findBestTemplate(const HueHistogram& histogram) const {
        const auto templates = buildDefaultTemplates();
        TemplateMatchResult bestResult;
        bestResult.cost = std::numeric_limits<float>::max();

        for (const HarmonyTemplate& harmonyTemplate : templates) {
            for (float rotation = 0.0f; rotation < 360.0f; rotation += options_.rotationStepDegrees) {
                const float cost = templateCost(histogram, harmonyTemplate, rotation);

                if (cost < bestResult.cost) {
                    bestResult.cost = cost;
                    bestResult.templateType = harmonyTemplate.type();
                    bestResult.rotationDegrees = rotation;
                }
            }
        }

        return bestResult;
    }   // findBestTemplate

}