#pragma once

#include <imgui.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

namespace SceneViewport {
	struct SceneWindowInfo {
		ImVec2 window_pos;
		ImVec2 window_size;
	};

	SceneWindowInfo draw_window_scene(ImTextureID draw_texture_id);
}