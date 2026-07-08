#pragma once
#include <color_harmonization/harmony/HarmonyTemplate.hpp>

namespace ColorHarmonization::harmony {

    [[nodiscard]]
    float gaussian(float x, float sigma);

    [[nodiscard]]
    float mapHueToNearestCenter(float hue, const HarmonyTemplate& harmonyTemplate, float rotationDegrees);

    [[nodiscard]]
    float mapHueLinearly(float hue, const HarmonyTemplate& harmonyTemplate, float rotationDegrees);

    [[nodiscard]]
    float mapHueWithGaussian(float hue, const HarmonyTemplate& harmonyTemplate, float rotationDegrees, float sigma);

    [[nodiscard]]
    Image<Hsv32f> mapImageHuesToNearestCenter(
        const Image<Hsv32f>& image,
        const HarmonyTemplate& harmonyTemplate,
        float rotationDegrees
    );

    [[nodiscard]]
    Image<Hsv32f> mapImageHuesLinearly(
        const Image<Hsv32f>& image,
        const HarmonyTemplate& harmonyTemplate,
        float rotationDegrees
    );

    [[nodiscard]]
    Image<Hsv32f> mapImageHuesWithGaussian(
        const Image<Hsv32f>& image,
        const HarmonyTemplate& harmonyTemplate,
        float rotationDegrees,
        float sigmaScale
    );

}
