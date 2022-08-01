# Projects Settings
cmake_minimum_required (VERSION 3.1)
project (opengl-cmake-skeleton)

# The main executable
add_executable(opengl-cmake-skeleton
  src/Application.cpp
  src/Application.cpp
  src/MyApplication.cpp
  src/MyApplication.cpp
  src/glError.hpp
  src/glError.cpp
  src/main.cpp
  src/Shader.hpp
  src/Shader.cpp
  src/controls.hpp
  src/controls.cpp
  src/vboindexer.hpp
  src/vboindexer.cpp
  src/Model.hpp
  src/Model.cpp
  src/stb_image.h
  src/png_loader.hpp
  src/png_loader.cpp
  src/objloader.hpp
  src/objloader.cpp
  src/RenderModel.hpp
  src/RenderModel.cpp
  src/os/fileSystem.hpp
  src/os/fileSystem.cpp
  src/RenderModelIndexed.hpp
  src/RenderModelIndexed.cpp
)

set(GLM_TEST_ENABLE ON)
set_property(TARGET opengl-cmake-skeleton PROPERTY CXX_STANDARD 11)
target_compile_options(opengl-cmake-skeleton PRIVATE -Wall)

add_subdirectory(imgui)
find_package(glfw3 REQUIRED CONFIG)
#add_subdirectory(lib/glfw EXCLUDE_FROM_ALL)
add_subdirectory(lib/glew EXCLUDE_FROM_ALL)
add_subdirectory(lib/glm EXCLUDE_FROM_ALL)
add_subdirectory(lib/stl_parser EXCLUDE_FROM_ALL)

target_link_libraries(opengl-cmake-skeleton
  PRIVATE glfw
  PRIVATE libglew_static
  PRIVATE glm
  PRIVATE imgui
  PRIVATE stl_parser
)

configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/src/asset.hpp.in
  ${CMAKE_CURRENT_BINARY_DIR}/src/asset.hpp
)
target_include_directories(opengl-cmake-skeleton
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
  PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/src
)

