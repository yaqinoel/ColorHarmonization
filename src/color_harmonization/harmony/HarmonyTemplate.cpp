#include <utility>
#include <algorithm>
#include <cmath>
#include <limits>

#include <color_harmonization/harmony/HarmonyTemplate.hpp>

namespace ColorHarmonization::harmony {
    HarmonyTemplate::HarmonyTemplate(HarmonyTemplateType type, std::vector<Sector> sectors)
        : type_(type), sectors_(std::move(sectors)) {}

    HarmonyTemplateType HarmonyTemplate::type() const noexcept {
        return type_;
    }

    const std::vector<Sector>& HarmonyTemplate::sectors() const noexcept {
        return sectors_;
    }

    std::string_view toString(HarmonyTemplateType type) {
        switch (type) {
            case HarmonyTemplateType::i :
                return "i";
            case HarmonyTemplateType::V :
                return "V";
            case HarmonyTemplateType::L :
                return "L";
            case HarmonyTemplateType::L_MIRROR :
                return "L_MIRROR";
            case HarmonyTemplateType::I :
                return "I";
            case HarmonyTemplateType::T :
                return "T";
            case HarmonyTemplateType::Y :
                return "Y";
            case HarmonyTemplateType::X :
                return "X";
        }

        return "Unknown";
    }

    std::vector<HarmonyTemplate> buildDefaultTemplates() {
        return {
            HarmonyTemplate{
                HarmonyTemplateType::i,
                {
                    Sector{0.0f, 18.0f}
                }
            },
            HarmonyTemplate{
                HarmonyTemplateType::V,
                {
                    Sector{0.0f, 93.6f}
                }
            },
            HarmonyTemplate{
                HarmonyTemplateType::T,
                {
                    Sector{0.0f, 180.0f}
                }
            },
            HarmonyTemplate{
                HarmonyTemplateType::L,
                {
                    Sector{0.0f, 79.2f},
                    Sector{90.0f, 18.0f}
                }
            },
            HarmonyTemplate{
                HarmonyTemplateType::L_MIRROR,
                {
                    Sector{0.0f, 79.2f},
                    Sector{270.0f, 18.0f}
                }
            },
            HarmonyTemplate{
                HarmonyTemplateType::I,
                {
                    Sector{0.0f, 18.0f},
                    Sector{180.0f, 18.0f}
                }
            },
            HarmonyTemplate{
                HarmonyTemplateType::Y,
                {
                    Sector{0.0f, 93.6f},
                    Sector{180.0f, 18.0f}
                }
            },
            HarmonyTemplate{
                HarmonyTemplateType::X,
                {
                    Sector{0.0f, 93.6f},
                    Sector{180.0f, 93.6f}
                }
            }
        };
    }   // build default template

    HarmonyTemplate getTemplateByType(HarmonyTemplateType type) {
        switch (type) {
            case HarmonyTemplateType::i :
                return HarmonyTemplate{
                    HarmonyTemplateType::i,
                    {
                        Sector{0.0f, 18.0f}
                    }
                };
            case HarmonyTemplateType::V :
                return HarmonyTemplate{
                    HarmonyTemplateType::V,
                    {
                        Sector{0.0f, 93.6f}
                    }
                };
            case HarmonyTemplateType::L :
                return HarmonyTemplate{
                    HarmonyTemplateType::L,
                    {
                        Sector{0.0f, 79.2f},
                        Sector{90.0f, 18.0f}
                    }
                };
            case HarmonyTemplateType::L_MIRROR :
                return HarmonyTemplate{
                    HarmonyTemplateType::L_MIRROR,
                    {
                        Sector{0.0f, 79.2f},
                        Sector{270.0f, 18.0f}
                    }
                };
            case HarmonyTemplateType::I :
                return HarmonyTemplate{
                    HarmonyTemplateType::I,
                    {
                        Sector{0.0f, 18.0f},
                        Sector{180.0f, 18.0f}
                    }
                };
            case HarmonyTemplateType::T :
                return HarmonyTemplate{
                    HarmonyTemplateType::T,
                    {
                        Sector{0.0f, 180.0f}
                    }
                };
            case HarmonyTemplateType::Y :
                return HarmonyTemplate{
                    HarmonyTemplateType::Y,
                    {
                        Sector{0.0f, 93.6f},
                        Sector{180.0f, 18.0f}
                    }
                };
            case HarmonyTemplateType::X :
                return HarmonyTemplate{
                    HarmonyTemplateType::X,
                    {
                        Sector{0.0f, 93.6f},
                        Sector{180.0f, 93.6f}
                    }
                };
        }
        throw std::invalid_argument("Unknown HarmonyTemplateType");
    }   // getTemplateByType

    float normalizeHue(float angle) {
        angle = std::fmod(angle, 360.f);
        if (angle < 0) {
            angle += 360.f;
        }
        return angle;
    }

    float circularDistance(float a, float b) {
        const float normalizedA = normalizeHue(a);
        const float normalizedB = normalizeHue(b);

        const float difference = std::fabs(normalizedA - normalizedB);

        return std::min(difference, 360.0f - difference);
    }

    float signedCircularDistance(float from, float to) {
        const float normalizedFrom = normalizeHue(from);
        const float normalizedTo = normalizeHue(to);
        float diff =  normalizedTo - normalizedFrom;

        if (diff > 180.f) {
            diff -= 360.f;
        }

        if (diff < -180.f) {
            diff += 360.f;
        }

        return diff;
    }

    float directedCircularDistance(float from, float to, MoveDirection direction) {
        if (direction == MoveDirection::Positive) {
            return normalizeHue(to - from);
        }
        return normalizeHue(from - to);
    }

    float distanceToTemplate(float hue, const HarmonyTemplate& harmonyTemplate, float rotationDegrees) {
        if (harmonyTemplate.sectors().empty()) {
            throw std::invalid_argument("Harmony template must contain sectors");
        }

        const auto& sectors = harmonyTemplate.sectors();
        const float targetHue = normalizeHue(hue);

        float minDistance = std::numeric_limits<float>::max();
        float currentDistance = 0.0f;

        for (const auto& sector : sectors) {
            const float shiftedCenter = sector.center + rotationDegrees;
            const float halfWidth = sector.width / 2.0f;

            float distanceToCenter = circularDistance(targetHue, shiftedCenter);
            if (distanceToCenter <= halfWidth) {
                return 0.0F;
            }

            currentDistance = distanceToCenter - halfWidth;

            if (currentDistance < minDistance) {
                minDistance = currentDistance;
            }
        }

        return minDistance;
    }   // distanceToTemplate

    float templateCost(const HueHistogram& histogram,const HarmonyTemplate& harmonyTemplate, float rotationDegrees) {
        float cost = 0.0f;
        for (std::size_t bin = 0; bin < histogram.binCount(); ++bin) {
            const float hue = histogram.angleAt(bin);
            const float weight = histogram.valueAt(bin);
            const float distance = distanceToTemplate(hue, harmonyTemplate, rotationDegrees);

            cost += weight * distance;
        }

        return cost;
    }

    float distanceToSector(const float hue, const Sector sector, float rotationDegrees) {
        const float shiftedCenter = sector.center + rotationDegrees;
        const float halfWidth = sector.width / 2.0f;

        float distanceToCenter = circularDistance(hue, shiftedCenter);

        return std::max(0.0f, distanceToCenter - halfWidth);
    }

    float directedDistanceToSector(const float hue, const Sector sector, float rotationDegrees, MoveDirection direction) {
        const float center = normalizeHue(sector.center + rotationDegrees);
        const float halfWidth = sector.width / 2.0f;

        if (circularDistance(hue, center) <= halfWidth) {
            return 0.0f;
        }

        float distanceToCenter = directedCircularDistance(hue, center, direction);

        return std::max(0.0f, distanceToCenter - halfWidth);
    }

}