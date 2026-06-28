#include <algorithm>
#include <stdexcept>
#include <cmath>
#include <fstream>
#include <iostream>
#include <ostream>

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
    HueHistogram HueHistogram::fromImage(const Image<Hsv32f>& image,const HueHistogramOptions& options) {
        HueHistogram histogram = HueHistogram(options.binCount);

        for (std::size_t y = 0; y < image.height(); ++y) {
            for (std::size_t x = 0; x < image.width(); ++x) {
                const Hsv32f& pixel = image.at(x,y);
                if (pixel.s < options.saturationThreshold) {
                    continue;
                }
                const std::size_t bin = histogram.binForHue(pixel.h);
                histogram.addForBin(bin, pixel.s * pixel.v);
            }
        }

        if (options.normalize) {
            histogram.normalize();
        }

        return histogram;
    }

    [[nodiscard]]
    std::size_t HueHistogram::binCount() const noexcept{
        return bins_.size();
    }

    [[nodiscard]]
    float HueHistogram::valueAt(std::size_t bin) const {
        // return bins_.at(bin);
        const std::size_t count = binCount();
        if ( bin >= count) {
            throw std::out_of_range(
                "Hue histogram bin index is out of range"
            );
        }
        return bins_[bin];
    }

    [[nodiscard]]
    float HueHistogram::angleAt(std::size_t bin) const {
        const std::size_t count = binCount();
        if ( bin >= count) {
            throw std::out_of_range(
                "Hue histogram bin index is out of range"
            );
        }
        return (static_cast<float>(bin) + 0.5f) * 360.0f / static_cast<float>(count);
    }

    void HueHistogram::addForBin(std::size_t bin, float weight) {
        // bins_.at(bin) += weight;
        const std::size_t count = binCount();
        if ( bin >= count) {
            throw std::out_of_range(
                "Hue histogram bin index is out of range"
            );
        }
        bins_[bin] += weight;
    }

    void HueHistogram::normalize() {
        float sum = 0.0f;

        for (const float value : bins_) {
            sum += value;
        }

        if (sum == 0.0f) {
            return;
        }

        for (float& value : bins_) {
            value /= sum;
        }
    }

    void HueHistogram::outputData(std::ostream& stream) const {
        stream << "Bin_Count: " <<  binCount() << std::endl;
        stream << "bin, angle, value" << std::endl;
        for (std::size_t bin = 0; bin < binCount(); ++bin) {
            stream << bin << ", " << angleAt(bin) << ", " << valueAt(bin) << std::endl;
        }
    }

    void HueHistogram::saveData(const std::filesystem::path& path) const {
        std::ofstream file(path);
        if (!file) {
            throw std::runtime_error( "Failed to open histogram output file: " + path.string() );
        }
        outputData(file);
        std::cout << "Saved file to " + path.string() << "!" << std::endl;
    }


    [[nodiscard]]
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
