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


Window Window::create(WndConstruct&& parameters)
{
	glfwInit();

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_TRUE);
	glfwWindowHint(GLFW_DEPTH_BITS, GL_TRUE);

	int numMonitors;
	GLFWmonitor** monitors = glfwGetMonitors(&numMonitors);
	GLFWmonitor* monitor = monitors[0];

	GLFWwindow* window = glfwCreateWindow(
		parameters.width,
		parameters.height,
		parameters.name,
		nullptr,
		nullptr
	);

	glfwMakeContextCurrent(window);

	return Window(M{
		.window = std::move(window),
		.renderer = std::make_unique<Renderer>(
			Renderer::WithResultOf{[&window]() {
				return Renderer::create(window);
			}}
		),
	});
}
