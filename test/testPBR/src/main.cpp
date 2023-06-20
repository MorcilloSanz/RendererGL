#include <iostream>
#include <vector>

#include <engine/renderer/Renderer.h>
#include <engine/renderer/TrackballCamera.h>

#include <GLFW/glfw3.h>

const int WIDTH = 1280;
const int HEIGHT = 900;
GLFWwindow* window;

Renderer::Ptr renderer;

Polytope::Ptr createSphere(int radius = 1, int latitudes = 64, int longitudes = 64) {

    Polytope::Ptr sphere = nullptr;

    std::vector<unsigned int> sphereIndices;

    if(longitudes < 3) longitudes = 3;
    if(latitudes < 2) latitudes = 2;

    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> uv;
    std::vector<unsigned int> indices;
    std::vector<Vec3f> vert;

    float nx, ny, nz, lengthInv = 1.0f / radius;

    struct Vertex {
        float x, y, z, s, t;
    };

    float deltaLatitude = M_PI / latitudes;
    float deltaLongitude = 2 * M_PI / longitudes;
    float latitudeAngle;
    float longitudeAngle;

    for (int i = 0; i <= latitudes; ++i) {
        
        latitudeAngle = M_PI / 2 - i * deltaLatitude;
        float xy = radius * cosf(latitudeAngle);
        float z = radius * sinf(latitudeAngle);

        for (int j = 0; j <= longitudes; ++j) {
            
            longitudeAngle = j * deltaLongitude;

            Vertex vertex;
            vertex.x = xy * cosf(longitudeAngle);
            vertex.y = xy * sinf(longitudeAngle);
            vertex.z = z;

            vertex.s = (float) j / longitudes;
            vertex.t = (float) i / latitudes;

            vertices.push_back(glm::vec3(vertex.x, vertex.y, vertex.z));
            uv.push_back(glm::vec2(vertex.s, vertex.t));

            nx = vertex.x * lengthInv;
            ny = vertex.y * lengthInv;
            nz = vertex.z * lengthInv;
            normals.push_back(glm::vec3(nx, ny, nz));

            Vec3f v(vertex.x, vertex.y, vertex.z, 1.0f, 1.0f, 1.0f, nx, ny, nz, vertex.s, vertex.t);
            vert.push_back(v);
        }
    }

    unsigned int k1, k2;
    for(int i = 0; i < latitudes; ++i) {

        k1 = i * (longitudes + 1);
        k2 = k1 + longitudes + 1;

        for(int j = 0; j < longitudes; ++j, ++k1, ++k2) {

            if (i != 0) {
                indices.push_back(k1 + 1);
                indices.push_back(k2);
                indices.push_back(k1);
            }

            if (i != (latitudes - 1)) {
                indices.push_back(k2 + 1);
                indices.push_back(k2);
                indices.push_back(k1 + 1);
            }
        }
    }

    sphere = Polytope::New(vert, indices);
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
    sphere->setFaceCulling(Polytope::FaceCulling::FRONT);
    sphere->scale(glm::vec3(1.5));

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

        renderer->setBackgroundColor(0.08, 0.08, 0.08);

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