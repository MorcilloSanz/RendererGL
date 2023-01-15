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
* Blinn-Phong lighting
* Gamma correction
* Skybox
* Mouse ray casting
* Object selection

# Dependencies
```
GLFW
GLEW    (included)
GLM     (included)
IMGUI   (included)
ASSIMP  (only for reading 3D models)
```

# Screenshots

Loading a 3D model with a skybox and ilumination

![alt text](https://github.com/MorcilloSanz/RendererGL/blob/main/img/skybox.gif)  

Blinn-Phong Lighting and Gamma Correction

![alt text](https://github.com/MorcilloSanz/RendererGL/blob/main/img/lighting.gif)  

Mouse Ray Casting

![alt text](https://github.com/MorcilloSanz/RendererGL/blob/main/img/mouseRayCasting.png)  

Screenshots

![alt text](https://github.com/MorcilloSanz/RendererGL/blob/main/img/1.png)  

![alt text](https://github.com/MorcilloSanz/RendererGL/blob/main/img/2.png)  

![alt text](https://github.com/MorcilloSanz/RendererGL/blob/main/img/3.png)  