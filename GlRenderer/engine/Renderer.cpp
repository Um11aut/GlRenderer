#include "Renderer.h"
#include <spdlog/spdlog.h>

Renderer Renderer::create()
{
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		spdlog::error("Failed to initialize GLAD");
		glfwTerminate();
	}

	const char* version = reinterpret_cast<const char*>(glGetString(GL_VERSION));
	spdlog::info("Loaded OpenGL with version: " + std::string(version));

	return Renderer(M{
		.shader = std::make_shared<Shader>(Shader::WithResultOf{[]() {
				return Shader::compile(Shader::ShaderModule{
					.vertexPath = "C:/Users/askk/source/repos/GlRenderer/resources/vertex.vert",
					.fragmentPath = "C:/Users/askk/source/repos/GlRenderer/resources/fragment.frag"
				});
			}
		}),
	});
}

Renderer::Renderer(WithResultOf&& res)
{
	Renderer other = res.func();

	m = std::move(other.m);
}

void Renderer::draw()
{
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	m.shader->use();
}

Renderer::Renderer(Renderer&& other) noexcept
{
	m = std::move(other.m);
}
