add_library(PerlinNoise PerlinNoise.cpp)
target_include_directories(PerlinNoise INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

install(TARGETS PerlinNoise DESTINATION lib)
install(FILES PerlinNoise.hpp DESTINATION include)