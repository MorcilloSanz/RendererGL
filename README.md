# RendererGL

RendererGL is a 3D renderer written in C++ and OpenGL. The main objective of this project is to create a framework that allows working with 3D graphics without the need to know computer graphics or OpenGL. So that both beginners and more experienced programmers can create a 3D scene with lighting, shadows and more advanced 3D models in an easy way. 

> **Warning** This project is still under development

*The file **main.cpp** is an example file using ImGui and RendererGL*

## Features

* Trackball and first person shooter camera
* Polytopes and groups in order to create a 3D scene
* Dynamic polytopes
* Polytope face culling
* Depth testing
* Blending
* Anti aliasing
* Textures
* Load 3D models (using ASSIMP)
* Skybox
* FrameCapturer (capture FrameBuffer and create a texture)
* Blinn-Phong lighting
* Emission
* Shadow Mapping (percentage closer filtering)
* Normal Mapping
* Gamma correction
* HDR
* Mouse ray casting
* Object selection

### Simple rotating cube example

```cpp
#include "engine/window/Window.h"
#include "engine/renderer/Renderer.h"
#include "engine/renderer/TrackballCamera.h"

Renderer::Ptr renderer;

// x y z  r g b
std::vector<Vec3f> cubeVertices {
    Vec3f(-0.5, -0.5,  0.5,   0.0f, 0.0f, 1.0f),
    Vec3f( 0.5, -0.5,  0.5,   1.0f, 0.0f, 1.0f),
    Vec3f( 0.5,  0.5,  0.5,   0.0f, 1.0f, 1.0f),
    Vec3f(-0.5,  0.5,  0.5,   0.0f, 1.0f, 0.5f),
    Vec3f(-0.5, -0.5, -0.5,   0.0f, 0.0f, 1.0f),
    Vec3f( 0.5, -0.5, -0.5,   1.0f, 0.0f, 1.0f),
    Vec3f( 0.5,  0.5, -0.5,   0.0f, 1.0f, 1.0f),
    Vec3f(-0.5,  0.5, -0.5,   0.0f, 1.0f, 0.5f)
};

std::vector<unsigned int> cubeIndices {
    0, 1, 2,  1, 5, 6,  7, 6, 5,
    2, 3, 0,  6, 2, 1,  5, 4, 7,
    4, 0, 3,  4, 5, 1,  3, 2, 6,
    3, 7, 4,  1, 0, 4,  6, 7, 3 
};

int main() {

    Window window("Cube example", 1280, 800);

    renderer = Renderer::New(window.getWidth(), window.getHeight());

    TrackballCamera camera = TrackballCamera::perspectiveCamera(glm::radians(45.0f), window.getWidth() / window.getHeight(), 0.1, 1000);
    camera.zoom(-5.5);
    renderer->setCamera(camera);

    Polytope::Ptr polytopeCube = Polytope::New(cubeVertices, cubeIndices);

    Group group;
    group.setLineWidth(2.f);
    group.translate(glm::vec3(-2.5, 0.5, 0));
    group.add(polytopeCube);

    renderer->addGroup(group);

    while (!window.windowShouldClose()) {

        // Update scene
        polytopeCube->rotate(0.15, glm::vec3(1, 0, 1));

        // Draw scene
        renderer->clear();
        renderer->render();
        window.update();
    }

    return 0;
}
```

## Screenshots

**Multiple point lights**

![alt text](https://github.com/MorcilloSanz/RendererGL/blob/main/img/lighting.png)

**Shadows**

![alt text](https://github.com/MorcilloSanz/RendererGL/blob/main/img/shadows.png)

**Emission**

![alt text](https://github.com/MorcilloSanz/RendererGL/blob/main/img/emission.png)

**Call of Duty Ghost dog**

![alt text](https://github.com/MorcilloSanz/RendererGL/blob/main/img/2.png)

## Contribution

RendererGL is an open source project under the MIT licence. Feel free to fork it and contribute

## Dependencies

* [GLFW](https://github.com/glfw/glfw) for creating a window with an OpenGL context
* [GLEW](https://github.com/nigels-com/glew) for loading OpenGL extensions
* [GLM](https://github.com/g-truc/glm) for linear algebra stuff
* [IMGUI](https://github.com/ocornut/imgui) for the user interface
* [ASSIMP](https://github.com/assimp/assimp) for loading 3D models from files (*.obj*, *.dae*, *...*)
* [STB](https://github.com/nothings/stb) for loading images from files (*.png*, *.tga*, *.jpg*, *...*)

## References

Dealing with OpenGL was much easier thanks to:

* [Learn OpenGL](https://learnopengl.com/)
* [The Cherno](https://www.youtube.com/@TheCherno)
* [ThinMatrix](https://www.youtube.com/@ThinMatrix)