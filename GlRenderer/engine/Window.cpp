#include "Window.h"

static Renderer* renderer_ptr;

static void window_size_callback(GLFWwindow* window, int width, int height) {
	renderer_ptr->draw();
	glfwSwapBuffers(window);
}

Window::Window(Window&& other) noexcept
{
	m = std::move(other.m);
}

void Window::run_loop()
{
	renderer_ptr = m.renderer.get();
	glfwSetWindowSizeCallback(m.window, window_size_callback);

	while (!glfwWindowShouldClose(m.window)) {
		m.renderer->draw();

		glfwSwapBuffers(m.window);
		
		glfwPollEvents();
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
	glfwWindowHint(GLFW_DEPTH_BITS, 24);

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