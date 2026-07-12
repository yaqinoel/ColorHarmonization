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

    enum class MoveDirection {
        Negative,
        Positive
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
    HarmonyTemplate getTemplateByType(HarmonyTemplateType type);

    [[nodiscard]]
    float normalizeHue(float angle);

    [[nodiscard]]
    float circularDistance(float a, float b);

    [[nodiscard]]
    float signedCircularDistance(float from, float to);

    [[nodiscard]]
    float directedCircularDistance(float from, float to, MoveDirection direction);

    [[nodiscard]]
    float signedDistanceToTemplate(float hue, const HarmonyTemplate& harmonyTemplate, float rotationDegrees);

    [[nodiscard]]
    float templateCost(const HueHistogram& histogram,const HarmonyTemplate& harmonyTemplate, float rotationDegrees);

    [[nodiscard]]
    float distanceToSector(const float hue, const Sector sector, float rotationDegrees);

}   // namespace ColorHarmonization::harmony