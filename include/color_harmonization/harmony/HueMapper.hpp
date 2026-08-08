#pragma once

#include <vector>
#include <color_harmonization/harmony/HarmonyTemplate.hpp>

namespace ColorHarmonization::harmony {

    [[nodiscard]]
    float gaussian(float x, float sigma);

    [[nodiscard]]
    Sector findNearestSectorByDirection(float hue, const HarmonyTemplate& harmonyTemplate, float rotationDegrees, MoveDirection direction);

    [[nodiscard]]
    float mapHueToNearestCenter(float hue, const HarmonyTemplate& harmonyTemplate, float rotationDegrees);

    [[nodiscard]]
    float mapHueToNearestCenterByDirection(float hue, const HarmonyTemplate& harmonyTemplate, float rotationDegrees, MoveDirection direction);

    [[nodiscard]]
    float mapHueLinearly(float hue, const HarmonyTemplate& harmonyTemplate, float rotationDegrees);

    [[nodiscard]]
    float mapHueLinearlyByDirection(float hue, const HarmonyTemplate& harmonyTemplate, float rotationDegrees, MoveDirection direction);

    [[nodiscard]]
    float mapHueWithGaussian(float hue, const HarmonyTemplate& harmonyTemplate, float rotationDegrees, float sigmaScale);

    [[nodiscard]]
    float mapHueWithGaussianByDirection(float hue, const HarmonyTemplate& harmonyTemplate, float rotationDegrees, float sigmaScale, MoveDirection direction);

    [[nodiscard]]
    Image<Hsv32f> mapImageHuesToNearestCenter(
        const Image<Hsv32f>& image,
        const HarmonyTemplate& harmonyTemplate,
        float rotationDegrees
    );

    [[nodiscard]]
    Image<Hsv32f> mapImageHuesToNearestCenter(
        const Image<Hsv32f>& image,
        const HarmonyTemplate& harmonyTemplate,
        float rotationDegrees,
        const std::vector<MoveDirection>& directionsLabelMap
    );

    [[nodiscard]]
    Image<Hsv32f> mapImageHuesLinearly(
        const Image<Hsv32f>& image,
        const HarmonyTemplate& harmonyTemplate,
        float rotationDegrees
    );

    [[nodiscard]]
    Image<Hsv32f> mapImageHuesLinearly(
        const Image<Hsv32f>& image,
        const HarmonyTemplate& harmonyTemplate,
        float rotationDegrees,
        const std::vector<MoveDirection>& directionsLabelMap
    );

    [[nodiscard]]
    Image<Hsv32f> mapImageHuesWithGaussian(
        const Image<Hsv32f>& image,
        const HarmonyTemplate& harmonyTemplate,
        float rotationDegrees,
        float sigmaScale
    );

    [[nodiscard]]
    Image<Hsv32f> mapImageHuesWithGaussian(
        const Image<Hsv32f>& image,
        const HarmonyTemplate& harmonyTemplate,
        float rotationDegrees,
        float sigmaScale,
        const std::vector<MoveDirection>& directionsLabelMap
    );

    [[nodiscard]]
    std::vector<MoveDirection> calculateLabelMapByGraphCut(
            const Image<Hsv32f>& image,
            const HarmonyTemplate& harmonyTemplate,
            float rotationDegrees,
            float lambda,
            float colorSigma
    );


}
