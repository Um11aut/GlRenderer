#pragma once
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <type_traits>
#include <functional>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include "external/Camera.h"

class Gui {
private:
	struct M {
		GLFWwindow* window;
		ImVec2 last_size;
	} m;

	explicit Gui(M m) : m(std::move(m)) {}
public:
	void render_scene(const uint32_t framebuffer_texture, std::unique_ptr<Camera>& camera);
	static Gui create(GLFWwindow* window);

	void invoke_start() const;


	void invoke_end() const;
	void destroy() const;

	struct WithResultOf {
		std::function<Gui()> func;
	};

	Gui(WithResultOf&& res);
};