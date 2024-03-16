#pragma once

#include <imgui.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

namespace SceneViewport {
	ImVec2 draw_window_scene(ImTextureID draw_texture_id);
}