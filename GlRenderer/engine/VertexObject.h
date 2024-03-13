#pragma once

#include <glad/glad.h>
#include <string>
#include <functional>

class VertexObject {
private:
	struct M {
		uint32_t VAO, VBO;
	} m;

	explicit VertexObject(M m) : m(std::move(m)) {}

public:
	static VertexObject create(float vertices[]);

	void invoke() const;
	void destroy() const;

	struct WithResultOf {
		std::function<VertexObject()> func;
	};

	~VertexObject();

	VertexObject(WithResultOf&& res) noexcept;
};