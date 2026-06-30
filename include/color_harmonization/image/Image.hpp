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

            template <typename Func>
            void forEachPixel(Func func) {
                for (std::size_t y = 0; y < height_; ++y) {
                    for (std::size_t x = 0; x < width_; ++x) {
                        func(x, y, at(x, y));
                    }
                }
            }

            template <typename Func>
            void forEachPixel(Func func) const{
                for (std::size_t y = 0; y < height_; ++y) {
                    for (std::size_t x = 0; x < width_; ++x) {
                        func(x, y, at(x, y));
                    }
                }
            }

            template <typename OutputPixel, typename Func>
            [[nodiscard]]
            Image<OutputPixel> mapPixels(Func func) const {
                Image<OutputPixel> result(width_, height_);
                for (std::size_t y = 0; y < height_; ++y) {
                    for (std::size_t x = 0; x < width_; ++x) {
                        result.at(x,y) = func(at(x,y));
                    }
                }
                return result;
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

