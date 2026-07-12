add_library(maxflow STATIC
  graph.cpp
  maxflow.cpp
)

target_compile_features(maxflow PRIVATE cxx_std_11)
target_compile_options(maxflow PRIVATE -O3 -w)
target_include_directories(maxflow PRIVATE .)
