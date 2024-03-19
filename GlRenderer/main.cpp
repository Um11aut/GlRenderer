#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include "engine/Window.h"
#include "engine/Renderer.h"
#include <GL/gl.h>

int main() {
	Window window = Window::create({
		.width = 800,
		.height = 600,
		.name = "Main Renderer"
	});

	window.run_loop();
}