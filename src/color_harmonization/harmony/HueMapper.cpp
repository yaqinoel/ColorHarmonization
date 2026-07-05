#include <limits>
#include <stdexcept>
#include <algorithm>
#include <cmath>
#include <color_harmonization/harmony/HueMapper.hpp>

namespace ColorHarmonization::harmony {

    float gaussian(float x, float sigma) {
        return std::exp(-(x * x) / (2.0f * sigma * sigma));
    }

    float mapHueToNearestCenter(float hue, const HarmonyTemplate& harmonyTemplate, float rotationDegrees) {
        if (harmonyTemplate.sectors().empty()) {
            throw std::invalid_argument("Harmony template must contain sectors");
        }

        Sector nearestSector;
        float minDistanceToSector = std::numeric_limits<float>::max();

        for (const auto& sector : harmonyTemplate.sectors()) {
            const float shiftedCenter = sector.center + rotationDegrees;
            const float halfWidth = sector.width / 2.0f;

            float distanceToCenter = circularDistance(hue, shiftedCenter);
            const float distanceToSector = std::max(0.0f, distanceToCenter - halfWidth);

            if (distanceToSector < minDistanceToSector) {
                minDistanceToSector = distanceToSector;
                nearestSector = sector;
            }
        }

        return normalizeHue(nearestSector.center + rotationDegrees);
    }

    float mapHueLinearly(float hue, const HarmonyTemplate& harmonyTemplate, float rotationDegrees) {
        if (harmonyTemplate.sectors().empty()) {
            throw std::invalid_argument("Harmony template must contain sectors");
        }

        Sector nearestSector;
        float nearestCenter = 0.0f;
        float minDistanceToSector = std::numeric_limits<float>::max();

        for (const auto& sector : harmonyTemplate.sectors()) {
            const float shiftedCenter = normalizeHue(sector.center + rotationDegrees);
            const float halfWidth = sector.width / 2.0f;

            float distanceToCenter = circularDistance(hue, shiftedCenter);
            const float distanceToSector = std::max(0.0f, distanceToCenter - halfWidth);

            if (distanceToSector < minDistanceToSector) {
                minDistanceToSector = distanceToSector;
                nearestSector = sector;
                nearestCenter = shiftedCenter;
            }
        }

        const float halfWidth = nearestSector.width / 2.0f;
        const float offsetFromCenter = signedCircularDistance(nearestCenter, hue);
        const float mappedOffset = offsetFromCenter / 180.f * halfWidth;

        return normalizeHue(nearestCenter + mappedOffset);
    }

    float mapHueWithGaussian(float hue, const HarmonyTemplate& harmonyTemplate, float rotationDegrees, float sigma) {
        if (harmonyTemplate.sectors().empty()) {
            throw std::invalid_argument("Harmony template must contain sectors");
        }

        Sector nearestSector;
        float nearestCenter = 0.0f;
        float minDistanceToSector = std::numeric_limits<float>::max();

        for (const auto& sector : harmonyTemplate.sectors()) {
            const float shiftedCenter = normalizeHue(sector.center + rotationDegrees);
            const float halfWidth = sector.width / 2.0f;

            float distanceToCenter = circularDistance(hue, shiftedCenter);
            const float distanceToSector = std::max(0.0f, distanceToCenter - halfWidth);

            if (distanceToSector < minDistanceToSector) {
                minDistanceToSector = distanceToSector;
                nearestSector = sector;
                nearestCenter = shiftedCenter;
            }
        }

        const float w = nearestSector.width / 2.0f;
        const float offsetFromCenter = signedCircularDistance(nearestCenter, hue);
        const float g = gaussian(std::abs(offsetFromCenter), sigma);
        const float sign = offsetFromCenter > 0.0f ? 1.0f : -1.0f;

        return normalizeHue(nearestCenter + sign * w * ( 1 - g ));
    }

    Image<Hsv32f> mapImageHuesToNearestCenter(
        const Image<Hsv32f>& image,
        const HarmonyTemplate& harmonyTemplate,
        float rotationDegrees
    ) {
        return image.mapPixels<Hsv32f>(
            [&harmonyTemplate, rotationDegrees](const Hsv32f& pixel) {
                Hsv32f mappedPixel = pixel;
                mappedPixel.h = mapHueToNearestCenter(pixel.h, harmonyTemplate, rotationDegrees);
                return mappedPixel;
        });
    }

    Image<Hsv32f> mapImageHuesLinearly(
        const Image<Hsv32f>& image,
        const HarmonyTemplate& harmonyTemplate,
        float rotationDegrees
    ) {
        return image.mapPixels<Hsv32f>(
            [&harmonyTemplate, rotationDegrees](const Hsv32f& pixel) {
                Hsv32f mappedPixel = pixel;;
                mappedPixel.h = mapHueLinearly(pixel.h, harmonyTemplate, rotationDegrees);
                return mappedPixel;
            }
        );

    }

    Image<Hsv32f> mapImageHuesWithGaussian(
        const Image<Hsv32f>& image,
        const HarmonyTemplate& harmonyTemplate,
        float rotationDegrees,
        float sigma
    ) {
        if (sigma <= 0.f) {
            throw std::invalid_argument("sigma must be positive");
        }
        return image.mapPixels<Hsv32f>(
            [&harmonyTemplate, rotationDegrees, sigma](const Hsv32f& pixel) {
                Hsv32f mappedPixel = pixel;;
                mappedPixel.h = mapHueWithGaussian(pixel.h, harmonyTemplate, rotationDegrees, sigma);
                return mappedPixel;
            }
        );
    }

}