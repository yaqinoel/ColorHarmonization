#include <algorithm>

#include <color_harmonization/harmony/HueHistogram.hpp>

namespace {
    std::size_t validateBinCount(std::size_t binCount) {
        if (binCount == 0) {
            throw std::invalid_argument(
                "bin count must be greater than zero"
            );
        }
        return binCount;
    }
}

namespace ColorHarmonization::harmony {

    HueHistogram::HueHistogram(std::size_t binCount)
        : bins_(validateBinCount(binCount), 0.0f){
    }

    [[nodiscard]]
    static HueHistogram HueHistogram::fromImage(const Image<Hsv32f>& image) {

    }

    [[nodiscard]]
    std::size_t HueHistogram::binCount() const noexcept{
        return bins_.size();
    }

    [[nodiscard]]
    float HueHistogram::valueAt(std::size_t bin) const {

    }

    [[nodiscard]]
    float HueHistogram::angleAt(std::size_t bin) const {

    }

    void HueHistogram::addForBin(std::size_t bin, float weight) {

    }

    void HueHistogram::normalize() {

    }

    std::size_t HueHistogram::binForHue(float hue) const {
        hue = std::fmod(hue, 360.0f);
        if (hue < 0.0F) {
            hue += 360.0F;
        }

        const std::size_t count = binCount();
        const std::size_t bin = static_cast<std::size_t>(hue / 360.0f * static_cast<float> (count));

        return std::min(bin, count-1);
    }

}
