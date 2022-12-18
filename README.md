# RendererGL

RendererGL is a basic 3D engine written in C++ and OpenGL

The file:

```
main.cpp
```

is a template using ImGui and RendererGL

## Engine

Features:

* Create a window, handle its events and create an OpenGL context
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

To do:

* Shadows
* PBR (physically based rendering)
* Normal, Parallax Mapping
* HDR
* Bloom
* SSAO

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

Mouse Ray Casting (Intersection between ray and a mesh)

![alt text](https://github.com/MorcilloSanz/RendererGL/blob/main/img/mouseRayCasting.gif)  

Some cute 3D models:

![alt text](https://github.com/MorcilloSanz/RendererGL/blob/main/img/1.png)  

![alt text](https://github.com/MorcilloSanz/RendererGL/blob/main/img/2.png)  

![alt text](https://github.com/MorcilloSanz/RendererGL/blob/main/img/3.png)  

![alt text](https://github.com/MorcilloSanz/RendererGL/blob/main/img/4.png)  
