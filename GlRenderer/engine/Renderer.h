#pragma once
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <memory>
#include <functional>
#include "Shader.h"
#include "VertexObject.h"
#include <glad/glad.h>

class Renderer {
private:
	struct M {
		std::shared_ptr<Shader> shader;
		std::shared_ptr<VertexObject> vertexObject;
	} m;

	explicit Renderer(M m) : m(std::move(m)) {}

public:
	static Renderer create();

	struct WithResultOf {
		std::function<Renderer()> func;
	};

	Renderer(WithResultOf&& res);

	void draw();
	void destroy() const;

	Renderer(Renderer&& other) noexcept;
	~Renderer();
};