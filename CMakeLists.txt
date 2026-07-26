cmake_minimum_required(VERSION 3.22.0)
project(Color_Harmonization)

add_library(stb INTERFACE)

# stb
target_include_directories(stb
    INTERFACE
        ${PROJECT_SOURCE_DIR}/third_party/stb
)

# maxflow
add_subdirectory(third_party/maxflow-v3.04)

target_include_directories(maxflow
    PUBLIC
        ${PROJECT_SOURCE_DIR}/third_party/maxflow-v3.04
)

# static library
add_library(color_harmonization_core STATIC
        src/color_harmonization/image/StbImplementation.cpp
        src/color_harmonization/image/ImageIO.cpp

        src/color_harmonization/color/ColorConversion.cpp

        src/color_harmonization/harmony/HueHistogram.cpp
        src/color_harmonization/harmony/HarmonyTemplate.cpp
        src/color_harmonization/harmony/TemplateMatcher.cpp
        src/color_harmonization/harmony/HueMapper.cpp

        src/color_harmonization/visualisation/HueHistogramRender.cpp
)

target_include_directories(color_harmonization_core
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
)

target_compile_features(color_harmonization_core
    PRIVATE cxx_std_17
)

target_link_libraries(color_harmonization_core
    PRIVATE
        stb
        maxflow
)

# Cli
add_executable(color_harmonizer
    app/cli_main.cpp
)

target_link_libraries(color_harmonizer
    PRIVATE
        color_harmonization_core
)

# Qt GUI
find_package(Qt6 REQUIRED COMPONENTS Widgets)
qt_standard_project_setup()

qt_add_executable(color_harmonizer_gui
    app/gui/main.cpp
    app/gui/MainWindow.cpp
    app/gui/MainWindow.hpp
    app/gui/ImagePanel.hpp
    app/gui/ImagePanel.cpp
    app/gui/ImageView.hpp
    app/gui/ImageView.cpp
)

target_compile_features(color_harmonizer_gui
    PRIVATE cxx_std_17
)

target_link_libraries(color_harmonizer_gui
    PRIVATE
        Qt6::Widgets
)
