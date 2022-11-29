#include <iostream>
#include <memory>

#include "engine/window/Window.h"

#include "engine/renderer/Renderer.h"
#include "engine/renderer/TrackballCamera.h"
#include "engine/renderer/FPSCamera.h"
#include "engine/renderer/TextureRenderer.h"
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
TextureRenderer textureRenderer;

// TrackBallCamera 
TrackballCamera camera;

// FPS camera
FPSCamera fpsCamera;
bool movementForward = false, movementBackward = false;
bool movementRight = false, movementLeft = false;

int main(void) {

    // Create window
    window = Window("RendererGL", 1080, 720);
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

    // Make vertices white in order to see the texture instead of an interpolation between the texture and the vertex color
    for(auto& vec : vertices)
        vec.r = vec.g = vec.b = 1;

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

    // 3D model from file
    Model model("/home/morcillosanz/Desktop/model/Bulbasaur/model.obj");
    model.setLineWidth(2.5f);
    model.translate(glm::vec3(0.0, 0.0, 1.5));
    //model.scale(glm::vec3(0.01, 0.01, 0.01));
    renderer.addGroup(model);

    // Init TextureRenderer
    textureRenderer = TextureRenderer(window.getWidth(), window.getHeight());

    // Enable Rendering Features
    renderer.enableBackFaceCulling();
    renderer.enableAntialiasing();
    renderer.enableBlending();

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

                ImGui::End();
            }
            // Render window
            static bool windowFocus = false;
            { 
                ImGui::Begin("Renderer", &p_open, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);       
                ImGui::Image((void*)(intptr_t)textureRenderer.getTexture(), ImGui::GetWindowSize());   // Render texture
                
                // If resize change texture viewport
                static ImVec2 previousSize(0, 0);
                if(ImGui::GetWindowSize().x != previousSize.x || ImGui::GetWindowSize().y != previousSize.y) {
                    textureRenderer.updateViewPort(window.getWidth(), window.getHeight());
                    // Restart trackball camera
                    float theta = camera.getTheta(), phi = camera.getPhi();
                    glm::vec3 center = camera.getCenter(), up = camera.getUp();
                    float radius = camera.getRadius();
                    camera = TrackballCamera::perspectiveCamera(glm::radians(45.0f), ImGui::GetWindowSize().x  / ImGui::GetWindowSize().y , 0.1, 1000);
                    camera.setTheta(theta);  camera.setPhi(phi);
                    camera.setCenter(center); camera.setUp(up);
                    camera.setRadius(radius);
                    // Restart fps camera
                    fpsCamera = FPSCamera::perspectiveCamera(glm::radians(45.0f), ImGui::GetWindowSize().x  / ImGui::GetWindowSize().y, 0.1, 1000);
                }
                previousSize = ImGui::GetWindowSize();

                windowFocus = ImGui::IsWindowFocused() || ImGui::IsWindowHovered();
                ImGui::End();
            }

            // Mouse events
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