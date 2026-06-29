#pragma once
#include <vector>
#include <string_view>

#include <color_harmonization/harmony/HueHistogram.hpp>

namespace ColorHarmonization::harmony {
    enum class HarmonyTemplateType {
        i,
        V,
        L,
        L_MIRROR,
        I,
        T,
        Y,
        X
    };

    struct Sector {
        float center = 0.0f;    // degrees
        float width = 0.0f;     // degrees
    };

    class HarmonyTemplate {
    public:
        HarmonyTemplate(HarmonyTemplateType type, std::vector<Sector> sectors);

        [[nodiscard]]
        HarmonyTemplateType type() const noexcept;

        [[nodiscard]]
        const std::vector<Sector>& sectors() const noexcept;

    private:
        HarmonyTemplateType type_;
        std::vector<Sector> sectors_;
    };

    [[nodiscard]]
    std::string_view toString(HarmonyTemplateType type);

    [[nodiscard]]
    std::vector<HarmonyTemplate> buildDefaultTemplates();

    [[nodiscard]]
    float normalizeHue(float angle);

    [[nodiscard]]
    float circularDistance(float a, float b);

    [[nodiscard]]
    float distanceToTemplate(float hue, const HarmonyTemplate& harmonyTemplate, float rotationDegrees);

    [[nodiscard]]
    float templateCost(const HueHistogram& histogram,const HarmonyTemplate& harmonyTemplate, float rotationDegrees);

}   // namespace ColorHarmonization::harmony