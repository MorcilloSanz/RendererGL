#include <iostream>
#include <vector>

#include <engine/renderer/Renderer.h>
#include <engine/renderer/TrackballCamera.h>
#include <engine/model/Model.h>
#include <engine/renderer/FrameCapturer.h>

#include <GLFW/glfw3.h>

#include "ImguiStyles.h"

// ImGui functions
void initImGui(ImGuiIO& io);
void dockSpace(bool* p_open);
void renderImGui(ImGuiIO& io);

const int WIDTH = 1280;
const int HEIGHT = 900;
GLFWwindow* window;

Renderer::Ptr renderer;
FrameCapturer::Ptr frameCapturer;

int main() {

    // Create window
    if (!glfwInit()) {
        std::cout << "Couldn't initialize window" << std::endl;
        return -1;
    }

    window = glfwCreateWindow(WIDTH, HEIGHT, "PBR example", NULL, NULL);
    if (!window) glfwTerminate();
    glfwMakeContextCurrent(window);

     // Init ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    initImGui(io);

    // Renderer
    renderer = Renderer::New(WIDTH, HEIGHT);
    renderer->enablePBR();

    // Lighting
    DirectionalLight light(glm::vec3(13, 15, 3));
    light.setColor(glm::vec3(0.25, 0.25, 1));
    renderer->addLight(light);

    DirectionalLight light2(glm::vec3(-13, 15, 3));
    light2.setColor(glm::vec3(1, 0.25, 0.25));
    renderer->addLight(light2);

    DirectionalLight light3(glm::vec3(-13, 13, 13));
    light3.setColor(glm::vec3(1));
    renderer->addLight(light3);

    // Camera
    double aspectRatio = static_cast<double>(WIDTH) / HEIGHT;
    TrackballCamera::Ptr camera = TrackballCamera::perspectiveCamera(glm::radians(45.0f), aspectRatio, 0.1, 1000);
    float sensitivity = 1.75f, panSensitivity = 1.0f, zoomSensitivity = 0.5f;
    camera->zoom(-2.5);
    renderer->setCamera(std::dynamic_pointer_cast<Camera>(camera));

    // Grid polytope
    float a = -20; float b = -a;
    float c = -20; float d = -c;

    float dx = 0.5f; float dz = dx;

    std::vector<Vec3f> gridVertices = {};
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

    // Model 
    Model::Ptr model = Model::New("/home/morcillosanz/Desktop/model/hand_sculpture/scene.gltf", true);
    Polytope::Ptr polytope = model->getPolytopes()[0];

    Texture::Ptr roughness = Texture::New("/home/morcillosanz/Desktop/model/hand-sculpture/source/hand_roughness.jpg", Texture::Type::TextureRoughness);
    Texture::Ptr normal = Texture::New("/home/morcillosanz/Desktop/model/hand-sculpture/source/hand_normal.jpg", Texture::Type::TextureNormal);
    Texture::Ptr ao = Texture::New("/home/morcillosanz/Desktop/model/hand-sculpture/source/hand_AO.jpg", Texture::Type::TextureAmbientOcclusion);

    polytope->addTexture(roughness);
    polytope->addTexture(normal);
    polytope->addTexture(ao);

    // Scene
    Scene::Ptr scene = Scene::New();
    scene->addGroup(groupGrid);
    scene->addModel(model);
    
    renderer->addScene(scene);

    // Init FrameCapturer
    frameCapturer = FrameCapturer::New(WIDTH, HEIGHT);

    // Main loop
    while (!glfwWindowShouldClose(window)) {

        frameCapturer->setBackgroundColor(0.1, 0.1, 0.1);

        // Clear
        renderer->clear();

        // Draw to texture instead of default
        frameCapturer->startCapturing();

        // Render
        renderer->render();

        // Go back to default
        frameCapturer->finishCapturing();

        {
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            bool p_open = true;
            dockSpace(&p_open);

            // Render window
            static bool windowFocus = false;
            { 
                ImGui::Begin("Renderer", &p_open, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
                windowFocus = ImGui::IsWindowFocused() || ImGui::IsWindowHovered();

                // Render graphics as a texture
                ImGui::Image((void*)(intptr_t)frameCapturer->getTexture()->getID(), ImGui::GetWindowSize());   
                
                // Resize window
                static ImVec2 previousSize(0, 0);
                if(ImGui::GetWindowSize().x != previousSize.x || ImGui::GetWindowSize().y != previousSize.y) {

                    // Restart trackball camera
                    float theta = camera->getTheta(), phi = camera->getPhi();
                    glm::vec3 center = camera->getCenter(), up = camera->getUp();
                    float radius = camera->getRadius();

                    // Update camera aspect ratio
                    *camera = *TrackballCamera::perspectiveCamera(glm::radians(45.0f), ImGui::GetWindowSize().x  / ImGui::GetWindowSize().y, 0.1, 1000);
                    camera->setTheta(theta);  camera->setPhi(phi);
                    camera->setCenter(center); camera->setUp(up);
                    camera->setRadius(radius);
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
                    camera->rotate(-dTheta * sensitivity, dPhi * sensitivity);
                }

                // Camera pan
                if(ImGui::IsMouseDragging(ImGuiMouseButton_Right) && windowFocus) {
                    float dx = (mousePositionRelative.x - previous.x) / (size.x / 2);
                    float dy = (mousePositionRelative.y - previous.y) / (size.y / 2);
                    previous = mousePositionRelative;
                    camera->pan(dx * panSensitivity, -dy * panSensitivity);
                }

                // Camera zoom
                if(windowFocus) camera->zoom(ImGui::GetIO().MouseWheel * zoomSensitivity);
               
                ImGui::End();
            }

            renderImGui(io);
        }

        // Update window
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Destroy imgui 
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    // Destroy window
    glfwTerminate();

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
    ImGui_ImplGlfw_InitForOpenGL(window, true);
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
    glfwGetFramebufferSize(window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        GLFWwindow* backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
    }
}