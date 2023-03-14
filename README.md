# RendererGL

RendererGL is a basic 3D engine written in C++ and OpenGL

The file:

```
main.cpp
```

is a template using ImGui and RendererGL

## Engine

Features:

* Create a window, handle its events and create an OpenGL context (GLFW)
* ImGui as user interface
* Trackball and First Person Shooter camera
* Polytopes and Groups in order to create a 3D scene
* Dynamic Polytopes
* Linear transforms
* Get and Set data directly from GPU
* Textures
* Load 3D models
* Depth testing, blending, face culling...
* Anti aliasing
* Texture Renderer (capture FrameBuffer and create a texture)
* Blinn-Phong lighting (directional, point and spot)
* Shadow Mapping
* Gamma correction
* Emission
* Skybox
* Mouse ray casting
* Object selection

## Cube example

Simple rotating cube example:

```cpp
Window window("Cube example", 1280, 800);

Renderer renderer;

TrackballCamera camera = TrackballCamera::perspectiveCamera(glm::radians(45.0f), window.getWidth() / window.getHeight(), 0.1, 1000);
float sensitivity = 1.5f, panSensitivity = 1.0f, zoomSensitivity = 1.0f;
camera.zoom(-5.5);
renderer.setCamera(camera);

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

Polytope::Ptr polytopeCube = Polytope::New(cubeVertices, cubeIndices);

Group group;
group.setLineWidth(2.f);
group.translate(glm::vec3(-2.5, 0.5, 0));
group.add(polytopeCube);

renderer.addGroup(group);

while (!window.windowShouldClose()) {

    // Rotate cube
    polytopeCube->rotate(0.15, glm::vec3(1, 0, 1));

    // Clear
    renderer.clear();
    // Render
    renderer.render();
    // Swap buffers
    window.update();
}
```

# Screenshots

Loading a 3D model with a skybox and ilumination

![alt text](https://github.com/MorcilloSanz/RendererGL/blob/main/img/skybox.gif)  

Multiple point lights

![alt text](https://github.com/MorcilloSanz/RendererGL/blob/main/img/lighting.png)

Shadows

![alt text](https://github.com/MorcilloSanz/RendererGL/blob/main/img/shadows.png)

Emission

![alt text](https://github.com/MorcilloSanz/RendererGL/blob/main/img/emission.png)

Call of Duty Ghost dog

![alt text](https://github.com/MorcilloSanz/RendererGL/blob/main/img/2.png)

Call of Duty Ghost soldier

![alt text](https://github.com/MorcilloSanz/RendererGL/blob/main/img/1.png)

# Dependencies

* [GLFW](https://github.com/glfw/glfw)
* [GLEW](https://github.com/nigels-com/glew)
* [GLM](https://github.com/g-truc/glm)
* [IMGUI](https://github.com/ocornut/imgui)
* [ASSIMP](https://github.com/assimp/assimp)
* [STB](https://github.com/nothings/stb)