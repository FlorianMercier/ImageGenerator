add_library(BitMapEncoder BitMapEncoder.cpp)
target_include_directories(BitMapEncoder INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

install(TARGETS BitMapEncoder DESTINATION lib)
install(FILES BitMapEncoder.hpp DESTINATION include)