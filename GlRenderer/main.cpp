#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include "engine/Window.h"
#include <GL/gl.h>
#include <iostream>

int main() {
	Window window1 = Window::create({
	.width = 1920,
	.height = 1080,
	.name = "Main Renderer"
		});

	window1.run_loop(); 
}