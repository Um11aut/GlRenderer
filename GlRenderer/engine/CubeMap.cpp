#include "CubeMap.h"
#include <vector>
#include <spdlog/spdlog.h>
#include <glm/gtc/type_ptr.hpp>

Camera* g_camera = nullptr;

CubeMap CubeMap::create(Textures&& textures, std::unique_ptr<Camera>& camera, const std::string& name)
{
	std::vector<std::string> faces = {textures.right, textures.left, textures.top, textures.bottom, textures.front, textures.back};

	uint32_t texture_id;
	glGenTextures(1, &texture_id);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture_id);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	int width, height, num_channels;
	for (uint32_t i = 0; i < faces.size(); ++i) {
		uint8_t* data = stbi_load(faces[i].c_str(), &width, &height, &num_channels, STBI_rgb_alpha);
		if (data)
		{
			if (num_channels == 3) {
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
					0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
				);
			}

			if (num_channels == 4) {
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
					0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data
				);
			}
			
			stbi_image_free(data);
		}
		else {
			spdlog::error("Failed to load cubemap texture: {}", faces[i]);
			stbi_image_free(data);
		}
	}

	std::unique_ptr<Shader> shader = std::make_unique<Shader>(Shader::WithResultOf{
		[]() {
			return Shader::compile(Shader::ShaderModule{
				.vertexPath = "resources/skybox.vert",
				.fragmentPath = "resources/skybox.frag"
			});
		 }
	});
	
	shader->invoke();
	glUniform1i(glGetUniformLocation(shader->get_program(), "skybox"), 0);

	uint32_t VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	uint32_t VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(CubeMap::skybox_vertices), CubeMap::skybox_vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	g_camera = camera.get();

	CubeMapUBO ubo_struct = {
		.proj = camera->get_proj()
	};

	return CubeMap(M{
		std::move(shader),
		texture_id,
		0,
		VAO, 
		ubo_struct,
		name
	});
}

void CubeMap::invoke() const
{
	glDepthFunc(GL_LEQUAL);
	glCullFace(GL_BACK);
	m.shader->invoke();

	glBindVertexArray(m.vao);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m.texture_id);

	glm::mat4 view = glm::mat4(glm::mat3(*g_camera->get_view()));
	glUniformMatrix4fv(glGetUniformLocation(m.shader->get_program(), "view"), 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(m.shader->get_program(), "projection"), 1, GL_FALSE, glm::value_ptr(*m.vp.proj));

	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);

	glUseProgram(0);
	glCullFace(GL_FRONT);
	glDepthFunc(GL_LESS);
}

CubeMap::CubeMap(WithResultOf&& res)
{
	CubeMap other = std::move(res.func());

	m = std::move(other.m);
}
