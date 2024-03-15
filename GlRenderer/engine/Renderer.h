#pragma once
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <memory>
#include <functional>
#include "Shader.h"
#include "VertexObject.h"
#include "UniformObject.h"
#include "Gui.h"
#include <glad/glad.h>

class Renderer {
private:
	struct M {
		std::unique_ptr<Gui> gui;
		std::unique_ptr<Shader> shader;
		std::unique_ptr<VertexObject> vertex_object;
		std::unique_ptr<UniformObject> uniform_object;
	} m;

	explicit Renderer(M m) : m(std::move(m)) {}

	template<typename... T>
	constexpr void invoke(std::unique_ptr<T>&... objects);
public:
	static Renderer create(GLFWwindow* window);

	struct WithResultOf {
		std::function<Renderer()> func;
	};

	Renderer(WithResultOf&& res);

	void draw();
	void destroy() const;

	Renderer(Renderer&& other) noexcept;
	~Renderer();
};
