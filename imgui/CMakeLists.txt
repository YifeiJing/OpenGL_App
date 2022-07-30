add_library(imgui STATIC imgui.cpp imgui_demo.cpp imgui_draw.cpp imgui_tables.cpp imgui_widgets.cpp backend/imgui_impl_glfw.cpp backend/imgui_impl_opengl3.cpp)
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED CONFIG)

target_include_directories(imgui PUBLIC .
                                           ./backend
                                           ${OPENGL__INCLUDE_DIR})
target_link_libraries(imgui ${OPENGL_LIBRARIES} glfw)
