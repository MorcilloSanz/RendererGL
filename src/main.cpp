#include <iostream>
#include <memory>

#include "engine/window/Window.h"

#include "engine/renderer/Renderer.h"
#include "engine/renderer/TrackballCamera.h"
#include "engine/renderer/FPSCamera.h"
#include "engine/renderer/TextureRenderer.h"
#include "engine/renderer/SkyBox.h"
#include "engine/renderer/MouseRayCasting.h"
#include "engine/model/Model.h"

#include "ImguiStyles.h"

// ImGui functions
void initImGui(ImGuiIO& io);
void dockSpace(bool* p_open);
void renderImGui(ImGuiIO& io);

// Window callbacks
void resizeFun(GLFWwindow* w, int width, int height);
void keyFun(GLFWwindow* window, int key, int scancode, int action, int mods);

// Camera functions
void updateFPSCamera(double xpos, double ypos);

// Window
Window window;

// Texture renderer
TextureRenderer textureRenderer; // DO NOT INSTANCIATE AGAIN, that would destroy the current object and delete TextureRenderer's buffers which ends up in OpenGL errors

// TrackBallCamera 
TrackballCamera camera;

// FPS camera
FPSCamera fpsCamera;
bool movementForward = false, movementBackward = false;
bool movementRight = false, movementLeft = false;

// Mouse Ray casting (gui)
bool enablePoint3d = false, enableDrawRay;
float rayLong = 100;

int main(void) {

    // Create window
    window = Window("RendererGL", 1280, 800);
    window.setResizeFun(resizeFun);
    window.setKeyFun(keyFun);

    // Init ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    initImGui(io);

    // Renderer
    Renderer renderer;

    // Trackball Camera
    camera = TrackballCamera::perspectiveCamera(glm::radians(45.0f), window.getWidth() / window.getHeight(), 0.1, 1000);
    float sensitivity = 1.5f, panSensitivity = 1.0f, zoomSensitivity = 1.0f;
    camera.zoom(-5.5);
    renderer.setCamera(camera);

    // First Person Shooter Camera
    fpsCamera = FPSCamera::perspectiveCamera(glm::radians(45.0f), window.getWidth() / window.getHeight(), 0.1, 1000);
    fpsCamera.setSensitivity(sensitivity / 10);

    // Light
    Light light(glm::vec3(2.743, 6.118, 16.245));
    renderer.setLight(light);
    renderer.disableLight();
    
    // Cube polytope -> Vertex: x y z r g b nx ny nz tx ty
    std::vector<Vec3f> vertices = {
        // Back face
        Vec3f(-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 1.0f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f), // Bottom-left
        Vec3f( 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f), // top-right
        Vec3f( 0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 1.0f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f), // bottom-right         
        Vec3f( 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f), // top-right
        Vec3f(-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 1.0f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f), // bottom-left
        Vec3f(-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 1.0f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f), // top-left
        // Front face
        Vec3f(-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f), // bottom-left
        Vec3f( 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 1.0f,  0.0f,  0.0f,  1.0f,  1.0f, 0.0f), // bottom-right
        Vec3f( 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f), // top-right
        Vec3f( 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f), // top-right
        Vec3f(-0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 1.0f,  0.0f,  0.0f,  1.0f,  0.0f, 1.0f), // top-left
        Vec3f(-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f), // bottom-left
        // Left face
        Vec3f(-0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f,  -1.0f,  0.0f,  0.0f, 0.0f, 1.0f), // top-right
        Vec3f(-0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  -1.0f,  0.0f,  0.0f, 0.0f, 0.0f), // top-left
        Vec3f(-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f,  -1.0f,  0.0f,  0.0f, 1.0f, 0.0f), // bottom-left
        Vec3f(-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f,  -1.0f,  0.0f,  0.0f, 1.0f, 0.0f), // bottom-left
        Vec3f(-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  -1.0f,  0.0f,  0.0f, 1.0f, 1.0f), // bottom-right
        Vec3f(-0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f,  -1.0f,  0.0f,  0.0f, 0.0f, 1.0f), // top-right
        // Right face
        Vec3f( 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f), // top-left
        Vec3f( 0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f), // bottom-right
        Vec3f( 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f), // top-right         
        Vec3f( 0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f), // bottom-right
        Vec3f( 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f), // top-left
        Vec3f( 0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f), // bottom-left     
        // Bottom face
        Vec3f(-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f), // top-right
        Vec3f( 0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f), // top-left
        Vec3f( 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 1.0f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f), // bottom-left
        Vec3f( 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 1.0f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f), // bottom-left
        Vec3f(-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f), // bottom-right
        Vec3f(-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f), // top-right
        // Top face
        Vec3f(-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 1.0f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f), // top-left
        Vec3f( 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f), // bottom-right
        Vec3f( 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f), // top-right     
        Vec3f( 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f), // bottom-right
        Vec3f(-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 1.0f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f), // top-left
        Vec3f(-0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f) // bottom-left
    };

    std::shared_ptr<Polytope> cubePolytope = std::make_shared<Polytope>(vertices);

    std::vector<Vec3f> cubeVertices {
        // Front square
        Vec3f(-0.5, -0.5,  0.5,  0.0f, 0.0f, 1.0f),
        Vec3f( 0.5, -0.5,  0.5,  1.0f, 0.0f, 1.0f),
        Vec3f( 0.5,  0.5,  0.5,  0.0f, 1.0f, 1.0f),
        Vec3f(-0.5,  0.5,  0.5,  0.0f, 1.0f, 0.5f),
        // Back square
        Vec3f(-0.5, -0.5, -0.5,  0.0f, 0.0f, 1.0f),
        Vec3f( 0.5, -0.5, -0.5,  1.0f, 0.0f, 1.0f),
        Vec3f( 0.5,  0.5, -0.5,  0.0f, 1.0f, 1.0f),
        Vec3f(-0.5,  0.5, -0.5,  0.0f, 1.0f, 0.5f)
    };

    std::vector<unsigned int> cubeIndices {
        //front   //right   //back
		0, 1, 2,  1, 5, 6,  7, 6, 5,
		2, 3, 0,  6, 2, 1,  5, 4, 7,
		//left    //bottom  //top
		4, 0, 3,  4, 5, 1,  3, 2, 6,
		3, 7, 4,  1, 0, 4,  6, 7, 3 
    };

    std::shared_ptr<Polytope> cubePolytopeIndices = std::make_shared<Polytope>(cubeVertices, cubeIndices);
    cubePolytopeIndices->translate(glm::vec3(1.5, 0, 0));

    // Make vertices white in order to see the texture instead of an interpolation between the texture and the vertex color in cubePolytope2
    for(auto& vec : vertices)
        vec.r = vec.g = vec.b = 1;

    // Cube Polytope 2 
    std::shared_ptr<Polytope> cubePolytope2 = std::make_shared<Polytope>(vertices);
    cubePolytope2->translate(glm::vec3(0.f, 1.5f, 0.f));
    cubePolytope2->rotate(45, glm::vec3(0, 0, 1));
    cubePolytope2->scale(glm::vec3(0.5, 0.5, 0.5));
    std::shared_ptr<Texture> texture = std::make_shared<Texture>("/home/morcillosanz/Desktop/model/texture2.png");
    cubePolytope2->addTexture(texture); // vertices2's colors are all white, thats why the texture looks like texture2.png

    // Cubes group
    Group group;
    group.setLineWidth(2.f);
    group.translate(glm::vec3(0, 0.5, 0));
    group.add(cubePolytope);
    group.add(cubePolytopeIndices);
    group.add(cubePolytope2);
    renderer.addGroup(group);

    // Grid polytope
    std::vector<Vec3f> gridVertices = {};
    float a = -30; float b = -a;
    float c = -30; float d = -c;
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
    std::shared_ptr<Polytope> gridPolytope = std::make_shared<Polytope>(gridVertices);

    Group groupGrid(GL_LINES);
    groupGrid.add(gridPolytope);
    renderer.addGroup(groupGrid);

    // Dynamic Polytope
    size_t length = 5000;
    std::shared_ptr<DynamicPolytope> dynamicPolytope = std::make_shared<DynamicPolytope>(length);

    Group groupDynamic(GL_LINE_STRIP);
    groupDynamic.setLineWidth(2.f);
    groupDynamic.translate(glm::vec3(-1.5, 0, 0));
    groupDynamic.add(dynamicPolytope);
    renderer.addGroup(groupDynamic);

    // Dynamic Polytope for mouse picking (ray casting)
    std::shared_ptr<DynamicPolytope> mousePickingPolytope = std::make_shared<DynamicPolytope>(length);
    Group groupMousePicking(GL_POINTS);
    groupMousePicking.setPointSize(8.f);
    groupMousePicking.add(mousePickingPolytope);
    renderer.addGroup(groupMousePicking);

    // Dynamic Polytope for ray casting drawing
    std::shared_ptr<DynamicPolytope> raysPolytope = std::make_shared<DynamicPolytope>(length);
    Group raysGroup(GL_LINES);
    raysGroup.setLineWidth(2.f);
    raysGroup.add(raysPolytope);
    renderer.addGroup(raysGroup);

    // 3D model from file
    Model model("/home/morcillosanz/Desktop/model/Bulbasaur/model.obj");
    model.setLineWidth(2.5f);
    model.translate(glm::vec3(0.0, 0.0, 1.5));
    //model.scale(glm::vec3(0.01, 0.01, 0.01));
    renderer.addGroup(model);

    // SkyBox
    std::vector<std::string> faces = {
        "/home/morcillosanz/Desktop/Yokohama3/posx.jpg",
        "/home/morcillosanz/Desktop/Yokohama3/negx.jpg",
        "/home/morcillosanz/Desktop/Yokohama3/posy.jpg",
        "/home/morcillosanz/Desktop/Yokohama3/negy.jpg",
        "/home/morcillosanz/Desktop/Yokohama3/posz.jpg",
        "/home/morcillosanz/Desktop/Yokohama3/negz.jpg"
    };
    std::shared_ptr<SkyBox> skyBox = std::make_shared<SkyBox>(faces);
    renderer.setSkyBox(skyBox);

    // Init TextureRenderer
    textureRenderer.updateViewPort(window.getWidth(), window.getHeight());

    // Enable Rendering Features
    renderer.enableBackFaceCulling();

    // Get First Vertex from cubePolytopeIndices
    Vec3f firstVertex = cubePolytopeIndices->getVertices()[0];

    // Main loop
    while (!window.windowShouldClose()) {

        // Clear
        renderer.clear();
        // Draw to texture instead of default
        textureRenderer.renderToTexture();
        // Render
        renderer.render();
        // Go back to default
        textureRenderer.renderToDefault();

        // Rotate cubePolytope2
        cubePolytope2->rotate(1, glm::vec3(1, 0, 1));

        // Update vertex from cubePolytope
        cubePolytopeIndices->updateVertex(0, firstVertex);
        firstVertex.z += 0.001;

        // Add vertices to dynamicPolytope
        {
            static size_t numVertices = 0;

            static float radius = 0.5f;
            static float theta = 0.0f;
            static float height = 0.1f;

            const float dTheta = 0.01f;
            const float dZ = 0.001f;
            
            if(numVertices < dynamicPolytope->getVertexLength()) {

                float x = radius * cos(theta);
                float y = height;
                float z = radius * sin(theta);

                float r = (float)numVertices / dynamicPolytope->getVertexLength();
                float g = 1 - r;
                float b = 0.5f;

                dynamicPolytope->addVertex(Vec3f(x, y, z, r, g, b));

                theta += dTheta;
                height += dZ;
                numVertices ++;
            }
        }

        // ImGUI
        {
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            bool p_open = true;
            dockSpace(&p_open);

            // Window
            {
                static float f = 0.0f;
                static int counter = 0;
                ImGui::Begin("Main Window");                         
                ImGui::TextColored(ImColor(200, 150, 255), "Main window controls");

                ImGui::Separator();

                if (ImGui::Button("Visible")) group.setVisible(!group.isVisible());
                ImGui::SameLine();
                if (ImGui::Button("Show wire")) {
                    group.setShowWire(!group.isShowWire());
                    model.setShowWire(!model.isShowWire());
                }
                ImGui::Separator();

                glm::vec3 backgroundColor = textureRenderer.getBackgroundColor();
                static float color[3] = {backgroundColor.r, backgroundColor.g, backgroundColor.b};
                ImGui::ColorEdit3("Background color", color, 0);
                textureRenderer.setBackgroundColor(color[0], color[1], color[2]);

                ImGui::Separator();

                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
                ImGui::End();
            }

            // Lighting window
            {
                ImGui::Begin("Lighting"); 
                ImGui::TextColored(ImColor(200, 150, 255), "Light configuration");
                ImGui::Text("Controls for your custom application");

                static bool enable = false;
                ImGui::Checkbox("Enable lighting", &enable);
                renderer.setLightEnabled(enable);

                static float ambientStrength = light.getAmbientStrength();
                ImGui::SliderFloat("Ambient strength", &ambientStrength, 0.f, 1.f);
                light.setAmbientStrength(ambientStrength);

                static float diffuseStrength = light.getDiffuseStrength();
                ImGui::SliderFloat("Diffuse strength", &diffuseStrength, 0.f, 1.f);
                light.setDiffuseStrength(diffuseStrength);

                static float specularStrength = light.getSpecularStrength();
                ImGui::SliderFloat("Specular strength", &specularStrength, 0.f, 1.f);
                light.setSpecularStrength(specularStrength);

                static float color[3] = {1, 1, 1};
                ImGui::ColorEdit3("Light color", color, 0);
                light.setColor(glm::vec3(color[0], color[1], color[2]));

                static float lx = light.getPosition().x;
                static float ly = light.getPosition().y;
                static float lz = light.getPosition().z;

                ImGui::Text("Light position");
                ImGui::SliderFloat("x:", &lx, -50.f, 50.f);
                ImGui::SliderFloat("y:", &ly, -50.f, 50.f);
                ImGui::SliderFloat("z:", &lz, -50.f, 50.f);
                light.setPosition(glm::vec3(lx, ly, lz));

                ImGui::Separator();

                if (ImGui::Button("Reset lighting")) {
                    lx = 2; ly = -8; lz = 5;
                    ambientStrength = 0.5f;
                    diffuseStrength = 0.5f;
                    specularStrength = 0.5f;
                }

                ImGui::End();
            }

            // Camera Window
            {
                ImGui::Begin("Camera");       

                ImGui::TextColored(ImColor(200, 150, 255), "Camera options");
                ImGui::Text("Camera sensitivity");

                ImGui::Separator();

                ImGui::SliderFloat("Sensitivity", &sensitivity, 0.01f, 50.f);
                ImGui::SliderFloat("Pan sensitivity", &panSensitivity, 0.01f, 50.f);
                ImGui::SliderFloat("Zoom sensitivity", &zoomSensitivity, 0.01f, 50.f);

                fpsCamera.setSensitivity(sensitivity / 10);

                ImGui::Separator();

                ImGui::Text("Camera rotation angles");
                float theta = camera.getTheta(), phi = camera.getPhi();
                ImGui::SliderFloat("Theta", &theta, 0, M_PI);
                ImGui::SliderFloat("Phi", &phi, 0, 2 * M_PI);
                camera.setTheta(theta);
                camera.setPhi(phi);

                ImGui::Separator();

                if (ImGui::Button("Reset camera")) {
                    camera.setTheta(M_PI_2); 
                    camera.setPhi(2 * M_PI);
                    camera.setRadius(5.5f);
                    camera.setCenter(glm::vec3(0, 0, 0));
                    camera.setUp(glm::vec3(0, 1, 0));
                    sensitivity = 1.5f;
                    panSensitivity = 1.0f;
                    zoomSensitivity = 1.0f;

                    fpsCamera.setEye(glm::vec3(0, 0, -1));
                    fpsCamera.setUp(glm::vec3(0, -1, 0));
                    fpsCamera.setCenter(glm::vec3(0, 0, 0));
                }
                ImGui::SameLine();
                if (ImGui::Button("Trackball camera")) {
                    renderer.setCamera(camera);
                }
                ImGui::SameLine();
                if (ImGui::Button("FPS camera")) {
                    renderer.setCamera(fpsCamera);
                    glfwSetInputMode(window.getGLFWwindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                }

                ImGui::Separator();

                ImGui::TextColored(ImColor(200, 150, 255), "Mouse Ray Casting");

                ImGui::Checkbox("Enable 3D Point", &enablePoint3d);
                ImGui::SameLine();
                ImGui::Checkbox("Enable Drawing Ray", &enableDrawRay);
                ImGui::SliderFloat("Ray long", &rayLong, 0.5f, 1500);

                ImGui::End();
            }
            // Render window
            static bool windowFocus = false;
            { 
                ImGui::Begin("Renderer", &p_open, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
                windowFocus = ImGui::IsWindowFocused() || ImGui::IsWindowHovered();

                // Render graphics as a texture
                ImGui::Image((void*)(intptr_t)textureRenderer.getTexture(), ImGui::GetWindowSize());   
                
                // Resize window
                static ImVec2 previousSize(0, 0);
                if(ImGui::GetWindowSize().x != previousSize.x || ImGui::GetWindowSize().y != previousSize.y) {
                    // Restart trackball camera
                    float theta = camera.getTheta(), phi = camera.getPhi();
                    glm::vec3 center = camera.getCenter(), up = camera.getUp();
                    float radius = camera.getRadius();
                    // Update camera aspect ratio
                    camera = TrackballCamera::perspectiveCamera(glm::radians(45.0f), ImGui::GetWindowSize().x  / ImGui::GetWindowSize().y, 0.1, 1000);
                    camera.setTheta(theta);  camera.setPhi(phi);
                    camera.setCenter(center); camera.setUp(up);
                    camera.setRadius(radius);
                    // Restart fps camera
                    fpsCamera = FPSCamera::perspectiveCamera(glm::radians(45.0f), ImGui::GetWindowSize().x  / ImGui::GetWindowSize().y, 0.1, 1000);
                }
                previousSize = ImGui::GetWindowSize();

                // Mouse Events
                ImVec2 size = ImGui::GetWindowSize();
                ImVec2 mousePositionAbsolute = ImGui::GetMousePos();
                ImVec2 screenPositionAbsolute = ImGui::GetItemRectMin();
                ImVec2 mousePositionRelative = ImVec2(mousePositionAbsolute.x - screenPositionAbsolute.x, mousePositionAbsolute.y - screenPositionAbsolute.y);

                static bool first = true;
                static ImVec2 previous(0, 0);

                if(ImGui::IsMouseDown(ImGuiMouseButton_Left) || ImGui::IsMouseDown(ImGuiMouseButton_Right)) {
                    if(first) {
                        previous = mousePositionRelative;
                        first = false;
                    }
                }else first = true;

                // Camera rotation
                if(ImGui::IsMouseDragging(ImGuiMouseButton_Left) && windowFocus) {
                    float dTheta = (mousePositionRelative.x - previous.x) / size.x;
                    float dPhi = (mousePositionRelative.y - previous.y) / size.y;
                    previous = mousePositionRelative;
                    camera.rotate(-dTheta * sensitivity, dPhi * sensitivity);
                }

                // Camera pan
                if(ImGui::IsMouseDragging(ImGuiMouseButton_Right) && windowFocus) {
                    float dx = (mousePositionRelative.x - previous.x) / (size.x / 2);
                    float dy = (mousePositionRelative.y - previous.y) / (size.y / 2);
                    previous = mousePositionRelative;
                    camera.pan(dx * panSensitivity, -dy * panSensitivity);
                }

                // Camera zoom
                if(windowFocus) camera.zoom(ImGui::GetIO().MouseWheel * zoomSensitivity);

                // FPS Camera
                updateFPSCamera(mousePositionRelative.x, mousePositionRelative.y);

                // Mouse Picking
                if(ImGui::IsMouseClicked(ImGuiMouseButton_Left) && (enablePoint3d || enableDrawRay) && windowFocus) {

                    MouseRayCasting mouseRayCasting(camera, ImGui::GetWindowSize().x, ImGui::GetWindowSize().y);
                    MouseRayCasting::Ray mouseRay = mouseRayCasting.getRay(mousePositionRelative.x, mousePositionRelative.y);

                    if(enablePoint3d) {
                        // The projection of this point belongs to the Screen Plane (X, Y)
                        glm::vec3 screenProjectedPoint = mouseRay.getScreenProjectedPoint();
                        Vec3f point3D = Vec3f(screenProjectedPoint.x, screenProjectedPoint.y, screenProjectedPoint.z, 1, 0, 0);
                        mousePickingPolytope->addVertex(point3D);
                    }

                    if(enableDrawRay) {
                        // Get the points of the ray from the screen to 'rayLong' distance
                        glm::vec3 begin = mouseRay.getPoint(rayLong);
                        glm::vec3 end = mouseRay.getPoint(1);
                        // Add these two vertices into the GL_LINES dynamic polytope
                        Vec3f vertex1(begin.x, begin.y, begin.z, 0, 1, 0);
                        Vec3f vertex2(end.x, end.y, end.z, 0, 0, 1);
                        raysPolytope->addVertex(vertex1);
                        raysPolytope->addVertex(vertex2);
                    }
                }

                ImGui::End();
            }
            
            // Rendering
            renderImGui(io);
        }

        window.update();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    window.terminate();
    return 0;
}

// ImGui functions

void initImGui(ImGuiIO& io) {
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows

    //ImGui::StyleColorsDark();
    Style();

    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    const char* glsl_version = "#version 130";
    ImGui_ImplGlfw_InitForOpenGL(window.getGLFWwindow(), true);
    ImGui_ImplOpenGL3_Init(glsl_version);
}

void dockSpace(bool* p_open) {

    static bool opt_fullscreen = true;
    static bool opt_padding = false;
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

    // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
    // because it would be confusing to have two docking targets within each others.
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    if (opt_fullscreen)
    {
        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    }
    else dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;


    // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
    // and handle the pass-thru hole, so we ask Begin() to not render a background.
    if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
        window_flags |= ImGuiWindowFlags_NoBackground;

    // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
    // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
    // all active windows docked into it will lose their parent and become undocked.
    // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
    // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
    if (!opt_padding) ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("DockSpace", p_open, window_flags);
    if (!opt_padding) ImGui::PopStyleVar();

    if (opt_fullscreen) ImGui::PopStyleVar(2);

    // Submit the DockSpace
    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    }

    if (ImGui::BeginMenuBar()) {
        if (ImGui::BeginMenu("Options")) {
            if (ImGui::MenuItem("Close", NULL, false, p_open != NULL)) exit(0);
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }

    ImGui::End();
}

void renderImGui(ImGuiIO& io) {
    ImGui::Render();
    int display_w, display_h;
    glfwGetFramebufferSize(window.getGLFWwindow(), &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        GLFWwindow* backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
    }
}

// Window functions

void resizeFun(GLFWwindow* w, int width, int height) {
    textureRenderer.updateViewPort(width, height);
    window.setWidth(width);
    window.setHeight(height);
}

void keyFun(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_W && action == GLFW_PRESS)          movementForward = true;
    else if(key == GLFW_KEY_W && action == GLFW_RELEASE)    movementForward = false;
    if (key == GLFW_KEY_S && action == GLFW_PRESS)          movementBackward = true;
    else if(key == GLFW_KEY_S && action == GLFW_RELEASE)    movementBackward = false;
    if (key == GLFW_KEY_A && action == GLFW_PRESS)          movementLeft = true;
    else if(key == GLFW_KEY_A && action == GLFW_RELEASE)    movementLeft = false;
    if (key == GLFW_KEY_D && action == GLFW_PRESS)          movementRight = true;
    else if (key == GLFW_KEY_D && action == GLFW_RELEASE)   movementRight = false;
    if(key == GLFW_KEY_ESCAPE) glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void updateFPSCamera(double xpos, double ypos) {
    // Look around
    fpsCamera.lookAround(xpos, ypos);
    // Movement
    if(movementForward) fpsCamera.move(FPSCamera::Movement::Forward);
    if(movementBackward) fpsCamera.move(FPSCamera::Movement::Backward);
    if(movementRight) fpsCamera.move(FPSCamera::Movement::Right);
    if(movementLeft) fpsCamera.move(FPSCamera::Movement::Left);
}