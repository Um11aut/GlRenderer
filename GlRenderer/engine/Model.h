#pragma once

#include <functional>
#include <type_traits>
#include <memory>
#include "Shader.h"
#include "VertexObject.h"
#include "UniformObject.h"

class Model {
private:
	struct M {
		std::unique_ptr<Shader> shader;
        std::unique_ptr<VertexObject> vertex_object;
        std::unique_ptr<UniformObject> uniform_object;

        std::shared_ptr<glm::mat4> model;
	} m;

	explicit Model(M m) : m(std::move(m)) {}
public:
	static Model create(std::unique_ptr<Camera>& camera);

    void invoke() const;
	void destroy() const;

    void set_position(const glm::vec3& new_position) const;

	struct WithResultOf {
		std::function<Model()> func;
	};

	Model(WithResultOf&& res);
};