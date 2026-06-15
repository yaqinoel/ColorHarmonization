#include <iostream>
#include <stb_image.h>


int main() {
    int width = 0;
    int height = 0;
    int channels = 0;
    constexpr int desired_channels = 3;

    unsigned char* data = stbi_load("./assets/input/lena.png", &width, &height, &channels, 0);

    if (data == NULL) {
        std::cerr
            << "Failed to load image: "
            << stbi_failure_reason()
            << std::endl;
        return 1;
    }

    std::cout
        << "Width:" << width
        << ", Height:" << height
        << ", original channels:" << channels
        << std::endl;

    stbi_image_free(data);

    return 0;
}