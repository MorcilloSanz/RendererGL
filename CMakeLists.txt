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

cmake_minimum_required(VERSION 3.10.0...3.22)

if(${CMAKE_VERSION} VERSION_LESS 3.12)
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()

# Project
project(RendererGL
        VERSION 0.1
        DESCRIPTION "OpenGL renderer"
        HOMEPAGE_URL "https://github.com/MorcilloSanz/RendererGL"
        LANGUAGES CXX)

message(STATUS "\n${PROJECT_NAME} ${RendererGL_VERSION}")
message(STATUS ${RendererGL_DESCRIPTION})
message(STATUS ${RendererGL_HOMEPAGE_URL})

set (CMAKE_CXX_STANDARD 17)
set(CMAKE_DEBUG_POSTFIX d)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D_DEBUG")
add_definitions(-DUNICODE)

# Libraries
find_package(GLEW REQUIRED)
find_package(assimp REQUIRED)

# Add src
include_directories(src)
add_subdirectory(src)

# Copy shaders into build folder
set(SHADERS_PATH "${CMAKE_CURRENT_SOURCE_DIR}/src/engine/opengl/glsl")
file(GLOB shaderFiles ${SHADERS_PATH}/*.frag ${SHADERS_PATH}/*.vert)
foreach(filename ${shaderFiles} )
    file(COPY ${filename} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/glsl)
endforeach()

# Add tests
add_subdirectory(test)