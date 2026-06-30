#include <stdexcept>
#include <string>
#include <cstdint>
#include <iostream>
#include <vector>

#include <stb_image.h>
#include <stb_image_write.h>

#include <color_harmonization/image/ImageIO.hpp>

namespace ColorHarmonization::ImageIO {

    [[nodiscard]]
    Image<Rgb8> loadRgb(const std::filesystem::path& path) {
        int width = 0;
        int height = 0;
        int sourceChannels = 0;
        constexpr int desiredChannels = 3;

        unsigned char* rawData = stbi_load(path.string().c_str(), &width, &height, &sourceChannels, desiredChannels);

        if (rawData == nullptr) {
            throw std::runtime_error("Failed to load image: " + path.string() );
        }

        if (width <= 0 || height <= 0) {
            stbi_image_free(rawData);
            throw std::runtime_error("Invalid image dimensions: " + path.string());
        }

        Image<Rgb8> image(static_cast<size_t> (width), static_cast<size_t> (height));

        for (std::size_t y = 0; y < image.height(); y++) {
            for (std::size_t x = 0; x < image.width(); x++) {
                const std::size_t rawDataIndex = (y * image.width() + x) * desiredChannels;

                image.at(x,y) = Rgb8{
                    rawData[rawDataIndex],
                    rawData[rawDataIndex + 1],
                    rawData[rawDataIndex + 2]
                };
            }
        }

        stbi_image_free(rawData);

        return image;
    }

    void savePng(const std::filesystem::path &path, const Image<Rgb8> &image) {

        constexpr int channels = 3;
        const int width = static_cast<int> (image.width());
        const int height = static_cast<int> (image.height());

        std::vector<uint8_t> buffer(image.width() * image.height() * channels);

        for (std::size_t y = 0; y < image.height(); y++) {
            for (std::size_t x = 0; x < image.width(); x++) {
                const Rgb8& pixel = image.at(x,y);
                const std::size_t rawDataIndex = (y * image.width() + x) * channels;
                buffer[rawDataIndex] = pixel.r;
                buffer[rawDataIndex + 1] = pixel.g;
                buffer[rawDataIndex + 2] = pixel.b;
            }
        }

        const int success = stbi_write_png(path.string().c_str(), width, height, channels, buffer.data(), width * channels);

        if (success == 0) {
            throw std::runtime_error("Failed to save image at : " + path.string());
        }

        std::cout<< "Saved image at :" << path.string() << std::endl;
    }

} // namespace Colorharmonization::ImageIO

