#pragma once

#include <glad/glad.h>
#include <algorithm>
#include <string>
#include "Shader.h"
#include "external/Camera.h"
#include "Texture.h"

class CubeMap {
private:
	static constexpr float skybox_vertices[] = {
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
	};

	struct CubeMapUBO {
		std::shared_ptr<glm::mat4> proj;
	};

	struct M {
		std::unique_ptr<Shader> shader;
		uint32_t texture_id;
		uint32_t ubo;
        uint32_t vao;
        CubeMapUBO vp;
	} m;

    explicit CubeMap(M m) : m(std::move(m)) {}
public:
	struct Textures {
		std::string right;
		std::string left;

		std::string top;
		std::string bottom;
	
		std::string front;
		std::string back;
	};

	static CubeMap create(Textures&& textures, std::unique_ptr<Camera>& camera);

	void invoke() const;

    struct WithResultOf {
        std::function<CubeMap()> func;
    };

    CubeMap(WithResultOf&& res);
};