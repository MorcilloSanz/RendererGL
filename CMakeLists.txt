cmake_minimum_required(VERSION 3.12)

project(RendererGL
        VERSION 0.1
        DESCRIPTION "OpenGL renderer"
        LANGUAGES CXX)

set(HEADERS 
        glew/glew.h
        # ImGui
        imgui/imgui.h
        imgui/backends/imgui_impl_glfw.h
        imgui/backends/imgui_impl_opengl3.h
        imgui/imgui_internal.h
        imgui/imstb_rectpack.h
        imgui/imstb_textedit.h
        imgui/imstb_truetype.h
        # Source
        src/ImguiStyles.h
        src/engine/Vec3.h
        src/engine/window/Window.h
        src/engine/opengl/buffer/Buffer.h
        src/engine/opengl/buffer/VertexArray.h
        src/engine/opengl/buffer/VertexBuffer.h
        src/engine/opengl/buffer/IndexBuffer.h
        src/engine/opengl/shader/Shader.h
        src/engine/group/Polytope.h
        src/engine/group/DynamicPolytope.h
        src/engine/group/Group.h
        src/engine/renderer/Renderer.h
        src/engine/renderer/TextureRenderer.h
        src/engine/renderer/Camera.h
        src/engine/renderer/TrackballCamera.h
        src/engine/renderer/FPSCamera.h
        src/engine/renderer/SkyBox.h
        src/engine/renderer/MouseRayCasting.h
        src/engine/lighting/Light.h
        src/engine/texture/vendor/stb_image.h
        src/engine/texture/Texture.h
        src/engine/model/Model.h
)

set(SOURCES
        glew/glew.cpp
        # ImGui
        imgui/imgui.cpp
        imgui/backends/imgui_impl_glfw.cpp
        imgui/backends/imgui_impl_opengl3.cpp
        imgui/imgui_demo.cpp
        imgui/imgui_draw.cpp
        imgui/imgui_tables.cpp
        imgui/imgui_widgets.cpp
        # Source
        src/main.cpp
        src/engine/window/Window.cpp
        src/engine/opengl/buffer/VertexArray.cpp
        src/engine/opengl/buffer/VertexBuffer.cpp
        src/engine/opengl/buffer/IndexBuffer.cpp
        src/engine/opengl/shader/Shader.cpp
        src/engine/group/Polytope.cpp
        src/engine/group/DynamicPolytope.cpp
        src/engine/group/Group.cpp
        src/engine/renderer/Renderer.cpp
        src/engine/renderer/Camera.cpp
        src/engine/renderer/TrackballCamera.cpp
        src/engine/renderer/FPSCamera.cpp
        src/engine/renderer/SkyBox.cpp
        src/engine/renderer/MouseRayCasting.cpp
        src/engine/lighting/Light.cpp
        src/engine/texture/Texture.cpp
        src/engine/model/Model.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})
target_link_libraries(${PROJECT_NAME} assimp GL dl X11 glfw)