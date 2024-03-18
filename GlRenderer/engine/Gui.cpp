#include "Gui.h"
#include <spdlog/spdlog.h>
#include <iostream>
#include "gui/SceneViewport.h"

void AppDockSpace(bool* p_open)
{
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

    // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
    // because it would be confusing to have two docking targets within each others.
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
    ImGui::SetNextWindowViewport(viewport->ID);
    window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

    // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
    // and handle the pass-thru hole, so we ask Begin() to not render a background.
    if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
        window_flags |= ImGuiWindowFlags_NoBackground;

    // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
    // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
    // all active windows docked into it will lose their parent and become undocked.
    // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
    // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
    ImGui::Begin("DockSpace Demo", p_open, window_flags);

    // Submit the DockSpace
    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
    {
        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);

       
    }

    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("Options"))
        {
            // Disabling fullscreen would allow the window to be moved to the front of other windows,
            // which we can't undo at the moment without finer window depth/z control.
            ImGui::Separator();

            if (ImGui::MenuItem("Flag: NoSplit", "", (dockspace_flags & ImGuiDockNodeFlags_NoSplit) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoSplit; }
            if (ImGui::MenuItem("Flag: NoResize", "", (dockspace_flags & ImGuiDockNodeFlags_NoResize) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoResize; }
            if (ImGui::MenuItem("Flag: NoDockingInCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_NoDockingInCentralNode) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoDockingInCentralNode; }
            if (ImGui::MenuItem("Flag: AutoHideTabBar", "", (dockspace_flags & ImGuiDockNodeFlags_AutoHideTabBar) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_AutoHideTabBar; }
            ImGui::Separator();

            if (ImGui::MenuItem("Close", NULL, false, p_open != NULL))
                *p_open = false;
            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }

    ImGui::End();
}

static bool g_mouseCaptured = false;
static ImVec2 g_viewport_size;

static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (ImGui::GetCurrentContext() && ImGui::GetIO().WantCaptureMouse)
        return;

    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS && g_mouseCaptured) {
        // Release mouse capture
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        g_mouseCaptured = false;
    }
    else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS && !g_mouseCaptured) {
        // Capture mouse
        glfwSetCursorPos(window, g_viewport_size.x / 2.0f, g_viewport_size.y / 2.0f);
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        g_mouseCaptured = true;
    }
}

static void mouse_move_callback(GLFWwindow* window, double xpos, double ypos) {
    if (!g_mouseCaptured) {
        return;
    }

    glfwSetCursorPos(window, g_viewport_size.x / 2.0f, g_viewport_size.y / 2.0f);

    Camera* camera = static_cast<Camera*>(glfwGetWindowUserPointer(window));

    double xOffset = g_viewport_size.x / 2.0f - xpos;
    double yOffset = g_viewport_size.y / 2.0f - ypos;

    camera->rotate_camera(xOffset, yOffset);
}

Gui Gui::create(GLFWwindow* window, std::unique_ptr<Camera>& camera)
{
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    std::shared_ptr<Camera::CameraControls> camera_view_controls = camera->get_camera_controls();
    std::shared_ptr<Camera::ProjectionControls> camera_proj_controls = camera->get_proj_controls();

    glfwSetWindowUserPointer(window, camera.get());

    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, mouse_move_callback);

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 460");
    return Gui(M{ window, camera_view_controls, camera_proj_controls });
}

void Gui::invoke_start() const
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    bool options_opened = true;
    AppDockSpace(&options_opened);
    draw_camera_controls_window();
}

void Gui::draw_camera_controls_window() const
{
    ImGui::Begin("Camera Controls");
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
        1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::SliderFloat("Camera Speed", &m.camera_view_controls->movementSpeed, 0.1f, 5.0f);
    ImGui::SliderFloat("Camera Sensitivity", &m.camera_view_controls->viewSensitivity, 0.01f, 0.3f);
    ImGui::SliderFloat("FOV", &m.camera_proj_controls->fov, 30.f, 120.f);
    if (ImGui::IsItemEdited()) {
        Camera* camera = static_cast<Camera*>(glfwGetWindowUserPointer(m.window));
        camera->update_projection(g_viewport_size);
    }
    ImGui::SliderFloat("Render Distance", &m.camera_proj_controls->farPlane, 100.f, 500.f);
    if (ImGui::IsItemEdited()) {
        Camera* camera = static_cast<Camera*>(glfwGetWindowUserPointer(m.window));
        camera->update_projection(g_viewport_size);
    }
    ImGui::End();
}

void Gui::render_scene(const uint32_t framebuffer_texture, std::unique_ptr<Camera>& camera) const
{
    ImVec2 window_size = SceneViewport::draw_window_scene(reinterpret_cast<ImTextureID>(framebuffer_texture));

    if (window_size.x != g_viewport_size.x || window_size.y != g_viewport_size.y) {
        camera->update_projection(window_size);
        g_viewport_size.x = window_size.x;
        g_viewport_size.y = window_size.y;
        spdlog::info("{} {}", g_viewport_size.x, g_viewport_size.y);
    }

    if (g_mouseCaptured) {
        camera->process_input(m.window);
    }
}

void Gui::invoke_end() const
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Gui::destroy() const
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

Gui::Gui(WithResultOf&& res)
{
    auto other = res.func();

    m = std::move(other.m);
}
