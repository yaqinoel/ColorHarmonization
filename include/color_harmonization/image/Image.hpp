#pragma once

#include <cstddef>
#include <stdexcept>
#include <vector>

namespace ColorHarmonization {

    template <typename Pixel>
    class Image {
        public:
            Image(std::size_t width, std::size_t height) :
                width_(width),
                height_(height),
                pixels_(width * height) {
            }

            [[nodiscard]]
            std::size_t width() const noexcept {
                return width_;
            }

            [[nodiscard]]
            std::size_t height() const noexcept{
                return height_;
            }

            [[nodiscard]]
            bool empty() const noexcept{
                return pixels_.empty();
            }

            Pixel& at(std::size_t x, std::size_t y) {
                checkCoordinates(x, y);
                return pixels_[index(x, y)];
            }

            const Pixel& at(std::size_t x, std::size_t y) const {
                checkCoordinates(x, y);
                return pixels_[index(x, y)];
            }

        private: 
            [[nodiscard]]
            std::size_t index(std::size_t x, std::size_t y) const noexcept{
                return y * width_ + x;
            }

            void checkCoordinates(std::size_t x, std::size_t y) const {
                if (x >= width_ || y >= height_) {
                    throw std::out_of_range(
                        "Image coordinates are out of range"
                    );
                }
            }

            std::size_t width_;
            std::size_t height_;
            std::vector<Pixel> pixels_;
    }; // class Image

} // namespace ColorHarmonization

