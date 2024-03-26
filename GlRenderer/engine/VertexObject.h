#pragma once

#include <glad/glad.h>
#include <string>
#include <functional>
#include "util/Ref.h"

class VertexObject {
private:
	struct M {
		uint32_t VAO, VBO, EBO;
		uint32_t indices_count;
	} m;

	explicit VertexObject(M m) : m(std::move(m)) {}

public:
	struct VertexBufferCreateInfo {
		const Ref<std::vector<float>> vertices; 
		const Ref<std::vector<float>> textureCoords;
		const Ref<std::vector<uint32_t>> indices;
		uint32_t vertices_count; 
		uint32_t indices_count;
		uint32_t texture_coords_count;
	};

	static VertexObject create(VertexBufferCreateInfo info);

	void invoke() const;
	void destroy() const;

	struct WithResultOf {
		std::function<VertexObject()> func;
	};

	~VertexObject();

	VertexObject(WithResultOf&& res) noexcept;
};