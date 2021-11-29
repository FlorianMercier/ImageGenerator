add_library(CellularNoise CellularNoise.cpp)
target_include_directories(CellularNoise INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

install(TARGETS CellularNoise DESTINATION lib)
install(FILES CellularNoise.hpp DESTINATION include)