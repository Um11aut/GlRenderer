#pragma once
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <type_traits>
#include <functional>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

class Gui {
private:
	struct M {
		GLFWwindow* window;
	} m;

	explicit Gui(M m) : m(std::move(m)) {}
public:
	static Gui create(GLFWwindow* window);

	void invoke_start() const;
	void invoke_end() const;
	void destroy() const;

	struct WithResultOf {
		std::function<Gui()> func;
	};

	Gui(WithResultOf&& res);
};