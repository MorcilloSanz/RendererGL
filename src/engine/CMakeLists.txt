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

project(RendererGL)

# Header Files
set(HEADERS 
        Vec3.h
        ptr.h
        opengl/buffer/Buffer.h
        opengl/buffer/VertexArray.h
        opengl/buffer/VertexBuffer.h
        opengl/buffer/IndexBuffer.h
        opengl/buffer/FrameBuffer.h
        opengl/buffer/RenderBuffer.h
        opengl/buffer/MultiSampleRenderBuffer.h
        opengl/shader/Shader.h
        group/Polytope.h
        group/DynamicPolytope.h
        group/Group.h
        group/Scene.h
        renderer/Renderer.h
        renderer/FrameCapturer.h
        renderer/Camera.h
        renderer/TrackballCamera.h
        renderer/FPSCamera.h
        renderer/SkyBox.h
        renderer/MouseRayCasting.h
        lighting/Light.h
        lighting/DirectionalLight.h
        lighting/PointLight.h
        lighting/SpotLight.h
        texture/vendor/stb_image.h
        texture/Texture.h
        texture/CubeMapTexture.h
        texture/DepthTexture.h
        texture/ColorBufferTexture.h
        texture/MultiSampleTexture.h
        model/Model.h
)

# CPP files
set(SOURCES
        opengl/buffer/VertexArray.cpp
        opengl/buffer/VertexBuffer.cpp
        opengl/buffer/IndexBuffer.cpp
        opengl/buffer/FrameBuffer.cpp
        opengl/buffer/RenderBuffer.cpp
        opengl/buffer/MultiSampleRenderBuffer.cpp
        opengl/shader/Shader.cpp
        group/Polytope.cpp
        group/DynamicPolytope.cpp
        group/Group.cpp
        group/Scene.cpp
        renderer/Renderer.cpp
        renderer/Camera.cpp
        renderer/TrackballCamera.cpp
        renderer/FPSCamera.cpp
        renderer/SkyBox.cpp
        renderer/MouseRayCasting.cpp
        lighting/Light.cpp
        lighting/DirectionalLight.cpp
        lighting/PointLight.cpp
        lighting/SpotLight.cpp
        texture/Texture.cpp
        texture/CubeMapTexture.cpp
        texture/DepthTexture.cpp
        texture/ColorBufferTexture.cpp
        texture/MultiSampleTexture.cpp
        model/Model.cpp
)

# Compile files
add_library(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Link libraries
target_link_libraries(${PROJECT_NAME} 
                assimp 
                GL 
                dl 
                X11 
                GLEW::GLEW
)