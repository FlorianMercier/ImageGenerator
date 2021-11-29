add_library(GeneratedTree GeneratedTree.cpp Branch.cpp Point.cpp)
target_include_directories(GeneratedTree INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

install(TARGETS GeneratedTree DESTINATION lib)
install(FILES GeneratedTree.hpp Branch.hpp Point.hpp draw2D.hpp DESTINATION include)