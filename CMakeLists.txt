cmake_minimum_required(VERSION 3.18)

project(ViewGenerator VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

option(USE_BITMAPENCODER "Use BitMapEncoder to write .bmp images" ON)

configure_file(ViewConfig.h.in ViewConfig.h)

if(USE_BITMAPENCODER)
    add_subdirectory(BitMapEncoder)
    list(APPEND EXTRA_LIBS BitMapEncoder)
endif()

add_subdirectory(CellularNoise)
add_subdirectory(PerlinNoise)
add_subdirectory(GeneratedTree)
list(APPEND EXTRA_LIBS CellularNoise)
list(APPEND EXTRA_LIBS PerlinNoise)
list(APPEND EXTRA_LIBS GeneratedTree)

add_executable(View main.cpp)

target_link_libraries(View PUBLIC ${EXTRA_LIBS})

target_include_directories(View PUBLIC "${PROJECT_BINARY_DIR}")

install(TARGETS View DESTINATION bin)
install(FILES "${PROJECT_BINARY_DIR}/ViewConfig.h" DESTINATION include)