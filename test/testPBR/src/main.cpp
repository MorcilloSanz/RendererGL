#include <iostream>
#include <vector>

#include <engine/renderer/Renderer.h>
#include <engine/renderer/TrackballCamera.h>
#include <engine/shapes/Sphere.h>

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

    window = glfwCreateWindow(WIDTH, HEIGHT, "PBR example", NULL, NULL);
    if (!window) glfwTerminate();
    glfwMakeContextCurrent(window);

    // Renderer
    renderer = Renderer::New(WIDTH, HEIGHT);
    renderer->enablePBR();

    // Lighting
    DirectionalLight light(glm::vec3(15));
    light.setColor(glm::vec3(0.25, 0.25, 1));
    renderer->addLight(light);

    DirectionalLight light2(glm::vec3(-15, 15, 15));
    light2.setColor(glm::vec3(1.0, 0.05, 0.05));
    renderer->addLight(light2);

    DirectionalLight light3(glm::vec3(0, 15, -1));
    light3.setColor(glm::vec3(0.45, 1.0, 0.45));
    renderer->addLight(light3);

    DirectionalLight light4(glm::vec3(0, -15, 0));
    light4.setColor(glm::vec3(1.0));
    renderer->addLight(light4);

    DirectionalLight light5(glm::vec3(15, 0, 0));
    light5.setColor(glm::vec3(1.0, 0.8f, 0.6f));
    renderer->addLight(light5);

    DirectionalLight light6(glm::vec3(8, 0, 0));
    light6.setColor(glm::vec3(1.0, 0.8f, 0.6f));
    renderer->addLight(light6);

    // Camera
    double aspectRatio = static_cast<double>(WIDTH) / HEIGHT;
    TrackballCamera::Ptr camera = TrackballCamera::perspectiveCamera(glm::radians(45.0f), aspectRatio, 0.1, 1000);
    camera->setPhi(M_PI / 3 - 0.1);
    camera->setTheta(M_PI / 4 - 0.1);
    camera->zoom(-6.5);
    renderer->setCamera(std::dynamic_pointer_cast<Camera>(camera));

    // Spheres
    Sphere::Ptr sphere = Sphere::New();
    sphere->setFaceCulling(Polytope::FaceCulling::FRONT);
    sphere->translate(glm::vec3(-3.0, 1.0, 0.0));

    Sphere::Ptr sphere2 = Sphere::New();
    sphere2->setFaceCulling(Polytope::FaceCulling::FRONT);
    sphere2->translate(glm::vec3(-1.0, 1.0, 0.0));

    Sphere::Ptr sphere3 = Sphere::New();
    sphere3->setFaceCulling(Polytope::FaceCulling::FRONT);
    sphere3->translate(glm::vec3(1.0, 1.0, 0.0));

    PBRMaterial::Ptr aoMaterial = PBRMaterial::New(glm::vec3(0, 0, 0), 0, 0, 2.5);
    sphere3->setMaterial(aoMaterial);

    // PBR materials
    Texture::Ptr textureAlbedo = Texture::New("/home/morcillosanz/Desktop/model/rustediron1-alt2-Unreal-Engine/rustediron2_basecolor.png", Texture::Type::TextureAlbedo);
    Texture::Ptr textureMetallic = Texture::New("/home/morcillosanz/Desktop/model/rustediron1-alt2-Unreal-Engine/rustediron2_metallic.png", Texture::Type::TextureMetallic);
    Texture::Ptr textureNormal = Texture::New("/home/morcillosanz/Desktop/model/rustediron1-alt2-Unreal-Engine/Srustediron2_normal.png", Texture::Type::TextureNormal);
    Texture::Ptr textureRoughness = Texture::New("/home/morcillosanz/Desktop/model/rustediron1-alt2-Unreal-Engine/rustediron2_roughness.png", Texture::Type::TextureRoughness);

    Texture::Ptr textureAlbedo2 = Texture::New("/home/morcillosanz/Desktop/model/painted-worn-asphalt-ue/painted-worn-asphalt_albedo.png", Texture::Type::TextureAlbedo);
    Texture::Ptr textureNormal2 = Texture::New("/home/morcillosanz/Desktop/model/painted-worn-asphalt-ue/painted-worn-asphalt_normal-dx.png", Texture::Type::TextureNormal);
    Texture::Ptr textureMetallic2 = Texture::New("/home/morcillosanz/Desktop/model/painted-worn-asphalt-ue/painted-worn-asphalt_metallic.png", Texture::Type::TextureMetallic);
    Texture::Ptr textureRoughness2 = Texture::New("/home/morcillosanz/Desktop/model/painted-worn-asphalt-ue/painted-worn-asphalt_roughness.png", Texture::Type::TextureRoughness);
    Texture::Ptr textureAmbientOcclusion2 = Texture::New("/home/morcillosanz/Desktop/model/painted-worn-asphalt-ue/painted-worn-asphalt_ao.png", Texture::Type::TextureAmbientOcclusion);
    Texture::Ptr textureHeight2 = Texture::New("/home/morcillosanz/Desktop/model/painted-worn-asphalt-ue/painted-worn-asphalt_height.png", Texture::Type::TextureHeight);

    Texture::Ptr textureAlbedo3 = Texture::New("/home/morcillosanz/Desktop/model/chipping-painted-wall-ue/chipping-painted-wall_albedo.png", Texture::Type::TextureAlbedo);
    Texture::Ptr textureMetallic3 = Texture::New("/home/morcillosanz/Desktop/model/chipping-painted-wall-ue/chipping-painted-wall_metallic.png", Texture::Type::TextureMetallic);
    Texture::Ptr textureNormal3 = Texture::New("/home/morcillosanz/Desktop/model/chipping-painted-wall-ue/chipping-painted-wall_normal-dx.png", Texture::Type::TextureNormal);
    Texture::Ptr textureRoughness3 = Texture::New("/home/morcillosanz/Desktop/model/chipping-painted-wall-ue/chipping-painted-wall_roughness.png", Texture::Type::TextureRoughness);

    sphere->addTexture(textureAlbedo);
    sphere->addTexture(textureMetallic);
    sphere->addTexture(textureNormal);
    sphere->addTexture(textureRoughness);

    sphere2->addTexture(textureAlbedo2);
    sphere2->addTexture(textureAmbientOcclusion2);
    sphere2->addTexture(textureNormal2);
    sphere2->addTexture(textureRoughness2);
    //sphere2->addTexture(textureHeight2);

    sphere3->addTexture(textureAlbedo3);
    sphere3->addTexture(textureMetallic3);
    sphere3->addTexture(textureNormal3);
    sphere3->addTexture(textureRoughness3);

    // Grid polytope
    std::vector<Vec3f> gridVertices = {};
    float a = -20; float b = -a;
    float c = -20; float d = -c;
    float dx = 0.5f; float dz = dx;
    while(a <= b) {
        gridVertices.push_back(Vec3f(a, 0, c, 0.3, 0.3, 0.3));
        gridVertices.push_back(Vec3f(a, 0, d, 0.3, 0.3, 0.3));
        a += dx;
    }
    a = -b;
    while(c <= d) {
        gridVertices.push_back(Vec3f(a, 0, c, 0.3, 0.3, 0.3));
        gridVertices.push_back(Vec3f(b, 0, c, 0.3, 0.3, 0.3));
        c += dz;
    }
    Polytope::Ptr gridPolytope = Polytope::New(gridVertices);

    PBRMaterial::Ptr pbrMaterial = PBRMaterial::New(glm::vec3(1.0), 0.2, 1.0, 1.0);
    gridPolytope->setMaterial(pbrMaterial);

    Group::Ptr groupGrid = Group::New(GL_LINES);
    groupGrid->setLineWidth(1.2f);
    groupGrid->add(gridPolytope);

    // Group
    Group::Ptr group = Group::New();
    group->add(sphere);
    group->add(sphere2);
    group->add(sphere3);

    group->translate(glm::vec3(0.75, 0, 0.5));
    
    // Scene
    Scene::Ptr scene = Scene::New();
    scene->addGroup(groupGrid);
    scene->addGroup(group);
    
    renderer->addScene(scene);

    // Main loop
    while (!glfwWindowShouldClose(window)) {

        sphere->rotate(0.25, glm::vec3(1, 0, 1));
        sphere2->rotate(0.25, glm::vec3(1, 0, 1));
        sphere3->rotate(0.25, glm::vec3(1, 0, 1));

        renderer->setBackgroundColor(0.1, 0.1, 0.1);

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