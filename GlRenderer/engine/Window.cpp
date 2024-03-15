#include "Window.h"


Window::Window(Window&& other) noexcept
{
	m = std::move(other.m);
}

void Window::run_loop()
{
	while (!glfwWindowShouldClose(m.window)) {
		glfwPollEvents();

		m.renderer->draw();

		glfwSwapBuffers(m.window);
	}
}

Window::~Window()
{
	spdlog::info("Window deleted!");
	m.renderer->destroy();
	glfwDestroyWindow(m.window);
}

void Window::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

Window Window::create(WndConstruct&& parameters)
{
	glfwInit();

	glfwWindowHint(GLFW_SAMPLES, 4);

	GLFWwindow* window = glfwCreateWindow(
		parameters.width,
		parameters.height,
		parameters.name,
		nullptr,
		nullptr
	);

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	return Window(M{
		.window = std::move(window),
		.renderer = std::make_unique<Renderer>(
			Renderer::WithResultOf{[&window]() {
				return Renderer::create(window);
			}}
		),
	});
}
