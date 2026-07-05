cmake_minimum_required(VERSION 3.20)
project(Color_Harmonization)

add_library(stb INTERFACE)

target_include_directories(stb
    INTERFACE
        ${PROJECT_SOURCE_DIR}/third_party/stb
)

add_executable(color_harmonizer
        app/main.cpp

        src/color_harmonization/image/StbImplementation.cpp
        src/color_harmonization/image/ImageIO.cpp

        src/color_harmonization/color/ColorConversion.cpp

        src/color_harmonization/harmony/HueHistogram.cpp
        src/color_harmonization/harmony/HarmonyTemplate.cpp
        src/color_harmonization/harmony/TemplateMatcher.cpp
        src/color_harmonization/harmony/HueMapper.cpp
)

target_include_directories(color_harmonizer
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
)

target_compile_features(color_harmonizer
    PRIVATE cxx_std_17
)

target_link_libraries(color_harmonizer
    PRIVATE stb
)

