#pragma once
#include <type_traits>
#include <fstream>
#include <sstream>
#include <optional>
#include <functional>
#include <glad/glad.h>

class Shader {
private:
	struct M {
		uint32_t shaderProgram;
	} m;

	explicit Shader(M m) : m(std::move(m)) {}

	static std::optional<std::string> open(const char* shaderPath);
	static void check(uint32_t module);
public:
	struct ShaderModule {
		const char* vertexPath;
		const char* fragmentPath;
	};

	static Shader compile(ShaderModule&& module);

	void invoke() const;

	inline void set_uniform_i1(const std::string& location) const { glUniform1i(glGetUniformLocation(m.shaderProgram, location.c_str()), 0); }
	inline const uint32_t get_program() const& { return m.shaderProgram; }

	struct WithResultOf {
		std::function<Shader()> func;
	};

	Shader(WithResultOf&& res);
};