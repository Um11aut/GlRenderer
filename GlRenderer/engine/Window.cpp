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
	glfwDestroyWindow(m.window);
}

Window Window::create(WndConstruct&& parameters)
{
	glfwInit();

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
		.renderer = std::make_shared<Renderer>(
			Renderer::WithResultOf{[]() {
				return Renderer::create();
			}}
		)
	});
}
