#include "Shader.h"
#include <spdlog/spdlog.h>
#include <format>

std::optional<std::string> Shader::open(const char* shaderPath)
{
	std::ifstream file;
	file.open(shaderPath);

	if (!file.is_open()) {
		spdlog::info(fmt::format("Failed to open file! File Path: ", shaderPath));
		return std::nullopt;
	}

	std::stringstream s_stream;
	s_stream << file.rdbuf();
	
	if (s_stream.fail()) {
		spdlog::info(fmt::format("Failed to stream content from the file! File Path: ", shaderPath));
		return std::nullopt;
	}
	
	std::string str = s_stream.str();

	if (str.empty()) {
		spdlog::info(fmt::format("File is empty! File Path: ", shaderPath));
		return std::nullopt;
	}

	return str;
}

void Shader::check(uint32_t module)
{
	int success;
	char infoLog[512];
	glGetShaderiv(module, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(module, 512, nullptr, infoLog);
		spdlog::error("Vertex shader compilation failed: \n{}", infoLog);
	}
}

Shader Shader::compile(ShaderModule&& module)
{
	std::string vertex_string = open(module.vertexPath).value();
	std::string fragment_string = open(module.fragmentPath).value();

	const char* vertex_content = vertex_string.c_str();
	const char* fragment_content = fragment_string.c_str();

	uint32_t vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_shader, 1, &vertex_content, nullptr);
	glCompileShader(vertex_shader);

	check(vertex_shader);

	uint32_t fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader, 1, &fragment_content, nullptr);
	glCompileShader(fragment_shader);

	check(fragment_shader);

	uint32_t shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertex_shader);
	glAttachShader(shaderProgram, fragment_shader);
	glLinkProgram(shaderProgram);

	int success;
	char infoLog[512];
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
		spdlog::error("Shader program linking failed: \n{}", infoLog);
	}

	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);

	return Shader(M{.shaderProgram = shaderProgram});
}

Shader Shader::compile_single(const char *module, GLenum shader_type)
{
    std::string string = open(module).value();

	const char* content = string.c_str();

	uint32_t shader = glCreateShader(shader_type);
	glShaderSource(shader, 1, &content, nullptr);
	glCompileShader(shader);

	check(shader);

	uint32_t shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, shader);
	glLinkProgram(shaderProgram);

	int success;
	char infoLog[512];
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
		spdlog::error("Shader program linking failed: \n{}", infoLog);
	}

	glDeleteShader(shader);

	return Shader(M{.shaderProgram = shaderProgram});
}

void Shader::invoke() const
{
	glUseProgram(m.shaderProgram);
}

Shader::Shader(WithResultOf&& res)
{
	Shader other = res.func();

	m = std::move(other.m);
}
