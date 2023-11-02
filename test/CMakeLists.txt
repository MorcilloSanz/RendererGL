#[[
    MIT License

    Copyright (c) 2022 Alberto Morcillo Sanz

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.
]]

# Header Files
set(IMGUI_HEADERS 
        ${CMAKE_CURRENT_LIST_DIR}/third_party/imgui/imgui.h
        ${CMAKE_CURRENT_LIST_DIR}/third_party/imgui/backends/imgui_impl_glfw.h
        ${CMAKE_CURRENT_LIST_DIR}/third_party/imgui/backends/imgui_impl_opengl3.h
        ${CMAKE_CURRENT_LIST_DIR}/third_party/imgui/imgui_internal.h
        ${CMAKE_CURRENT_LIST_DIR}/third_party/imgui/imstb_rectpack.h
        ${CMAKE_CURRENT_LIST_DIR}/third_party/imgui/imstb_textedit.h
        ${CMAKE_CURRENT_LIST_DIR}/third_party/imgui/imstb_truetype.h
)

# CPP files
set(IMGUI_SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/third_party/imgui/imgui.cpp
        ${CMAKE_CURRENT_LIST_DIR}/third_party/imgui/backends/imgui_impl_glfw.cpp
        ${CMAKE_CURRENT_LIST_DIR}/third_party/imgui/backends/imgui_impl_opengl3.cpp
        ${CMAKE_CURRENT_LIST_DIR}/third_party/imgui/imgui_demo.cpp
        ${CMAKE_CURRENT_LIST_DIR}/third_party/imgui/imgui_draw.cpp
        ${CMAKE_CURRENT_LIST_DIR}/third_party/imgui/imgui_tables.cpp
        ${CMAKE_CURRENT_LIST_DIR}/third_party/imgui/imgui_widgets.cpp

)

include_directories(${CMAKE_CURRENT_LIST_DIR}/third_party)

add_subdirectory(testImGuiGLFW)
add_subdirectory(testCube)
add_subdirectory(testLighting)
add_subdirectory(testPBR)
add_subdirectory(testModelPBR)
add_subdirectory(testPointCloud)
add_subdirectory(testHDRI)