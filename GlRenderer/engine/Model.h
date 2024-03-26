#pragma once

#include <functional>
#include <type_traits>
#include <memory>
#include "Shader.h"
#include "VertexObject.h"
#include "UniformObject.h"
#include "Texture.h"
#include "Entity.h"
#include "util/Ref.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <string>

class Model : public Entity {
private:
	struct ModelLoaderParameters {
		const aiScene* scene;
		aiNode* node;
		aiMesh* mesh;
	};

	struct M {
		std::string model_name;

        std::unique_ptr<Texture> texture;
		std::unique_ptr<Shader> shader;
        std::unique_ptr<VertexObject> vertex_object;
        std::unique_ptr<UniformObject> uniform_object;

		ModelLoaderParameters model_parameters;

        std::shared_ptr<glm::mat4> model;
		glm::vec3 model_position;
	} m;

	explicit Model(M m) : m(std::move(m)) {}
public:
	struct ModelCreateInfo {
		Ref<std::unique_ptr<Camera>> camera;
		const std::string& name;
		const std::string& path;
	};

	static Model create(ModelCreateInfo info);

    void invoke() const override;
	void destroy() const;

    void set_position(const glm::vec3& new_position);
	inline const glm::vec3 get_position() const { return m.model_position; }
	inline std::string get_name() const override { return m.model_name; }

	struct WithResultOf {
		std::function<Model()> func;
	};

	Model(WithResultOf&& res);
};