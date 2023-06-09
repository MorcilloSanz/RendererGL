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