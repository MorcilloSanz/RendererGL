cmake_minimum_required(VERSION 3.12)
set (CMAKE_CXX_STANDARD 17)

project(RendererGL
        VERSION 0.1
        DESCRIPTION "OpenGL renderer"
        LANGUAGES CXX)

# Header Files
set(HEADERS 
        glew/glew.h

        imgui/imgui.h
        imgui/backends/imgui_impl_glfw.h
        imgui/backends/imgui_impl_opengl3.h
        imgui/imgui_internal.h
        imgui/imstb_rectpack.h
        imgui/imstb_textedit.h
        imgui/imstb_truetype.h

        src/ImguiStyles.h
        src/engine/Vec3.h
        src/engine/ptr.h
        src/engine/window/Window.h
        src/engine/opengl/buffer/Buffer.h
        src/engine/opengl/buffer/VertexArray.h
        src/engine/opengl/buffer/VertexBuffer.h
        src/engine/opengl/buffer/IndexBuffer.h
        src/engine/opengl/buffer/FrameBuffer.h
        src/engine/opengl/buffer/RenderBuffer.h
        src/engine/opengl/buffer/MultiSampleRenderBuffer.h
        src/engine/opengl/shader/Shader.h
        src/engine/group/Polytope.h
        src/engine/group/DynamicPolytope.h
        src/engine/group/Group.h
        src/engine/group/Scene.h
        src/engine/renderer/Renderer.h
        src/engine/renderer/FrameCapturer.h
        src/engine/renderer/Camera.h
        src/engine/renderer/TrackballCamera.h
        src/engine/renderer/FPSCamera.h
        src/engine/renderer/SkyBox.h
        src/engine/renderer/MouseRayCasting.h
        src/engine/lighting/Light.h
        src/engine/lighting/DirectionalLight.h
        src/engine/lighting/PointLight.h
        src/engine/lighting/SpotLight.h
        src/engine/texture/vendor/stb_image.h
        src/engine/texture/Texture.h
        src/engine/texture/CubeMapTexture.h
        src/engine/texture/DepthTexture.h
        src/engine/texture/ColorBufferTexture.h
        src/engine/texture/MultiSampleTexture.h
        src/engine/model/Model.h
)

# CPP files
set(SOURCES
        glew/glew.cpp

        imgui/imgui.cpp
        imgui/backends/imgui_impl_glfw.cpp
        imgui/backends/imgui_impl_opengl3.cpp
        imgui/imgui_demo.cpp
        imgui/imgui_draw.cpp
        imgui/imgui_tables.cpp
        imgui/imgui_widgets.cpp

        src/main.cpp
        src/engine/window/Window.cpp
        src/engine/opengl/buffer/VertexArray.cpp
        src/engine/opengl/buffer/VertexBuffer.cpp
        src/engine/opengl/buffer/IndexBuffer.cpp
        src/engine/opengl/buffer/FrameBuffer.cpp
        src/engine/opengl/buffer/RenderBuffer.cpp
        src/engine/opengl/buffer/MultiSampleRenderBuffer.cpp
        src/engine/opengl/shader/Shader.cpp
        src/engine/group/Polytope.cpp
        src/engine/group/DynamicPolytope.cpp
        src/engine/group/Group.cpp
        src/engine/group/Scene.cpp
        src/engine/renderer/Renderer.cpp
        src/engine/renderer/Camera.cpp
        src/engine/renderer/TrackballCamera.cpp
        src/engine/renderer/FPSCamera.cpp
        src/engine/renderer/SkyBox.cpp
        src/engine/renderer/MouseRayCasting.cpp
        src/engine/lighting/Light.cpp
        src/engine/lighting/DirectionalLight.cpp
        src/engine/lighting/PointLight.cpp
        src/engine/lighting/SpotLight.cpp
        src/engine/texture/Texture.cpp
        src/engine/texture/CubeMapTexture.cpp
        src/engine/texture/DepthTexture.cpp
        src/engine/texture/ColorBufferTexture.cpp
        src/engine/texture/MultiSampleTexture.cpp
        src/engine/model/Model.cpp
)

# Copy shaders into build folder
set(SHADERS_PATH "${CMAKE_CURRENT_SOURCE_DIR}/src/engine/opengl/glsl")
file(GLOB shaderFiles ${SHADERS_PATH}/*.frag ${SHADERS_PATH}/*.vert)
foreach(filename ${shaderFiles} )
    file(COPY ${filename} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/glsl)
endforeach()

# Compile files
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Link libraries
target_link_libraries(${PROJECT_NAME} assimp GL dl X11 glfw)