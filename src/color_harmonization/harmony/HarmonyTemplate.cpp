#include <utility>
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
    }

}