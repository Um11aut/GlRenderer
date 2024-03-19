#include "SceneViewport.h"

double m_width;
double m_height;

ImVec2 SceneViewport::draw_window_scene(ImTextureID draw_texture_id)
{
    ImGuiIO& io = ImGui::GetIO();
    bool isMouseOverViewport = false;

    // Begin ImGui rendering
    ImGui::Begin("Scene");

    // Check if the mouse is hovering over the OpenGL framebuffer viewport
    ImVec2 viewportPos = ImGui::GetCursorScreenPos();
    ImVec2 viewportSize = ImGui::GetContentRegionAvail();
    ImVec2 mousePos = ImGui::GetMousePos();
    isMouseOverViewport = mousePos.x >= viewportPos.x && mousePos.y >= viewportPos.y &&
        mousePos.x < viewportPos.x + viewportSize.x &&
        mousePos.y < viewportPos.y + viewportSize.y;

    ImVec2 size = ImGui::GetWindowSize();

    if (isMouseOverViewport)
    {
        ImGui::GetIO().WantCaptureMouse = true; // Capture mouse if it's over the viewport
    }

    // Your existing rendering code
    ImGui::BeginChild("GameRender");
    float width = ImGui::GetContentRegionAvail().x;
    float height = ImGui::GetContentRegionAvail().y;

    ImGui::Image(
        draw_texture_id,
        ImVec2(width, height)
    );

    ImGui::EndChild();
    ImGui::End();

    io.WantCaptureMouse = !isMouseOverViewport;

    return size;
}
