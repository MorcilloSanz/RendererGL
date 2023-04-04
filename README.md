# RendererGL

RendererGL is a 3D renderer written in C++ and OpenGL

The file:

```
main.cpp
```

is an example file using ImGui and RendererGL

## Features

* Trackball and first person shooter camera
* Polytopes and groups in order to create a 3D scene
* Polytope face culling
* Depth testing and blending
* Anti aliasing
* Dynamic polytopes
* Textures
* Load 3D models
* Skybox
* Texture renderer (capture FrameBuffer and create a texture)
* Blinn-Phong lighting (directional, point and spot)
* Emission
* Shadow Mapping (percentage closer filtering)
* Normal Mapping
* Gamma correction
* HDR
* Mouse ray casting
* Object selection

## Cube example

Simple rotating cube example:

```cpp
#include "engine/window/Window.h"
#include "engine/renderer/Renderer.h"
#include "engine/renderer/TrackballCamera.h"

Window window;

Renderer::Ptr renderer;

// x y z r g b
std::vector<Vec3f> cubeVertices {
    // Front square
    Vec3f(-0.5, -0.5,  0.5,   0.0f, 0.0f, 1.0f),
    Vec3f( 0.5, -0.5,  0.5,   1.0f, 0.0f, 1.0f),
    Vec3f( 0.5,  0.5,  0.5,   0.0f, 1.0f, 1.0f),
    Vec3f(-0.5,  0.5,  0.5,   0.0f, 1.0f, 0.5f),
    // Back square
    Vec3f(-0.5, -0.5, -0.5,   0.0f, 0.0f, 1.0f),
    Vec3f( 0.5, -0.5, -0.5,   1.0f, 0.0f, 1.0f),
    Vec3f( 0.5,  0.5, -0.5,   0.0f, 1.0f, 1.0f),
    Vec3f(-0.5,  0.5, -0.5,   0.0f, 1.0f, 0.5f)
};

std::vector<unsigned int> cubeIndices {
    //front   //right   //back
    0, 1, 2,  1, 5, 6,  7, 6, 5,
    2, 3, 0,  6, 2, 1,  5, 4, 7,
    //left    //bottom  //top
    4, 0, 3,  4, 5, 1,  3, 2, 6,
    3, 7, 4,  1, 0, 4,  6, 7, 3 
};

int main() {

    window = Window("Cube example", 1280, 800);

    renderer = Renderer::New(window.getWidth(), window.getHeight());

    TrackballCamera camera = TrackballCamera::perspectiveCamera(glm::radians(45.0f), window.getWidth() / window.getHeight(), 0.1, 1000);
    float sensitivity = 1.5f, panSensitivity = 1.0f, zoomSensitivity = 1.0f;
    camera.zoom(-5.5);
    renderer->setCamera(camera);

    Polytope::Ptr polytopeCube = Polytope::New(cubeVertices, cubeIndices);

    Group group;
    group.setLineWidth(2.f);
    group.translate(glm::vec3(-2.5, 0.5, 0));
    group.add(polytopeCube);

    renderer->addGroup(group);

    while (!window.windowShouldClose()) {

        // Rotate cube
        polytopeCube->rotate(0.15, glm::vec3(1, 0, 1));

        // Clear
        renderer->clear();

        // Render
        renderer->render();

        // Swap buffers
        window.update();
    }

    return 0;
}
```

# Screenshots

**Multiple point lights**

![alt text](https://github.com/MorcilloSanz/RendererGL/blob/main/img/lighting.png)

**Shadows**

![alt text](https://github.com/MorcilloSanz/RendererGL/blob/main/img/shadows.png)

**Emission**

![alt text](https://github.com/MorcilloSanz/RendererGL/blob/main/img/emission.png)

**Call of Duty Ghost dog**

![alt text](https://github.com/MorcilloSanz/RendererGL/blob/main/img/2.png)

# Dependencies

* [GLFW](https://github.com/glfw/glfw)
* [GLEW](https://github.com/nigels-com/glew)
* [GLM](https://github.com/g-truc/glm)
* [IMGUI](https://github.com/ocornut/imgui)
* [ASSIMP](https://github.com/assimp/assimp)
* [STB](https://github.com/nothings/stb)