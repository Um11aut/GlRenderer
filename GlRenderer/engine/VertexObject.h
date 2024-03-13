#pragma once

#include <glad/glad.h>
#include <string>
#include <functional>

class VertexObject {
private:
	struct M {
		uint32_t shaderProgram;
	} m;

	explicit VertexObject(M m) : m(std::move(m)) {}

public:
	static VertexObject create();

	struct WithResultOf {
		std::function<VertexObject()> func;
	};

	VertexObject(WithResultOf&& res) noexcept;
};