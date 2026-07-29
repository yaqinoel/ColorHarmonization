#pragma once

#include <QImage>
#include <color_harmonization/image/Image.hpp>
#include <color_harmonization/image/Pixel.hpp>

namespace ColorHarmonization::gui {

    [[nodiscard]]
    Image<Rgb8> fromQtImage(const QImage& image);

    [[nodiscard]]
    QImage toQtImage(const Image<Rgb8>& image);

}