#pragma once
#include <GLFW/glfw3.h>
#include <memory>
#include <functional>
#include "Shader.h"
#include <glad/glad.h>

class Renderer {
private:
	struct M {
		std::shared_ptr<Shader> shader;
	} m;

	explicit Renderer(M m) : m(std::move(m)) {}

public:
	static Renderer create();

	struct WithResultOf {
		std::function<Renderer()> func;
	};

	Renderer(WithResultOf&& res);

	void draw();

	Renderer(Renderer&& other) noexcept;
};