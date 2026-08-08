#include <limits>
#include <stdexcept>
#include <algorithm>
#include <cmath>
#include <color_harmonization/harmony/HueMapper.hpp>
#include "graph.h"

namespace ColorHarmonization::harmony {

    float gaussian(float x, float sigma) {
        return std::exp(-(x * x) / (2.0f * sigma * sigma));
    }

    Sector findNearestSectorByDirection(float hue, const HarmonyTemplate& harmonyTemplate, float rotationDegrees, MoveDirection direction) {
        if (harmonyTemplate.sectors().empty()) {
            throw std::invalid_argument("Harmony template must contain sectors");
        }

        Sector nearestSector;
        float minDistanceToSector = std::numeric_limits<float>::max();

        for (const auto& sector : harmonyTemplate.sectors()) {
            const float distanceToSector = directedDistanceToSector(hue, sector, rotationDegrees, direction);
            if (distanceToSector < minDistanceToSector) {
                minDistanceToSector = distanceToSector;
                nearestSector = sector;
            }
        }

        return nearestSector;
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

    float mapHueToNearestCenterByDirection(float hue, const HarmonyTemplate& harmonyTemplate, float rotationDegrees, MoveDirection direction) {
        if (harmonyTemplate.sectors().empty()) {
            throw std::invalid_argument("Harmony template must contain sectors");
        }

        Sector nearestSector = findNearestSectorByDirection(hue, harmonyTemplate, rotationDegrees, direction);

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

    float mapHueLinearlyByDirection(float hue, const HarmonyTemplate& harmonyTemplate, float rotationDegrees, MoveDirection direction) {
        if (harmonyTemplate.sectors().empty()) {
            throw std::invalid_argument("Harmony template must contain sectors");
        }

        Sector nearestSector = findNearestSectorByDirection(hue, harmonyTemplate, rotationDegrees, direction);

        const float nearestCenter = normalizeHue(nearestSector.center + rotationDegrees);

        const float halfWidth = nearestSector.width / 2.0f;
        const float offsetFromCenterByMoveDirection = directedCircularDistance(hue, nearestCenter, direction);
        const float side = direction == MoveDirection::Positive ? -1.0f : 1.0f;
        const float mappedOffset =  side * offsetFromCenterByMoveDirection / 360.f * halfWidth;

        return normalizeHue(nearestCenter + mappedOffset);
    }

    float mapHueWithGaussian(float hue, const HarmonyTemplate& harmonyTemplate, float rotationDegrees, float sigmaScale) {
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
        const float sigma = nearestSector.width * sigmaScale;
        const float offsetFromCenter = signedCircularDistance(nearestCenter, hue);
        const float g = gaussian(std::abs(offsetFromCenter), sigma);
        const float sign = offsetFromCenter > 0.0f ? 1.0f : -1.0f;

        return normalizeHue(nearestCenter + sign * w * ( 1 - g ));
    }

    float mapHueWithGaussianByDirection(float hue, const HarmonyTemplate& harmonyTemplate, float rotationDegrees, float sigmaScale, MoveDirection direction) {
        if (harmonyTemplate.sectors().empty()) {
            throw std::invalid_argument("Harmony template must contain sectors");
        }

        Sector nearestSector = findNearestSectorByDirection(hue, harmonyTemplate, rotationDegrees, direction);

        const float nearestCenter = normalizeHue(nearestSector.center + rotationDegrees);

        const float w = nearestSector.width / 2.0f;
        const float sigma = nearestSector.width * sigmaScale;
        const float offsetFromCenterByMoveDirection = directedCircularDistance(hue, nearestCenter, direction);
        const float g = gaussian(offsetFromCenterByMoveDirection, sigma);
        const float side = direction == MoveDirection::Positive ? -1.0f : 1.0f;

        return normalizeHue(nearestCenter + side * w * ( 1 - g ));
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

    Image<Hsv32f> mapImageHuesToNearestCenter(
        const Image<Hsv32f>& image,
        const HarmonyTemplate& harmonyTemplate,
        float rotationDegrees,
        const std::vector<MoveDirection>& directionsLabelMap
    ) {
        const std::size_t width = image.width();
        const std::size_t expectedSize = image.width() * image.height();
        if (directionsLabelMap.size() != expectedSize) {
            throw std::invalid_argument("Direction label map size does not match image size");
        }
        return image.mapPixelsWithCoords<Hsv32f>(
            [&harmonyTemplate, rotationDegrees, &directionsLabelMap, width](std::size_t x, std::size_t y, const Hsv32f& pixel) {
                Hsv32f mappedPixel = pixel;
                std::size_t index = x + y * width;
                mappedPixel.h = mapHueToNearestCenterByDirection(pixel.h, harmonyTemplate, rotationDegrees, directionsLabelMap[index]);
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

    Image<Hsv32f> mapImageHuesLinearly(
        const Image<Hsv32f>& image,
        const HarmonyTemplate& harmonyTemplate,
        float rotationDegrees,
        const std::vector<MoveDirection>& directionsLabelMap
    ) {
        const std::size_t width = image.width();
        const std::size_t expectedSize = image.width() * image.height();
        if (directionsLabelMap.size() != expectedSize) {
            throw std::invalid_argument("Direction label map size does not match image size");
        }
        return image.mapPixelsWithCoords<Hsv32f>(
            [&harmonyTemplate, rotationDegrees, &directionsLabelMap, width](std::size_t x, std::size_t y, const Hsv32f& pixel) {
                Hsv32f mappedPixel = pixel;
                std::size_t index = x + y * width;
                mappedPixel.h = mapHueLinearlyByDirection(pixel.h, harmonyTemplate, rotationDegrees, directionsLabelMap[index]);
                return mappedPixel;
        });
    }

    Image<Hsv32f> mapImageHuesWithGaussian(
        const Image<Hsv32f>& image,
        const HarmonyTemplate& harmonyTemplate,
        float rotationDegrees,
        float sigmaScale
    ) {
        if (sigmaScale <= 0.f) {
            throw std::invalid_argument("sigma must be positive");
        }
        return image.mapPixels<Hsv32f>(
            [&harmonyTemplate, rotationDegrees, sigmaScale](const Hsv32f& pixel) {
                Hsv32f mappedPixel = pixel;;
                mappedPixel.h = mapHueWithGaussian(pixel.h, harmonyTemplate, rotationDegrees, sigmaScale);
                return mappedPixel;
            }
        );
    }

    Image<Hsv32f> mapImageHuesWithGaussian(
        const Image<Hsv32f>& image,
        const HarmonyTemplate& harmonyTemplate,
        float rotationDegrees,
        float sigmaScale,
        const std::vector<MoveDirection>& directionsLabelMap
    ) {
        if (sigmaScale <= 0.f) {
            throw std::invalid_argument("sigma must be positive");
        }
        const std::size_t expectedSize = image.width() * image.height();
        if (directionsLabelMap.size() != expectedSize) {
            throw std::invalid_argument("Direction label map size does not match image size");
        }
        const std::size_t width = image.width();
        return image.mapPixelsWithCoords<Hsv32f>(
            [&harmonyTemplate, rotationDegrees, sigmaScale, &directionsLabelMap, width](std::size_t x, std::size_t y, const Hsv32f& pixel) {
                Hsv32f mappedPixel = pixel;
                std::size_t index = x + y * width;
                mappedPixel.h = mapHueWithGaussianByDirection(pixel.h, harmonyTemplate, rotationDegrees, sigmaScale, directionsLabelMap[index]);
                return mappedPixel;
        });
    }

    std::vector<MoveDirection> calculateLabelMapByGraphCut(
            const Image<Hsv32f>& image,
            const HarmonyTemplate& harmonyTemplate,
            float rotationDegrees,
            float lambda,
            float colorSigma
    ) {

        if (harmonyTemplate.sectors().empty()) {
            throw std::invalid_argument(
                "Harmony template must contain sectors"
            );
        }

        if (lambda <= 0.0f || colorSigma <= 0.0f) {
            throw std::invalid_argument("lambda or color sigma must be positive");
        }

        typedef Graph<float, float, float> GraphType;

        const std::size_t numPixels = image.width() * image.height();
        std::vector<MoveDirection> label_map(numPixels);

        GraphType *graph = new GraphType(numPixels, numPixels * 2);
        graph->add_node(numPixels);

        image.forEachPixel([&](
            std::size_t x, std::size_t y, const Hsv32f& pixel
            ) {
                const float normalizedHue = normalizeHue(pixel.h);
                const Sector leftSector = findNearestSectorByDirection(normalizedHue, harmonyTemplate,rotationDegrees, MoveDirection::Negative);
                const Sector rightSector = findNearestSectorByDirection(normalizedHue, harmonyTemplate,rotationDegrees, MoveDirection::Positive);

                const float costToLeftSector = directedDistanceToSector(normalizedHue, leftSector, rotationDegrees, MoveDirection::Negative) * pixel.s * pixel.v;
                const float costToRightSector = directedDistanceToSector(normalizedHue, rightSector, rotationDegrees, MoveDirection::Positive) * pixel.s * pixel.v;

                std::size_t node = y * image.width() + x;

                graph->add_tweights(node, costToRightSector, costToLeftSector);

                if (x + 1 < image.width()) {
                    std::size_t node_right = y * image.width() + x + 1;
                    Hsv32f pixelRight = image.at(x+1, y);

                    const float hueDiff = circularDistance(pixel.h, pixelRight.h);
                    const float saturationWeight = pixel.s * pixelRight.s;

                    const float penalty = lambda * saturationWeight * std::exp(-(hueDiff * hueDiff) / (2.0f * colorSigma * colorSigma));
                    graph->add_edge(node, node_right, penalty, penalty);
                }

                if (y + 1 < image.height()) {
                    std::size_t node_down = (y + 1) * image.width() + x;
                    Hsv32f pixelDown = image.at(x, y+1);

                    const float hueDiff = circularDistance(pixel.h, pixelDown.h);
                    const float saturationWeight = pixel.s * pixelDown.s;

                    const float penalty = lambda * saturationWeight * std::exp(-(hueDiff * hueDiff) / (2.0f * colorSigma * colorSigma));
                    graph->add_edge(node, node_down, penalty, penalty);
                }
        });

        graph->maxflow();

        for (int i = 0; i < numPixels; ++i) {
            label_map[i] = (graph->what_segment(i) == GraphType::SOURCE) ? MoveDirection::Negative : MoveDirection::Positive;
        }
        delete graph;

        return label_map;
    }

}