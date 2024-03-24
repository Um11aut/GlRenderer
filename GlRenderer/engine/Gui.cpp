#include "Gui.h"
#include <spdlog/spdlog.h>
#include <iostream>
#include <format>
#include "gui/SceneViewport.h"

static bool g_mouseCaptured = false;
static std::shared_ptr<ImVec2> g_viewport_size;
static ImVec2 g_viewport_pos;

static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (ImGui::GetCurrentContext() && ImGui::GetIO().WantCaptureMouse)
        return;

    // Check if the Alt key is pressed (GLFW_MOD_ALT is equivalent to GLFW_MOD_ALT)
    if ((mods & GLFW_MOD_ALT) && button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS && !g_mouseCaptured) {
        // Capture mouse
        glfwSetCursorPos(window, g_viewport_size->x / 2.0f + g_viewport_pos.x, g_viewport_size->y / 2.0f + g_viewport_pos.y);
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        g_mouseCaptured = true;
    }
    else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS && g_mouseCaptured) {
        // Release mouse capture
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        g_mouseCaptured = false;
    }
}

static void mouse_move_callback(GLFWwindow* window, double xpos, double ypos) {
    if (!g_mouseCaptured) {
        return;
    }

    ImVec2 scene_center(g_viewport_size->x / 2.0f + g_viewport_pos.x, g_viewport_size->y / 2.0f + g_viewport_pos.y);

    glfwSetCursorPos(window, scene_center.x, scene_center.y);

    Camera* camera = static_cast<Camera*>(glfwGetWindowUserPointer(window));

    double xOffset = scene_center.x - xpos;
    double yOffset = scene_center.y - ypos;

    camera->rotate_camera(xOffset, yOffset);
}

Gui Gui::create(GLFWwindow* window, std::unique_ptr<Camera>& camera)
{
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.Fonts->AddFontFromFileTTF("resources/font/Poppins.ttf", 16.0f);

    g_viewport_size = std::make_shared<ImVec2>(1920,1080);

    std::shared_ptr<ImVec2> scene_viewport_size = g_viewport_size;

    Gui::enable_dark_theme();

    glfwSetWindowUserPointer(window, camera.get());

    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, mouse_move_callback);

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 460");
    return Gui(M{ window, camera->get_camera_controls(), camera->get_proj_controls(), camera->get_view_controls(), scene_viewport_size });
}

void Gui::invoke_start() const
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void Gui::draw_camera_controls_window() const
{
    ImGui::Begin("Camera Controls");
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
        1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    glm::vec3 camera_position = m.camera_view_controls->cameraPosition;
    ImGui::Text("Camera Position. X: %.3f Y: %.3f Z: %.3f", camera_position.x, camera_position.y, camera_position.z);
    ImGui::SliderFloat("Camera Speed", &m.camera_controls->movementSpeed, 0.1f, 5.0f);
    ImGui::SliderFloat("Camera Sensitivity", &m.camera_controls->viewSensitivity, 0.01f, 0.3f);
    ImGui::SliderFloat("FOV", &m.camera_proj_controls->fov, 30.f, 120.f);
    if (ImGui::IsItemEdited()) {
        Camera* camera = static_cast<Camera*>(glfwGetWindowUserPointer(m.window));
        camera->update_projection(*g_viewport_size);
    }
    ImGui::SliderFloat("Render Distance", &m.camera_proj_controls->farPlane, 100.f, 500.f);
    if (ImGui::IsItemEdited()) {
        Camera* camera = static_cast<Camera*>(glfwGetWindowUserPointer(m.window));
        camera->update_projection(*g_viewport_size);
    }
    ImGui::SliderFloat("Camera Acceleration", &m.camera_controls->cameraAcceleration, 0.05f, 0.3f);
    ImGui::End();
}

void Gui::enable_dark_theme()
{
    ImVec4* colors = ImGui::GetStyle().Colors;
    colors[ImGuiCol_Text]                   = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    colors[ImGuiCol_TextDisabled]           = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
    colors[ImGuiCol_WindowBg]               = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
    colors[ImGuiCol_ChildBg]                = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_Border]                 = ImVec4(0.19f, 0.19f, 0.19f, 0.29f);
    colors[ImGuiCol_BorderShadow]           = ImVec4(0.00f, 0.00f, 0.00f, 0.24f);
    colors[ImGuiCol_FrameBg]                = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
    colors[ImGuiCol_FrameBgHovered]         = ImVec4(0.19f, 0.19f, 0.19f, 0.54f);
    colors[ImGuiCol_FrameBgActive]          = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
    colors[ImGuiCol_TitleBg]                = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_TitleBgActive]          = ImVec4(0.06f, 0.06f, 0.06f, 1.00f);
    colors[ImGuiCol_TitleBgCollapsed]       = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_MenuBarBg]              = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    colors[ImGuiCol_ScrollbarBg]            = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
    colors[ImGuiCol_ScrollbarGrab]          = ImVec4(0.34f, 0.34f, 0.34f, 0.54f);
    colors[ImGuiCol_ScrollbarGrabHovered]   = ImVec4(0.40f, 0.40f, 0.40f, 0.54f);
    colors[ImGuiCol_ScrollbarGrabActive]    = ImVec4(0.56f, 0.56f, 0.56f, 0.54f);
    colors[ImGuiCol_CheckMark]              = ImVec4(0.33f, 0.67f, 0.86f, 1.00f);
    colors[ImGuiCol_SliderGrab]             = ImVec4(0.34f, 0.34f, 0.34f, 0.54f);
    colors[ImGuiCol_PopupBg]                = ImVec4(0.10f, 0.10f, 0.10f, 1.0f);
    colors[ImGuiCol_SliderGrabActive]       = ImVec4(0.56f, 0.56f, 0.56f, 0.54f);
    colors[ImGuiCol_Button]                 = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
    colors[ImGuiCol_ButtonHovered]          = ImVec4(0.19f, 0.19f, 0.19f, 0.54f);
    colors[ImGuiCol_ButtonActive]           = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
    colors[ImGuiCol_Header]                 = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
    colors[ImGuiCol_HeaderHovered]          = ImVec4(0.00f, 0.00f, 0.00f, 0.36f);
    colors[ImGuiCol_HeaderActive]           = ImVec4(0.20f, 0.22f, 0.23f, 0.33f);
    colors[ImGuiCol_Separator]              = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
    colors[ImGuiCol_SeparatorHovered]       = ImVec4(0.44f, 0.44f, 0.44f, 0.29f);
    colors[ImGuiCol_SeparatorActive]        = ImVec4(0.40f, 0.44f, 0.47f, 1.00f);
    colors[ImGuiCol_ResizeGrip]             = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
    colors[ImGuiCol_ResizeGripHovered]      = ImVec4(0.44f, 0.44f, 0.44f, 0.29f);
    colors[ImGuiCol_ResizeGripActive]       = ImVec4(0.40f, 0.44f, 0.47f, 1.00f);
    colors[ImGuiCol_Tab]                    = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
    colors[ImGuiCol_TabHovered]             = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    colors[ImGuiCol_TabActive]              = ImVec4(0.20f, 0.20f, 0.20f, 0.36f);
    colors[ImGuiCol_TabUnfocused]           = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
    colors[ImGuiCol_TabUnfocusedActive]     = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    colors[ImGuiCol_DockingPreview]         = ImVec4(0.33f, 0.67f, 0.86f, 1.00f);
    colors[ImGuiCol_PlotLines]              = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_PlotLinesHovered]       = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_PlotHistogram]          = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_PlotHistogramHovered]   = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_TableHeaderBg]          = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
    colors[ImGuiCol_TableBorderStrong]      = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
    colors[ImGuiCol_TableBorderLight]       = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
    colors[ImGuiCol_TableRowBg]             = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_TableRowBgAlt]          = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
    colors[ImGuiCol_TextSelectedBg]         = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
    colors[ImGuiCol_DragDropTarget]         = ImVec4(0.33f, 0.67f, 0.86f, 1.00f);
    colors[ImGuiCol_NavHighlight]           = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);

    colors[ImGuiCol_NavWindowingHighlight]  = ImVec4(1.00f, 0.00f, 0.00f, 0.70f);
    colors[ImGuiCol_NavWindowingDimBg]      = ImVec4(1.00f, 0.00f, 0.00f, 0.20f);
    colors[ImGuiCol_ModalWindowDimBg]       = ImVec4(0.30f, 0.30f, 0.30f, 0.8f);

    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowPadding                     = ImVec2(8.00f, 8.00f);
    style.FramePadding                      = ImVec2(5.00f, 2.00f);
    style.CellPadding                       = ImVec2(6.00f, 6.00f);
    style.ItemSpacing                       = ImVec2(6.00f, 6.00f);
    style.ItemInnerSpacing                  = ImVec2(6.00f, 6.00f);
    style.TouchExtraPadding                 = ImVec2(0.00f, 0.00f);
    style.IndentSpacing                     = 25;
    style.ScrollbarSize                     = 15;
    style.GrabMinSize                       = 10;
    style.WindowBorderSize                  = 1;
    style.ChildBorderSize                   = 1;
    style.PopupBorderSize                   = 1;
    style.FrameBorderSize                   = 1;
    style.TabBorderSize                     = 1;
    style.WindowRounding                    = 7;
    style.ChildRounding                     = 4;
    style.FrameRounding                     = 3;
    style.PopupRounding                     = 4;
    style.ScrollbarRounding                 = 9;
    style.GrabRounding                      = 3;
    style.LogSliderDeadzone                 = 4;
    style.TabRounding                       = 4;
}

void Gui::draw_main_dockspace(bool *p_open)
{
    // Variables to configure the Dockspace example.
    static bool opt_fullscreen = true; // Is the Dockspace full-screen?
    static bool opt_padding = false; // Is there padding (a blank space) between the window edge and the Dockspace?
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None; // Config flags for the Dockspace

    // In this example, we're embedding the Dockspace into an invisible parent window to make it more configurable.
    // We set ImGuiWindowFlags_NoDocking to make sure the parent isn't dockable into because this is handled by the Dockspace.
    //
    // ImGuiWindowFlags_MenuBar is to show a menu bar with config options. This isn't necessary to the functionality of a
    // Dockspace, but it is here to provide a way to change the configuration flags interactively.
    // You can remove the MenuBar flag if you don't want it in your app, but also remember to remove the code which actually
    // renders the menu bar, found at the end of this function.
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

    // Is the example in Fullscreen mode?
    if (opt_fullscreen)
    {
        // If so, get the main viewport:
        const ImGuiViewport* viewport = ImGui::GetMainViewport();

        // Set the parent window's position, size, and viewport to match that of the main viewport. This is so the parent window
        // completely covers the main viewport, giving it a "full-screen" feel.
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);

        // Set the parent window's styles to match that of the main viewport:
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f); // No corner rounding on the window
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f); // No border around the window

        // Manipulate the window flags to make it inaccessible to the user (no titlebar, resize/move, or navigation)
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    }
    else
    {
        // The example is not in Fullscreen mode (the parent window can be dragged around and resized), disable the
        // ImGuiDockNodeFlags_PassthruCentralNode flag.
        dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
    }

    // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
    // and handle the pass-thru hole, so the parent window should not have its own background:
    if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
        window_flags |= ImGuiWindowFlags_NoBackground;

    // If the padding option is disabled, set the parent window's padding size to 0 to effectively hide said padding.
    if (!opt_padding)
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

    // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
    // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
    // all active windows docked into it will lose their parent and become undocked.
    // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
    // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
    ImGui::Begin("DockSpace Demo", p_open, window_flags);

    // Remove the padding configuration - we pushed it, now we pop it:
    if (!opt_padding)
        ImGui::PopStyleVar();

    // Pop the two style rules set in Fullscreen mode - the corner rounding and the border size.
    if (opt_fullscreen)
        ImGui::PopStyleVar(2);

    // Check if Docking is enabled:
    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
    {
        // If it is, draw the Dockspace with the DockSpace() function.
        // The GetID() function is to give a unique identifier to the Dockspace - here, it's "MyDockSpace".
        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    }
    else
    {
        // Docking is DISABLED - Show a warning message
    }

    // This is to show the menu bar that will change the config settings at runtime.
    // If you copied this demo function into your own code and removed ImGuiWindowFlags_MenuBar at the top of the function,
    // you should remove the below if-statement as well.
    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("Options"))
        {
            // Disabling fullscreen would allow the window to be moved to the front of other windows,
            // which we can't undo at the moment without finer window depth/z control.
            ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen);
            ImGui::MenuItem("Padding", NULL, &opt_padding);
            ImGui::Separator();

            // Display a menu item for each Dockspace flag, clicking on one will toggle its assigned flag.
            if (ImGui::MenuItem("Flag: NoSplit",                "", (dockspace_flags & ImGuiDockNodeFlags_NoSplit) != 0))                 { dockspace_flags ^= ImGuiDockNodeFlags_NoSplit; }
            if (ImGui::MenuItem("Flag: NoResize",               "", (dockspace_flags & ImGuiDockNodeFlags_NoResize) != 0))                { dockspace_flags ^= ImGuiDockNodeFlags_NoResize; }
            if (ImGui::MenuItem("Flag: NoDockingInCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_NoDockingInCentralNode) != 0))  { dockspace_flags ^= ImGuiDockNodeFlags_NoDockingInCentralNode; }
            if (ImGui::MenuItem("Flag: AutoHideTabBar",         "", (dockspace_flags & ImGuiDockNodeFlags_AutoHideTabBar) != 0))          { dockspace_flags ^= ImGuiDockNodeFlags_AutoHideTabBar; }
            if (ImGui::MenuItem("Flag: PassthruCentralNode",    "", (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode) != 0, opt_fullscreen)) { dockspace_flags ^= ImGuiDockNodeFlags_PassthruCentralNode; }
            ImGui::Separator();

            // Display a menu item to close this example.
            if (ImGui::MenuItem("Close", NULL, false, p_open != NULL))
                if (p_open != NULL) // Remove MSVC warning C6011 (NULL dereference) - the `p_open != NULL` in MenuItem() does prevent NULL derefs, but IntelliSense doesn't analyze that deep so we need to add this in ourselves.
                    *p_open = false; // Changing this variable to false will close the parent window, therefore closing the Dockspace as well.
            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }

    // End the parent window that contains the Dockspace:
    ImGui::End();
}
void Gui::render_scene(const uint32_t framebuffer_texture, std::unique_ptr<Camera>& camera) const
{
    SceneViewport::SceneWindowInfo info = SceneViewport::draw_window_scene(reinterpret_cast<ImTextureID>(framebuffer_texture));

    if (info.window_size.x != g_viewport_size->x || info.window_size.y != g_viewport_size->y) {
        camera->update_projection(info.window_size);
        g_viewport_size->x = info.window_size.x;
        g_viewport_size->y = info.window_size.y;
        
        spdlog::info("Window size: {} {}", g_viewport_size->x, g_viewport_size->y);
    }

    if(info.window_pos.x != g_viewport_pos.x || info.window_pos.y != g_viewport_pos.y) {
        g_viewport_pos.x = info.window_pos.x;
        g_viewport_pos.y = info.window_pos.y;

        spdlog::info("Window pos: {} {}", info.window_pos.x, info.window_pos.y);
    }

    if (g_mouseCaptured) {
        camera->process_input(m.window);
    }
}

void Gui::draw_models_control(std::vector<std::unique_ptr<Model>> &models, std::unique_ptr<Camera>& camera) const
{
    ImGui::Begin("Models");

    if(models.empty()) {
        ImGui::End();
        return;
    }
    
    static int selectedModelIndex = 0; // Variable to store the selected model index
    static std::string selectedName = models[0]->get_name();
    static bool showAddModelPopup = false;
    
    // Dropdown menu to select models
    if (ImGui::BeginCombo("Select", fmt::format("{}##{}", selectedName, selectedModelIndex + 1).c_str()))
    {
        for (size_t i = 0; i < models.size(); ++i)
        {
            bool isSelected = (selectedModelIndex == i);
            std::string model_name = models[i]->get_name();
            if (ImGui::Selectable(fmt::format("{}##{}", model_name, i + 1).c_str(), isSelected))
            {
                selectedModelIndex = static_cast<int>(i); // Set the selected model index
                selectedName = model_name;
            }
            if (isSelected)
            {
                ImGui::SetItemDefaultFocus(); // Set the selected item as default focus
            }
        }
        ImGui::EndCombo();
    }
    
    std::unique_ptr<Model>& selectedModel = models[selectedModelIndex]; // Get the selected model
    
    auto position = selectedModel->get_position();
    
    // Append model index to the slider IDs
    std::string posXID = fmt::format("X##{}", selectedModelIndex);
    std::string posYID = fmt::format("Y##{}", selectedModelIndex);
    std::string posZID = fmt::format("Z##{}", selectedModelIndex);
    
    ImGui::PushItemWidth(50); // Adjust the width as needed
    ImGui::DragFloat(posXID.c_str(), &position.x, 0.5f);
    if (ImGui::IsItemEdited()) {
        selectedModel->set_position(position);
    }
    ImGui::SameLine();
    ImGui::DragFloat(posYID.c_str(), &position.y, 0.5f);
    if (ImGui::IsItemEdited()) {
        selectedModel->set_position(position);
    }
    ImGui::SameLine();
    ImGui::DragFloat(posZID.c_str(), &position.z, 0.5f);
    if (ImGui::IsItemEdited()) {
        selectedModel->set_position(position);
    }
    ImGui::PopItemWidth();
    ImGui::Separator();

    if (ImGui::Button("Add Model"))
    {
        showAddModelPopup = true;
       
    }

    ImGui::End();

    if (showAddModelPopup)
    {
        ImGui::OpenPopup("Add Model Popup");
        if (ImGui::BeginPopupModal("Add Model Popup", &showAddModelPopup))
        {
            static char modelName[64] = "";

            ImGui::Text("Enter model details:");
            ImGui::InputText("Model Name", modelName, sizeof(modelName));

            if (ImGui::Button("Add") && strlen(modelName) > 0) // Check if name field is not empty
            {
                 models.emplace_back(std::make_unique<Model>(Model::WithResultOf([&camera](){
                    return Model::create(camera, modelName);
                })));
                
                memset(modelName, 0, sizeof(modelName));
                
                ImGui::CloseCurrentPopup();
                showAddModelPopup = false;
            }

            ImGui::SameLine();
            
            if (ImGui::Button("Close"))
            {
                ImGui::CloseCurrentPopup();
                showAddModelPopup = false;
            }
            
            ImGui::EndPopup();
        }
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
