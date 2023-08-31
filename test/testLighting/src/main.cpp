#include <iostream>
#include <vector>

#include <engine/renderer/Renderer.h>
#include <engine/renderer/TrackballCamera.h>
#include <engine/shapes/Cube.h>

#include <GLFW/glfw3.h>

const int WIDTH = 1280;
const int HEIGHT = 900;
GLFWwindow* window;

Renderer::Ptr renderer;

int main() {

    // Create window
    if (!glfwInit()) {
        std::cout << "Couldn't initialize window" << std::endl;
        return -1;
    }

    window = glfwCreateWindow(WIDTH, HEIGHT, "Light example", NULL, NULL);
    
    if (!window) glfwTerminate();

    glfwMakeContextCurrent(window);

    // Renderer
    renderer = Renderer::New(WIDTH, HEIGHT);

    // Lighting
    renderer->enableLight();

    PointLight light1(glm::vec3(2, 0, 2));
    light1.setColor(glm::vec3(1, 0, 0));
    renderer->addLight(light1);

    DirectionalLight light2(glm::vec3(1));
    light2.setColor(glm::vec3(1));
    renderer->addLight(light2);

    // Camera
    double aspectRatio = static_cast<double>(WIDTH) / HEIGHT;
    TrackballCamera camera = TrackballCamera::perspectiveCamera(glm::radians(45.0f), aspectRatio, 0.1, 1000);
    camera.zoom(-2.5);

    renderer->setCamera(camera);

    // Scene
    Cube::Ptr cube = Cube::New();

    Group::Ptr group = Group::New();
    group->add(cube);

    Scene::Ptr scene = Scene::New();
    scene->addGroup(group);

    renderer->addScene(scene);

    // Main loop
    while (!glfwWindowShouldClose(window)) {

        // Update scene
        cube->rotate(0.55, glm::vec3(1, 0, 1));

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