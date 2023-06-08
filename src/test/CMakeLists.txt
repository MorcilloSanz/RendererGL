project(test)

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