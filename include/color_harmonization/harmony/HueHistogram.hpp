#pragma once
#include <vector>

#include <color_harmonization/image/Pixel.hpp>
#include <color_harmonization/image/Image.hpp>

namespace ColorHarmonization::harmony {

    struct hueHistogramOptions {
        std::size_t binCount = 360;
        float saturationThreshold = 0.01f;
        bool normalize = true;
    };

    class HueHistogram {
    public:
        explicit HueHistogram(std::size_t binCount);

        [[nodiscard]]
        static HueHistogram fromImage(const Image<Hsv32f>& image, const hueHistogramOptions& options = {});

        [[nodiscard]]
        std::size_t binCount() const noexcept;

        [[nodiscard]]
        float valueAt(std::size_t bin) const;

        [[nodiscard]]
        float angleAt(std::size_t bin) const;

        void addForBin(std::size_t bin, float weight);

        void normalize();

    private:
        std::size_t binForHue(float hue) const;
        std::vector<float> bins_;
    };

}
