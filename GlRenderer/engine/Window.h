#pragma once
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <type_traits>
#include <spdlog/spdlog.h>
#include "Renderer.h"

class Window {
private:
	struct M {
		GLFWwindow* window;
		std::unique_ptr<Renderer> renderer;
	} m;

	explicit Window(M m) : m(std::move(m)) {}

	static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
public:
	struct WndConstruct {
		uint32_t width;
		uint32_t height;
		const char* name;
	};

	static Window create(WndConstruct&& parameters);
	
	explicit Window(Window& other) = delete;
	explicit Window(Window&& other) noexcept;

	void run_loop();

	~Window();
};