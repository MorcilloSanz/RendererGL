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

project(testImGuiGLFW)

# Header Files
set(HEADERS 
        imgui/imgui.h
        imgui/backends/imgui_impl_glfw.h
        imgui/backends/imgui_impl_opengl3.h
        imgui/imgui_internal.h
        imgui/imstb_rectpack.h
        imgui/imstb_textedit.h
        imgui/imstb_truetype.h

        src/ImguiStyles.h
)

# CPP files
set(SOURCES
        imgui/imgui.cpp
        imgui/backends/imgui_impl_glfw.cpp
        imgui/backends/imgui_impl_opengl3.cpp
        imgui/imgui_demo.cpp
        imgui/imgui_draw.cpp
        imgui/imgui_tables.cpp
        imgui/imgui_widgets.cpp

        src/main.cpp
)

# Copy shaders into build folder
set(SHADERS_PATH "${CMAKE_SOURCE_DIR}/src/engine/opengl/glsl")
file(GLOB shaderFiles ${SHADERS_PATH}/*.frag ${SHADERS_PATH}/*.vert)
foreach(filename ${shaderFiles} )
    file(COPY ${filename} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/glsl)
endforeach()

# Executable
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Linker
target_link_libraries(${PROJECT_NAME} glfw RendererGL)