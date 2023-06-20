#include <iostream>
#include <vector>

#include <engine/renderer/Renderer.h>
#include <engine/renderer/TrackballCamera.h>

#include <GLFW/glfw3.h>

const int WIDTH = 1280;
const int HEIGHT = 900;
GLFWwindow* window;

Renderer::Ptr renderer;

Polytope::Ptr createSphere() {

    Polytope::Ptr sphere;

    std::vector<Vec3f> vertices;
    std::vector<unsigned int> indices;

    const unsigned int X_SEGMENTS = 64;
    const unsigned int Y_SEGMENTS = 64;
    const float PI = 3.14159265359f;

    for (unsigned int x = 0; x <= X_SEGMENTS; ++x) {
        for (unsigned int y = 0; y <= Y_SEGMENTS; ++y) {

            float xSegment = (float)x / (float)X_SEGMENTS;
            float ySegment = (float)y / (float)Y_SEGMENTS;

            float xPos = std::cos(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
            float yPos = std::cos(ySegment * PI);
            float zPos = std::sin(xSegment * 2.0f * PI) * std::sin(ySegment * PI);

            // x y z  r g b nx ny nz tx ty
            Vec3f vertex(xPos, yPos, zPos, 1.0f, 1.0f, 1.0f, xPos, yPos, zPos, xSegment, ySegment);
            vertices.push_back(vertex);
        }
    }

    bool oddRow = false;
    for (unsigned int y = 0; y <=Y_SEGMENTS -1 ; ++y) {
        if (!oddRow) { // even rows: y == 0, y == 2; and so on
            for (unsigned int x = 0; x <= X_SEGMENTS; ++x) {
                indices.push_back(y * (X_SEGMENTS + 1) + x);
                indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
            }
        }
        else {
            for (int x = X_SEGMENTS; x > 0; --x) {
                indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
                indices.push_back(y * (X_SEGMENTS + 1) + x);
            }
        }
        oddRow = !oddRow;
    }

    int verticesSize = vertices.size();
    int indicesSize = indices.size();

    sphere = Polytope::New(vertices, indices);
    return sphere;
}

int main() {

    // Create window
    if (!glfwInit()) {
        std::cout << "Couldn't initialize window" << std::endl;
        return -1;
    }

    window = glfwCreateWindow(WIDTH, HEIGHT, "PBR example", NULL, NULL);
    if (!window) glfwTerminate();
    glfwMakeContextCurrent(window);

    // Renderer
    renderer = Renderer::New(WIDTH, HEIGHT);
    renderer->enablePBR();

    // Lighting
    PointLight light1(glm::vec3(-3, 0, 3));
    light1.setColor(glm::vec3(1, 0, 0));
    renderer->addLight(light1);

    DirectionalLight light2(glm::vec3(15));
    light2.setColor(glm::vec3(1));
    renderer->addLight(light2);

    // Camera
    double aspectRatio = static_cast<double>(WIDTH) / HEIGHT;
    TrackballCamera camera = TrackballCamera::perspectiveCamera(glm::radians(45.0f), aspectRatio, 0.1, 1000);
    camera.zoom(-5.5);
    renderer->setCamera(camera);

    // Polytope
    Polytope::Ptr sphere = createSphere();

    // PBR materials
    Texture::Ptr textureAlbedo = Texture::New("/home/morcillosanz/Desktop/model/rustediron1-alt2-Unreal-Engine/rustediron2_basecolor.png", Texture::Type::TextureAlbedo);
    Texture::Ptr textureMetallic = Texture::New("/home/morcillosanz/Desktop/model/rustediron1-alt2-Unreal-Engine/rustediron2_metallic.png", Texture::Type::TextureMetallic);
    Texture::Ptr textureNormal = Texture::New("/home/morcillosanz/Desktop/model/rustediron1-alt2-Unreal-Engine/Srustediron2_normal.png", Texture::Type::TextureNormal);
    Texture::Ptr textureRoughness = Texture::New("/home/morcillosanz/Desktop/model/rustediron1-alt2-Unreal-Engine/rustediron2_roughness.png", Texture::Type::TextureRoughness);

    sphere->addTexture(textureAlbedo);
    sphere->addTexture(textureMetallic);
    sphere->addTexture(textureNormal);
    sphere->addTexture(textureRoughness);

    // Group
    Group::Ptr group = Group::New();
    group->add(sphere);

    // Scene
    Scene::Ptr scene = Scene::New();
    scene->addGroup(group);
    renderer->addScene(scene);

    // Main loop
    while (!glfwWindowShouldClose(window)) {

        sphere->rotate(0.25, glm::vec3(1, 0, 1));

        // Draw scene
        renderer->clear();
        renderer->render();
        
        // Update window
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Destroy window
    glfwTerminate();

    return 0;
}